/**
 * @file gtkpounce.h GTK+ Buddy Pounce API
 * @ingroup pidgin
 */

/* pidgin
 *
 * Pidgin is the legal property of its developers, whose names are too numerous
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
#ifndef _PIDGINPOUNCE_H_
#define _PIDGINPOUNCE_H_

#include "pounce.h"

#ifndef PURPLE_DISABLE_DEPRECATED
/**
 * Displays a New Buddy Pounce or Edit Buddy Pounce dialog.
 *
 * @param account    The optional account to use.
 * @param name       The optional name to pounce on.
 * @param cur_pounce The current buddy pounce, if editing an existing one.
 *
 * @deprecated Use pidgin_pounce_editor_show_with_parent() instead.
 */
void pidgin_pounce_editor_show(PurpleAccount *account, const char *name,
								PurplePounce *cur_pounce);
#endif

/**
 * Displays a New Buddy Pounce or Edit Buddy Pounce dialog.
 *
 * @param parent     The parent window.
 * @param account    The optional account to use.
 * @param name       The optional name to pounce on.
 * @param cur_pounce The current buddy pounce, if editing an existing one.
 *
 * @since 2.3.0
 */
void pidgin_pounce_editor_show_with_parent(GtkWindow *parent, PurpleAccount *account, const char *name,
								PurplePounce *cur_pounce);

#ifndef PURPLE_DISABLE_DEPRECATED
/**
 * Shows the pounces manager window.
 *
 * @deprecated Use pidgin_pounces_manager_show_with_parent() instead.
 */
void pidgin_pounces_manager_show(void);
#endif

/**
 * Shows the pounces manager window.
 *
 * @since 2.3.0
 */
void pidgin_pounces_manager_show_with_parent(GtkWindow *parent);

/**
 * Hides the pounces manager window.
 */
void pidgin_pounces_manager_hide(void);

/**
 * Returns the gtkpounces handle
 *
 * @return The handle to the GTK+ pounces system
 */
void *pidgin_pounces_get_handle(void);

/**
 * Initializes the GTK+ pounces subsystem.
 */
void pidgin_pounces_init(void);

#endif /* _PIDGINPOUNCE_H_ */
