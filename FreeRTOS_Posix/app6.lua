dofile '../ext_display.lua'

local main = coroutine.wrap(
    function ()
        local t = display.cb('+A', 1000, function() print'-A' end)
        t.cancel()
        display.cb('+B', 2000, function()
            print'-B'
            display.cb('+C', 2000, function ()
                print'-C'
            end)
        end)

        timer.fg(6000)

        display.fg('+D', 1000)
        print'-D'
        local f = display.bg('+E',2000)
        assert(f.fg_done())
        print'-E'

        local f = display.bg('+F', 1000)
        assert(f.cancel())
        assert(f.fg_done(), 'cancelled')
    end)
main()
