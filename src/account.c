/**
 * @file account.c Account API
 * @ingroup core
 *
 * gaim
 *
 * Copyright (C) 2003 Christian Hammond <chipx86@gnupdate.org>
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
#include "account.h"
#include "debug.h"
#include "prefs.h"
#include "prpl.h"
#include "signals.h"
#include "server.h"
#include "util.h"

typedef enum
{
	TAG_NONE = 0,
	TAG_PROTOCOL,
	TAG_NAME,
	TAG_PASSWORD,
	TAG_ALIAS,
	TAG_USERINFO,
	TAG_BUDDYICON,
	TAG_PUBLIC_IP,
	TAG_SETTING,
	TAG_TYPE,
	TAG_HOST,
	TAG_PORT

} AccountParserTag;

typedef struct
{
	GaimPrefType type;

	char *ui;

	union
	{
		int integer;
		char *string;
		gboolean bool;

	} value;

} GaimAccountSetting;

typedef struct
{
	AccountParserTag tag;

	GaimAccount *account;
	GaimProtocol protocol;
	GaimProxyInfo *proxy_info;
	char *protocol_id;

	GString *buffer;

	GaimPrefType setting_type;
	char *setting_ui;
	char *setting_name;

	gboolean in_proxy;

} AccountParserData;

static GList   *accounts = NULL;
static guint    accounts_save_timer = 0;
static gboolean accounts_loaded = FALSE;

static void
delete_setting(void *data)
{
	GaimAccountSetting *setting = (GaimAccountSetting *)data;

	if (setting->ui != NULL)
		g_free(setting->ui);

	if (setting->type == GAIM_PREF_STRING)
		g_free(setting->value.string);

	g_free(setting);
}

static gboolean
accounts_save_cb(gpointer unused)
{
	gaim_accounts_sync();
	accounts_save_timer = 0;

	return FALSE;
}

static void
schedule_accounts_save()
{
	if (!accounts_save_timer)
		accounts_save_timer = g_timeout_add(5000, accounts_save_cb, NULL);
}

GaimAccount *
gaim_account_new(const char *username, const char *protocol_id)
{
	GaimAccount *account = NULL;

	g_return_val_if_fail(username    != NULL, NULL);

	if(protocol_id)
		account = gaim_accounts_find_with_prpl_id(username, protocol_id);

	if (account != NULL)
		return account;

	account = g_new0(GaimAccount, 1);

	gaim_account_set_username(account, username);

	if(protocol_id)
		gaim_account_set_protocol_id(account, protocol_id);
	else
		gaim_account_set_protocol(account, GAIM_PROTO_DEFAULT);

	account->settings = g_hash_table_new_full(g_str_hash, g_str_equal,
											  g_free, delete_setting);
	account->ui_settings = g_hash_table_new_full(g_str_hash, g_str_equal,
				g_free, (GDestroyNotify)g_hash_table_destroy);

	return account;
}

void
gaim_account_destroy(GaimAccount *account)
{
	g_return_if_fail(account != NULL);

	gaim_debug(GAIM_DEBUG_INFO, "account",
			   "Destroying account %p\n", account);

	if (account->gc != NULL)
		gaim_connection_destroy(account->gc);

	gaim_debug(GAIM_DEBUG_INFO, "account",
			   "Continuing to destroy account %p\n", account);

	if (account->username    != NULL) g_free(account->username);
	if (account->alias       != NULL) g_free(account->alias);
	if (account->password    != NULL) g_free(account->password);
	if (account->user_info   != NULL) g_free(account->user_info);
	if (account->protocol_id != NULL) g_free(account->protocol_id);

	g_hash_table_destroy(account->settings);
	g_hash_table_destroy(account->ui_settings);

	g_free(account);
}

GaimConnection *
gaim_account_connect(GaimAccount *account)
{
	GaimConnection *gc;

	g_return_val_if_fail(account != NULL, NULL);

	if (gaim_account_get_connection(account) != NULL)
		return NULL;

	gc = gaim_connection_new(account);

	gaim_debug(GAIM_DEBUG_INFO, "account",
			   "Connecting to account %p. gc = %p\n", account, gc);

	gaim_connection_connect(gc);

	return gc;
}

void
gaim_account_disconnect(GaimAccount *account)
{
	GaimConnection *gc;

	g_return_if_fail(account != NULL);
	g_return_if_fail(gaim_account_is_connected(account));

	gaim_debug(GAIM_DEBUG_INFO, "account",
			   "Disconnecting account %p\n", account);

	gc = gaim_account_get_connection(account);

	gaim_account_set_connection(account, NULL);

	gaim_connection_disconnect(gc);
}

void
gaim_account_set_username(GaimAccount *account, const char *username)
{
	g_return_if_fail(account != NULL);

	if (account->username != NULL)
		g_free(account->username);

	account->username = (username == NULL ? NULL : g_strdup(username));

	schedule_accounts_save();
}

void
gaim_account_set_password(GaimAccount *account, const char *password)
{
	g_return_if_fail(account != NULL);

	if (account->password != NULL)
		g_free(account->password);

	account->password = (password == NULL ? NULL : g_strdup(password));

	schedule_accounts_save();
}

void
gaim_account_set_alias(GaimAccount *account, const char *alias)
{
	g_return_if_fail(account != NULL);

	if (account->alias != NULL)
		g_free(account->alias);

	account->alias = (alias == NULL ? NULL : g_strdup(alias));

	schedule_accounts_save();
}

void
gaim_account_set_user_info(GaimAccount *account, const char *user_info)
{
	g_return_if_fail(account != NULL);

	if (account->user_info != NULL)
		g_free(account->user_info);

	account->user_info = (user_info == NULL ? NULL : g_strdup(user_info));

	schedule_accounts_save();
}

void
gaim_account_set_buddy_icon(GaimAccount *account, const char *icon)
{
	g_return_if_fail(account != NULL);

	if (account->buddy_icon != NULL)
		g_free(account->buddy_icon);

	account->buddy_icon = (icon == NULL ? NULL : g_strdup(icon));
	if (account->gc)
		serv_set_buddyicon(account->gc, icon);
	
	schedule_accounts_save();
}

void
gaim_account_set_protocol(GaimAccount *account, GaimProtocol protocol)
{
	g_return_if_fail(account != NULL);

	gaim_account_set_protocol_id(account, gaim_prpl_num_to_id(protocol));
}

void
gaim_account_set_protocol_id(GaimAccount *account, const char *protocol_id)
{
	g_return_if_fail(account     != NULL);
	g_return_if_fail(protocol_id != NULL);

	if (account->protocol_id != NULL)
		g_free(account->protocol_id);

	account->protocol_id = g_strdup(protocol_id);

	schedule_accounts_save();
}

void
gaim_account_set_connection(GaimAccount *account, GaimConnection *gc)
{
	g_return_if_fail(account != NULL);

	account->gc = gc;
}

void
gaim_account_set_remember_password(GaimAccount *account, gboolean value)
{
	g_return_if_fail(account != NULL);

	account->remember_pass = value;

	schedule_accounts_save();
}

void
gaim_account_set_check_mail(GaimAccount *account, gboolean value)
{
	g_return_if_fail(account != NULL);

	gaim_account_set_bool(account, "check-mail", value);
}

void
gaim_account_set_auto_login(GaimAccount *account, const char *ui,
							gboolean value)
{
	g_return_if_fail(account != NULL);
	g_return_if_fail(ui      != NULL);

	gaim_account_set_ui_bool(account, ui, "auto-login", value);
}

void
gaim_account_set_public_ip(GaimAccount *account, const char *ip)
{
	g_return_if_fail(account != NULL);

	if (account->ip != NULL)
		g_free(account->ip);

	account->ip = (ip == NULL ? NULL : g_strdup(ip));

	schedule_accounts_save();
}

void
gaim_account_set_proxy_info(GaimAccount *account, GaimProxyInfo *info)
{
	g_return_if_fail(account != NULL);

	if (account->proxy_info != NULL)
		gaim_proxy_info_destroy(account->proxy_info);

	account->proxy_info = info;

	schedule_accounts_save();
}

void
gaim_account_clear_settings(GaimAccount *account)
{
	g_return_if_fail(account != NULL);

	g_hash_table_destroy(account->settings);

	account->settings = g_hash_table_new_full(g_str_hash, g_str_equal,
											  g_free, delete_setting);
}

void
gaim_account_set_int(GaimAccount *account, const char *name, int value)
{
	GaimAccountSetting *setting;

	g_return_if_fail(account != NULL);
	g_return_if_fail(name    != NULL);

	setting = g_new0(GaimAccountSetting, 1);

	setting->type          = GAIM_PREF_INT;
	setting->value.integer = value;

	g_hash_table_insert(account->settings, g_strdup(name), setting);

	schedule_accounts_save();
}

void
gaim_account_set_string(GaimAccount *account, const char *name,
						const char *value)
{
	GaimAccountSetting *setting;

	g_return_if_fail(account != NULL);
	g_return_if_fail(name    != NULL);

	setting = g_new0(GaimAccountSetting, 1);

	setting->type         = GAIM_PREF_STRING;
	setting->value.string = g_strdup(value);

	g_hash_table_insert(account->settings, g_strdup(name), setting);

	schedule_accounts_save();
}

void
gaim_account_set_bool(GaimAccount *account, const char *name, gboolean value)
{
	GaimAccountSetting *setting;

	g_return_if_fail(account != NULL);
	g_return_if_fail(name    != NULL);

	gaim_debug(GAIM_DEBUG_INFO, "account", "Setting bool: %s, %d\n",
			   name, value);

	setting = g_new0(GaimAccountSetting, 1);

	setting->type       = GAIM_PREF_BOOLEAN;
	setting->value.bool = value;

	g_hash_table_insert(account->settings, g_strdup(name), setting);

	schedule_accounts_save();
}

static GHashTable *
get_ui_settings_table(GaimAccount *account, const char *ui)
{
	GHashTable *table;

	table = g_hash_table_lookup(account->ui_settings, ui);

	if (table == NULL) {
		table = g_hash_table_new_full(g_str_hash, g_str_equal, g_free,
									  delete_setting);
		g_hash_table_insert(account->ui_settings, g_strdup(ui), table);
	}

	return table;
}

void
gaim_account_set_ui_int(GaimAccount *account, const char *ui,
						const char *name, int value)
{
	GaimAccountSetting *setting;
	GHashTable *table;

	g_return_if_fail(account != NULL);
	g_return_if_fail(ui      != NULL);
	g_return_if_fail(name    != NULL);

	setting = g_new0(GaimAccountSetting, 1);

	setting->type          = GAIM_PREF_INT;
	setting->ui            = g_strdup(ui);
	setting->value.integer = value;

	table = get_ui_settings_table(account, ui);

	g_hash_table_insert(table, g_strdup(name), setting);

	schedule_accounts_save();
}

void
gaim_account_set_ui_string(GaimAccount *account, const char *ui,
						   const char *name, const char *value)
{
	GaimAccountSetting *setting;
	GHashTable *table;

	g_return_if_fail(account != NULL);
	g_return_if_fail(ui      != NULL);
	g_return_if_fail(name    != NULL);

	setting = g_new0(GaimAccountSetting, 1);

	setting->type         = GAIM_PREF_STRING;
	setting->ui           = g_strdup(ui);
	setting->value.string = g_strdup(value);

	table = get_ui_settings_table(account, ui);

	g_hash_table_insert(table, g_strdup(name), setting);

	schedule_accounts_save();
}

void
gaim_account_set_ui_bool(GaimAccount *account, const char *ui,
						 const char *name, gboolean value)
{
	GaimAccountSetting *setting;
	GHashTable *table;

	g_return_if_fail(account != NULL);
	g_return_if_fail(ui      != NULL);
	g_return_if_fail(name    != NULL);

	setting = g_new0(GaimAccountSetting, 1);

	setting->type       = GAIM_PREF_BOOLEAN;
	setting->ui         = g_strdup(ui);
	setting->value.bool = value;

	table = get_ui_settings_table(account, ui);

	g_hash_table_insert(table, g_strdup(name), setting);

	schedule_accounts_save();
}

gboolean
gaim_account_is_connected(const GaimAccount *account)
{
	g_return_val_if_fail(account != NULL, FALSE);

	return (account->gc != NULL &&
			gaim_connection_get_state(account->gc) != GAIM_DISCONNECTED);
}

const char *
gaim_account_get_username(const GaimAccount *account)
{
	g_return_val_if_fail(account != NULL, NULL);

	return account->username;
}

const char *
gaim_account_get_password(const GaimAccount *account)
{
	g_return_val_if_fail(account != NULL, NULL);

	return account->password;
}

const char *
gaim_account_get_alias(const GaimAccount *account)
{
	g_return_val_if_fail(account != NULL, NULL);

	return account->alias;
}

const char *
gaim_account_get_user_info(const GaimAccount *account)
{
	g_return_val_if_fail(account != NULL, NULL);

	return account->user_info;
}

const char *
gaim_account_get_buddy_icon(const GaimAccount *account)
{
	g_return_val_if_fail(account != NULL, NULL);

	return account->buddy_icon;
}

GaimProtocol
gaim_account_get_protocol(const GaimAccount *account)
{
	g_return_val_if_fail(account != NULL, -1);

	return gaim_prpl_id_to_num(gaim_account_get_protocol_id(account));
}

const char *
gaim_account_get_protocol_id(const GaimAccount *account)
{
	g_return_val_if_fail(account != NULL, NULL);

	return account->protocol_id;
}

GaimConnection *
gaim_account_get_connection(const GaimAccount *account)
{
	g_return_val_if_fail(account != NULL, NULL);

	return account->gc;
}

gboolean
gaim_account_get_remember_password(const GaimAccount *account)
{
	g_return_val_if_fail(account != NULL, FALSE);

	return account->remember_pass;
}

gboolean
gaim_account_get_check_mail(const GaimAccount *account)
{
	g_return_val_if_fail(account != NULL, FALSE);

	return gaim_account_get_bool(account, "check-mail", FALSE);
}

gboolean
gaim_account_get_auto_login(const GaimAccount *account, const char *ui)
{
	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(ui      != NULL, FALSE);

	return gaim_account_get_ui_bool(account, ui, "auto-login", FALSE);
}

const char *
gaim_account_get_public_ip(const GaimAccount *account)
{
	g_return_val_if_fail(account != NULL, NULL);

	return account->ip;
}

GaimProxyInfo *
gaim_account_get_proxy_info(const GaimAccount *account)
{
	g_return_val_if_fail(account != NULL, NULL);

	return account->proxy_info;
}

int
gaim_account_get_int(const GaimAccount *account, const char *name,
					 int default_value)
{
	GaimAccountSetting *setting;

	g_return_val_if_fail(account != NULL, default_value);
	g_return_val_if_fail(name    != NULL, default_value);

	setting = g_hash_table_lookup(account->settings, name);

	if (setting == NULL)
		return default_value;

	g_return_val_if_fail(setting->type == GAIM_PREF_INT, default_value);

	return setting->value.integer;
}

const char *
gaim_account_get_string(const GaimAccount *account, const char *name,
						const char *default_value)
{
	GaimAccountSetting *setting;

	g_return_val_if_fail(account != NULL, default_value);
	g_return_val_if_fail(name    != NULL, default_value);

	setting = g_hash_table_lookup(account->settings, name);

	if (setting == NULL)
		return default_value;

	g_return_val_if_fail(setting->type == GAIM_PREF_STRING, default_value);

	return setting->value.string;
}

gboolean
gaim_account_get_bool(const GaimAccount *account, const char *name,
					  gboolean default_value)
{
	GaimAccountSetting *setting;

	g_return_val_if_fail(account != NULL, default_value);
	g_return_val_if_fail(name    != NULL, default_value);

	gaim_debug(GAIM_DEBUG_INFO, "account", "looking for bool setting: %s\n",
			   name);

	setting = g_hash_table_lookup(account->settings, name);

	if (setting == NULL)
		return default_value;

	gaim_debug(GAIM_DEBUG_INFO, "account", "bool setting found: %s, %d\n",
			   name, setting->value.bool);

	g_return_val_if_fail(setting->type == GAIM_PREF_BOOLEAN, default_value);

	return setting->value.bool;
}

int
gaim_account_get_ui_int(const GaimAccount *account, const char *ui,
						const char *name, int default_value)
{
	GaimAccountSetting *setting;
	GHashTable *table;

	g_return_val_if_fail(account != NULL, default_value);
	g_return_val_if_fail(ui      != NULL, default_value);
	g_return_val_if_fail(name    != NULL, default_value);

	if ((table = g_hash_table_lookup(account->ui_settings, ui)) == NULL)
		return default_value;

	if ((setting = g_hash_table_lookup(table, name)) == NULL)
		return default_value;

	g_return_val_if_fail(setting->type == GAIM_PREF_INT, default_value);

	return setting->value.integer;
}

const char *
gaim_account_get_ui_string(const GaimAccount *account, const char *ui,
						   const char *name, const char *default_value)
{
	GaimAccountSetting *setting;
	GHashTable *table;

	g_return_val_if_fail(account != NULL, default_value);
	g_return_val_if_fail(ui      != NULL, default_value);
	g_return_val_if_fail(name    != NULL, default_value);

	if ((table = g_hash_table_lookup(account->ui_settings, ui)) == NULL)
		return default_value;

	if ((setting = g_hash_table_lookup(table, name)) == NULL)
		return default_value;

	g_return_val_if_fail(setting->type == GAIM_PREF_STRING, default_value);

	return setting->value.string;
}

gboolean
gaim_account_get_ui_bool(const GaimAccount *account, const char *ui,
						 const char *name, gboolean default_value)
{
	GaimAccountSetting *setting;
	GHashTable *table;

	g_return_val_if_fail(account != NULL, default_value);
	g_return_val_if_fail(ui      != NULL, default_value);
	g_return_val_if_fail(name    != NULL, default_value);

	if ((table = g_hash_table_lookup(account->ui_settings, ui)) == NULL)
		return default_value;

	if ((setting = g_hash_table_lookup(table, name)) == NULL)
		return default_value;

	g_return_val_if_fail(setting->type == GAIM_PREF_BOOLEAN, default_value);

	return setting->value.bool;
}

/* XML Stuff */
static void
free_parser_data(gpointer user_data)
{
	AccountParserData *data = user_data;

	if (data->buffer != NULL)
		g_string_free(data->buffer, TRUE);

	if (data->setting_name != NULL)
		g_free(data->setting_name);

	g_free(data);
}

