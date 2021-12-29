#include "lib/lib_color.h"

#include "translationLayer.h"
#include "server.h"
#include "utils/coreUtils.h"
#include "color.h"
// 

static const struct luaL_Reg color_meta[] =
{
    {NULL, NULL},
};

static const struct luaL_Reg color_static_methods[] =
{
    {"new", lib_color_new},
    {NULL, NULL},
};

static const struct luaL_Reg color_static_setter[] =
{
    {NULL, NULL},
};

static const struct luaL_Reg color_static_getter[] =
{
    REGISTER_COLOR(AQUA),
    REGISTER_COLOR(AQUAMARINE),
    REGISTER_COLOR(AZURE),
    REGISTER_COLOR(BEIGE),
    REGISTER_COLOR(BISQUE),
    REGISTER_COLOR(BLACK),
    REGISTER_COLOR(BLANCHEDALMOND),
    REGISTER_COLOR(BLUE),
    REGISTER_COLOR(BLUEVIOLET),
    REGISTER_COLOR(BROWN),
    REGISTER_COLOR(BURLYWOOD),
    REGISTER_COLOR(CADETBLUE),
    REGISTER_COLOR(CHARTREUSE),
    REGISTER_COLOR(CHOCOLATE),
    REGISTER_COLOR(CORAL),
    REGISTER_COLOR(CORNSILK),
    REGISTER_COLOR(CRIMSON),
    REGISTER_COLOR(DARKBLUE),
    REGISTER_COLOR(DARKCYAN),
    REGISTER_COLOR(DARKGOLDENROD),
    REGISTER_COLOR(DARKGRAY),
    REGISTER_COLOR(DARKGREEN),
    REGISTER_COLOR(DARKKHAKI),
    REGISTER_COLOR(DARKMAGENTA),
    REGISTER_COLOR(DARKOLIVEGREEN),
    REGISTER_COLOR(DARKORANGE),
    REGISTER_COLOR(DARKORCHID),
    REGISTER_COLOR(DARKRED),
    REGISTER_COLOR(DARKSALMON),
    REGISTER_COLOR(DARKSEAGREEN),
    REGISTER_COLOR(DARKSLATEBLUE),
    REGISTER_COLOR(DARKSLATEGRAY),
    REGISTER_COLOR(DARKTURQUOISE),
    REGISTER_COLOR(DARKVIOLET),
    REGISTER_COLOR(DEEPPINK),
    REGISTER_COLOR(DEEPSKYBLUE),
    REGISTER_COLOR(DIMGRAY),
    REGISTER_COLOR(DODGERBLUE),
    REGISTER_COLOR(FIREBRICK),
    REGISTER_COLOR(FLORALWHITE),
    REGISTER_COLOR(FORESTGREEN),
    REGISTER_COLOR(FUCHSIA),
    REGISTER_COLOR(GAINSBORO),
    REGISTER_COLOR(GHOSTWHITE),
    REGISTER_COLOR(GOLD),
    REGISTER_COLOR(GOLDENROD),
    REGISTER_COLOR(GRAY),
    REGISTER_COLOR(GREEN),
    REGISTER_COLOR(GREENYELLOW),
    REGISTER_COLOR(HONEYDEW),
    REGISTER_COLOR(HOTPINK),
    REGISTER_COLOR(INDIANRED),
    REGISTER_COLOR(INDIGO),
    REGISTER_COLOR(IVORY),
    REGISTER_COLOR(KHAKI),
    REGISTER_COLOR(LAVENDER),
    REGISTER_COLOR(LAVENDERBLUSH),
    REGISTER_COLOR(LAWNGREEN),
    REGISTER_COLOR(LEMONCHIFFON),
    REGISTER_COLOR(LIGHTBLUE),
    REGISTER_COLOR(LIGHTCORAL),
    REGISTER_COLOR(LIGHTCYAN),
    REGISTER_COLOR(LIGHTGOLDENRODYELLOW),
    REGISTER_COLOR(LIGHTGRAY),
    REGISTER_COLOR(LIGHTGREEN),
    REGISTER_COLOR(LIGHTPINK),
    REGISTER_COLOR(LIGHTSALMON),
    REGISTER_COLOR(LIGHTSEAGREEN),
    REGISTER_COLOR(LIGHTSKYBLUE),
    REGISTER_COLOR(LIGHTSLATEGRAY),
    REGISTER_COLOR(LIGHTSTEELBLUE),
    REGISTER_COLOR(LIGHTYELLOW),
    REGISTER_COLOR(LIME),
    REGISTER_COLOR(LIMEGREEN),
    REGISTER_COLOR(LINEN),
    REGISTER_COLOR(MAGENTA),
    REGISTER_COLOR(MAROON),
    REGISTER_COLOR(MEDIUMAQUAMARINE),
    REGISTER_COLOR(MEDIUMBLUE),
    REGISTER_COLOR(MEDIUMORCHID),
    REGISTER_COLOR(MEDIUMPURPLE),
    REGISTER_COLOR(MEDIUMSEAGREEN),
    REGISTER_COLOR(MEDIUMSLATEBLUE),
    REGISTER_COLOR(MEDIUMSPRINGGREEN),
    REGISTER_COLOR(MEDIUMTURQUOISE),
    REGISTER_COLOR(MEDIUMVIOLETRED),
    REGISTER_COLOR(MIDNIGHTBLUE),
    REGISTER_COLOR(MINTCREAM),
    REGISTER_COLOR(MISTYROSE),
    REGISTER_COLOR(MOCCASIN),
    REGISTER_COLOR(NAVAJOWHITE),
    REGISTER_COLOR(NAVY),
    REGISTER_COLOR(OLDLACE),
    REGISTER_COLOR(OLIVE),
    REGISTER_COLOR(OLIVEDRAB),
    REGISTER_COLOR(ORANGE),
    REGISTER_COLOR(ORANGERED),
    REGISTER_COLOR(ORCHID),
    REGISTER_COLOR(PALEGOLDENROD),
    REGISTER_COLOR(PALEGREEN),
    REGISTER_COLOR(PALETURQUOISE),
    REGISTER_COLOR(PALEVIOLETRED),
    REGISTER_COLOR(PAPAYAWHIP),
    REGISTER_COLOR(PEACHPUFF),
    REGISTER_COLOR(PERU),
    REGISTER_COLOR(PINK),
    REGISTER_COLOR(PLUM),
    REGISTER_COLOR(POWDERBLUE),
    REGISTER_COLOR(PURPLE),
    REGISTER_COLOR(RED),
    REGISTER_COLOR(ROSYBROWN),
    REGISTER_COLOR(ROYALBLUE),
    REGISTER_COLOR(SADDLEBROWN),
    REGISTER_COLOR(SALMON),
    REGISTER_COLOR(SANDYBROWN),
    REGISTER_COLOR(SEAGREEN),
    REGISTER_COLOR(SEASHELL),
    REGISTER_COLOR(SIENNA),
    REGISTER_COLOR(SILVER),
    REGISTER_COLOR(SKYBLUE),
    REGISTER_COLOR(SLATEBLUE),
    REGISTER_COLOR(SLATEGRAY),
    REGISTER_COLOR(SLATEGREY),
    REGISTER_COLOR(SNOW),
    REGISTER_COLOR(SPRINGGREEN),
    REGISTER_COLOR(STEELBLUE),
    REGISTER_COLOR(TAN),
    REGISTER_COLOR(TEAL),
    REGISTER_COLOR(THISTLE),
    REGISTER_COLOR(TOMATO),
    REGISTER_COLOR(TURQUOISE),
    REGISTER_COLOR(VIOLET),
    REGISTER_COLOR(WHEAT),
    REGISTER_COLOR(WHITE),
    REGISTER_COLOR(WHITESMOKE),
    REGISTER_COLOR(YELLOW),
    REGISTER_COLOR(YELLOWGREEN),
    {NULL, NULL},
};

