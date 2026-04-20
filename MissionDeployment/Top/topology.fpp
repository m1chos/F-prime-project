module MissionDeployment {

  # ----------------------------------------------------------------------
  # Symbolic constants for port numbers
  # ----------------------------------------------------------------------

    enum Ports_RateGroups {
      rateGroup1
      rateGroup2
    }

    enum Ports_StaticMemory {
      framer
      deframer
      deframing
    }

  topology MissionDeployment {

    # ----------------------------------------------------------------------
    # Instances used in the topology
    # ----------------------------------------------------------------------

    instance cmdDisp
    instance commDriver
    instance deframer
    instance eventLogger
    instance fatalAdapter
    instance fatalHandler
    instance framer
    instance rateDriver
    instance rateGroup1
    instance rateGroup2
    instance rateGroupDriver
    instance staticMemory
    instance systemResources
    instance textLogger
    instance timeHandler
    instance tlmSend

    # GPIO Instances
    instance GreenLedOut
    instance RedLedOut
    instance BlueLedOut
    instance YellowLedOut
    instance r_gpioDriver
    instance g_gpioDriver
    instance b_gpioDriver
    instance y_gpioDriver

    # MPU 
    instance mpu

    instance payload
    # ----------------------------------------------------------------------
    # Pattern graph specifiers
    # ----------------------------------------------------------------------

    command connections instance cmdDisp

    event connections instance eventLogger

    telemetry connections instance tlmSend

    text event connections instance textLogger

    time connections instance timeHandler

    # ----------------------------------------------------------------------
    # Direct graph specifiers
    # ----------------------------------------------------------------------

    connections RateGroups {
      # Block driver
      rateDriver.CycleOut -> rateGroupDriver.CycleIn

      # Rate group 1
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup1] -> rateGroup1.CycleIn
      rateGroup1.RateGroupMemberOut[0] -> commDriver.schedIn
      rateGroup1.RateGroupMemberOut[1] -> tlmSend.Run
      rateGroup1.RateGroupMemberOut[2] -> systemResources.run
      rateGroup1.RateGroupMemberOut[3] -> mpu.run
      rateGroup1.RateGroupMemberOut[4] -> payload.run

      # Rate group 2
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup2] -> rateGroup2.CycleIn
      rateGroup2.RateGroupMemberOut[0] -> RedLedOut.run
      rateGroup2.RateGroupMemberOut[1] -> GreenLedOut.run
      rateGroup2.RateGroupMemberOut[2] -> BlueLedOut.run
      rateGroup2.RateGroupMemberOut[3] -> YellowLedOut.run

    }

    connections FaultProtection {
      eventLogger.FatalAnnounce -> fatalHandler.FatalReceive
    }

    connections Downlink {

      tlmSend.PktSend -> framer.comIn
      eventLogger.PktSend -> framer.comIn

      framer.framedAllocate -> staticMemory.bufferAllocate[Ports_StaticMemory.framer]
      framer.framedOut -> commDriver.$send

      commDriver.deallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.framer]

    }
    
    connections Uplink {

      commDriver.allocate -> staticMemory.bufferAllocate[Ports_StaticMemory.deframer]
      commDriver.$recv -> deframer.framedIn
      deframer.framedDeallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.deframer]

      deframer.comOut -> cmdDisp.seqCmdBuff
      cmdDisp.seqCmdStatus -> deframer.cmdResponseIn

      deframer.bufferAllocate -> staticMemory.bufferAllocate[Ports_StaticMemory.deframing]
      deframer.bufferDeallocate -> staticMemory.bufferDeallocate[Ports_StaticMemory.deframing]
      
    }

    connections MissionDeployment {
      # Add here connections to user-defined components
      RedLedOut.gpioSet   -> r_gpioDriver.gpioWrite
      GreenLedOut.gpioSet -> g_gpioDriver.gpioWrite
      BlueLedOut.gpioSet  -> b_gpioDriver.gpioWrite
      
      payload.RedLedOut   -> r_gpioDriver.gpioWrite
      payload.GreenLedOut-> g_gpioDriver.gpioWrite
      payload.BlueLedOut  -> b_gpioDriver.gpioWrite

      mpu.MpuDataOut[0] -> payload.MpuDataIn

      cmdDisp.compCmdSend[20] -> payload.cmdIn
      payload.cmdRegOut -> cmdDisp.compCmdReg
      payload.cmdResponseOut -> cmdDisp.compCmdStat

      payload.tlmOut -> tlmSend.TlmRecv
      payload.logOut -> eventLogger.LogRecv
      payload.timeCaller -> timeHandler.timeGetPort
     }

  }

}