static void
start_element_handler(GMarkupParseContext *context,
					  const gchar *element_name,
					  const gchar **attribute_names,
					  const gchar **attribute_values,
					  gpointer user_data, GError **error)
{
	const char *value;
	AccountParserData *data = user_data;
	GHashTable *atts;
	int i;

	atts = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

	for (i = 0; attribute_names[i] != NULL; i++) {
		g_hash_table_insert(atts, g_strdup(attribute_names[i]),
							g_strdup(attribute_values[i]));
	}

	if (data->buffer != NULL) {
		g_string_free(data->buffer, TRUE);
		data->buffer = NULL;
	}

	if (!strcmp(element_name, "protocol"))
		data->tag = TAG_PROTOCOL;
	else if (!strcmp(element_name, "name") || !strcmp(element_name, "username"))
		data->tag = TAG_NAME;
	else if (!strcmp(element_name, "password"))
		data->tag = TAG_PASSWORD;
	else if (!strcmp(element_name, "alias"))
		data->tag = TAG_ALIAS;
	else if (!strcmp(element_name, "userinfo"))
		data->tag = TAG_USERINFO;
	else if (!strcmp(element_name, "buddyicon"))
		data->tag = TAG_BUDDYICON;
	else if (!strcmp(element_name, "public-ip"))
		data->tag = TAG_PUBLIC_IP;
	else if (!strcmp(element_name, "proxy")) {
		data->in_proxy = TRUE;

		data->proxy_info = gaim_proxy_info_new();
	}
	else if (!strcmp(element_name, "type"))
		data->tag = TAG_TYPE;
	else if (!strcmp(element_name, "host"))
		data->tag = TAG_HOST;
	else if (!strcmp(element_name, "port"))
		data->tag = TAG_PORT;
	else if (!strcmp(element_name, "settings")) {
		if ((value = g_hash_table_lookup(atts, "ui")) != NULL) {
			data->setting_ui = g_strdup(value);
		}
	}
	else if (!strcmp(element_name, "setting")) {
		data->tag = TAG_SETTING;

		if ((value = g_hash_table_lookup(atts, "name")) != NULL)
			data->setting_name = g_strdup(value);

		if ((value = g_hash_table_lookup(atts, "type")) != NULL) {
			if (!strcmp(value, "string"))
				data->setting_type = GAIM_PREF_STRING;
			else if (!strcmp(value, "int"))
				data->setting_type = GAIM_PREF_INT;
			else if (!strcmp(value, "bool"))
				data->setting_type = GAIM_PREF_BOOLEAN;
		}
	}

	g_hash_table_destroy(atts);
}

