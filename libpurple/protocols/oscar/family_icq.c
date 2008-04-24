/*
 * Purple's oscar protocol plugin
 * This file is the legal property of its developers.
 * Please see the AUTHORS file distributed alongside this file.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
*/

/*
 * Family 0x0015 - Encapsulated ICQ.
 *
 */

#include "oscar.h"

#ifdef OLDSTYLE_ICQ_OFFLINEMSGS
int aim_icq_reqofflinemsgs(OscarData *od)
{
	FlapConnection *conn;
	ByteStream bs;
	aim_snacid_t snacid;
	int bslen;

	if (!od || !(conn = flap_connection_findbygroup(od, 0x0015)))
		return -EINVAL;

	bslen = 2 + 4 + 2 + 2;

	byte_stream_new(&bs, 4 + bslen);

	snacid = aim_cachesnac(od, 0x0015, 0x0002, 0x0000, NULL, 0);

	/* For simplicity, don't bother using a tlvlist */
	byte_stream_put16(&bs, 0x0001);
	byte_stream_put16(&bs, bslen);

	byte_stream_putle16(&bs, bslen - 2);
	byte_stream_putle32(&bs, atoi(od->sn));
	byte_stream_putle16(&bs, 0x003c); /* I command thee. */
	byte_stream_putle16(&bs, snacid); /* eh. */

	flap_connection_send_snac(od, conn, 0x0015, 0x0002, 0x0000, snacid, &bs);

	byte_stream_destroy(&bs);

	return 0;
}

int aim_icq_ackofflinemsgs(OscarData *od)
{
	ByteStream bs;
	FlapFrame *frame;
	aim_snacid_t snacid;
	int bslen;

	if (!od || !(conn = flap_connection_findbygroup(od, 0x0015)))
		return -EINVAL;

	bslen = 2 + 4 + 2 + 2;

	byte_stream_new(&bs, 4 + bslen);

	snacid = aim_cachesnac(od, 0x0015, 0x0002, 0x0000, NULL, 0);

	/* For simplicity, don't bother using a tlvlist */
	byte_stream_put16(&bs, 0x0001);
	byte_stream_put16(&bs, bslen);

	byte_stream_putle16(&bs, bslen - 2);
	byte_stream_putle32(&bs, atoi(od->sn));
	byte_stream_putle16(&bs, 0x003e); /* I command thee. */
	byte_stream_putle16(&bs, snacid); /* eh. */

	flap_connection_send_snac(od, conn, 0x0015, 0x0002, 0x0000, snacid, &bs);

	byte_stream_destroy(&bs);

	return 0;
}
#endif /* OLDSTYLE_ICQ_OFFLINEMSGS */

int
aim_icq_setsecurity(OscarData *od, gboolean auth_required, gboolean webaware)
{
	FlapConnection *conn;
	ByteStream bs;
	aim_snacid_t snacid;
	int bslen;

	if (!od || !(conn = flap_connection_findbygroup(od, 0x0015)))
		return -EINVAL;

	bslen = 2+4+2+2+2+2+2+1+1+1+1+1+1;

	byte_stream_new(&bs, 4 + bslen);

	snacid = aim_cachesnac(od, 0x0015, 0x0002, 0x0000, NULL, 0);

	/* For simplicity, don't bother using a tlvlist */
	byte_stream_put16(&bs, 0x0001);
	byte_stream_put16(&bs, bslen);

	byte_stream_putle16(&bs, bslen - 2);
	byte_stream_putle32(&bs, atoi(od->sn));
	byte_stream_putle16(&bs, 0x07d0); /* I command thee. */
	byte_stream_putle16(&bs, snacid); /* eh. */
	byte_stream_putle16(&bs, 0x0c3a); /* shrug. */
	byte_stream_putle16(&bs, 0x030c);
	byte_stream_putle16(&bs, 0x0001);
	byte_stream_putle8(&bs, webaware);
	byte_stream_putle8(&bs, 0xf8);
	byte_stream_putle8(&bs, 0x02);
	byte_stream_putle8(&bs, 0x01);
	byte_stream_putle8(&bs, 0x00);
	byte_stream_putle8(&bs, !auth_required);

	flap_connection_send_snac(od, conn, 0x0015, 0x0002, 0x0000, snacid, &bs);

	byte_stream_destroy(&bs);

	return 0;
}

