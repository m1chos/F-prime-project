// ======================================================================
// \title  Payload.hpp
// \author michos
// \brief  hpp file for Payload component implementation class
// ======================================================================

#ifndef Components_Payload_HPP
#define Components_Payload_HPP

#include "Components/Payload/PayloadComponentAc.hpp"

namespace Components {

  class Payload :
    public PayloadComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct Payload object
      Payload(
          const char* const compName //!< The component name
      );

      //! Destroy Payload object
      ~Payload();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for MpuDataIn
      void MpuDataIn_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          const Components::Vector& acceleration
      );

  };

}

#endif
