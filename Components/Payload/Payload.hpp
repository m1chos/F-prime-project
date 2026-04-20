// ======================================================================
// \title  Payload.hpp
// \author michos
// \brief  hpp file for Payload component implementation class
// ======================================================================

#ifndef Components_Payload_HPP
#define Components_Payload_HPP

#include "Components/Payload/PayloadComponentAc.hpp"

#include <array>
#include <cmath>

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

    static const U8 FILTER_SIZE = 10;
    static constexpr F32 HUM_LIMIT_LOW = 20.0; 
    static constexpr F32 HUM_LIMIT_HIGH = 60.0;


    std::array<F32, FILTER_SIZE> accDataArrayX;
    std::array<F32, FILTER_SIZE> accDataArrayY;
    std::array<F32, FILTER_SIZE> accDataArrayZ;
    std::array<F32, FILTER_SIZE> hum_data;

    Components::Vector avgAcceleration;
    Components::HumVector avgHumidity;
    
    F32 calculateAverage(const std::array<F32, FILTER_SIZE>& dataArray) const;

    Payload_Axis m_lastDominantAxis = Payload_Axis::UNKNOWN;
    Payload_Axis currentAxis = Payload_Axis::X;  
    
    Fw::On m_state;

    U32 m_sampleCounter;

    NATIVE_INT_TYPE m_counter = 0;
    NATIVE_INT_TYPE h_counter = 0;
    void StartPayload_cmdHandler(FwOpcodeType opCode,U32 cmdSeq, Fw::On on_off);

    Fw::Logic GreenLedState;
    Fw::Logic RedLedState;
    Fw::Logic BlueLedState;
    Fw::Logic YellowLedState;

      void MpuDataIn_handler(
        NATIVE_INT_TYPE portNum, 
        const Components::Vector &acceleration 
    );
 
      void HumDataIn_handler(
        NATIVE_INT_TYPE portNum, 
        const Components::HumVector &humidity 
    );

    void run_handler(
        NATIVE_INT_TYPE portNum, 
        NATIVE_UINT_TYPE context
      );

     void acceleration_led_handler();

     void humidity_led_handler();


  };

}

#endif