/**
 * Change your ICQ password.
 *
 * @param od The oscar session
 * @param passwd The new password.  If this is longer than 8 characters it
 *        will be truncated.
 * @return Return 0 if no errors, otherwise return the error number.
 */
int aim_icq_changepasswd(OscarData *od, const char *passwd)
{
	FlapConnection *conn;
	ByteStream bs;
	aim_snacid_t snacid;
	int bslen, passwdlen;

	if (!passwd)
		return -EINVAL;

	if (!od || !(conn = flap_connection_findbygroup(od, 0x0015)))
		return -EINVAL;

	passwdlen = strlen(passwd);
	if (passwdlen > MAXICQPASSLEN)
		passwdlen = MAXICQPASSLEN;
	bslen = 2+4+2+2+2+2+passwdlen+1;

	byte_stream_new(&bs, 4 + bslen);

	snacid = aim_cachesnac(od, 0x0015, 0x0002, 0x0000, NULL, 0);

	/* For simplicity, don't bother using a tlvlist */
	byte_stream_put16(&bs, 0x0001);
	byte_stream_put16(&bs, bslen);

	byte_stream_putle16(&bs, bslen - 2);
	byte_stream_putle32(&bs, atoi(od->sn));
	byte_stream_putle16(&bs, 0x07d0); /* I command thee. */
	byte_stream_putle16(&bs, snacid); /* eh. */
	byte_stream_putle16(&bs, 0x042e); /* shrug. */
	byte_stream_putle16(&bs, passwdlen+1);
	byte_stream_putstr(&bs, passwd);
	byte_stream_putle8(&bs, '\0');

	flap_connection_send_snac(od, conn, 0x0015, 0x0002, 0x0000, snacid, &bs);

	byte_stream_destroy(&bs);

	return 0;
}

int aim_icq_getallinfo(OscarData *od, const char *uin)
{
	FlapConnection *conn;
	ByteStream bs;
	aim_snacid_t snacid;
	int bslen;
	struct aim_icq_info *info;

	if (!uin || uin[0] < '0' || uin[0] > '9')
		return -EINVAL;

	if (!od || !(conn = flap_connection_findbygroup(od, 0x0015)))
		return -EINVAL;

	bslen = 2 + 4 + 2 + 2 + 2 + 4;

	byte_stream_new(&bs, 4 + bslen);

	snacid = aim_cachesnac(od, 0x0015, 0x0002, 0x0000, NULL, 0);

	/* For simplicity, don't bother using a tlvlist */
	byte_stream_put16(&bs, 0x0001);
	byte_stream_put16(&bs, bslen);

	byte_stream_putle16(&bs, bslen - 2);
	byte_stream_putle32(&bs, atoi(od->sn));
	byte_stream_putle16(&bs, 0x07d0); /* I command thee. */
	byte_stream_putle16(&bs, snacid); /* eh. */
	byte_stream_putle16(&bs, 0x04b2); /* shrug. */
	byte_stream_putle32(&bs, atoi(uin));

	flap_connection_send_snac(od, conn, 0x0015, 0x0002, 0x0000, snacid, &bs);

	byte_stream_destroy(&bs);

	/* Keep track of this request and the ICQ number and request ID */
	info = (struct aim_icq_info *)g_new0(struct aim_icq_info, 1);
	info->reqid = snacid;
	info->uin = atoi(uin);
	info->next = od->icq_info;
	od->icq_info = info;

	return 0;
}

int aim_icq_getalias(OscarData *od, const char *uin)
{
	FlapConnection *conn;
	ByteStream bs;
	aim_snacid_t snacid;
	int bslen;
	struct aim_icq_info *info;

	if (!uin || uin[0] < '0' || uin[0] > '9')
		return -EINVAL;

	if (!od || !(conn = flap_connection_findbygroup(od, 0x0015)))
		return -EINVAL;

	bslen = 2 + 4 + 2 + 2 + 2 + 4;

	byte_stream_new(&bs, 4 + bslen);

	snacid = aim_cachesnac(od, 0x0015, 0x0002, 0x0000, NULL, 0);

	/* For simplicity, don't bother using a tlvlist */
	byte_stream_put16(&bs, 0x0001);
	byte_stream_put16(&bs, bslen);

	byte_stream_putle16(&bs, bslen - 2);
	byte_stream_putle32(&bs, atoi(od->sn));
	byte_stream_putle16(&bs, 0x07d0); /* I command thee. */
	byte_stream_putle16(&bs, snacid); /* eh. */
	byte_stream_putle16(&bs, 0x04ba); /* shrug. */
	byte_stream_putle32(&bs, atoi(uin));

	flap_connection_send_snac(od, conn, 0x0015, 0x0002, 0x0000, snacid, &bs);
	
	byte_stream_destroy(&bs);
	
	/* Keep track of this request and the ICQ number and request ID */
	info = (struct aim_icq_info *)g_new0(struct aim_icq_info, 1);
	info->reqid = snacid;
	info->uin = atoi(uin);
	info->next = od->icq_info;
	od->icq_info = info;

	return 0;
}

