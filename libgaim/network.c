/**
 * @file network.c Network Implementation
 * @ingroup core
 *
 * gaim
 *
 * Gaim is the legal property of its developers, whose names are too numerous
 * to list here.  Please refer to the COPYRIGHT file distributed with this
 * source distribution.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "internal.h"

#ifndef _WIN32
#include <resolv.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <net/if.h>
#include <sys/ioctl.h>
#else
#include <nspapi.h>
#endif

/* Solaris */
#if defined (__SVR4) && defined (__sun)
#include <sys/sockio.h>
#endif

#include "debug.h"
#include "account.h"
#include "network.h"
#include "prefs.h"
#include "stun.h"
#include "upnp.h"

#ifdef HAVE_LIBNM
#include <libnm_glib.h>

libnm_glib_ctx *nm_context = NULL;
guint nm_callback_idx = 0;

#elif defined _WIN32
static int current_network_count;
#endif

struct _GaimNetworkListenData {
	int listenfd;
	int socket_type;
	gboolean retry;
	gboolean adding;
	GaimNetworkListenCallback cb;
	gpointer cb_data;
};

#ifdef HAVE_LIBNM
void nm_callback_func(libnm_glib_ctx* ctx, gpointer user_data);
#endif

const unsigned char *
gaim_network_ip_atoi(const char *ip)
{
	static unsigned char ret[4];
	gchar *delimiter = ".";
	gchar **split;
	int i;

	g_return_val_if_fail(ip != NULL, NULL);

	split = g_strsplit(ip, delimiter, 4);
	for (i = 0; split[i] != NULL; i++)
		ret[i] = atoi(split[i]);
	g_strfreev(split);

	/* i should always be 4 */
	if (i != 4)
		return NULL;

	return ret;
}

void
gaim_network_set_public_ip(const char *ip)
{
	g_return_if_fail(ip != NULL);

	/* XXX - Ensure the IP address is valid */

	gaim_prefs_set_string("/core/network/public_ip", ip);
}

const char *
gaim_network_get_public_ip(void)
{
	return gaim_prefs_get_string("/core/network/public_ip");
}

const char *
gaim_network_get_local_system_ip(int fd)
{
	char buffer[1024];
	static char ip[16];
	char *tmp;
	struct ifconf ifc;
	struct ifreq *ifr;
	struct sockaddr_in *sinptr;
	guint32 lhost = htonl(127 * 256 * 256 * 256 + 1);
	long unsigned int add;
	int source = fd;

	if (fd < 0)
		source = socket(PF_INET,SOCK_STREAM, 0);

	ifc.ifc_len = sizeof(buffer);
	ifc.ifc_req = (struct ifreq *)buffer;
	ioctl(source, SIOCGIFCONF, &ifc);

	if (fd < 0)
		close(source);

	tmp = buffer;
	while (tmp < buffer + ifc.ifc_len)
	{
		ifr = (struct ifreq *)tmp;
		tmp += sizeof(struct ifreq);

		if (ifr->ifr_addr.sa_family == AF_INET)
		{
			sinptr = (struct sockaddr_in *)&ifr->ifr_addr;
			if (sinptr->sin_addr.s_addr != lhost)
			{
				add = ntohl(sinptr->sin_addr.s_addr);
				g_snprintf(ip, 16, "%lu.%lu.%lu.%lu",
					((add >> 24) & 255),
					((add >> 16) & 255),
					((add >> 8) & 255),
					add & 255);

				return ip;
			}
		}
	}

	return "0.0.0.0";
}

