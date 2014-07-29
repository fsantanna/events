dofile '../ext_timer.lua'
dofile '../ext_send.lua'

--[[
setmetatable(_G, {
    __index = function (t, k)
        error('global variables are not supported: "'..k..'"')
    end,
    __newindex = function (t, k, v)
        error('global variables are not supported: "'..k..'"')
    end,
})
]]

--events.post('EVT_TIMER_OUT_START', 3000)

events.listen('EVT_TIMER_IN_EXPIRED',
    function ()
        --print'timer expired'
    end)


-- Crio uma "main" que chama a "timer" bloqueante que esconde as callbacks e corrotinas.

local G = { sending=false }
--local sending = false

events.listen_and_spawn('EVT_BUTTON_IN_PRESSED', function (_,_,G)
    if G.sending then
        return
    end
    G.sending = true
    local send1 = async_send('1')
    local _,v2 = sync_send('22')
    local _,v1 = send1.sync()
    print('1 + 22 = 3', v1 + v2)
    assert(v1+v2 == 3)
    G.sending = false
end, nil,G)

local main = coroutine.wrap(
    function ()
--[[
        for i=1, 5 do
            print('i = '..i)
            sync_timer(1000)
        end
        print 'fim'

        local f = async_timer(10000)
        f.abort()
        print('f', f.sync())
        events.post('EVT_BUTTON_IN_PRESSED')
        events.post('EVT_LUA_OUT_STRING',
                    "events.post('EVT_BUTTON_IN_PRESSED')")
]]
        timer.cb(1000, function() print'1000' end)
        timer.cb(2000, function()
            print'2000'
            timer.cb(2000, function ()
                print'4000'
            end)
        end)

        timer.fg(1000)
        print'>1000'
        timer.fg(1000)
        print'>2000'

        local f = timer.bg(1000)
        f.fg()
        print'>3000'
    end)
main()
