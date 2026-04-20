module Components {
    @ Humidity Sensor
    passive component HumSensor {
          
        sync input port run: Svc.Sched
        output port HumDataOut: [4] HUM_data

        event HumInitFail severity warning high \
        format "Humidity Sensor not found"

        event HumInitSucc severity activity high \
        format "Humidity Sensor found"

        telemetry humidity: HumVector id 0 update always format "{}"


 
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