static void
end_element_handler(GMarkupParseContext *context, const gchar *element_name,
					gpointer user_data,  GError **error)
{
	AccountParserData *data = user_data;
	gchar *buffer;

	if (data->buffer == NULL)
		return;

	buffer = g_string_free(data->buffer, FALSE);
	data->buffer = NULL;

	if (data->tag == TAG_PROTOCOL) {
		data->protocol_id = g_strdup(buffer);
	}
	else if (data->tag == TAG_NAME) {
		if (data->in_proxy) {
			gaim_proxy_info_set_username(data->proxy_info, buffer);
		}
		else {
			data->account = gaim_account_new(buffer, data->protocol_id);

			gaim_accounts_add(data->account);

			g_free(data->protocol_id);

			data->protocol_id = NULL;
		}
	}
	else if (data->tag == TAG_PASSWORD) {
		if (*buffer != '\0') {
			if (data->in_proxy) {
				gaim_proxy_info_set_password(data->proxy_info, buffer);
			}
			else {
				gaim_account_set_password(data->account, buffer);
				gaim_account_set_remember_password(data->account, TRUE);
			}
		}
	}
	else if (data->tag == TAG_ALIAS) {
		if (*buffer != '\0')
			gaim_account_set_alias(data->account, buffer);
	}
	else if (data->tag == TAG_USERINFO) {
		if (*buffer != '\0')
			gaim_account_set_user_info(data->account, buffer);
	}
	else if (data->tag == TAG_BUDDYICON) {
		if (*buffer != '\0')
			gaim_account_set_buddy_icon(data->account, buffer);
	}
	else if (data->tag == TAG_PUBLIC_IP) {
		if (*buffer != '\0')
			gaim_account_set_public_ip(data->account, buffer);
	}
	else if (data->tag == TAG_TYPE) {
		if (data->in_proxy) {
			if (!strcmp(buffer, "global"))
				gaim_proxy_info_set_type(data->proxy_info,
										 GAIM_PROXY_USE_GLOBAL);
			else if (!strcmp(buffer, "http"))
				gaim_proxy_info_set_type(data->proxy_info, GAIM_PROXY_HTTP);
			else if (!strcmp(buffer, "socks4"))
				gaim_proxy_info_set_type(data->proxy_info, GAIM_PROXY_SOCKS4);
			else if (!strcmp(buffer, "socks5"))
				gaim_proxy_info_set_type(data->proxy_info, GAIM_PROXY_SOCKS5);
			else
				gaim_debug(GAIM_DEBUG_ERROR, "account",
						   "Invalid proxy type found when loading account "
						   "information for %s\n",
						   gaim_account_get_username(data->account));
		}
	}
	else if (data->tag == TAG_HOST) {
		if (data->in_proxy && *buffer != '\0')
			gaim_proxy_info_set_host(data->proxy_info, buffer);
	}
	else if (data->tag == TAG_PORT) {
		if (data->in_proxy && *buffer != '\0')
			gaim_proxy_info_set_port(data->proxy_info, atoi(buffer));
	}
	else if (data->tag == TAG_SETTING) {
		if (*buffer != '\0') {
			if (data->setting_ui != NULL) {
				if (data->setting_type == GAIM_PREF_STRING)
					gaim_account_set_ui_string(data->account, data->setting_ui,
											   data->setting_name, buffer);
				else if (data->setting_type == GAIM_PREF_INT)
					gaim_account_set_ui_int(data->account, data->setting_ui,
											data->setting_name, atoi(buffer));
				else if (data->setting_type == GAIM_PREF_BOOLEAN)
					gaim_account_set_ui_bool(data->account, data->setting_ui,
											 data->setting_name,
											 (*buffer == '0' ? FALSE : TRUE));
			}
			else {
				if (data->setting_type == GAIM_PREF_STRING)
					gaim_account_set_string(data->account, data->setting_name,
											buffer);
				else if (data->setting_type == GAIM_PREF_INT)
					gaim_account_set_int(data->account, data->setting_name,
										 atoi(buffer));
				else if (data->setting_type == GAIM_PREF_BOOLEAN)
					gaim_account_set_bool(data->account, data->setting_name,
										  (*buffer == '0' ? FALSE : TRUE));
			}
		}

		g_free(data->setting_name);
		data->setting_name = NULL;
	}
	else if (!strcmp(element_name, "proxy")) {
		data->in_proxy = FALSE;

		if (gaim_proxy_info_get_type(data->proxy_info) == GAIM_PROXY_NONE) {
			gaim_proxy_info_destroy(data->proxy_info);
			data->proxy_info = NULL;
		}
		else if (*buffer != '\0') {
			gaim_account_set_proxy_info(data->account, data->proxy_info);
		}
	}
	else if (!strcmp(element_name, "settings")) {
		if (data->setting_ui != NULL) {
			g_free(data->setting_ui);
			data->setting_ui = NULL;
		}
	}

	data->tag = TAG_NONE;

	g_free(buffer);
}

