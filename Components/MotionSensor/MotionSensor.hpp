// ======================================================================
// \title  MotionSensor.hpp
// \author michos
// \brief  hpp file for MotionSensor component implementation class
// ======================================================================

#ifndef MotionSensor_HPP
#define MotionSensor_HPP

#include "Components/MotionSensor/MotionSensorComponentAc.hpp"

#include <chrono>
#include <random>

namespace Components {

   struct AccelData {
    F32 accelX;
    F32 accelY;
    F32 accelZ;
   };
  
  class MotionSensor :
    public MotionSensorComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object MotionSensor
      //!
      MotionSensor(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object MotionSensor
      //!
      void init(
          //const NATIVE_INT_TYPE queueDepth, /*!< The queue depth*/
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      Drv::I2cStatus configure(
           U32 i2cAddress
      );

      //! Destroy object MotionSensor
      //!
      ~MotionSensor();

      // ----------------------------------------------------------------------
      // Custom Functions
      // ----------------------------------------------------------------------


    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for run
      //!
      void run_handler(
          NATIVE_INT_TYPE portNum, 
          NATIVE_UINT_TYPE context 
      );

      static constexpr U32 QMI8658_I2C_ADDRESS = 0x6B;
      static constexpr U8 QMI8658_WHO_AM_I_REG = 0x00;
      static constexpr U8 QMI8658_WHO_AM_I_VALUE = 0x05;
      static constexpr U8 QMI8658_ACCEL_X_L_REG = 0x35;
      static constexpr U32 QMI8658_ACCEL_DATA_SIZE = 6;
      static constexpr F32 QMI8658_ACCEL_SCALE = 16384.0f;
      static constexpr U8 QMI8658_CTRL2_REG = 0x03;
      static constexpr U8 QMI8658_CTRL7_REG = 0x08;
      static constexpr U8 QMI8658_ACCEL_RANGE_2G = 0x00;
      static constexpr U8 QMI8658_ACCEL_ODR_125HZ = 0x06;
      static constexpr U8 QMI8658_ACCEL_ENABLE = 0x01;
 
      Drv::I2cStatus readRegister(
        U8 reg,
        U8& value
      );

      Drv::I2cStatus writeRegister(
        U8 reg,
        U8 value
      );

      Drv::I2cStatus readRegisterblock(
      U8 startRegister,
      U8* data,
      U32 size
      );

      U32 m_i2cAddress = 0; 
      bool m_usesSimulation = true;
      
      
      I8 dominantAxis = 0;
      
      Components::Vector acc_data;
      AccelData a;

      Drv::I2cStatus readAcceleration(
      AccelData& acceleration
      );

      Drv::I2cStatus configureAccelerometer();
            
      F32 dominantValue;
      F32 otherValue;
      std::mt19937 randomGenerator;
      std::uniform_real_distribution<F32> distribution;
      std::chrono::steady_clock::time_point lastSwitchTime = std::chrono::steady_clock::now();

      void update();
      void getAccel(AccelData* data);

  };

}

#endif