const char *
gaim_network_get_my_ip(int fd)
{
	const char *ip = NULL;
	GaimStunNatDiscovery *stun;

	/* Check if the user specified an IP manually */
	if (!gaim_prefs_get_bool("/core/network/auto_ip")) {
		ip = gaim_network_get_public_ip();
		if ((ip != NULL) && (*ip != '\0'))
			return ip;
	}

	/* Check if STUN discovery was already done */
	stun = gaim_stun_discover(NULL);
	if ((stun != NULL) && (stun->status == GAIM_STUN_STATUS_DISCOVERED))
		return stun->publicip;

	/* Attempt to get the IP from a NAT device using UPnP */
	ip = gaim_upnp_get_public_ip();
	if (ip != NULL)
	  return ip;

	/* Just fetch the IP of the local system */
	return gaim_network_get_local_system_ip(fd);
}


static void
gaim_network_set_upnp_port_mapping_cb(gboolean success, gpointer data)
{
	GaimNetworkListenData *listen_data;

	listen_data = data;
	/* TODO: Once we're keeping track of upnp requests... */
	/* listen_data->pnp_data = NULL; */

	if (!success) {
		gaim_debug_info("network", "Couldn't create UPnP mapping\n");
		if (listen_data->retry) {
			listen_data->retry = FALSE;
			listen_data->adding = FALSE;
			/* TODO: Need to keep track of this return value! */
			gaim_upnp_remove_port_mapping(
				gaim_network_get_port_from_fd(listen_data->listenfd),
				(listen_data->socket_type == SOCK_STREAM) ? "TCP" : "UDP",
				gaim_network_set_upnp_port_mapping_cb, listen_data);
			return;
		}
	} else if (!listen_data->adding) {
		/* We've tried successfully to remove the port mapping.
		 * Try to add it again */
		listen_data->adding = TRUE;
		/* TODO: Need to keep track of this return value! */
		gaim_upnp_set_port_mapping(
			gaim_network_get_port_from_fd(listen_data->listenfd),
			(listen_data->socket_type == SOCK_STREAM) ? "TCP" : "UDP",
			gaim_network_set_upnp_port_mapping_cb, listen_data);
		return;
	}

	if (listen_data->cb)
		listen_data->cb(listen_data->listenfd, listen_data->cb_data);

	gaim_network_listen_cancel(listen_data);
}


static GaimNetworkListenData *
gaim_network_do_listen(unsigned short port, int socket_type, GaimNetworkListenCallback cb, gpointer cb_data)
{
	int listenfd = -1;
	const int on = 1;
	GaimNetworkListenData *listen_data;
#ifdef HAVE_GETADDRINFO
	int errnum;
	struct addrinfo hints, *res, *next;
	char serv[6];

	/*
	 * Get a list of addresses on this machine.
	 */
	snprintf(serv, sizeof(serv), "%hu", port);
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = socket_type;
	errnum = getaddrinfo(NULL /* any IP */, serv, &hints, &res);
	if (errnum != 0) {
#ifndef _WIN32
		gaim_debug_warning("network", "getaddrinfo: %s\n", gai_strerror(errnum));
		if (errnum == EAI_SYSTEM)
			gaim_debug_warning("network", "getaddrinfo: system error: %s\n", strerror(errno));
#else
		gaim_debug_warning("network", "getaddrinfo: Error Code = %d\n", errnum);
#endif
		return NULL;
	}

	/*
	 * Go through the list of addresses and attempt to listen on
	 * one of them.
	 * XXX - Try IPv6 addresses first?
	 */
	for (next = res; next != NULL; next = next->ai_next) {
		listenfd = socket(next->ai_family, next->ai_socktype, next->ai_protocol);
		if (listenfd < 0)
			continue;
		if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0)
			gaim_debug_warning("network", "setsockopt: %s\n", strerror(errno));
		if (bind(listenfd, next->ai_addr, next->ai_addrlen) == 0)
			break; /* success */
		/* XXX - It is unclear to me (datallah) whether we need to be
		   using a new socket each time */
		close(listenfd);
	}

	freeaddrinfo(res);

	if (next == NULL)
		return NULL;