static void
text_handler(GMarkupParseContext *context, const gchar *text,
			 gsize text_len, gpointer user_data, GError **error)
{
	AccountParserData *data = user_data;

	if (data->buffer == NULL)
		data->buffer = g_string_new_len(text, text_len);
	else
		g_string_append_len(data->buffer, text, text_len);
}

static GMarkupParser accounts_parser =
{
	start_element_handler,
	end_element_handler,
	text_handler,
	NULL,
	NULL
};

gboolean
gaim_accounts_load()
{
	gchar *filename = g_build_filename(gaim_user_dir(), "accounts.xml", NULL);
	gchar *contents = NULL;
	gsize length;
	GMarkupParseContext *context;
	GError *error = NULL;
	AccountParserData *parser_data;

	if (filename == NULL) {
		accounts_loaded = TRUE;
		return FALSE;
	}

	if (!g_file_get_contents(filename, &contents, &length, &error)) {
		gaim_debug(GAIM_DEBUG_ERROR, "accounts",
				   "Error reading accounts: %s\n", error->message);
		g_error_free(error);
		g_free(filename);
		accounts_loaded = TRUE;

		return FALSE;
	}

	parser_data = g_new0(AccountParserData, 1);

	context = g_markup_parse_context_new(&accounts_parser, 0,
										 parser_data, free_parser_data);

	if (!g_markup_parse_context_parse(context, contents, length, NULL)) {
		g_markup_parse_context_free(context);
		g_free(contents);
		g_free(filename);
		accounts_loaded = TRUE;

		return FALSE;
	}

	if (!g_markup_parse_context_end_parse(context, NULL)) {
		gaim_debug(GAIM_DEBUG_ERROR, "accounts", "Error parsing %s\n",
				   filename);
		g_markup_parse_context_free(context);
		g_free(contents);
		g_free(filename);
		accounts_loaded = TRUE;

		return FALSE;
	}

	g_markup_parse_context_free(context);
	g_free(contents);
	g_free(filename);
	accounts_loaded = TRUE;

	return TRUE;
}