int aim_icq_getsimpleinfo(OscarData *od, const char *uin)
{
	FlapConnection *conn;
	ByteStream bs;
	aim_snacid_t snacid;
	int bslen;

	if (!uin || uin[0] < '0' || uin[0] > '9')
		return -EINVAL;

	if (!od || !(conn = flap_connection_findbygroup(od, 0x0015)))
		return -EINVAL;

	bslen = 2 + 4 + 2 + 2 + 2 + 4;

	byte_stream_new(&bs, 4 + bslen);

	snacid = aim_cachesnac(od, 0x0015, 0x0002, 0x0000, NULL, 0);

	/* For simplicity, don't bother using a tlvlist */
	byte_stream_put16(&bs, 0x0001);
	byte_stream_put16(&bs, bslen);

	byte_stream_putle16(&bs, bslen - 2);
	byte_stream_putle32(&bs, atoi(od->sn));
	byte_stream_putle16(&bs, 0x07d0); /* I command thee. */
	byte_stream_putle16(&bs, snacid); /* eh. */
	byte_stream_putle16(&bs, 0x051f); /* shrug. */
	byte_stream_putle32(&bs, atoi(uin));

	flap_connection_send_snac(od, conn, 0x0015, 0x0002, 0x0000, snacid, &bs);

	byte_stream_destroy(&bs);

	return 0;
}

#if 0
int aim_icq_sendxmlreq(OscarData *od, const char *xml)
{
	FlapConnection *conn;
	ByteStream bs;
	aim_snacid_t snacid;
	int bslen;

	if (!xml || !strlen(xml))
		return -EINVAL;

	if (!od || !(conn = flap_connection_findbygroup(od, 0x0015)))
		return -EINVAL;

	bslen = 2 + 10 + 2 + strlen(xml) + 1;

	byte_stream_new(&bs, 4 + bslen);

	snacid = aim_cachesnac(od, 0x0015, 0x0002, 0x0000, NULL, 0);

	/* For simplicity, don't bother using a tlvlist */
	byte_stream_put16(&bs, 0x0001);
	byte_stream_put16(&bs, bslen);

	byte_stream_putle16(&bs, bslen - 2);
	byte_stream_putle32(&bs, atoi(od->sn));
	byte_stream_putle16(&bs, 0x07d0); /* I command thee. */
	byte_stream_putle16(&bs, snacid); /* eh. */
	byte_stream_putle16(&bs, 0x0998); /* shrug. */
	byte_stream_putle16(&bs, strlen(xml) + 1);
	byte_stream_putraw(&bs, (guint8 *)xml, strlen(xml) + 1);

	flap_connection_send_snac(od, conn, 0x0015, 0x0002, 0x0000, snacid, &bs);
	
	byte_stream_destroy(&bs);
	
	return 0;
}
#endif

/*
 * Send an SMS message.  This is the non-US way.  The US-way is to IM
 * their cell phone number (+19195551234).
 *
 * We basically construct and send an XML message.  The format is:
 * <icq_sms_message>
 *   <destination>full_phone_without_leading_+</destination>
 *   <text>message</text>
 *   <codepage>1252</codepage>
 *   <senders_UIN>self_uin</senders_UIN>
 *   <senders_name>self_name</senders_name>
 *   <delivery_receipt>Yes|No</delivery_receipt>
 *   <time>Wkd, DD Mmm YYYY HH:MM:SS TMZ</time>
 * </icq_sms_message>
 *
 * Yeah hi Peter, whaaaat's happening.  If there's any way to use
 * a codepage other than 1252 that would be great.  Thaaaanks.
 */
