/**
 * @file switchboard.h MSN switchboard functions
 *
 * gaim
 *
 * Copyright (C) 2003-2004 Christian Hammond <chipx86@gnupdate.org>
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
#ifndef _MSN_SWITCHBOARD_H_
#define _MSN_SWITCHBOARD_H_

typedef struct _MsnSwitchBoard MsnSwitchBoard;

#include "conversation.h"

#include "msg.h"
#include "user.h"

#include "servconn.h"

struct _MsnSwitchBoard
{
	MsnSession *session;
	MsnServConn *servconn;
	char *im_user;

	char *auth_key;
	char *session_id;

	gboolean invited;
	gboolean destroying;

	GaimConversation *conv;

	gboolean ready; /* When it's actually usable */
	/* gboolean in_use; */

	int current_users;
	int total_users;
	GList *users;

	int chat_id;

	gboolean hidden;

	gboolean user_joined;
	GQueue *im_queue;
};

/**
 * Initialize the variables for switchboard creation.
 */
void msn_switchboard_init(void);

/**
 * Destroy the variables for switchboard creation.
 */
void msn_switchboard_end(void);

/**
 * Creates a new switchboard.
 *
 * @param session The MSN session.
 *
 * @return The new switchboard.
 */
MsnSwitchBoard *msn_switchboard_new(MsnSession *session);

/**
 * Destroys a switchboard.
 *
 * @param swboard The switchboard to destroy.
 */
void msn_switchboard_destroy(MsnSwitchBoard *swboard);

#if 0
/**
 * Sets the user the switchboard is supposed to connect to.
 *
 * @param swboard The switchboard.
 * @param user    The user.
 */
void msn_switchboard_set_user(MsnSwitchBoard *swboard, const char *user);

/**
 * Returns the user the switchboard is supposed to connect to.
 *
 * @param swboard The switchboard.
 *
 * @return The user.
 */
const char *msn_switchboard_get_user(MsnSwitchBoard *swboard);
#endif

/**
 * Sets the auth key the switchboard must use when connecting.
 *
 * @param swboard The switchboard.
 * @param key     The auth key.
 */
void msn_switchboard_set_auth_key(MsnSwitchBoard *swboard, const char *key);

/**
 * Returns the auth key the switchboard must use when connecting.
 *
 * @param swboard The switchboard.
 *
 * @return The auth key.
 */
const char *msn_switchboard_get_auth_key(MsnSwitchBoard *swboard);

/**
 * Sets the session ID the switchboard must use when connecting.
 *
 * @param swboard The switchboard.
 * @param id      The session ID.
 */
void msn_switchboard_set_session_id(MsnSwitchBoard *swboard, const char *id);

/**
 * Returns the session ID the switchboard must use when connecting.
 *
 * @param swboard The switchboard.
 *
 * @return The session ID.
 */
const char *msn_switchboard_get_session_id(MsnSwitchBoard *swboard);

/**
 * Sets whether or not the user was invited to this switchboard.
 *
 * @param swboard The switchboard.
 * @param invite  @c TRUE if invited, @c FALSE otherwise.
 */
void msn_switchboard_set_invited(MsnSwitchBoard *swboard, gboolean invited);

/**
 * Returns whether or not the user was invited to this switchboard.
 *
 * @param swboard The switchboard.
 *
 * @return @c TRUE if invited, @c FALSE otherwise.
 */
gboolean msn_switchboard_is_invited(MsnSwitchBoard *swboard);

/**
 * Connects to a switchboard.
 *
 * @param swboard The switchboard.
 * @param host    The switchboard server host.
 * @param port    The switcbharod server port.
 *
 * @return @c TRUE if able to connect, or @c FALSE otherwise.
 */
gboolean msn_switchboard_connect(MsnSwitchBoard *swboard,
								 const char *host, int port);
void msn_switchboard_disconnect(MsnSwitchBoard *swboard);
void msn_switchboard_send_msg(MsnSwitchBoard *swboard, MsnMessage *msg);

gboolean msn_switchboard_chat_leave(MsnSwitchBoard *swboard);
gboolean msn_switchboard_chat_invite(MsnSwitchBoard *swboard, const char *who);

void msn_switchboard_request(MsnSwitchBoard *swboard);
void msn_switchboard_request_add_user(MsnSwitchBoard *swboard, const char *user);
void msn_switchboard_queue_msg(MsnSwitchBoard *swboard, MsnMessage *msg);
void msn_switchboard_process_queue(MsnSwitchBoard *swboard);

/**
 * Processes application/x-msnmsgrp2p messages.
 *
 * @param cmdproc The command processor.
 * @param msg     The message.
 */
void msn_p2p_msg(MsnCmdProc *cmdproc, MsnMessage *msg);
void msn_emoticon_msg(MsnCmdProc *cmdproc, MsnMessage *msg);
void msn_invite_msg(MsnCmdProc *cmdproc, MsnMessage *msg);

#endif /* _MSN_SWITCHBOARD_H_ */
