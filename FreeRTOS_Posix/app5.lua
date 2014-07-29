dofile '../ext_timer.lua'

local main = coroutine.wrap(
    function ()
        local t = timer.cb(1000, function() print'-1000' end)
        t.cancel()
        timer.cb(2000, function()
            print'-2000'
            timer.cb(2000, function ()
                print'-4000'
            end)
        end)

        timer.fg(1000)
        print'+1000'
        local f = timer.bg(2000)
        timer.fg(1000)
        print'+2000'
        assert(f.fg_done())
        print'+3000'

        local f = timer.bg(1000)
        assert(f.cancel())
        assert(f.fg_done(), 'cancelled')
    end)
main()
