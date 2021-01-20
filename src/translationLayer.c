#include "translationLayer.h"
#include "lib/actions/actions.h"
#include "lib/actions/libcontainer.h"
#include "lib/config/config.h"
#include "lib/config/localconfig.h"
#include "lib/layout/lib_layout.h"
#include "lib/info/info.h"
#include "parseConfig.h"
#include "tile/tile.h"

static const struct luaL_Reg action[] =
{
    {"arrange", lib_arrange},
    {"set_nmaster", set_nmaster},
    {"set_resize_direction", set_resize_direction},
    {"resize_main", resize_main},
    {"get_nmaster", get_nmaster},
    {"focus_on_hidden_stack", focus_on_hidden_stack},
    {"toggle_consider_layer_shell", toggle_consider_layer_shell},
    {"focus_on_stack", focus_on_stack},
    {"kill", lib_kill_client},
    {"move_resize", move_resize},
    {"move_client_to_workspace", lib_move_client_to_workspace},
    {"quit", lib_quit},
    {"load_layout", lib_load_layout},
    {"spawn", spawn},
    {"set_floating", set_floating},
    {"toggle_floating", toggle_floating},
    {"toggle_view", toggle_view},
    {"view", view},
    {"zoom", lib_zoom},
    {NULL, NULL},
};

static const struct luaL_Reg container[] =
{
    {"container_setsticky", container_setsticky}
};

static const struct luaL_Reg info[] =
{
    {"get_this_container_count", get_this_container_count},
    {"this_container_position", this_container_position},
    {NULL, NULL},
};

static const struct luaL_Reg config[] = 
{
    {"reload", reload_config},
    {"set_border_color", lib_set_border_color},
    {"set_borderpx", lib_set_borderpx},
    {"set_focus_color", lib_set_focus_color},
    {"set_gaps", lib_set_gaps},
    {"set_mod", lib_set_mod},
    {"set_root_color", lib_set_root_color},
    {"set_sloppy_focus", lib_set_sloppy_focus},
    {"set_repeat_rate", lib_set_repeat_rate},
    {"set_repeat_delay", lib_set_repeat_delay},
    {"set_default_layout", lib_set_default_layout},
    {"set_workspaces", lib_set_workspaces},
    {"set_rules", lib_set_rules},
    {"set_layouts", lib_set_layouts},
    {"set_monrules", lib_set_monrules},
    {"set_keybind", lib_set_keybinds},
    {"set_buttons", lib_set_buttons},
    {NULL, NULL},
};

static const struct luaL_Reg localconfig[] =
{
    {"set_arrange_by_focus", local_set_arrange_by_focus},
    {"set_border_color", local_set_border_color},
    {"set_borderpx", local_set_borderpx},
    {"set_focus_color", local_set_focus_color},
    {"set_gaps", local_set_gaps},
    {"set_sloppy_focus", local_set_sloppy_focus},
    {NULL, NULL},
};

static const struct luaL_Reg layout[] =
{
    {"set", set_layout},
    {NULL, NULL},
};

void load_libs(lua_State *L)
{
    luaL_newlib(L, action);
    lua_setglobal(L, "action");
    luaL_newlib(L, container);
    lua_setglobal(L, "container");
    luaL_newlib(L, info);
    lua_setglobal(L, "info");
    luaL_newlib(L, config);
    lua_setglobal(L, "config");
    luaL_newlib(L, localconfig);
    lua_setglobal(L, "lconfig");
    luaL_newlib(L, layout);
    lua_setglobal(L, "layout");
}
