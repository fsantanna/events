local t = display.cb_show('+A', 1000, function() print'-A' end)
t.cancel()
display.cb_show('+B', 2000, function()
    print'-B'
    display.cb_show('+C', 2000, function ()
        print'-C'
    end)
end)

timer.fg_sleep(6000)

display.fg_show('+D', 1000)
print'-D'
local f = display.bg_show('+E',2000)
assert(f.wait())
print'-E'

local f = display.bg_show('+F', 1000)
assert(f.cancel())
assert(f.wait(), 'cancelled')
