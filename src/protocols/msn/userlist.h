#ifndef _MSN_USERLIST_H_
#define _MSN_USERLIST_H_

typedef struct _MsnUserList MsnUserList;

#include "cmdproc.h"
#include "user.h"
#include "group.h"

typedef enum
{
	MSN_LIST_FL,
	MSN_LIST_AL,
	MSN_LIST_BL,
	MSN_LIST_RL

} MsnListId;

typedef struct
{
	char *who;
	char *old_group_name;

} MsnMoveBuddy;

struct _MsnUserList
{
	MsnSession *session;

	/* MsnUsers *users; */
	/* MsnGroups *groups; */

	GList *users;
	GList *groups;

	int fl_users_count;

};

MsnListId msn_get_list_id(const char *list);

void msn_got_add_user(MsnSession *session, MsnUser *user,
					  MsnListId list_id, int group_id);
void msn_got_rem_user(MsnSession *session, MsnUser *user,
					  MsnListId list_id, int group_id);
void msn_got_lst_user(MsnSession *session, MsnUser *user,
					  int list_op, GSList *group_ids);

MsnUserList *msn_userlist_new(MsnSession *session);
void msn_userlist_destroy(MsnUserList *userlist);
void msn_userlist_add_user(MsnUserList *userlist, MsnUser *user);
void msn_userlist_remove_user(MsnUserList *userlist, MsnUser *user);
MsnUser *msn_userlist_find_user(MsnUserList *userlist,
								const char *passport);
void msn_userlist_add_group(MsnUserList *userlist, MsnGroup *group);
void msn_userlist_remove_group(MsnUserList *userlist, MsnGroup *group);
MsnGroup *msn_userlist_find_group_with_id(MsnUserList *userlist, int id);
MsnGroup *msn_userlist_find_group_with_name(MsnUserList *userlist,
											const char *name);
int msn_userlist_find_group_id(MsnUserList *userlist,
							   const char *group_name);
const char *msn_userlist_find_group_name(MsnUserList *userlist,
										 int group_id);
void msn_userlist_rename_group_id(MsnUserList *userlist, int group_id,
								  const char *new_name);
void msn_userlist_remove_group_id(MsnUserList *userlist, int group_id);

void msn_userlist_rem_buddy(MsnUserList *userlist, const char *who,
							int list_id, const char *group_name);
void msn_userlist_add_buddy(MsnUserList *userlist, const char *who,
							int list_id, const char *group_name);
void msn_userlist_move_buddy(MsnUserList *userlist, const char *who,
							 const char *old_group_name,
							 const char *new_group_name);

#endif /* _MSN_USERLIST_H_ */