int aim_icq_sendsms(OscarData *od, const char *name, const char *msg, const char *alias)
{
	FlapConnection *conn;
	ByteStream bs;
	aim_snacid_t snacid;
	int bslen, xmllen;
	char *xml;
	const char *timestr;
	time_t t;
	struct tm *tm;
	gchar *stripped;

	if (!od || !(conn = flap_connection_findbygroup(od, 0x0015)))
		return -EINVAL;

	if (!name || !msg || !alias)
		return -EINVAL;

	time(&t);
	tm = gmtime(&t);
	timestr = purple_utf8_strftime("%a, %d %b %Y %T %Z", tm);

	stripped = purple_markup_strip_html(msg);

	/* The length of xml included the null terminating character */
	xmllen = 209 + strlen(name) + strlen(stripped) + strlen(od->sn) + strlen(alias) + strlen(timestr) + 1;

	xml = g_new(char, xmllen);
	snprintf(xml, xmllen, "<icq_sms_message>"
		"<destination>%s</destination>"
		"<text>%s</text>"
		"<codepage>1252</codepage>"
		"<senders_UIN>%s</senders_UIN>"
		"<senders_name>%s</senders_name>"
		"<delivery_receipt>Yes</delivery_receipt>"
		"<time>%s</time>"
		"</icq_sms_message>",
		name, stripped, od->sn, alias, timestr);

	bslen = 36 + xmllen;

	byte_stream_new(&bs, 4 + bslen);

	snacid = aim_cachesnac(od, 0x0015, 0x0002, 0x0000, NULL, 0);

	/* For simplicity, don't bother using a tlvlist */
	byte_stream_put16(&bs, 0x0001);
	byte_stream_put16(&bs, bslen);

	byte_stream_putle16(&bs, bslen - 2);
	byte_stream_putle32(&bs, atoi(od->sn));
	byte_stream_putle16(&bs, 0x07d0); /* I command thee. */
	byte_stream_putle16(&bs, snacid); /* eh. */

	/* From libicq200-0.3.2/src/SNAC-SRV.cpp */
	byte_stream_putle16(&bs, 0x1482);
	byte_stream_put16(&bs, 0x0001);
	byte_stream_put16(&bs, 0x0016);
	byte_stream_put32(&bs, 0x00000000);
	byte_stream_put32(&bs, 0x00000000);
	byte_stream_put32(&bs, 0x00000000);
	byte_stream_put32(&bs, 0x00000000);

	byte_stream_put16(&bs, 0x0000);
	byte_stream_put16(&bs, xmllen);
	byte_stream_putstr(&bs, xml);
	byte_stream_put8(&bs, 0x00);

	flap_connection_send_snac(od, conn, 0x0015, 0x0002, 0x0000, snacid, &bs);

	byte_stream_destroy(&bs);

	g_free(xml);
	g_free(stripped);

	return 0;
}

/*
 * getstatusnote may be a misleading name because the response
 * contains a lot of different information but currently it's only
 * used to get that.
 */
int aim_icq_getstatusnote(OscarData *od, const char *uin, guint8 *note_hash, guint16 note_hash_len)
{
	FlapConnection *conn;
	ByteStream bs;
	aim_snacid_t snacid;
	int bslen;

	purple_debug_misc("oscar", "aim_icq_getstatusnote: requesting status note for %s.\n", uin);

	if (!od || !(conn = flap_connection_findbygroup(od, 0x0015)))
	{
		purple_debug_misc("oscar", "aim_icq_getstatusnote: no connection.\n");
		return -EINVAL;
	}

	bslen = 2 + 4 + 2 + 2 + 2 + 2 + 58 + strlen(uin);
	byte_stream_new(&bs, 4 + bslen);

	snacid = aim_cachesnac(od, 0x0015, 0x0002, 0x0000, NULL, 0);

	byte_stream_put16(&bs, 0x0001);
	byte_stream_put16(&bs, bslen);

	byte_stream_putle16(&bs, bslen - 2);
	byte_stream_putle32(&bs, atoi(od->sn));
	byte_stream_putle16(&bs, 0x07d0); /* I command thee. */
	byte_stream_putle16(&bs, snacid); /* eh. */
	byte_stream_putle16(&bs, 0x0fa0); /* shrug. */
	byte_stream_putle16(&bs, 58 + strlen(uin));

	byte_stream_put32(&bs, 0x05b90002);    /* don't ask */
	byte_stream_put32(&bs, 0x80000000);
	byte_stream_put32(&bs, 0x00000006);
	byte_stream_put32(&bs, 0x00010002);
	byte_stream_put32(&bs, 0x00020000);
	byte_stream_put32(&bs, 0x04e30000);
	byte_stream_put32(&bs, 0x00020002);
	byte_stream_put32(&bs, 0x00000001);

	byte_stream_put16(&bs, 24 + strlen(uin));
	byte_stream_put32(&bs, 0x003c0010);
	byte_stream_putraw(&bs, note_hash, 16); /* status note hash */
	byte_stream_put16(&bs, 0x0032);        /* buddy uin */
	byte_stream_put16(&bs, strlen(uin));
	byte_stream_putstr(&bs, uin);

	flap_connection_send_snac(od, conn, 0x0015, 0x0002, 0x000, snacid, &bs);

	byte_stream_destroy(&bs);

	return 0;
}

