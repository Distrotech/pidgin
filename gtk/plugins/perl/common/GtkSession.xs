#include "gtkmodule.h"

MODULE = Gaim::Gtk::Session  PACKAGE = Gaim::Gtk::Session  PREFIX = gaim_gtk_session_
PROTOTYPES: ENABLE

void
gaim_gtk_session_init(argv0, previous_id, config_dir)
	gchar * argv0
	gchar * previous_id
	gchar * config_dir

void
gaim_gtk_session_end()
