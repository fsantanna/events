events.post('EVT_TIMER_OUT_START', 3000)

events.listen('EVT_TIMER_IN_EXPIRED',
    function ()
        --print'timer expired'
    end)


-- Crio uma "main" que chama a "timer" bloqueante que esconde as callbacks e corrotinas.

dofile '../ext_timer.lua'

local main = coroutine.wrap(
    function ()
        for i=1, 5 do
            print('i = '..i)
            sync_timer(1000)
        end
        print 'fim'

        local f = async_timer(10000)
        f.abort()
        print('f', f.sync())
    end)
main()

local sending = false

listen_and_spawn ('EVT_BUTTON_IN_PRESSED', function ()
    if sending then
        return
    end
    sending = true
    local send1 = async_send()
    local v2 = sync_send()
    local v1 = send1.sync()
    sending = false
end)