static const struct luaL_Reg color_m[] =
{
    {NULL, NULL},
};

static const struct luaL_Reg color_setter[] =
{
    {NULL, NULL},
};

static const struct luaL_Reg color_getter[] =
{
    {NULL, NULL},
};

void lua_load_color(lua_State *L)
{
    // 
    create_class(L,
            color_meta,
            color_static_methods,
            color_m,
            color_setter,
            color_getter,
            CONFIG_COLOR);

    create_static_accessor(L,
            "Color",
            color_static_methods,
            color_static_setter,
            color_static_getter);

    luaL_newlib(L, color_static_methods);
    lua_setglobal(L, "Color");
}

struct color check_color(lua_State *L, int narg)
{
    void **ud = luaL_checkudata(L, narg, CONFIG_COLOR);
    luaL_argcheck(L, ud != NULL, narg, "`color' expected");
    return *(struct color *)ud;
}

static void create_lua_color(lua_State *L, struct color color)
{
    struct color *user_color = lua_newuserdata(L, sizeof(struct color));
    *user_color = color;

    luaL_setmetatable(L, CONFIG_COLOR);
}

// static methods
int lib_color_new(lua_State *L)
{
    float alpha = luaL_checknumber(L, -1);
    lua_pop(L, 1);

    float blue = luaL_checknumber(L, -1);
    lua_pop(L, 1);

    float green = luaL_checknumber(L, -1);
    lua_pop(L, 1);

    float red = luaL_checknumber(L, -1);
    lua_pop(L, 1);

    struct color color = {
        .red = red,
        .green = green,
        .blue = blue, 
        .alpha = alpha,
    };
    create_lua_color(L, color);
    return 1;
}

