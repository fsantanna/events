events = {
}

local listeners = {}

function events.listen (id, cb)
    local t = { id, cb }
    listeners[#listeners + 1] = t
end

function events.unlisten (id, cb)
    -- ao contrario por causa do remove
    for i=#listeners, 1, -1 do
        local t = listeners[i]
        if (t.id==id or id=='EVT_NONE') and (t.cb==nil or t.cb==cb) then
            table.remove(listeners, i)
        end
    end
end

function events.dispatch (id, param)
    -- guarda "cbs" em vez de chamar direto, pois uma "cb" pode chamar "unlisten"
    local cbs = {}
    for _, t in ipairs(listeners) do
        local id_, cb = unpack(t)
        if id == id_ then
            cbs[#cbs + 1] = cb
        end
    end
    for _, cb in ipairs(cbs) do
        cb(param)
    end
end