#else
	struct sockaddr_in sockin;

	if ((listenfd = socket(AF_INET, socket_type, 0)) < 0) {
		gaim_debug_warning("network", "socket: %s\n", strerror(errno));
		return NULL;
	}

	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0)
		gaim_debug_warning("network", "setsockopt: %s\n", strerror(errno));

	memset(&sockin, 0, sizeof(struct sockaddr_in));
	sockin.sin_family = PF_INET;
	sockin.sin_port = htons(port);

	if (bind(listenfd, (struct sockaddr *)&sockin, sizeof(struct sockaddr_in)) != 0) {
		gaim_debug_warning("network", "bind: %s\n", strerror(errno));
		close(listenfd);
		return NULL;
	}
#endif

	if (socket_type == SOCK_STREAM && listen(listenfd, 4) != 0) {
		gaim_debug_warning("network", "listen: %s\n", strerror(errno));
		close(listenfd);
		return NULL;
	}
	fcntl(listenfd, F_SETFL, O_NONBLOCK);

	gaim_debug_info("network", "Listening on port: %hu\n", gaim_network_get_port_from_fd(listenfd));

	listen_data = g_new0(GaimNetworkListenData, 1);
	listen_data->listenfd = listenfd;
	listen_data->adding = TRUE;
	listen_data->retry = TRUE;
	listen_data->cb = cb;
	listen_data->cb_data = cb_data;

	/* TODO: Need to keep track of this return value! */
	gaim_upnp_set_port_mapping(
			gaim_network_get_port_from_fd(listenfd),
			(socket_type == SOCK_STREAM) ? "TCP" : "UDP",
			gaim_network_set_upnp_port_mapping_cb, listen_data);

	return listen_data;
}

GaimNetworkListenData *
gaim_network_listen(unsigned short port, int socket_type,
		GaimNetworkListenCallback cb, gpointer cb_data)
{
	g_return_val_if_fail(port != 0, NULL);

	return gaim_network_do_listen(port, socket_type, cb, cb_data);
}

GaimNetworkListenData *
gaim_network_listen_range(unsigned short start, unsigned short end,
		int socket_type, GaimNetworkListenCallback cb, gpointer cb_data)
{
	GaimNetworkListenData *ret = NULL;

	if (gaim_prefs_get_bool("/core/network/ports_range_use")) {
		start = gaim_prefs_get_int("/core/network/ports_range_start");
		end = gaim_prefs_get_int("/core/network/ports_range_end");
	} else {
		if (end < start)
			end = start;
	}

	for (; start <= end; start++) {
		ret = gaim_network_do_listen(start, socket_type, cb, cb_data);
		if (ret != NULL)
			break;
	}

	return ret;
}

void gaim_network_listen_cancel(GaimNetworkListenData *listen_data)
{
	g_free(listen_data);
}

unsigned short
gaim_network_get_port_from_fd(int fd)
{
	struct sockaddr_in addr;
	socklen_t len;

	g_return_val_if_fail(fd >= 0, 0);

	len = sizeof(addr);
	if (getsockname(fd, (struct sockaddr *) &addr, &len) == -1) {
		gaim_debug_warning("network", "getsockname: %s\n", strerror(errno));
		return 0;
	}

	return ntohs(addr.sin_port);
}