// static getters
COLOR_GETTER(AQUA);
COLOR_GETTER(AQUAMARINE);
COLOR_GETTER(AZURE);
COLOR_GETTER(BEIGE);
COLOR_GETTER(BISQUE);
COLOR_GETTER(BLACK);
COLOR_GETTER(BLANCHEDALMOND);
COLOR_GETTER(BLUE);
COLOR_GETTER(BLUEVIOLET);
COLOR_GETTER(BROWN);
COLOR_GETTER(BURLYWOOD);
COLOR_GETTER(CADETBLUE);
COLOR_GETTER(CHARTREUSE);
COLOR_GETTER(CHOCOLATE);
COLOR_GETTER(CORAL);
COLOR_GETTER(CORNSILK);
COLOR_GETTER(CRIMSON);
COLOR_GETTER(DARKBLUE);
COLOR_GETTER(DARKCYAN);
COLOR_GETTER(DARKGOLDENROD);
COLOR_GETTER(DARKGRAY);
COLOR_GETTER(DARKGREEN);
COLOR_GETTER(DARKKHAKI);
COLOR_GETTER(DARKMAGENTA);
COLOR_GETTER(DARKOLIVEGREEN);
COLOR_GETTER(DARKORANGE);
COLOR_GETTER(DARKORCHID);
COLOR_GETTER(DARKRED);
COLOR_GETTER(DARKSALMON);
COLOR_GETTER(DARKSEAGREEN);
COLOR_GETTER(DARKSLATEBLUE);
COLOR_GETTER(DARKSLATEGRAY);
COLOR_GETTER(DARKTURQUOISE);
COLOR_GETTER(DARKVIOLET);
COLOR_GETTER(DEEPPINK);
COLOR_GETTER(DEEPSKYBLUE);
COLOR_GETTER(DIMGRAY);
COLOR_GETTER(DODGERBLUE);
COLOR_GETTER(FIREBRICK);
COLOR_GETTER(FLORALWHITE);
COLOR_GETTER(FORESTGREEN);
COLOR_GETTER(FUCHSIA);
COLOR_GETTER(GAINSBORO);
COLOR_GETTER(GHOSTWHITE);
COLOR_GETTER(GOLD);
COLOR_GETTER(GOLDENROD);
COLOR_GETTER(GRAY);
COLOR_GETTER(GREEN);
COLOR_GETTER(GREENYELLOW);
COLOR_GETTER(HONEYDEW);
COLOR_GETTER(HOTPINK);
COLOR_GETTER(INDIANRED);
COLOR_GETTER(INDIGO);
COLOR_GETTER(IVORY);
COLOR_GETTER(KHAKI);
COLOR_GETTER(LAVENDER);
COLOR_GETTER(LAVENDERBLUSH);
COLOR_GETTER(LAWNGREEN);
COLOR_GETTER(LEMONCHIFFON);
COLOR_GETTER(LIGHTBLUE);
COLOR_GETTER(LIGHTCORAL);
COLOR_GETTER(LIGHTCYAN);
COLOR_GETTER(LIGHTGOLDENRODYELLOW);
COLOR_GETTER(LIGHTGRAY);
COLOR_GETTER(LIGHTGREEN);
COLOR_GETTER(LIGHTPINK);
COLOR_GETTER(LIGHTSALMON);
COLOR_GETTER(LIGHTSEAGREEN);
COLOR_GETTER(LIGHTSKYBLUE);
COLOR_GETTER(LIGHTSLATEGRAY);
COLOR_GETTER(LIGHTSTEELBLUE);
COLOR_GETTER(LIGHTYELLOW);
COLOR_GETTER(LIME);
COLOR_GETTER(LIMEGREEN);
COLOR_GETTER(LINEN);
COLOR_GETTER(MAGENTA);
COLOR_GETTER(MAROON);
COLOR_GETTER(MEDIUMAQUAMARINE);
COLOR_GETTER(MEDIUMBLUE);
COLOR_GETTER(MEDIUMORCHID);
COLOR_GETTER(MEDIUMPURPLE);
COLOR_GETTER(MEDIUMSEAGREEN);
COLOR_GETTER(MEDIUMSLATEBLUE);
COLOR_GETTER(MEDIUMSPRINGGREEN);
COLOR_GETTER(MEDIUMTURQUOISE);
COLOR_GETTER(MEDIUMVIOLETRED);
COLOR_GETTER(MIDNIGHTBLUE);
COLOR_GETTER(MINTCREAM);
COLOR_GETTER(MISTYROSE);
COLOR_GETTER(MOCCASIN);
COLOR_GETTER(NAVAJOWHITE);
COLOR_GETTER(NAVY);
COLOR_GETTER(OLDLACE);
COLOR_GETTER(OLIVE);
COLOR_GETTER(OLIVEDRAB);
COLOR_GETTER(ORANGE);
COLOR_GETTER(ORANGERED);
COLOR_GETTER(ORCHID);
COLOR_GETTER(PALEGOLDENROD);
COLOR_GETTER(PALEGREEN);
COLOR_GETTER(PALETURQUOISE);
COLOR_GETTER(PALEVIOLETRED);
COLOR_GETTER(PAPAYAWHIP);
COLOR_GETTER(PEACHPUFF);
COLOR_GETTER(PERU);
COLOR_GETTER(PINK);
COLOR_GETTER(PLUM);
COLOR_GETTER(POWDERBLUE);
COLOR_GETTER(PURPLE);
COLOR_GETTER(RED);
COLOR_GETTER(ROSYBROWN);
COLOR_GETTER(ROYALBLUE);
COLOR_GETTER(SADDLEBROWN);
COLOR_GETTER(SALMON);
COLOR_GETTER(SANDYBROWN);
COLOR_GETTER(SEAGREEN);
COLOR_GETTER(SEASHELL);
COLOR_GETTER(SIENNA);
COLOR_GETTER(SILVER);
COLOR_GETTER(SKYBLUE);
COLOR_GETTER(SLATEBLUE);
COLOR_GETTER(SLATEGRAY);
COLOR_GETTER(SLATEGREY);
COLOR_GETTER(SNOW);
COLOR_GETTER(SPRINGGREEN);
COLOR_GETTER(STEELBLUE);
COLOR_GETTER(TAN);
COLOR_GETTER(TEAL);
COLOR_GETTER(THISTLE);
COLOR_GETTER(TOMATO);
COLOR_GETTER(TURQUOISE);
COLOR_GETTER(VIOLET);
COLOR_GETTER(WHEAT);
COLOR_GETTER(WHITE);
COLOR_GETTER(WHITESMOKE);
COLOR_GETTER(YELLOW);
COLOR_GETTER(YELLOWGREEN);
