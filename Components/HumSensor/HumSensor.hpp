// ======================================================================
// \title  HumSensor.hpp
// \author michos
// \brief  hpp file for HumSensor component implementation class
// ======================================================================

#ifndef Components_HumSensor_HPP
#define Components_HumSensor_HPP

#include "Components/HumSensor/HumSensorComponentAc.hpp"

#include <chrono>
#include <random>

namespace Components {

  struct HumData {
    F32 hum;
  };

  class HumSensor :
    public HumSensorComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct HumSensor object
      HumSensor(
          const char* const compName //!< The component name
      );

      //! Destroy HumSensor object
      ~HumSensor();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for run
      void run_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          NATIVE_UINT_TYPE context //!< The call order
      );

 I8 humarea = 0;
      
      Components::HumVector hum_data;
      HumData h;
      
#ifdef _BOARD_RPIPICO
      bool i2c_status = false;
      calData calib = { 0 };
#else
      bool i2c_status = true;
      F32 humValue;
      std::mt19937 randomGenerator;
      std::uniform_real_distribution<F32> distribution;
      std::chrono::steady_clock::time_point lastSwitchTime = std::chrono::steady_clock::now();
      F32 currentHumidity;
      F32 targetHumidity;

      void update();
      void getHum(HumData* data);
#endif






  };

}

#endif
