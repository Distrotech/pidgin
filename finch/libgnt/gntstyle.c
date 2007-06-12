/**
 * GNT - The GLib Ncurses Toolkit
 *
 * GNT is the legal property of its developers, whose names are too numerous
 * to list here.  Please refer to the COPYRIGHT file distributed with this
 * source distribution.
 *
 * This library is free software; you can redistribute it and/or modify
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

#include "gntstyle.h"
#include "gntcolors.h"

#include "gntws.h"

#include <glib.h>
#include <ctype.h>
#include <glib/gprintf.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORKSPACES 99

#if GLIB_CHECK_VERSION(2,6,0)
static GKeyFile *gkfile;
#endif

static GHashTable *unknowns;
static char * str_styles[GNT_STYLES];
static int int_styles[GNT_STYLES];
static int bool_styles[GNT_STYLES];

const char *gnt_style_get(GntStyle style)
{
	return str_styles[style];
}

const char *gnt_style_get_from_name(const char *name)
{
	return g_hash_table_lookup(unknowns, name);
}

gboolean gnt_style_get_bool(GntStyle style, gboolean def)
{
	int i;
	const char * str;

	if (bool_styles[style] != -1)
		return bool_styles[style];
	
	str = gnt_style_get(style);

	if (str)
	{
		if (strcmp(str, "false") == 0)
			def = FALSE;
		else if (strcmp(str, "true") == 0)
			def = TRUE;
		else if (sscanf(str, "%d", &i) == 1)
		{
			if (i)
				def = TRUE;
			else
				def = FALSE;
		}
	}

	bool_styles[style] = def;
	return bool_styles[style];
}

static void
refine(char *text)
{
	char *s = text, *t = text;

	while (*s)
	{
		if (*s == '^' && *(s + 1) == '[')
		{
			*t = '\033';  /* escape */
			s++;
		}
		else if (*s == '\\')
		{
			if (*(s + 1) == '\0')
				*t = ' ';
			else
			{
				s++;
				if (*s == 'r' || *s == 'n')
					*t = '\r';
				else if (*s == 't')
					*t = '\t';
				else
					*t = *s;
			}
		}
		else
			*t = *s;
		t++;
		s++;
	}
	*t = '\0';
}

static char *
parse_key(const char *key)
{
	return (char *)gnt_key_translate(key);
}

void gnt_style_read_workspaces(GntWM *wm)
{
#if GLIB_CHECK_VERSION(2,6,0)
	int i;
	gchar *name;
	gsize c;

	for (i = 1; i < MAX_WORKSPACES; ++i) {
		int j;
		GntWS *ws;
		gchar **titles;
		char *group = calloc(12, 1);
		g_sprintf(group, "Workspace-%d", i);
		name = g_key_file_get_value(gkfile, group, "name", NULL);
		if (!name)
			return;

		ws = g_object_new(GNT_TYPE_WS, NULL);
		gnt_ws_set_name(ws, name);
		gnt_wm_add_workspace(wm, ws);
		g_free(name);

		titles = g_key_file_get_string_list(gkfile, group, "window-names", &c, NULL);
		if (titles) {
			for (j = 0; j < c; ++j)
				g_hash_table_replace(wm->name_places, g_strdup(titles[j]), ws);
			g_strfreev(titles);
		}

		titles = g_key_file_get_string_list(gkfile, group, "window-titles", &c, NULL);
		if (titles) {
			for (j = 0; j < c; ++j)
				g_hash_table_replace(wm->title_places, g_strdup(titles[j]), ws);
			g_strfreev(titles);
		}
		g_free(group);
	}
#endif
}
void gnt_style_read_actions(GType type, GntBindableClass *klass)
{
#if GLIB_CHECK_VERSION(2,6,0)
	char *name;
	GError *error = NULL;

	name = g_strdup_printf("%s::binding", g_type_name(type));

	if (g_key_file_has_group(gkfile, name))
	{
		gsize len = 0;
		char **keys;
		
		keys = g_key_file_get_keys(gkfile, name, &len, &error);
		if (error)
		{
			g_printerr("GntStyle: %s\n", error->message);
			g_error_free(error);
			g_free(name);
			return;
		}

		while (len--)
		{
			char *key, *action;

			key = g_strdup(keys[len]);
			action = g_key_file_get_string(gkfile, name, keys[len], &error);

			if (error)
			{
				g_printerr("GntStyle: %s\n", error->message);
				g_error_free(error);
				error = NULL;
			}
			else
			{
				const char *keycode = parse_key(key);
				if (keycode == NULL) {
					g_printerr("GntStyle: Invalid key-binding %s\n", key);
				} else {
					gnt_bindable_register_binding(klass, action, keycode, NULL);
				}
			}
			g_free(key);
			g_free(action);
		}
		g_strfreev(keys);
	}
	g_free(name);
#endif
}

