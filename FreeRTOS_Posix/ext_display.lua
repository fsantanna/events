dofile '../ext_timer.lua'

display = {}

function display.cb_show (msg, dt, cb)
    --event.post('EVT_OUT_DISPLAY', msg)
    print(msg)
    local h = timer.cb_run_after(dt, function ()
                                --event.post('EVT_OUT_DISPLAY', '')
                                print('...')
                                cb()
                           end)
    return {
        cancel = function ()
            h.cancel()
            --event.post('EVT_OUT_DISPLAY', '')
            print('...')
        end,
    }
end

function display.fg_show (msg, dt)
    --event.post('EVT_OUT_DISPLAY', msg)
    print(msg)
    timer.fg_sleep(dt)
    --event.post('EVT_OUT_DISPLAY', '')
    print('...')
end

function display.bg_show (msg, dt)
    --event.post('EVT_OUT_DISPLAY', msg)
    print(msg)
    local f = timer.bg_sleep(dt)

    return {
        wait = function ()
            local ret = f.wait()
            if ret then
                --event.post('EVT_OUT_DISPLAY', '')
                print('...')
            end
            return ret
        end,
        cancel = function ()
            --event.post('EVT_OUT_DISPLAY', '')
            print('...')
            return f.cancel()
        end,
    }
end
