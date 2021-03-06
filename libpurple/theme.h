/* purple
 *
 * Purple is the legal property of its developers, whose names are too numerous
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
 */

#ifndef PURPLE_THEME_H
#define PURPLE_THEME_H
/**
 * SECTION:theme
 * @section_id: libpurple-theme
 * @short_description: <filename>theme.h</filename>
 * @title: Theme Abstact Class
 */

#include <glib.h>
#include <glib-object.h>
#include "imgstore.h"

typedef struct _PurpleTheme        PurpleTheme;
typedef struct _PurpleThemeClass   PurpleThemeClass;

#define PURPLE_TYPE_THEME            (purple_theme_get_type ())
#define PURPLE_THEME(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), PURPLE_TYPE_THEME, PurpleTheme))
#define PURPLE_THEME_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), PURPLE_TYPE_THEME, PurpleThemeClass))
#define PURPLE_IS_THEME(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PURPLE_TYPE_THEME))
#define PURPLE_IS_THEME_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), PURPLE_TYPE_THEME))
#define PURPLE_THEME_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), PURPLE_TYPE_THEME, PurpleThemeClass))

/**
 * PurpleTheme:
 *
 * A purple theme.
 * This is an abstract class for Purple to use with the Purple theme manager.
 */
struct _PurpleTheme
{
	GObject parent;
};

struct _PurpleThemeClass
{
	GObjectClass parent_class;

	/*< private >*/
	void (*purple_reserved1)(void);
	void (*purple_reserved2)(void);
	void (*purple_reserved3)(void);
	void (*purple_reserved4)(void);
};

/**************************************************************************/
/* Purple Theme API                                                       */
/**************************************************************************/
G_BEGIN_DECLS

/**
 * purple_theme_get_type:
 *
 * Returns: The #GType for a theme.
 */
GType purple_theme_get_type(void);

/**
 * purple_theme_get_name:
 * @theme:  The purple theme.
 *
 * Returns the name of the PurpleTheme object.
 *
 * Returns: The string representing the name of the theme.
 */
const gchar *purple_theme_get_name(PurpleTheme *theme);

/**
 * purple_theme_set_name:
 * @theme: The purple theme.
 * @name:  The name of the PurpleTheme object.
 *
 * Sets the name of the PurpleTheme object.
 */
void purple_theme_set_name(PurpleTheme *theme, const gchar *name);

/**
 * purple_theme_get_description:
 * @theme:  The purple theme.
 *
 * Returns the description of the PurpleTheme object.
 *
 * Returns: A short description of the theme.
 */
const gchar *purple_theme_get_description(PurpleTheme *theme);

/**
 * purple_theme_set_description:
 * @theme:  The purple theme.
 * @description: The description of the PurpleTheme object.
 *
 * Sets the description of the PurpleTheme object.
 */
void purple_theme_set_description(PurpleTheme *theme, const gchar *description);

/**
 * purple_theme_get_author:
 * @theme:  The purple theme.
 *
 * Returns the author of the PurpleTheme object.
 *
 * Returns: The author of the theme.
 */
const gchar *purple_theme_get_author(PurpleTheme *theme);

/**
 * purple_theme_set_author:
 * @theme:  The purple theme.
 * @author: The author of the PurpleTheme object.
 *
 * Sets the author of the PurpleTheme object.
 */
void purple_theme_set_author(PurpleTheme *theme, const gchar *author);

/**
 * purple_theme_get_type_string:
 * @theme:  The purple theme.
 *
 * Returns the type (string) of the PurpleTheme object.
 *
 * Returns: The string representing the type.
 */
const gchar *purple_theme_get_type_string(PurpleTheme *theme);

/**
 * purple_theme_get_dir:
 * @theme:  The purple theme.
 *
 * Returns the directory of the PurpleTheme object.
 *
 * Returns: The string representing the theme directory.
 */
const gchar *purple_theme_get_dir(PurpleTheme *theme);

/**
 * purple_theme_set_dir:
 * @theme:  The purple theme.
 * @dir:    The directory of the PurpleTheme object.
 *
 * Sets the directory of the PurpleTheme object.
 */
void purple_theme_set_dir(PurpleTheme *theme, const gchar *dir);

/**
 * purple_theme_get_image:
 * @theme:  The purple theme.
 *
 * Returns the image preview of the PurpleTheme object.
 *
 * Returns: The image preview of the PurpleTheme object.
 */
const gchar *purple_theme_get_image(PurpleTheme *theme);

/**
 * purple_theme_get_image_full:
 * @theme:  The purple theme.
 *
 * Returns the image preview and directory of the PurpleTheme object.
 *
 * Returns: The image preview of the PurpleTheme object.
 */
gchar *purple_theme_get_image_full(PurpleTheme *theme);

/**
 * purple_theme_set_image:
 * @theme:	 The purple theme.
 * @img:    The image preview of the PurpleTheme object.
 *
 * Sets the directory of the PurpleTheme object.
 */
void purple_theme_set_image(PurpleTheme *theme, const gchar *img);

G_END_DECLS

#endif /* PURPLE_THEME_H */