void gnt_styles_get_keyremaps(GType type, GHashTable *hash)
{
#if GLIB_CHECK_VERSION(2,6,0)
	char *name;
	GError *error = NULL;
	
	name = g_strdup_printf("%s::remap", g_type_name(type));

	if (g_key_file_has_group(gkfile, name))
	{
		gsize len = 0;
		char **keys;
		
		keys = g_key_file_get_keys(gkfile, name, &len, &error);
		if (error)
		{
			g_printerr("GntStyle: %s\n", error->message);
			g_error_free(error);
			g_free(name);
			return;
		}

		while (len--)
		{
			char *key, *replace;

			key = g_strdup(keys[len]);
			replace = g_key_file_get_string(gkfile, name, keys[len], &error);

			if (error)
			{
				g_printerr("GntStyle: %s\n", error->message);
				g_error_free(error);
				error = NULL;
				g_free(key);
			}
			else
			{
				refine(key);
				refine(replace);
				g_hash_table_insert(hash, key, replace);
			}
		}
		g_strfreev(keys);
	}

	g_free(name);
#endif
}

#if GLIB_CHECK_VERSION(2,6,0)
static void
read_general_style(GKeyFile *kfile)
{
	GError *error = NULL;
	gsize nkeys;
	char **keys = g_key_file_get_keys(kfile, "general", &nkeys, &error);
	int i;
	struct
	{
		const char *style;
		GntStyle en;
	} styles[] = {{"shadow", GNT_STYLE_SHADOW},
	              {"customcolor", GNT_STYLE_COLOR},
	              {"mouse", GNT_STYLE_MOUSE},
	              {"wm", GNT_STYLE_WM},
	              {"remember_position", GNT_STYLE_REMPOS},
	              {NULL, 0}};

	if (error)
	{
		g_printerr("GntStyle: %s\n", error->message);
		g_error_free(error);
	}
	else
	{
		for (i = 0; styles[i].style; i++)
		{
			str_styles[styles[i].en] =
					g_key_file_get_string(kfile, "general", styles[i].style, NULL);
		}
		for (i = 0; i < nkeys; i++)
			g_hash_table_replace(unknowns, g_strdup(keys[i]),
					g_strdup(g_key_file_get_string(kfile, "general", keys[i], NULL)));
	}
	g_strfreev(keys);
}
#endif

void gnt_style_read_configure_file(const char *filename)
{
#if GLIB_CHECK_VERSION(2,6,0)
	GError *error = NULL;
	gkfile = g_key_file_new();
	unknowns = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

	if (!g_key_file_load_from_file(gkfile, filename, G_KEY_FILE_NONE, &error))
	{
		g_printerr("GntStyle: %s\n", error->message);
		g_error_free(error);
		return;
	}
	gnt_colors_parse(gkfile);
	read_general_style(gkfile);
#endif
}

void gnt_init_styles()
{
	int i;
	for (i = 0; i < GNT_STYLES; i++)
	{
		str_styles[i] = NULL;
		int_styles[i] = -1;
		bool_styles[i] = -1;
	}
}

void gnt_uninit_styles()
{
	int i;
	for (i = 0; i < GNT_STYLES; i++)
		g_free(str_styles[i]);

#if GLIB_CHECK_VERSION(2,6,0)
	g_key_file_free(gkfile);
#endif
}

