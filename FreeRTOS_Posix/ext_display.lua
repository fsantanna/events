dofile '../ext_timer.lua'

display = {}

function display.cb (msg, dt, cb)
    --event.post('EVT_OUT_DISPLAY', msg)
    print(msg)
    local h = timer.cb(dt, function ()
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

function display.fg (msg, dt)
    --event.post('EVT_OUT_DISPLAY', msg)
    print(msg)
    timer.fg(dt)
    --event.post('EVT_OUT_DISPLAY', '')
    print('...')
end

function display.bg (msg, dt)
    --event.post('EVT_OUT_DISPLAY', msg)
    print(msg)
    local f = timer.bg(dt)

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
