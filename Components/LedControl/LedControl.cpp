// ======================================================================
// \title  LedControl.cpp
// \author Angelos Motsios
// \brief  cpp file for LedControl component implementation class
// ======================================================================

#include "Components/LedControl/LedControl.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  LedControl ::
    LedControl(const char* const compName) :
      LedControlComponentBase(compName)
  {

  }

  LedControl ::
    ~LedControl()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void LedControl ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    if (this->state != this->new_state) {
      if (this->isConnected_gpioSet_OutputPort(0)) {
        this->gpioSet_out(0, (Fw::On::ON == new_state) ? Fw::Logic::HIGH : Fw::Logic::LOW);
      }

      this->state = new_state;
      this->tlmWrite_led_state(this->state);
    }
  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void LedControl ::
    TOGGLE_LED_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        Fw::On on_off
    )
  {
    // Create a variable to represent the command response
    auto cmdResp = Fw::CmdResponse::OK;

    if (!on_off.isValid()) {
      this->log_WARNING_LO_InvalidLedStateArg(on_off);
      // Update command response with a validation error
      cmdResp = Fw::CmdResponse::VALIDATION_ERROR;
    } else {
      this->new_state = on_off;
      this->log_ACTIVITY_HI_SetLedState(new_state);
    }
    
    this->cmdResponse_out(opCode, cmdSeq, cmdResp);
  }

}
