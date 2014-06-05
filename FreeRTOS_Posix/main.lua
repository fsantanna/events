events.post('E_EVT_TIMER_OUT_START', 3000)

events.listen('E_EVT_TIMER_IN_EXPIRED',
    function ()
        print'timer expired'
    end)