static void aim_icq_freeinfo(struct aim_icq_info *info) {
	int i;

	if (!info)
		return;
	g_free(info->nick);
	g_free(info->first);
	g_free(info->last);
	g_free(info->email);
	g_free(info->homecity);
	g_free(info->homestate);
	g_free(info->homephone);
	g_free(info->homefax);
	g_free(info->homeaddr);
	g_free(info->mobile);
	g_free(info->homezip);
	g_free(info->personalwebpage);
	if (info->email2)
		for (i = 0; i < info->numaddresses; i++)
			g_free(info->email2[i]);
	g_free(info->email2);
	g_free(info->workcity);
	g_free(info->workstate);
	g_free(info->workphone);
	g_free(info->workfax);
	g_free(info->workaddr);
	g_free(info->workzip);
	g_free(info->workcompany);
	g_free(info->workdivision);
	g_free(info->workposition);
	g_free(info->workwebpage);
	g_free(info->info);
	g_free(info->status_note_title);
	g_free(info);
}

/**
 * Subtype 0x0003 - Response to 0x0015/0x002, contains an ICQesque packet.
 */
static int
icqresponse(OscarData *od, FlapConnection *conn, aim_module_t *mod, FlapFrame *frame, aim_modsnac_t *snac, ByteStream *bs)
{
	int ret = 0;
	GSList *tlvlist;
	aim_tlv_t *datatlv;
	ByteStream qbs;
	guint32 ouruin;
	guint16 cmdlen, cmd, reqid;

	if (!(tlvlist = aim_tlvlist_read(bs)) || !(datatlv = aim_tlv_gettlv(tlvlist, 0x0001, 1))) {
		aim_tlvlist_free(tlvlist);
		purple_debug_misc("oscar", "corrupt ICQ response\n");
		return 0;
	}

	byte_stream_init(&qbs, datatlv->value, datatlv->length);

	cmdlen = byte_stream_getle16(&qbs);
	ouruin = byte_stream_getle32(&qbs);
	cmd = byte_stream_getle16(&qbs);
	reqid = byte_stream_getle16(&qbs);

	purple_debug_misc("oscar", "icq response: %d bytes, %u, 0x%04x, 0x%04x\n", cmdlen, ouruin, cmd, reqid);

	if (cmd == 0x0041) { /* offline message */
#ifdef OLDSTYLE_ICQ_OFFLINEMSGS
		struct aim_icq_offlinemsg msg;
		aim_rxcallback_t userfunc;

		memset(&msg, 0, sizeof(msg));

		msg.sender = byte_stream_getle32(&qbs);
		msg.year = byte_stream_getle16(&qbs);
		msg.month = byte_stream_getle8(&qbs);
		msg.day = byte_stream_getle8(&qbs);
		msg.hour = byte_stream_getle8(&qbs);
		msg.minute = byte_stream_getle8(&qbs);
		msg.type = byte_stream_getle8(&qbs);
		msg.flags = byte_stream_getle8(&qbs);
		msg.msglen = byte_stream_getle16(&qbs);
		msg.msg = byte_stream_getstr(&qbs, msg.msglen);

		if ((userfunc = aim_callhandler(od, SNAC_FAMILY_ICQ, SNAC_SUBTYPE_ICQ_OFFLINEMSG)))
			ret = userfunc(od, conn, frame, &msg);

		g_free(msg.msg);

	} else if (cmd == 0x0042) {
		aim_rxcallback_t userfunc;

		if ((userfunc = aim_callhandler(od, SNAC_FAMILY_ICQ, SNAC_SUBTYPE_ICQ_OFFLINEMSGCOMPLETE)))
			ret = userfunc(od, conn, frame);
#endif /* OLDSTYLE_ICQ_OFFLINEMSGS */

	} else if (cmd == 0x07da) { /* information */
		guint16 subtype;
		struct aim_icq_info *info;
		aim_rxcallback_t userfunc;

		subtype = byte_stream_getle16(&qbs);
		byte_stream_advance(&qbs, 1); /* 0x0a */

		/* find other data from the same request */
		for (info = od->icq_info; info && (info->reqid != reqid); info = info->next);
		if (!info) {
			info = (struct aim_icq_info *)g_new0(struct aim_icq_info, 1);
			info->reqid = reqid;
			info->next = od->icq_info;
			od->icq_info = info;
		}

		switch (subtype) {
		case 0x00a0: { /* hide ip status */
			/* nothing */
		} break;

		case 0x00aa: { /* password change status */
			/* nothing */
		} break;

		case 0x00c8: { /* general and "home" information */
			info->nick = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->first = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->last = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->email = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->homecity = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->homestate = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->homephone = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->homefax = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->homeaddr = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->mobile = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->homezip = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->homecountry = byte_stream_getle16(&qbs);
			/* 0x0a 00 02 00 */
			/* 1 byte timezone? */
			/* 1 byte hide email flag? */
		} break;

		case 0x00dc: { /* personal information */
			info->age = byte_stream_getle8(&qbs);
			info->unknown = byte_stream_getle8(&qbs);
			info->gender = byte_stream_getle8(&qbs); /* Not specified=0x00, Female=0x01, Male=0x02 */
			info->personalwebpage = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->birthyear = byte_stream_getle16(&qbs);
			info->birthmonth = byte_stream_getle8(&qbs);
			info->birthday = byte_stream_getle8(&qbs);
			info->language1 = byte_stream_getle8(&qbs);
			info->language2 = byte_stream_getle8(&qbs);
			info->language3 = byte_stream_getle8(&qbs);
			/* 0x00 00 01 00 00 01 00 00 00 00 00 */
		} break;

		case 0x00d2: { /* work information */
			info->workcity = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->workstate = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->workphone = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->workfax = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->workaddr = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->workzip = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->workcountry = byte_stream_getle16(&qbs);
			info->workcompany = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->workdivision = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->workposition = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			byte_stream_advance(&qbs, 2); /* 0x01 00 */
			info->workwebpage = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
		} break;

		case 0x00e6: { /* additional personal information */
			info->info = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs)-1);
		} break;

		case 0x00eb: { /* email address(es) */
			int i;
			info->numaddresses = byte_stream_getle16(&qbs);
			info->email2 = (char **)g_new0(char *, info->numaddresses);
			for (i = 0; i < info->numaddresses; i++) {
				info->email2[i] = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
				if (i+1 != info->numaddresses)
					byte_stream_advance(&qbs, 1); /* 0x00 */
			}
		} break;

		case 0x00f0: { /* personal interests */
		} break;

		case 0x00fa: { /* past background and current organizations */
		} break;

		case 0x0104: { /* alias info */
			info->nick = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->first = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->last = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			byte_stream_advance(&qbs, byte_stream_getle16(&qbs)); /* email address? */
			/* Then 0x00 02 00 */
		} break;

		case 0x010e: { /* unknown */
			/* 0x00 00 */
		} break;

		case 0x019a: { /* simple info */
			byte_stream_advance(&qbs, 2);
			info->uin = byte_stream_getle32(&qbs);
			info->nick = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->first = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->last = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			info->email = byte_stream_getstr(&qbs, byte_stream_getle16(&qbs));
			/* Then 0x00 02 00 00 00 00 00 */
		} break;

		/* status note title and send request for status note text */
		case 0x0fb4: {
			GSList *tlvlist;
			aim_tlv_t *tlv;
			FlapConnection *conn;
			char *uin = NULL;
			char *status_note_title = NULL;

			conn = flap_connection_findbygroup(od, 0x0004);
			if (conn == NULL)
			{
				purple_debug_misc("oscar", "icq/0x0fb4: flap connection was not found.\n");
				break;
			}

			byte_stream_advance(&qbs, 0x02); /* length */
			byte_stream_advance(&qbs, 0x2f); /* unknown stuff */

			tlvlist = aim_tlvlist_read(&qbs);

			tlv = aim_tlv_gettlv(tlvlist, 0x0032, 1);
			if (tlv != NULL)
				/* Get user number */
				uin = aim_tlv_getvalue_as_string(tlv);

			tlv = aim_tlv_gettlv(tlvlist, 0x0226, 1);
			if (tlv != NULL)
				/* Get status note title */
				status_note_title = aim_tlv_getvalue_as_string(tlv);

			aim_tlvlist_free(tlvlist);

			if (uin == NULL || status_note_title == NULL)
			{
				purple_debug_misc("oscar", "icq/0x0fb4: uin or "
						"status_note_title was not found\n");
				g_free(uin);
				g_free(status_note_title);
				break;
			}

			if (status_note_title[0] == '\0')
			{
				PurpleAccount *account;
				PurpleBuddy *buddy;
				PurplePresence *presence;
				PurpleStatus *status;

				account = purple_connection_get_account(od->gc);
				buddy = purple_find_buddy(account, uin);
				presence = purple_buddy_get_presence(buddy);
				status = purple_presence_get_active_status(presence);

				purple_prpl_got_user_status(account, uin,
						purple_status_get_id(status),
						"message", NULL, NULL);

				g_free(status_note_title);
			}
			else
			{
				struct aim_icq_info *info;
				ByteStream bs;
				guint32 bslen;
				aim_snacid_t snacid;
				guchar cookie[8];

				info = g_new0(struct aim_icq_info, 1);

				if (info == NULL)
				{
					g_free(uin);
					g_free(status_note_title);

					break;
				}

				bslen = 13 + strlen(uin) + 30 + 6 + 4 + 55 + 85 + 4;
				byte_stream_new(&bs, 4 + bslen);

				snacid = aim_cachesnac(od, 0x0004, 0x0006, 0x0000, NULL, 0);

				aim_icbm_makecookie(cookie);

				byte_stream_putraw(&bs, cookie, 8); /* ICBM cookie */
				byte_stream_put16(&bs, 0x0002); /* message channel */
				byte_stream_put8(&bs, strlen(uin)); /* uin */
				byte_stream_putstr(&bs, uin);

				byte_stream_put16(&bs, 0x0005); /* rendez vous data */
				byte_stream_put16(&bs, 0x00b2);
				byte_stream_put16(&bs, 0x0000); /* request */
				byte_stream_putraw(&bs, cookie, 8); /* ICBM cookie */
				byte_stream_put32(&bs, 0x09461349); /* ICQ server relaying */
				byte_stream_put16(&bs, 0x4c7f);
				byte_stream_put16(&bs, 0x11d1);
				byte_stream_put32(&bs, 0x82224445);
				byte_stream_put32(&bs, 0x53540000);

				byte_stream_put16(&bs, 0x000a); /* unknown TLV */
				byte_stream_put16(&bs, 0x0002);
				byte_stream_put16(&bs, 0x0001);

				byte_stream_put16(&bs, 0x000f); /* unknown TLV */
				byte_stream_put16(&bs, 0x0000);

				byte_stream_put16(&bs, 0x2711); /* extended data */
				byte_stream_put16(&bs, 0x008a);
				byte_stream_putle16(&bs, 0x001b); /* length */
				byte_stream_putle16(&bs, 0x0009); /* version */
				byte_stream_putle32(&bs, 0x00000000); /* plugin: none */
				byte_stream_putle32(&bs, 0x00000000);
				byte_stream_putle32(&bs, 0x00000000);
				byte_stream_putle32(&bs, 0x00000000);
				byte_stream_putle16(&bs, 0x0000); /* unknown */
				byte_stream_putle32(&bs, 0x00000000); /* client capabilities flags */
				byte_stream_put8(&bs, 0x00); /* unknown */
				byte_stream_putle16(&bs, 0x0064); /* downcounter? */
				byte_stream_putle16(&bs, 0x000e); /* length */
				byte_stream_putle16(&bs, 0x0064); /* downcounter? */
				byte_stream_putle32(&bs, 0x00000000); /* unknown */
				byte_stream_putle32(&bs, 0x00000000);
				byte_stream_putle32(&bs, 0x00000000);
				byte_stream_put8(&bs, 0x1a); /* message type: plugin message descibed by text string */
				byte_stream_put8(&bs, 0x00); /* message flags */
				byte_stream_putle16(&bs, 0x0000); /* status code */
				byte_stream_putle16(&bs, 0x0001); /* priority code */
				byte_stream_putle16(&bs, 0x0000); /* text length */

				byte_stream_put8(&bs, 0x3a); /* message dump */
				byte_stream_put32(&bs, 0x00811a18);
				byte_stream_put32(&bs, 0xbc0e6c18);
				byte_stream_put32(&bs, 0x47a5916f);
				byte_stream_put32(&bs, 0x18dcc76f);
				byte_stream_put32(&bs, 0x1a010013);
				byte_stream_put32(&bs, 0x00000041);
				byte_stream_put32(&bs, 0x77617920);
				byte_stream_put32(&bs, 0x53746174);
				byte_stream_put32(&bs, 0x7573204d);
				byte_stream_put32(&bs, 0x65737361);
				byte_stream_put32(&bs, 0x67650100);
				byte_stream_put32(&bs, 0x00000000);
				byte_stream_put32(&bs, 0x00000000);
				byte_stream_put32(&bs, 0x00000000);
				byte_stream_put32(&bs, 0x00000015);
				byte_stream_put32(&bs, 0x00000000);
				byte_stream_put32(&bs, 0x0000000d);
				byte_stream_put32(&bs, 0x00000074);
				byte_stream_put32(&bs, 0x6578742f);
				byte_stream_put32(&bs, 0x782d616f);
				byte_stream_put32(&bs, 0x6c727466);

				byte_stream_put16(&bs, 0x0003); /* server ACK requested */
				byte_stream_put16(&bs, 0x0000);

				info->uin = atoi(uin);
				info->status_note_title = status_note_title;

				memcpy(&info->icbm_cookie, cookie, 8);

				info->next = od->icq_info;
				od->icq_info = info;

				flap_connection_send_snac(od, conn, 0x0004, 0x0006, 0x0000, snacid, &bs);

				byte_stream_destroy(&bs);
			}

			g_free(uin);

		} break;

		} /* End switch statement */

		if (!(snac->flags & 0x0001)) {
			if (subtype != 0x0104)
				if ((userfunc = aim_callhandler(od, SNAC_FAMILY_ICQ, SNAC_SUBTYPE_ICQ_INFO)))
					ret = userfunc(od, conn, frame, info);

			if (info->uin && info->nick)
				if ((userfunc = aim_callhandler(od, SNAC_FAMILY_ICQ, SNAC_SUBTYPE_ICQ_ALIAS)))
					ret = userfunc(od, conn, frame, info);

			if (od->icq_info == info) {
				od->icq_info = info->next;
			} else {
				struct aim_icq_info *cur;
				for (cur=od->icq_info; (cur->next && (cur->next!=info)); cur=cur->next);
				if (cur->next)
					cur->next = cur->next->next;
			}
			aim_icq_freeinfo(info);
		}
	}

	aim_tlvlist_free(tlvlist);

	return ret;
}

static int
snachandler(OscarData *od, FlapConnection *conn, aim_module_t *mod, FlapFrame *frame, aim_modsnac_t *snac, ByteStream *bs)
{
	if (snac->subtype == 0x0003)
		return icqresponse(od, conn, mod, frame, snac, bs);

	return 0;
}

static void
icq_shutdown(OscarData *od, aim_module_t *mod)
{
	struct aim_icq_info *del;

	while (od->icq_info) {
		del = od->icq_info;
		od->icq_info = od->icq_info->next;
		aim_icq_freeinfo(del);
	}

	return;
}

int
icq_modfirst(OscarData *od, aim_module_t *mod)
{
	mod->family = 0x0015;
	mod->version = 0x0001;
	mod->toolid = 0x0110;
	mod->toolversion = 0x047c;
	mod->flags = 0;
	strncpy(mod->name, "icq", sizeof(mod->name));
	mod->snachandler = snachandler;
	mod->shutdown = icq_shutdown;

	return 0;
}
