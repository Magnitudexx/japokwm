#include "keybinding.h"

#include <string.h>
#include <X11/Xlib.h>

#include "input_manager.h"
#include "server.h"
#include "tile/tileUtils.h"
#include "utils/parseConfigUtils.h"
#include "stringop.h"
#include "workspace.h"
#include "monitor.h"

const char *mods[8] = {"Shift_L", "Caps_Lock", "Control_L", "Alt_L", "", "", "Super_L", "ISO_Level3_Shift"};
const char *modkeys[4] = {"Alt_L", "Num_Lock", "ISO_Level3_Shift", "Super_L"};
const char *mouse[3] = {"Pointer_Button1", "Pointer_Button2", "Pointer_Button3"};

static bool sym_is_modifier(const char *sym);
static size_t modifiers;
/*
 * convert mod to mask
 */
inline static unsigned int mod_to_mask(unsigned int x)
{
    return 1 << x;
}

static void mod_to_string(char **dest, unsigned int mod)
{
    for (int i = 0; i < LENGTH(mods); i++) {
        modifiers = mod;
        if ((mod & mod_to_mask(i)) != 0) {
            append_string(dest, mods[i]);
            append_string(dest, "-");
        }
    }
}

static void mod_to_binding(char **dest, int mods)
{
    mod_to_string(dest, mods);
}

static void sym_to_binding(char **dest, int sym)
{
    append_string(dest, XKeysymToString(sym));
}

// this function converts a string to a xkeysym string element
static char *resolve_keybind_element(struct options *options, const char *bind)
{
    if (strcmp(bind, "mod") == 0) {
        char *resolved = strdup(modkeys[options->modkey]);
        return resolved;
    }
    if (strcmp(bind, "M1") == 0) {
        char *resolved = strdup(mouse[0]);
        return resolved;
    }
    if (strcmp(bind, "M2") == 0) {
        char *resolved = strdup(mouse[1]);
        return resolved;
    }
    if (strcmp(bind, "M3") == 0) {
        char *resolved = strdup(mouse[2]);
        return resolved;
    }
    if (strcmp(bind, "C") == 0) {
        char *resolved = strdup("Control_L");
        return resolved;
    }
    if (strcmp(bind, "S") == 0) {
        char *resolved = strdup("Shift_L");
        return resolved;
    }
    char *resolved = strdup(bind);
    return resolved;
}

char *sort_keybinding_element(struct options *options, const char *binding_element)
{
    GPtrArray *bindarr = split_string(binding_element, "-");

    for (int i = 0; i < bindarr->len; i++) {
        char *bind_atom = g_ptr_array_index(bindarr, i);

        char *resolved_bind_atom = resolve_keybind_element(options, bind_atom);
        free(bind_atom);
        bind_atom = resolved_bind_atom;

        g_ptr_array_index(bindarr, i) = bind_atom;
    }

    GPtrArray *mods = g_ptr_array_copy(bindarr, NULL, NULL);

    char *non_modifier = NULL;
    // most of the time the modifier is at the back so we loop backwards as an
    // optimization and there can only be one non modifier in a binding element
    // so we break instantly if found
    for (int i = mods->len-1; i >= 0; i--) {
        char *bind_atom = g_ptr_array_index(mods, i);

        if (sym_is_modifier(bind_atom))
            continue;
        non_modifier = strdup(bind_atom);
        g_ptr_array_remove_index_fast(mods, i);
        break;
    }

    g_ptr_array_sort(mods, cmp_strptr);

    if (non_modifier) {
        g_ptr_array_add(mods, non_modifier);
    }

    char *result = join_string((const char **)mods->pdata, mods->len, "-");

    if (non_modifier) {
        free(non_modifier);
    }
    g_ptr_array_free(bindarr, true);
    return result;
}

char *sort_keybinding(struct options *options, const char *binding)
{
    GPtrArray *bindarr = split_string(binding, " ");

    for (int i = 0; i < bindarr->len; i++) {
        char *bind_el = g_ptr_array_index(bindarr, i);
        char *res = sort_keybinding_element(options, bind_el);
        free(bind_el);
        g_ptr_array_index(bindarr, i) = res;
    }

    char *res = join_string((const char **)bindarr->pdata, bindarr->len, " ");

    g_ptr_array_free(bindarr, true);

    return res;
}

static bool is_same_keybind_element(struct layout *lt, const char *bind, const char *bind2)
{
    bool ret_val = false;
    if (strcmp(bind, bind2) == 0) {
        return true;
    }

    return ret_val;
}

struct keybinding *has_keybind_same_existing_elements(
        GPtrArray *keybindings,
        GPtrArray *registered_key_combos)
{
    char *binding = join_string((const char **)registered_key_combos->pdata, registered_key_combos->len, " ");

    struct keybinding *ret_keybinding = NULL;
    // TODO this basically uses linear search which is bad keybindings are
    // already sorted
    for (int i = 0; i < keybindings->len; i++) {
        struct keybinding *keybind = g_ptr_array_index(keybindings, i);
        if (strcmp(binding, keybind->binding) == 0) {
            ret_keybinding = keybind; 
            break;
        }
    }
    return ret_keybinding;
}