static void
write_setting(gpointer key, gpointer value, gpointer user_data)
{
	GaimAccountSetting *setting;
	const char *name;
	FILE *fp;

	setting = (GaimAccountSetting *)value;
	name    = (const char *)key;
	fp      = (FILE *)user_data;

	if (setting->type == GAIM_PREF_INT) {
		fprintf(fp, "   <setting name='%s' type='int'>%d</setting>\n",
				name, setting->value.integer);
	}
	else if (setting->type == GAIM_PREF_STRING) {
		fprintf(fp, "   <setting name='%s' type='string'>%s</setting>\n",
				name, setting->value.string);
	}
	else if (setting->type == GAIM_PREF_BOOLEAN) {
		fprintf(fp, "   <setting name='%s' type='bool'>%d</setting>\n",
				name, setting->value.bool);
	}
}

static void
write_ui_setting_list(gpointer key, gpointer value, gpointer user_data)
{
	GHashTable *table;
	const char *ui;
	FILE *fp;

	table = (GHashTable *)value;
	ui    = (const char *)key;
	fp    = (FILE *)user_data;

	fprintf(fp, "  <settings ui='%s'>\n", ui);
	g_hash_table_foreach(table, write_setting, fp);
	fprintf(fp, "  </settings>\n");
}

static void
gaim_accounts_write(FILE *fp, GaimAccount *account)
{
	GaimProxyInfo *proxy_info;
	GaimProxyType proxy_type;
	const char *password, *alias, *user_info, *buddy_icon, *ip;
	char *esc;

	fprintf(fp, " <account>\n");
	fprintf(fp, "  <protocol>%s</protocol>\n", account->protocol_id);
	esc = g_markup_escape_text(gaim_account_get_username(account), -1);
	fprintf(fp, "  <name>%s</name>\n", esc);
	g_free(esc);

	if (gaim_account_get_remember_password(account) &&
		(password = gaim_account_get_password(account)) != NULL) {
		esc = g_markup_escape_text(password, -1);
		fprintf(fp, "  <password>%s</password>\n", esc);
		g_free(esc);
	}

	if ((alias = gaim_account_get_alias(account)) != NULL) {
		esc = g_markup_escape_text(alias, -1);
		fprintf(fp, "  <alias>%s</alias>\n", esc);
		g_free(esc);
	}

	if ((user_info = gaim_account_get_user_info(account)) != NULL) {
		esc = g_markup_escape_text(user_info, -1);
		fprintf(fp, "  <userinfo>%s</userinfo>\n", esc);
		g_free(esc);
	}

	if ((buddy_icon = gaim_account_get_buddy_icon(account)) != NULL) {
		esc = g_markup_escape_text(buddy_icon, -1);
		fprintf(fp, "  <buddyicon>%s</buddyicon>\n", esc);
		g_free(esc);
	}

	if ((ip = gaim_account_get_public_ip(account)) != NULL) {
		fprintf(fp, "  <public-ip>%s</public-ip>\n", ip);
	}

	fprintf(fp, "  <settings>\n");
	g_hash_table_foreach(account->settings, write_setting, fp);
	fprintf(fp, "  </settings>\n");

	g_hash_table_foreach(account->ui_settings, write_ui_setting_list, fp);

	if ((proxy_info = gaim_account_get_proxy_info(account)) != NULL &&
		(proxy_type = gaim_proxy_info_get_type(proxy_info)) != GAIM_PROXY_NONE)
	{
		const char *value;
		int int_value;

		fprintf(fp, "  <proxy>\n");
		fprintf(fp, "   <type>%s</type>\n",
				(proxy_type == GAIM_PROXY_USE_GLOBAL ? "global" :
				 proxy_type == GAIM_PROXY_HTTP       ? "http"   :
				 proxy_type == GAIM_PROXY_SOCKS4     ? "socks4" :
				 proxy_type == GAIM_PROXY_SOCKS5     ? "socks5" : "unknown"));

		if (proxy_type != GAIM_PROXY_USE_GLOBAL) {
			if ((value = gaim_proxy_info_get_host(proxy_info)) != NULL)
				fprintf(fp, "   <host>%s</host>\n", value);

			if ((int_value = gaim_proxy_info_get_port(proxy_info)) != 0)
				fprintf(fp, "   <port>%d</port>\n", int_value);

			if ((value = gaim_proxy_info_get_username(proxy_info)) != NULL)
				fprintf(fp, "   <username>%s</username>\n", value);

			if ((value = gaim_proxy_info_get_password(proxy_info)) != NULL)
				fprintf(fp, "   <password>%s</password>\n", value);
		}

		fprintf(fp, "  </proxy>\n");
	}

	fprintf(fp, " </account>\n");
}

