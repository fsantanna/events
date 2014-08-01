dofile '../ext_timer.lua'

display = {}

function display.cb (msg, dt, cb)
    --events.post('EVT_OUT_DISPLAY', msg)
    print(msg)
    local h = timer.cb(dt, function ()
                                --events.post('EVT_OUT_DISPLAY', '')
                                print('...')
                                cb()
                           end)
    return {
        cancel = function ()
            h.cancel()
            --events.post('EVT_OUT_DISPLAY', '')
            print('...')
        end,
    }
end

function display.fg (msg, dt)
    --events.post('EVT_OUT_DISPLAY', msg)
    print(msg)
    timer.fg(dt)
    --events.post('EVT_OUT_DISPLAY', '')
    print('...')
end

function display.bg (msg, dt)
    --events.post('EVT_OUT_DISPLAY', msg)
    print(msg)
    local f = timer.bg(dt)

    return {
        wait = function ()
            local ret = f.wait()
            if ret then
                --events.post('EVT_OUT_DISPLAY', '')
                print('...')
            end
            return ret
        end,
        cancel = function ()
            --events.post('EVT_OUT_DISPLAY', '')
            print('...')
            return f.cancel()
        end,
    }
end
