require "defaultConfig"

config.set_sloppy_focus(true)
config.set_borderpx(2)
config.set_gaps(20, 20)
config.set_root_color({0.3, 0.3, 0.3, 1.0})
config.set_focus_color({1.0, 0.0, 0.0, 1.0})
config.set_border_color({0.0, 0.0, 1.0, 1.0})

Tag_names = {"0:1", "1:2", "2:3", "3:4", "4:5", "5:6", "6:7", "7:8"}

-- where to put things
Rules = {
    -- {"termite", "termite", function(n) container.container_setsticky(n, true) end},
}

Layouts = {
    {"[M]", "master"},
    {"[]=", "two_pane"},
    {"||",  "monocle"},
    {"--",  "tmp" },
}

Default_layout = Layouts[1]

Monrules = {
    -- name mfact nmaster scale layout transform
    { "", 0.55, 1, 1, Layouts[1], Monitor_transformation.NORMAL },
}

Xkb_rules = {}
Repeat_rate = 25
Repeat_delay = 600
Termcmd = "/usr/bin/termite"

Mod = Mod1
-- maps (between 1 and 4)
Keys = {
    {"mod-S-Return",   function() action.spawn(Termcmd) end},
    {"mod-a",          function() action.set_nmaster(2) end},
    {"mod-x",          function() action.set_nmaster(1) end},
    {"mod-k",          function() action.focus_on_stack(-1) end},
    {"mod-j",          function() action.focus_on_stack(1) end},
    {"mod-S-j",        function() action.focus_on_hidden_stack(1) end},
    {"mod-S-k",        function() action.focus_on_hidden_stack(-1) end},
    {"mod-S-c",        function() action.kill() end},
    {"mod-S-q",        function() action.quit() end},
    {"mod-space",      function() Set_layout() end},
    {"mod-m",          function() Set_layout(1) end},
    {"mod-S-t",        function() Set_layout(2) end},
    {"mod-w",          function() Set_layout(3) end},
    {"mod-b",          function() action.toggle_consider_layer_shell() end},
    {"mod-S-w",        function() Set_layout(4) end},
    {"mod-S-h",        function() action.resize_main(-1/10) end},
    {"mod-S-l",        function() action.resize_main(1/10) end},
    {"mod-S-s",        function() action.write_this_overlay("tmp") end},
    {"mod-Return",     function() action.zoom() end},
    {"mod-1",          function() action.view(0) end},
    {"mod-2",          function() action.view(1) end},
    {"mod-3",          function() action.view(2) end},
    {"mod-4",          function() action.view(3) end},
    {"mod-5",          function() action.view(4) end},
    {"mod-6",          function() action.view(5) end},
    {"mod-7",          function() action.view(6) end},
    {"mod-8",          function() action.view(7) end},
    {"mod-9",          function() action.view(8) end},
    {"mod-S-1",        function() action.move_client_to_workspace(0) end},
    {"mod-S-2",        function() action.move_client_to_workspace(1) end},
    {"mod-S-3",        function() action.move_client_to_workspace(2) end},
    {"mod-S-4",        function() action.move_client_to_workspace(3) end},
    {"mod-S-5",        function() action.move_client_to_workspace(4) end},
    {"mod-S-6",        function() action.move_client_to_workspace(5) end},
    {"mod-S-7",        function() action.move_client_to_workspace(6) end},
    {"mod-S-8",        function() action.move_client_to_workspace(7) end},
    {"mod-S-9",        function() action.move_client_to_workspace(8) end},
    {"mod-r",          function() config.reload() end},
    {"mod-t",          function() action.set_floating(false)    end},
    -- {"m-period",            function() focusmon(1) end},
    -- {"m-comma",             function() focusmon(-1) end},
    -- {"m-a",                 function() action.set_tabcount(action.get_tabcount()+1) end},
    -- {"m-x",                 function() action.set_tabcount(action.get_tabcount()-1) end},
    -- {"m-d",                 function() incnmaster(-1) end},
    -- {"m-parenright",        function() tag(~0) end},
    -- {"m-greater",           function() tagmon(1) end},
    -- {"m-less",              function() tagmon(-1) end},
}

Buttons = {
    {Mod.." "..Btn_left,    function() action.move_resize(Cursor_mode.CUR_MOVE) end},
    {Mod.." "..Btn_right,   function() action.move_resize(Cursor_mode.CUR_RESIZE) end},
}
