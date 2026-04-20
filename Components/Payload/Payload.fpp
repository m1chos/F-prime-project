module Components {
    @ Payload Component for IMU processing
    active component Payload {

        ##########################################################################
        # Ports                                                                  #
        ##########################################################################
 
        async input port run: Svc.Sched

        async input port MpuDataIn: Components.MPU_data
        async input port HumDataIn: Components.HUM_data

        output port GreenLedOut: Drv.GpioWrite
        output port RedLedOut: Drv.GpioWrite
        output port BlueLedOut: Drv.GpioWrite
        output port YellowLedOut: Drv.GpioWrite

        ##########################################################################
        # Telemetry                                                              #
        ##########################################################################

        telemetry RedLedState: Fw.Logic id 21
        telemetry GreenLedState: Fw.Logic id 22
        telemetry BlueLedState: Fw.Logic id 23
        telemetry YellowLedState: Fw.Logic id 24

        telemetry avgAcc: Vector id 0
        telemetry avgHum: HumVector id 1
        
        telemetry State: Fw.On id 20

        ##########################################################################
        # Events                                                                 #
        ##########################################################################

        event DominantAxisChanged(axis: Axis) severity activity high format "Dominant axis changed to {}"

        event HumidityDanger(avghumidity: HumVector) severity activity high format "Humidity is on dangerous levels: {}"

        enum Axis{
            X,
            Y,
            Z,
            UNKNOWN
        }
       
        event PayloadState(On: Fw.On) severity activity high format "Payload state is now {}"

        event InvalidPayloadArg(BadArg: Fw.On) severity warning low format "Invalid Payload operation arguement: {}"  

        ##########################################################################
        # Commands                                                               #
        ##########################################################################
        sync command StartPayload(on_off: Fw.On) opcode 0

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