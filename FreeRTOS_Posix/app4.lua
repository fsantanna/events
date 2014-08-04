function cb_timer ()
    print 'timer expired'
    event.post('EVT_TIMER_OUT_START', 1000)
end
listener.add('EVT_TIMER_IN_EXPIRED', cb_timer)
event.post('EVT_TIMER_OUT_START', 1000)
