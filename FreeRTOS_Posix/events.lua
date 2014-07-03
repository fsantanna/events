events = {
}

local listeners = {}

function events.listen (id, cb, isSpawn)
    local t = { id, cb, isSpawn }
    listeners[#listeners + 1] = t
end

function events.listen_and_spawn (id, cb)
    return events.listen(id, cb, true)
end

function events.unlisten (id, cb)
    -- percorre ao contrario por causa do remove
    for i=#listeners, 1, -1 do
        local t = listeners[i]
        if (t.id==id or id=='EVT_NONE') and (t.cb==nil or t.cb==cb) then
            table.remove(listeners, i)
        end
    end
end

-- Dispatch guarda callbacks a serem chamadas em "__cbs" (em vez de chamá-las
-- diretamente), pois uma "cb" pode chamar "unlisten" e alterar a tabela sendo
-- percorrida.
local __cbs = {}

function events.dispatch (id, param)
    for _, t in ipairs(listeners) do
        local id_, cb, isSpawn = unpack(t)
        if id == id_ then
            if isSpawn then
                cb = coroutine.wrap(cb)
            end
            __cbs[#__cbs + 1] = cb
        end
    end
    for i=1, #__cbs do
        __cbs[i](param)
        __cbs[i] = nil
    end
end
