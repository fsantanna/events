dofile '../ext_timer.lua'

local main = coroutine.wrap(
    function ()
        timer.cb(1000, function() print'1000' end)
        timer.cb(2000, function()
            print'2000'
            timer.cb(2000, function ()
                print'4000'
            end)
        end)

        timer.fg(1000)
        print'>1000'
        local f = timer.bg(2000)
        timer.fg(1000)
        print'>2000'
        f.fg()
        print'>3000'
    end)
main()