#ifdef _WIN32
#ifndef NS_NLA
#define NS_NLA 15
#endif
static gint
wgaim_get_connected_network_count(void)
{
	guint net_cnt = 0;

	WSAQUERYSET qs;
	HANDLE h;
	gint retval;
	int errorid;

	memset(&qs, 0, sizeof(WSAQUERYSET));
	qs.dwSize = sizeof(WSAQUERYSET);
	qs.dwNameSpace = NS_NLA;

	retval = WSALookupServiceBegin(&qs, LUP_RETURN_ALL, &h);
	if (retval != ERROR_SUCCESS) {
		gchar *msg;
		errorid = WSAGetLastError();
		msg = g_win32_error_message(errorid);
		gaim_debug_warning("network", "Couldn't retrieve NLA SP lookup handle. "
						"NLA service is probably not running. Message: %s (%d).\n",
						msg, errorid);
		g_free(msg);

		return -1;
	} else {
		char buf[1024];
		WSAQUERYSET *res = (LPWSAQUERYSET) buf;
		DWORD size = sizeof(buf);
		while ((retval = WSALookupServiceNext(h, 0, &size, res)) == ERROR_SUCCESS) {
			net_cnt++;
			gaim_debug_info("network", "found network '%s'\n",
					res->lpszServiceInstanceName ? res->lpszServiceInstanceName : "(NULL)");
			size = sizeof(buf);
		}

		errorid = WSAGetLastError();
		if (!(errorid == WSA_E_NO_MORE || errorid == WSAENOMORE)) {
			gchar *msg = g_win32_error_message(errorid);
			gaim_debug_error("network", "got unexpected NLA response %s (%d)\n", msg, errorid);
			g_free(msg);

			net_cnt = -1;
		}

		retval = WSALookupServiceEnd(h);
	}

	return net_cnt;

}

static gboolean wgaim_network_change_thread_cb(gpointer data)
{
	gint new_count;
	GaimConnectionUiOps *ui_ops = gaim_connections_get_ui_ops();

	new_count = wgaim_get_connected_network_count();

	if (new_count < 0)
		return FALSE;

	gaim_debug_info("network", "Received Network Change Notification. Current network count is %d, previous count was %d.\n", new_count, current_network_count);

	if (new_count > 0 && ui_ops != NULL && ui_ops->network_connected != NULL) {
		ui_ops->network_connected();
	} else if (new_count == 0 && current_network_count > 0 &&
			   ui_ops != NULL && ui_ops->network_disconnected != NULL) {
		ui_ops->network_disconnected();
	}

	current_network_count = new_count;

	return FALSE;
}

static gpointer wgaim_network_change_thread(gpointer data)
{
	HANDLE h;
	WSAQUERYSET qs;
	time_t last_trigger = time(NULL);

	int WSAAPI (*MyWSANSPIoctl) (
		HANDLE hLookup, DWORD dwControlCode, LPVOID lpvInBuffer,
		DWORD cbInBuffer, LPVOID lpvOutBuffer, DWORD cbOutBuffer,
		LPDWORD lpcbBytesReturned, LPWSACOMPLETION lpCompletion) = NULL;
 
	if (!(MyWSANSPIoctl = (void*) wgaim_find_and_loadproc("ws2_32.dll", "WSANSPIoctl"))) {
		g_thread_exit(NULL);
		return NULL;
	}

	while (TRUE) {
		int retval;
		DWORD retLen = 0;

		memset(&qs, 0, sizeof(WSAQUERYSET));
		qs.dwSize = sizeof(WSAQUERYSET);
		qs.dwNameSpace = NS_NLA;
		if (WSALookupServiceBegin(&qs, 0, &h) == SOCKET_ERROR) {
			int errorid = WSAGetLastError();
			gchar *msg = g_win32_error_message(errorid);
			gaim_debug_warning("network", "Couldn't retrieve NLA SP lookup handle. "
				"NLA service is probably not running. Message: %s (%d).\n",
				msg, errorid);
			g_free(msg);
			g_thread_exit(NULL);
			return NULL;
		}

		/* Make sure at least 30 seconds have elapsed since the last
		 * notification so we don't peg the cpu if this keeps changing. */
		if ((time(NULL) - last_trigger) < 30)
			Sleep(30000);

		last_trigger = time(NULL);

		/* This will block until there is a network change */
		if (MyWSANSPIoctl(h, SIO_NSP_NOTIFY_CHANGE, NULL, 0, NULL, 0, &retLen, NULL) == SOCKET_ERROR) {
			int errorid = WSAGetLastError();
			gchar *msg = g_win32_error_message(errorid);
			gaim_debug_warning("network", "Unable to wait for changes. Message: %s (%d).\n",
				msg, errorid);
			g_free(msg);
		}

		retval = WSALookupServiceEnd(h);

		g_idle_add(wgaim_network_change_thread_cb, NULL);

	}

	g_thread_exit(NULL);
	return NULL;
}
#endif

