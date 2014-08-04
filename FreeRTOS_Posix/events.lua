-- internal API
events = {
}

-- external APIs (from the manual)
listener = {
    -- add
    -- add_lightweight
    -- remove
}
event = {
    -- post (set from ext_lua.c)
}

local listeners = {}

function events.listen (id, cb, isSpawn)
    local t = { id, cb, isSpawn }
    listeners[#listeners + 1] = t
end

function events.listen_and_spawn (id, cb)
--[[
    local var_env = debug.getupvalue(cb, 2)
    if var_env then
        local var = debug.getupvalue(cb, 1)
        error('callback cannot hold upvalues: "'..var..'"')
    end
]]
    return events.listen(id, cb, true)
end

function events.unlisten (id, cb)
    -- percorre ao contrario por causa do remove
    for i=#listeners, 1, -1 do
        local t = listeners[i]
        local id_, cb_, isSpawn = unpack(t)
        if (id_==id or id=='EVT_NONE') and (cb==nil or cb_==cb) then
            table.remove(listeners, i)
        end
    end
end

listener.add             = events.listen_and_spawn
listener.add_lightweight = events.listen
listener.remove          = events.unlisten

-- Dispatch guarda callbacks a serem chamadas em "__cbs" (em vez de chamá-las
-- diretamente), pois uma "cb" pode chamar "unlisten" e alterar a tabela sendo
-- percorrida.
local __cbs = {}

function events.dispatch (id, param)
    local I = 0
    for _, t in ipairs(listeners) do
        local id_, cb, isSpawn = unpack(t)
        if id == id_ then
            I = I + 1
            __cbs[I] = t
        end
    end
    for i=1, I do
        local t = __cbs[i]
        local id, cb, isSpawn = unpack(t)
        if isSpawn then
            cb = coroutine.wrap(cb)
        end
        cb(param, select(4,unpack(t)))
    end
end
