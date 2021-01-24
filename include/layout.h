#ifndef LAYOUT_H
#define LAYOUT_H

#include <stdbool.h>
#include <lua.h>
#include <lauxlib.h>
#include "options.h"

struct bounds {
    float min_width;
    float max_width;
    float min_height;
    float max_height;
};

struct layout {
    const char *name;
    const char *symbol;
    // the amount of slave windows plus the master are (+1)
    int n;
    // the amount master windows
    int nmaster;
    int resize_dir;
    int lua_layout_ref;
    int lua_layout_index;
    int lua_layout_copy_data_ref;
    int lua_layout_original_copy_data_ref;
    int lua_layout_master_copy_data_ref;
    int lua_box_data_ref;

    struct bounds layout_bounds;
    struct bounds master_bounds;

    struct options options;
};

bool is_same_layout(struct layout layout, struct layout layout2);
int lua_copy_table(lua_State *L);
void copy_layout(struct layout *dest_lt, struct layout *src_lt);
void push_layout(struct layout lt_stack[static 2], struct layout lt);
struct layout get_default_layout();
void set_layout(lua_State *L, struct layout *lt);
void load_layout(lua_State *L, struct layout *lt, const char *layout_name);
void load_default_layout(lua_State *L, struct layout *lt);

extern struct layout default_layout;
#endif /* LAYOUT_H */
