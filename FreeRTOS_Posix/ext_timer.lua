timer = {}

local __ticks = 0
local __queue = {}
local __sort = function (a,b)
                    return a[1] < b[1]
               end

local __cb = function ()
    __ticks = __ticks + 1
    events.post('EVT_TIMER_OUT_START', 100)

    for i=1, #__queue do
        local t = __queue[i]
        local dt, cb = unpack(t)
        dt = dt - 1
        t[1] = dt
        if dt <= 0 then
            cb()
        end
    end

    for i=#__queue, 1, -1 do
        if __queue[i][1] <= 0 then
            table.remove(__queue, i)
        end
    end
end
events.listen('EVT_TIMER_IN_EXPIRED', __cb)
events.post('EVT_TIMER_OUT_START', 100)

function timer.cb (dt, cb)
    __queue[#__queue+1] = { dt/100, cb}
    table.sort(__queue, __sort)
end

function timer.fg (dt)
    local co = coroutine.running()
    timer.cb(dt, function ()
        assert(coroutine.resume(co))
    end)
    coroutine.yield()
end

function timer.bg (dt)
    local state = nil   -- nil, 'done', co
    coroutine.wrap(function ()
        timer.fg(dt)
        if state == nil then
            state = 'done'
        else
            assert(state ~= 'done')
            coroutine.resume(state)
        end
    end)()
    return {
        fg = function ()
            if state == nil then
                state = coroutine.running()
                coroutine.yield()
            else
                assert(state == 'done')
            end
        end
    }
end
