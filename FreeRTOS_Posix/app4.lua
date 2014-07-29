function cb_timer ()
    print 'timer expired'
    events.post('EVT_TIMER_OUT_START', 1000)
end
events.listen('EVT_TIMER_IN_EXPIRED', cb_timer)
events.post('EVT_TIMER_OUT_START', 1000)
