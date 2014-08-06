local t = timer.cb_run_after(1000, function() print'-1000' end)
t.cancel()
timer.cb_run_after(2000, function()
    print'-2000'
    timer.cb_run_after(2000, function ()
        print'-4000'
    end)
end)

timer.fg_sleep(1000)
print'+1000'
local f = timer.bg_sleep(2000)
timer.fg_sleep(1000)
print'+2000'
assert(f.wait())
print'+3000'

local f = timer.bg_sleep(1000)
assert(f.cancel())
assert(f.wait(), 'cancelled')
