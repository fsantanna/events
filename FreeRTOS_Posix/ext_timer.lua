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
events.listen('EVT_TIMER_IN_EXPIRED', __cb)
events.post('EVT_TIMER_OUT_START', 100)

function timer.cb (dt, cb)
    local t = { dt/100, cb, true }
    __queue[#__queue+1] = t
    table.sort(__queue, __sort)
    return {
        cancel = function ()
                    t[3] = false    -- cancel timer
                  end
    }
end

function timer.fg (dt)
    local co = coroutine.running()
    local h = timer.cb(dt, function ()
        assert(coroutine.resume(co))
    end)
    coroutine.yield(h)
end

function timer.bg (dt)
    local state = nil   -- nil, 'done', 'cancelled', co
    local co = coroutine.create(function ()
        timer.fg(dt)
        if state == nil then
            state = 'done'
        else
            assert(state ~= 'done')
            assert(coroutine.resume(state,true))
        end
    end)
    local ok, h = coroutine.resume(co)      -- result of timer.cb()
    assert(ok, h)
    return {
        fg_done = function ()
            if state == nil then
                state = coroutine.running()
                return coroutine.yield()    -- wait result
            elseif state == 'done' then
                return true                 -- already terminated ok
            else
                assert(state == 'cancelled')
                return false                -- already cancelled
            end
        end,
        cancel = function ()
            h.cancel()                      -- cancel cb
            if state == 'done' then
                return false                -- already done
            elseif state == 'cancelled' then
                return true                 -- already cancelled, but ok
            elseif state == nil then
                state = 'cancelled'
                return true                 -- cancelled
            else
                assert( type(state)=='thread' )
                assert(coroutine.resume(state,false))
                state = 'cancelled'
                return true                 -- cancelled
            end
        end,
    }
end