void
gaim_accounts_sync(void)
{
	FILE *fp;
	const char *user_dir = gaim_user_dir();
	char *filename;
	char *filename_real;

	if (!accounts_loaded) {
		gaim_debug(GAIM_DEBUG_WARNING, "accounts",
				   "Writing accounts to disk.\n");
		schedule_accounts_save();
		return;
	}

	if (user_dir == NULL)
		return;

	gaim_debug(GAIM_DEBUG_INFO, "accounts", "Writing accounts to disk.\n");

	fp = fopen(user_dir, "r");

	if (fp == NULL)
		mkdir(user_dir, S_IRUSR | S_IWUSR | S_IXUSR);
	else
		fclose(fp);

	filename = g_build_filename(user_dir, "accounts.xml.save", NULL);

	if ((fp = fopen(filename, "w")) != NULL) {
		GList *l;

		fprintf(fp, "<?xml version='1.0' encoding='UTF-8' ?>\n\n");
		fprintf(fp, "<accounts>\n");

		for (l = gaim_accounts_get_all(); l != NULL; l = l->next)
			gaim_accounts_write(fp, l->data);

		fprintf(fp, "</accounts>\n");

		fclose(fp);
		chmod(filename, S_IRUSR | S_IWUSR);
	}
	else {
		gaim_debug(GAIM_DEBUG_ERROR, "accounts", "Unable to write %s\n",
				   filename);
	}

	filename_real = g_build_filename(user_dir, "accounts.xml", NULL);

	if (rename(filename, filename_real) < 0) {
		gaim_debug(GAIM_DEBUG_ERROR, "accounts", "Error renaming %s to %s\n",
				   filename, filename_real);
	}

	g_free(filename);
	g_free(filename_real);
}

