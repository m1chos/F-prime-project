// ======================================================================
// \title  MotionSensor.hpp
// \author [YourName]
// \brief  hpp file for MotionSensor component implementation class
// ======================================================================

#ifndef MotionSensor_HPP
#define MotionSensor_HPP

#include "Components/MotionSensor/MotionSensorComponentAc.hpp"

#ifdef _BOARD_RPIPICO
#include "FastIMU.h"
#include <Wire.h>
// Ορισμός της διεύθυνσης I2C του αισθητήρα
#define IMU_ADDRESS 0x68

#else 
#include <chrono>
#include <random>
#endif

namespace Components {

  #ifndef _BOARD_RPIPICO
   struct AccelData {
    F32 accelX;
    F32 accelY;
    F32 accelZ;
   };
  #endif
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

      //! Destroy object MotionSensor
      //!
      ~MotionSensor();

      // ----------------------------------------------------------------------
      // Custom Functions
      // ----------------------------------------------------------------------

      //! Initialize I2C and MPU6050
      #ifdef _BOARD_RPIPICO
      void init_i2c();
      #endif

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for run
      //!
      void run_handler(
          NATIVE_INT_TYPE portNum, /*!< The port number*/
          NATIVE_UINT_TYPE context /*!< The call order*/
      );

       I8 dominantAxis = 0;
      
      Components::Vector acc_data;
      AccelData a;
      
#ifdef _BOARD_RPIPICO
      bool i2c_status = false;
      calData calib = { 0 };
      MPU6050* mpu = new MPU6050();
      MPU6050* mpu;
#else
      bool i2c_status = true;
      F32 dominantValue;
      F32 otherValue;
      std::mt19937 randomGenerator;
      std::uniform_real_distribution<F32> distribution;
      std::chrono::steady_clock::time_point lastSwitchTime = std::chrono::steady_clock::now();

      void update();
      void getAccel(AccelData* data);
#endif
  };

} // end namespace Components

#endif