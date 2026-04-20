// ======================================================================
// \title  Payload.cpp
// \author michos
// \brief  cpp file for Payload component implementation class
// ======================================================================

#include "Components/Payload/Payload.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  Payload ::
    Payload(const char* const compName) :
      PayloadComponentBase(compName)
  {

  }

  Payload ::
    ~Payload()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void Payload ::
    MpuDataIn_handler(
        NATIVE_INT_TYPE portNum,
        const Components::Vector& acceleration
    )
  {
    // TODO
  }

}