void
gaim_accounts_add(GaimAccount *account)
{
	g_return_if_fail(account != NULL);

	if (g_list_find(accounts, account) != NULL)
		return;

	accounts = g_list_append(accounts, account);

	schedule_accounts_save();
}

void
gaim_accounts_remove(GaimAccount *account)
{
	g_return_if_fail(account != NULL);

	accounts = g_list_remove(accounts, account);

	schedule_accounts_save();
}

void
gaim_accounts_delete(GaimAccount *account)
{
	GaimBlistNode *gnode, *bnode;

	g_return_if_fail(account != NULL);

	gaim_accounts_remove(account);

	for (gnode = gaim_get_blist()->root; gnode != NULL; gnode = gnode->next) {
		if (!GAIM_BLIST_NODE_IS_GROUP(gnode))
			continue;

		for (bnode = gnode->child; bnode != NULL; bnode = bnode->next) {
			if (GAIM_BLIST_NODE_IS_BUDDY(bnode)) {
				struct buddy *b = (struct buddy *)bnode;

				if (b->account == account)
					gaim_blist_remove_buddy(b);
			}
			else if (GAIM_BLIST_NODE_IS_CHAT(bnode)) {
				struct chat *c = (struct chat *)bnode;

				if (c->account == account)
					gaim_blist_remove_chat(c);
			}
		}
	}

	gaim_blist_save();

	gaim_account_destroy(account);
}

