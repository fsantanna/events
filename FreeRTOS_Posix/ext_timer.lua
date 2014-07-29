function async_timer (v)
    events.post('EVT_TIMER_OUT_START', v)

    local _state    = 'active'  -- active, aborted, terminated
    local _awaiting = false     -- sync() awaiting?
    local _ret                  -- operation return value
    local _co                   -- coroutine calling "sync"

    local cb
    cb = function (v)
            if _state ~= 'aborted' then
                _state = 'terminated'   -- término normal
            end
            _ret = v
            events.unlisten(nil,cb)
            if _awaiting then
                assert(coroutine.resume(_co))
            end
         end
    events.listen('EVT_TIMER_IN_EXPIRED', cb)

    return {
        sync = function ()
            _awaiting = true
            if _state == 'active' then
                _co = coroutine.running()
                coroutine.yield()
            end
            return (_state~='aborted'), _ret
        end,
        abort = function ()
            _state = 'aborted'
            events.post('EVT_TIMER_OUT_STOP')
            cb()
        end,
    }
end

function sync_timer (v)
    return async_timer(v).sync()
end

-------

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
    local f = coroutine.wrap(function ()
        timer.fg(dt)
        if state == nil then
            state = 'done'
        else
            assert(state ~= 'done')
            coroutine.resume(state)
        end
    end)
    f()
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

--[[
-- Implementação mais simples de "sync_timer".
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
]]
