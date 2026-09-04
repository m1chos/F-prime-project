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
  Payload :: Payload(const char* const compName) : PayloadComponentBase(compName)
  {

  }
  //Destructor
  Payload :: ~Payload() 
  {
    
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void Payload::run_handler(NATIVE_INT_TYPE portNum, NATIVE_UINT_TYPE context)
  {
      this->tlmWrite_State(this->m_state);

      if (this->m_state == Fw::On::ON) {
          this->BlueLedState = Fw::Logic::HIGH;
      }else {
          this->BlueLedState = Fw::Logic::LOW;
      }

      if (this->isConnected_BlueLedOut_OutputPort(0)) {
          this->BlueLedOut_out(0, this->BlueLedState);
      }
    acceleration_led_handler();
    humidity_led_handler();

  }

    F32 Payload::calculateAverage(
    const std::array<F32, FILTER_SIZE>& dataArray,
    U8 sampleCount
    ) const {
    if (sampleCount == 0) {
        return 0.0;
    }

    F32 sum = 0.0;

    for (PlatformSizeType i = 0; i < sampleCount; ++i) {
        sum += dataArray[i];
    }

    return sum / sampleCount;
}

  void Payload::MpuDataIn_handler(NATIVE_INT_TYPE portNum, const Components::Vector &acceleration)
  {
         
    if (this->m_state == Fw::On::ON) {
           
      if (this->RedLedState == Fw::Logic::LOW) {
          this->RedLedState = Fw::Logic::HIGH;
      }else {
          this->RedLedState = Fw::Logic::LOW;
        }

   #ifdef _BOARD_RPIPICO
    RedLedOut_out(0, RedLedState);
   #endif
      this->tlmWrite_RedLedState(RedLedState);
      
      
      
        this->accDataArrayX[this->m_counter] = acceleration[0];
        this->accDataArrayY[this->m_counter] = acceleration[1];
        this->accDataArrayZ[this->m_counter] = acceleration[2];

        if (this->m_accSampleCount < FILTER_SIZE) {
            this->m_accSampleCount++;
        }

        this->m_counter = (this->m_counter + 1) % FILTER_SIZE;

        avgAcceleration[0] = calculateAverage(this->accDataArrayX, this->m_accSampleCount);
        avgAcceleration[1] = calculateAverage(this->accDataArrayY, this->m_accSampleCount);
        avgAcceleration[2] = calculateAverage(this->accDataArrayZ, this->m_accSampleCount);
      this->tlmWrite_avgAcc(avgAcceleration);
      }  
  }

void Payload::acceleration_led_handler() {      

    currentAxis = Payload_Axis::X;  
    F32 maxValue = fabs(avgAcceleration[0]);

    if (fabs(avgAcceleration[1]) > maxValue) {
      currentAxis = Payload_Axis::Y;
      maxValue = fabs(avgAcceleration[1]);
    }
    
    if (fabs(avgAcceleration[2]) > maxValue) {
      currentAxis = Payload_Axis::Z;
      maxValue = fabs(avgAcceleration[2]);
    }

   if (this->m_state == Fw::On::ON) {
      if (currentAxis == Payload_Axis::Z) {
        GreenLedState = Fw::Logic::HIGH;
      } else {
        GreenLedState = Fw::Logic::LOW;
      }
  #ifdef _BOARD_RPIPICO
      GreenLedOut_out(0, GreenLedState);
  #endif
      this->tlmWrite_GreenLedState(GreenLedState);
    }
  if (currentAxis != this->m_lastDominantAxis) {
      this->log_ACTIVITY_HI_DominantAxisChanged(currentAxis);
      this->m_lastDominantAxis = currentAxis;
    }
    
  }

void Payload::HumDataIn_handler(NATIVE_INT_TYPE portNum, const Components::HumVector &humidity)
  {
         
    if (this->m_state == Fw::On::ON) {
           
      this->hum_data[this->h_counter] = humidity[0];
    if (this->m_humSampleCount < FILTER_SIZE) {
    this->m_humSampleCount++;
    }

    this->h_counter = (this->h_counter + 1) % FILTER_SIZE;

    avgHumidity = calculateAverage(this->hum_data, this->m_humSampleCount);
       
      this->tlmWrite_avgHum(avgHumidity);
      }  
  }

void Payload :: humidity_led_handler() {

   if (this->m_state == Fw::On::ON) {

    F32 currentAvgHum = avgHumidity[0];
      if (currentAvgHum <= HUM_LIMIT_LOW  || currentAvgHum >= HUM_LIMIT_HIGH) {
        YellowLedState = Fw::Logic::HIGH;
        this->log_ACTIVITY_HI_HumidityDanger(currentAvgHum);
      } else {
        YellowLedState = Fw::Logic::LOW;
      }
  #ifdef _BOARD_RPIPICO
      YellowLedOut_out(0, YellowLedState);
  #endif
      this->tlmWrite_YellowLedState(YellowLedState);
    }
 }

  void Payload :: StartPayload_cmdHandler(
          FwOpcodeType opCode,
          U32 cmdSeq,
          Fw::On on_off
      )
    {
     auto cmdResp = Fw::CmdResponse::OK;

      if (!on_off.isValid()) {
      this->log_WARNING_LO_InvalidPayloadArg(on_off);
      BlueLedState = Fw::Logic::LOW;
      cmdResp = Fw::CmdResponse::VALIDATION_ERROR;
    }  else {
    this->m_state = on_off;

    if (this->m_state == Fw::On::ON) {
        this->BlueLedState = Fw::Logic::HIGH;
    } else {
        this->BlueLedState = Fw::Logic::LOW;
        this->RedLedState = Fw::Logic::LOW;
        this->GreenLedState = Fw::Logic::LOW;
        this->YellowLedState = Fw::Logic::LOW;
    }

    this->log_ACTIVITY_HI_PayloadState(this->m_state);
}
#ifdef _BOARD_RPIPICO
    BlueLedOut_out(0, BlueLedState);
    RedLedOut_out(0, RedLedState);
    GreenLedOut_out(0, GreenLedState);
    YellowLedOut_out(0, YellowLedState);
#endif
      this->tlmWrite_State(m_state);
      this->tlmWrite_BlueLedState(BlueLedState);
      this->tlmWrite_RedLedState(RedLedState);
      this->tlmWrite_GreenLedState(GreenLedState);
      this->tlmWrite_YellowLedState(YellowLedState);
      this->cmdResponse_out(opCode, cmdSeq, cmdResp);
    }

} 