// ======================================================================
// \title  PressureSensor.hpp
// \author michos
// \brief  hpp file for PressureSensor component implementation class
// ======================================================================

#ifndef Components_PressureSensor_HPP
#define Components_PressureSensor_HPP
#include "Components/PressureSensor/PressureSensorComponentAc.hpp"

#include <chrono>
#include <random>

namespace Components {

  struct BarData {
    F32 bar;
  };

  class PressureSensor :
    public PressureSensorComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct PressureSensor object
      PressureSensor(
          const char* const compName //!< The component name
      );

      //! Destroy PressureSensor object
      ~PressureSensor();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for run
      void run_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          NATIVE_UINT_TYPE context //!< The call order
      );

 I8 bararea = 0;
      
      Components::BarVector bar_data;
      BarData b;
      
#ifdef _BOARD_RPIPICO
      bool i2c_status = false;
      calData calib = { 0 };
#else
      bool i2c_status = true;
      F32 pressureValue;
      std::mt19937 randomGenerator;
      std::uniform_real_distribution<F32> distribution;
      std::chrono::steady_clock::time_point lastSwitchTime = std::chrono::steady_clock::now();
      F32 currentPressure;
      F32 targetPressure;
      void update();
      void getBar(BarData* data);
#endif






  };

}

#endif
