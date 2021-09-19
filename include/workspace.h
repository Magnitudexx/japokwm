#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <stdio.h>
#include <stdlib.h>
#include "utils/coreUtils.h"

#include "bitset/bitset.h"

struct layout;
struct focus_set;
struct visual_set;
struct client;
struct container;
struct server;

/* when an action should change the workspace and the tagsets associated with it
 * you should use this macro.
 * NOTE: use to jump to the end of the current action*/
#define DO_ACTION_LOCALLY(workspace, action) \
    do {\
        struct container_set *con_set = workspace->con_set;\
        do {\
            action\
        } while (0);\
        \
        do {\
            struct tagset *_tagset = workspace_get_active_tagset(workspace);\
            assert(tagset_exist_on(_tagset, con) == true);\
            con_set = _tagset->con_set;\
            action\
        } while (0);\
    } while (0)

#define DO_ACTION_GLOBALLY(workspaces, action) \
    do {\
        for (int i = 0; i < server.tagsets->len; i++) {\
            struct tagset *tagset = g_ptr_array_index(server.tagsets, i);\
            tagset->applied_action = false;\
        }\
        for (int _i = 0; _i < workspaces->len; _i++) {\
            struct workspace *_ws = g_ptr_array_index(workspaces, _i);\
            \
            struct container_set *con_set = _ws->con_set;\
            do {\
                action\
            } while (0);\
            \
            do {\
                struct tagset *_tagset = _ws->tagset;\
                if (!_tagset)\
                    continue;\
                if (_tagset->applied_action)\
                    continue;\
                if (!tagset_exist_on(_tagset, con))\
                    continue;\
                con_set = _tagset->con_set;\
                _tagset->applied_action = true;\
                action\
            } while (0);\
        }\
    } while (0)

/* A tag is simply a workspace that can be focused (like a normal workspace)
 * and can selected: which just means that all clients on the selected tags
 * will be combined to be shown on the focused tag
 * using this struct requires to use tagsetCreate and later tagsetDestroy
 * */
struct workspace {
    GPtrArray *loaded_layouts;
    struct layout *previous_layout;
    struct layout *layout;

    size_t id;
    char *name;

    BitSet *prev_workspaces;
    // the last monitor the workspace was on
    struct monitor *prev_m;
    // the latest tagset
    struct tagset *tagset;
    // the tagset that currently has this workspace selected
    struct tagset *selected_tagset;

    GPtrArray *independent_containers;

    struct container_set *con_set;
    struct focus_set *focus_set;
    struct visual_set *visual_set;
};

GPtrArray *create_workspaces(GPtrArray *tag_names);
struct workspace *create_workspace(const char *name, size_t id, struct layout *lt);
void destroy_workspace(struct workspace *ws);

void update_workspaces(GPtrArray *workspaces, GPtrArray *tag_names);
void update_workspace_ids(GPtrArray *workspaces);

bool is_workspace_occupied(struct workspace *ws);
bool workspace_is_visible(struct workspace *ws);
bool is_workspace_the_selected_one(struct workspace *ws);
bool is_workspace_extern(struct workspace *ws);
bool workspace_is_active(struct workspace *ws);

int get_workspace_container_count(struct workspace *ws);
bool is_workspace_empty(struct workspace *ws);

void focus_most_recent_container(struct workspace *ws);
struct container *get_container(struct workspace *ws, int i);

struct workspace *find_next_unoccupied_workspace(GPtrArray *workspaces, struct workspace *ws);
struct workspace *get_workspace(int id);
struct workspace *get_next_empty_workspace(GPtrArray *workspaces, size_t i);
struct workspace *get_prev_empty_workspace(GPtrArray *workspaces, size_t i);

struct tagset *workspace_get_selected_tagset(struct workspace *ws);
struct tagset *workspace_get_tagset(struct workspace *ws);
struct tagset *workspace_get_active_tagset(struct workspace *ws);

struct monitor *workspace_get_selected_monitor(struct workspace *ws);
struct monitor *workspace_get_monitor(struct workspace *ws); 

void focus_next_unoccupied_workspace(struct monitor *m, GPtrArray *workspaces, struct workspace *ws);
void copy_layout_from_selected_workspace(GPtrArray *workspaces);
void destroy_workspaces(GPtrArray *workspaces);
void layout_set_set_layout(lua_State *L);
void push_layout(struct workspace *ws, struct layout *lt);
void load_default_layout(lua_State *L);
void load_layout(lua_State *L, const char *name);
void reset_loaded_layout(struct workspace *ws);
void remove_loaded_layouts(GPtrArray *workspaces);
void workspace_rename(struct workspace *ws, const char *name);
void workspace_update_names(struct server *server, GPtrArray *workspaces);
struct container *workspace_get_focused_container(struct workspace *ws);

void workspace_add_container_to_containers(struct workspace *ws, struct container *con, int i);
void workspace_add_container_to_focus_stack(struct workspace *ws, struct container *con);
void add_container_to_stack(struct workspace *ws, struct container *con);

void list_set_append_container_to_focus_stack(struct workspace *ws, struct container *con);
void list_set_add_container_to_focus_stack(struct workspace *ws, struct container *con);
void workspace_remove_container_from_containers_locally(struct workspace *ws, struct container *con);
void workspace_add_container_to_containers_locally(struct workspace *ws, struct container *con, int i);
void workspace_remove_container_from_focus_stack_locally(struct workspace *ws, struct container *con);
void workspace_add_container_to_focus_stack_locally(struct workspace *ws, struct container *con);
void workspace_remove_container_from_floating_stack_locally(struct workspace *ws, struct container *con);
void workspace_add_container_to_floating_stack_locally(struct workspace *ws, struct container *con, int i);

void workspace_remove_container_from_visual_stack_layer(struct workspace *ws, struct container *con);
void workspace_add_container_to_visual_stack_layer(struct workspace *ws, struct container *con);
void workspace_remove_container_from_visual_stack_normal(struct workspace *ws, struct container *con);
void workspace_add_container_to_visual_stack_normal(struct workspace *ws, struct container *con);

void workspace_remove_container(struct workspace *ws, struct container *con);
void workspace_remove_container_from_focus_stack(struct workspace *ws, struct container *con);
void workspace_remove_independent_container(struct workspace *ws, struct container *con);

GArray *container_array2D_get_positions_array(GPtrArray2D *containers);
GArray *container_array_get_positions_array(GPtrArray *containers);
void workspace_repush(struct workspace *ws, struct container *con, int i);

bool workspace_sticky_contains_client(struct workspace *ws, struct client *client);

#endif /* WORKSPACE_H */