bool has_keybind_same_amount_of_elements(GPtrArray *registered_key_combos, const char *bind)
{
    GPtrArray *bind2combos = split_string(bind, " ");

    bool same_length = bind2combos->len == registered_key_combos->len;
    g_ptr_array_free(bind2combos, true);
    return same_length;
}

// WARNING: This function will change the state of the windowmanager that means that
// certain things will be freed so don't trust your local variables that were
// assigned before calling this function anymore. Global variables should be
// fine. They might contain a different value after calling this function thou.
static bool process_binding(lua_State *L, const char *bind, GPtrArray *keybindings_ptr)
{
    // calling arbitrary lua functions will change the state of the
    // windowmanager which may lead to the keybindings array to be freed by one
    // of those pesky functions. To protect ourself from that we create a copy.
    GPtrArray *keybindings = g_ptr_array_copy(keybindings_ptr, copy_keybinding, NULL);

    bool handled = false;

    struct keybinding *keybinding = has_keybind_same_existing_elements(
            keybindings,
            server.registered_key_combos
            );
    if (!keybinding)
        return false;

    handled = true;
    list_clear(server.registered_key_combos, free);
    lua_rawgeti(L, LUA_REGISTRYINDEX, keybinding->lua_func_ref);
    lua_call_safe(L, 0, 0, 0);
    server_allow_reloading_config();

    g_ptr_array_unref(keybindings);
    return handled;
}

struct keybinding *create_keybinding(const char *binding, int lua_func_ref)
{
    struct keybinding *keybinding = calloc(1, sizeof(*keybinding));
    keybinding->binding = strdup(binding);
    keybinding->lua_func_ref = lua_func_ref;
    return keybinding;
}

void destroy_keybinding(struct keybinding *keybinding)
{
    free(keybinding->binding);
    if (keybinding->lua_func_ref > 0) {
        luaL_unref(L, LUA_REGISTRYINDEX, keybinding->lua_func_ref);
    }
    free(keybinding);
}

void destroy_keybinding0(void *keybinding)
{
    destroy_keybinding(keybinding);
}

static int millisec_get_available_seconds(int milli_sec)
{
    int available_seconds = milli_sec/1000;
    return available_seconds;
}

static long millisec_to_nanosec(long milli_sec)
{
    return milli_sec * 100000;
}

static void reset_keycombo_timer(timer_t timer)
{
    struct monitor *m = server_get_selected_monitor();
    struct workspace *ws = monitor_get_active_workspace(m);
    struct layout *lt = workspace_get_layout(ws);
    long timeout = lt->options->key_combo_timeout;

    if (wl_event_source_timer_update(server.combo_timer_source, timeout) < 0) {
        printf("failed to disarm key repeat timer \n");
    }
}

static bool sym_is_modifier(const char *sym)
{
    for (int i = 0; i < LENGTH(mods); i++) {
        const char *mod = mods[i];
        if (strcmp(mod, sym) == 0) {
            return true;
        }
    }
    return false;
}

void *copy_keybinding(const void *keybinding_ptr, void *user_data)
{
    const struct keybinding *keybinding = keybinding_ptr;
    lua_rawgeti(L, LUA_REGISTRYINDEX, keybinding->lua_func_ref);
    int ref = luaL_ref(L, LUA_REGISTRYINDEX);
    struct keybinding *kb_dup = create_keybinding(keybinding->binding, ref);
    return kb_dup;
}

int cmp_keybinding(const void *keybinding1, const void *keybinding2)
{
    const struct keybinding *k1 = *(void **)keybinding1;
    const struct keybinding *k2 = *(void **)keybinding2;
    int ret_val = cmp_str(k1->binding, k2->binding);
    return ret_val;
}

char *mod_to_keybinding(int mods, int sym)
{
    char *mod_bind = strdup("");
    mod_to_binding(&mod_bind, mods);
    char *sym_bind = strdup("");
    sym_to_binding(&sym_bind, sym);

    char *bind = g_strconcat(mod_bind, sym_bind, NULL);

    free(mod_bind);
    free(sym_bind);

    return bind;
}

bool handle_keyboard_key(const char *bind)
{
    reset_keycombo_timer(server.combo_timer_source);

    struct monitor *m = server_get_selected_monitor();
    struct workspace *ws = monitor_get_active_workspace(m);
    struct layout *lt = workspace_get_layout(ws);

    char *sorted_bind = sort_keybinding(lt->options, bind);
    debug_print("bind: %s\n", sorted_bind);

    g_ptr_array_add(server.registered_key_combos, strdup(sorted_bind));
    if (!has_keybind_same_existing_elements(lt->options->keybindings, server.registered_key_combos)) {
        list_clear(server.registered_key_combos, free);
        g_ptr_array_add(server.registered_key_combos, strdup(sorted_bind));

        // try again with no registered key combos
        if (!has_keybind_same_existing_elements(lt->options->keybindings, server.registered_key_combos)) {
            return false;
        }
    }

    process_binding(L, sorted_bind, lt->options->keybindings);
    return true;
}

bool key_state_has_modifiers(size_t mods)
{
    return modifiers & mods;
}
