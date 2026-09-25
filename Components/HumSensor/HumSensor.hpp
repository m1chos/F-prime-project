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
#include "Drv/I2cDriverPorts/I2cStatusEnumAc.hpp"
#include "Os/Task.hpp"


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

      Drv::I2cStatus configure(U32 i2cAddress);

    PRIVATE:

      static constexpr U16 SHTC3_WAKEUP_COMMAND = 0x3517;
      static constexpr U16 SHTC3_SLEEP_COMMAND = 0xB098;
      static constexpr U16 SHTC3_READ_ID_COMMAND = 0xEFC8;
      static constexpr U16 SHTC3_MEASURE_COMMAND = 0x7866;

      static constexpr U32 SHTC3_MEASUREMENT_DATA_SIZE = 6;
      static constexpr U32 SHTC3_ID_DATA_SIZE = 3;

      Drv::I2cStatus writeCommand(U16 command);
      Drv::I2cStatus readData(U8* data, U32 size);
      Drv::I2cStatus readMeasurement(HumData& measurement);
      Drv::I2cStatus readSensorId(U16& sensorId);

      U8 calculateCrc(const U8* data, U32 size);

      U32 m_i2cAddress = 0;
      bool m_usesSimulation = true;

      
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
