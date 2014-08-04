local QUEUE = {}

listener.add('EVT_SEND_IN_ACK', function (len)
    local req = table.remove(QUEUE, 1)
    req.cb(len)
    if #QUEUE > 0 then
        req = QUEUE[#QUEUE]
        event.post('EVT_SEND_OUT_START', req.val)
    end
end)

function enqueue (req)
    QUEUE[#QUEUE+1] = req
    if #QUEUE == 1 then
        event.post('EVT_SEND_OUT_START', req.val)
    end
end

function dequeue (req)
    for i, old in ipairs(QUEUE) do
        if req == old then
            if i > 1 then
                table.remove(QUEUE, i)
            else
                -- TODO: request CANCEL?
            end
            return
        end
    end
end

function async_send (v)
    local _state    = 'active'  -- active, aborted, terminated
    local _awaiting = false     -- sync() awaiting?
    local _ret                  -- operation return value
    local _co                   -- coroutine calling "sync"
    local _req

    local cb
    cb = function (ret)
        if _state ~= 'aborted' then
            _state = 'terminated'   -- término normal
        end
        _ret = ret
        events.unlisten(nil,cb)
        if _awaiting then
            assert(coroutine.resume(_co))
        end
     end

    _req = { val=v, cb=cb }
    enqueue(_req)

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
            dequeue(_req)
            cb()
        end,
    }
end

function sync_send (v)
    return async_send(v).sync()
end
