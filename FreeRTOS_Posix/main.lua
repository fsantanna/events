dofile '../ext_timer.lua'
dofile '../ext_display.lua'
--dofile '../ext_send.lua'

--[[
setmetatable(_G, {
    __index = function (t, k)
        error('global variables are not supported: "'..k..'"')
    end,
    __newindex = function (t, k, v)
        error('global variables are not supported: "'..k..'"')
    end,
})
]]

function bg (f)
    local state = nil   -- nil, 'done', 'cancelled', co
    local co = coroutine.create(function ()
        f()
        if state == nil then
            state = 'done'
        else
            assert(state ~= 'done')
            assert(coroutine.resume(state,true))
        end
    end)
    local ok, ret = coroutine.resume(co)    -- result of first coroutine yield
    assert(ok, ret)                         -- expects "ret.cancel()"
    return {
        wait = function ()
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
            assert(ret.cancel,'not implemented')
            ret.cancel()                    -- cancel from yielded object
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

local f = assert(loadfile'../app6.lua')
coroutine.wrap(f)()
