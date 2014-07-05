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
                coroutine.resume(_co)
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
