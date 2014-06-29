events.post('EVT_TIMER_OUT_START', 3000)

events.listen('EVT_TIMER_IN_EXPIRED',
    function ()
        --print'timer expired'
    end)


function timer (v)
    events.post('EVT_TIMER_OUT_START', v)
    local co = coroutine.running()
    local cb
    cb = function ()
            events.unlisten(nil,cb)
            coroutine.resume(co)
         end
    events.listen('EVT_TIMER_IN_EXPIRED', cb)
    coroutine.yield()
end

-- Crio uma "main" que chama a "timer" bloqueante que esconde as callbacks e corrotinas.

local main = coroutine.wrap(
    function ()
        print 'inicio'
        for i=1, 5 do
            print('i = '..i)
            timer(1000)
        end
        print 'fim'
    end)
main()