gboolean
gaim_network_is_available(void)
{
#ifdef HAVE_LIBNM
	/* Try NetworkManager first, maybe we'll get lucky */
	int libnm_retval = -1;

	if (nm_context)
	{
		if ((libnm_retval = libnm_glib_get_network_state(nm_context)) == LIBNM_NO_NETWORK_CONNECTION)
		{
			gaim_debug_warning("network", "NetworkManager not active or reports no connection (retval = %i)\n", libnm_retval);
			return FALSE;
		}
		if (libnm_retval == LIBNM_ACTIVE_NETWORK_CONNECTION)	return TRUE;
	}
#elif defined _WIN32
	return (current_network_count > 0);
#endif
	return TRUE;
}

#ifdef HAVE_LIBNM
void
nm_callback_func(libnm_glib_ctx* ctx, gpointer user_data)
{
	GList *l;
	GaimAccount *account;
	static libnm_glib_state prev = LIBNM_NO_DBUS;
	libnm_glib_state current;
	GaimConnectionUiOps *ui_ops = gaim_connections_get_ui_ops();

	current = libnm_glib_get_network_state(ctx);
	gaim_debug_info("network","Entering nm_callback_func!\n");

	switch(current)
	{
	case LIBNM_ACTIVE_NETWORK_CONNECTION:
		/* Call res_init in case DNS servers have changed */
		res_init();
		if (ui_ops != NULL && ui_ops->network_connected != NULL)
			ui_ops->network_connected();
		prev = current;
		break;
	case LIBNM_NO_NETWORK_CONNECTION:
		if (prev != LIBNM_ACTIVE_NETWORK_CONNECTION)
			break;
		if (ui_ops != NULL && ui_ops->network_disconnected != NULL)
			ui_ops->network_disconnected();
		prev = current;
		break;
	case LIBNM_NO_DBUS:
	case LIBNM_NO_NETWORKMANAGER:
	case LIBNM_INVALID_CONTEXT:
	default:
		break;
	}
}
#endif

void
gaim_network_init(void)
{
#ifdef _WIN32
	GError *err = NULL;
	gint cnt = wgaim_get_connected_network_count();

	if (cnt < 0) /* Assume there is a network */
		current_network_count = 1;
	/* Don't listen for network changes if we can't tell anyway */
	else
	{
		current_network_count = cnt;
		if (!g_thread_create(wgaim_network_change_thread, NULL, FALSE, &err))
			gaim_debug_error("network", "Couldn't create Network Monitor thread: %s\n", err ? err->message : "");
	}
#endif

	gaim_prefs_add_none  ("/core/network");
	gaim_prefs_add_bool  ("/core/network/auto_ip", TRUE);
	gaim_prefs_add_string("/core/network/public_ip", "");
	gaim_prefs_add_bool  ("/core/network/ports_range_use", FALSE);
	gaim_prefs_add_int   ("/core/network/ports_range_start", 1024);
	gaim_prefs_add_int   ("/core/network/ports_range_end", 2048);

	gaim_upnp_discover(NULL, NULL);

#ifdef HAVE_LIBNM
	nm_context = libnm_glib_init();
	if(nm_context)
		nm_callback_idx = libnm_glib_register_callback(nm_context, nm_callback_func, NULL, g_main_context_default());
#endif
}

void
gaim_network_uninit(void)
{
#ifdef HAVE_LIBNM
	/* FIXME: If anyone can think of a more clever way to shut down libnm without
	 * using a global variable + this function, please do. */
	if(nm_context && nm_callback_idx)
		libnm_glib_unregister_callback(nm_context, nm_callback_idx);

	if(nm_context)
		libnm_glib_shutdown(nm_context);
#endif
}