void
gaim_accounts_auto_login(const char *ui)
{
	GaimAccount *account;
	GList *l;

	g_return_if_fail(ui != NULL);

	for (l = gaim_accounts_get_all(); l != NULL; l = l->next) {
		account = l->data;

		if (gaim_account_get_auto_login(account, ui))
			gaim_account_connect(account);
	}
}

void
gaim_accounts_reorder(GaimAccount *account, size_t new_index)
{
	size_t index;
	GList *l;

	g_return_if_fail(account != NULL);
	g_return_if_fail(new_index >= 0 && new_index <= g_list_length(accounts));

	index = g_list_index(accounts, account);

	if (index == -1) {
		gaim_debug(GAIM_DEBUG_ERROR, "accounts",
				   "Unregistered account (%s) discovered during reorder!\n",
				   gaim_account_get_username(account));
		return;
	}

	l = g_list_nth(accounts, index);

	if (new_index > index)
		new_index--;

	/* Remove the old one. */
	accounts = g_list_delete_link(accounts, l);

	/* Insert it where it should go. */
	accounts = g_list_insert(accounts, account, new_index);

	schedule_accounts_save();
}

GList *
gaim_accounts_get_all(void)
{
	return accounts;
}

GaimAccount *
gaim_accounts_find(const char *name, GaimProtocol protocol)
{
	GaimAccount *account = NULL;
	GList *l;
	char *who;

	g_return_val_if_fail(name != NULL, NULL);

	who = g_strdup(normalize(name));

	for (l = gaim_accounts_get_all(); l != NULL; l = l->next) {
		account = (GaimAccount *)l->data;

		if (!strcmp(normalize(gaim_account_get_username(account)), who)) {
			if (protocol != -1) {
				if (gaim_account_get_protocol(account) == protocol)
					break;
			}
			else
				break;
		}

		account = NULL;
	}

	g_free(who);

	return account;
}

GaimAccount *
gaim_accounts_find_with_prpl_id(const char *name, const char *protocol_id)
{
	GaimAccount *account = NULL;
	GList *l;
	char *who;

	g_return_val_if_fail(name != NULL, NULL);

	who = g_strdup(normalize(name));

	for (l = gaim_accounts_get_all(); l != NULL; l = l->next) {
		account = (GaimAccount *)l->data;

		if (!strcmp(normalize(gaim_account_get_username(account)), who) &&
			!strcmp(account->protocol_id, protocol_id)) {

			break;
		}

		account = NULL;
	}

	g_free(who);

	return account;
}

void *
gaim_accounts_get_handle(void)
{
	static int handle;

	return &handle;
}

void
gaim_accounts_init(void)
{
	void *handle = gaim_accounts_get_handle();

	gaim_signal_register(handle, "account-connecting",
						 gaim_marshal_VOID__POINTER, NULL, 1,
						 gaim_value_new(GAIM_TYPE_SUBTYPE,
										GAIM_SUBTYPE_ACCOUNT));

	gaim_signal_register(handle, "account-away",
						 gaim_marshal_VOID__POINTER_POINTER_POINTER, NULL, 3,
						 gaim_value_new(GAIM_TYPE_SUBTYPE,
										GAIM_SUBTYPE_ACCOUNT),
						 gaim_value_new(GAIM_TYPE_STRING),
						 gaim_value_new(GAIM_TYPE_STRING));

	gaim_signal_register(handle, "account-setting-info",
						 gaim_marshal_VOID__POINTER_POINTER, NULL, 2,
						 gaim_value_new(GAIM_TYPE_SUBTYPE,
										GAIM_SUBTYPE_ACCOUNT),
						 gaim_value_new(GAIM_TYPE_STRING));

	gaim_signal_register(handle, "account-set-info",
						 gaim_marshal_VOID__POINTER_POINTER, NULL, 2,
						 gaim_value_new(GAIM_TYPE_SUBTYPE,
										GAIM_SUBTYPE_ACCOUNT),
						 gaim_value_new(GAIM_TYPE_STRING));

	gaim_signal_register(handle, "account-warned",
						 gaim_marshal_VOID__POINTER_POINTER_UINT, NULL, 3,
						 gaim_value_new(GAIM_TYPE_SUBTYPE,
										GAIM_SUBTYPE_ACCOUNT),
						 gaim_value_new(GAIM_TYPE_STRING),
						 gaim_value_new(GAIM_TYPE_UINT));
}

void
gaim_accounts_uninit(void)
{
	gaim_signals_unregister_by_instance(gaim_accounts_get_handle());
}
