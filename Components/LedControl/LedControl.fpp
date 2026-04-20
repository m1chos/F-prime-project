module Components {

    @ Will control RGB LED depending the values of MPU6050
    passive component LedControl {

        ###############################################################################
        # COMMANDS
        ###############################################################################
        
        @ Command to toggle the LED once
        sync command TOGGLE_LED(
            on_off: Fw.On @< Toggle led
        )

        ###############################################################################
        # EVENTS
        ###############################################################################

        event InvalidLedStateArg(BadArg: Fw.On) \
            severity warning low \
            format "Invalid Led State arguement: {}"
        
        event SetLedState(state: Fw.On) \
            severity activity high \
            format "Set Led State: {}"

        ###############################################################################
        # Telemetry
        ###############################################################################
        
        @ Led State
        telemetry led_state: Fw.On id 2 update always format "{}"

        ###############################################################################
        # PORTS
        ###############################################################################

        @ Port receiving calls from the rate group
        sync input port run: Svc.Sched

        @ Port sending calls to the GPIO driver
        output port gpioSet: Drv.GpioWrite

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}