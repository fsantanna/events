timer = {}

local __ticks = 0
local __queue = {}
local __sort = function (a,b)
                    return a[1] < b[1]
               end

local __cb = function ()
    __ticks = __ticks + 1
    event.post('EVT_TIMER_OUT_START', 100)

    for i=1, #__queue do
        local t = __queue[i]
        local dt, cb, alive = unpack(t)
        dt = dt - 1
        t[1] = dt
        if (dt <= 0) and alive then
            t[3] = false    -- avoids re-execution on error
            cb()
        end
    end

    for i=#__queue, 1, -1 do
        if __queue[i][1] <= 0 then
            table.remove(__queue, i)
        end
    end
end
listener.add('EVT_TIMER_IN_EXPIRED', __cb)
event.post('EVT_TIMER_OUT_START', 100)

function timer.cb_run_after (dt, cb)
    local t = { dt/100, cb, true }
    __queue[#__queue+1] = t
    table.sort(__queue, __sort)
    return {
        cancel = function ()
                    t[3] = false    -- cancel timer
                  end
    }
end

function timer.fg_sleep (dt)
    local co = coroutine.running()
    local h = timer.cb_run_after(dt, function ()
        assert(coroutine.resume(co))
    end)
    coroutine.yield(h)
end

function timer.bg_sleep (dt)
    return bg(function()
        timer.fg_sleep(dt)
    end)
end
