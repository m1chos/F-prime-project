// ======================================================================
// \title  MotionSensor.cpp
// \author [YourName]
// \brief  cpp file for MotionSensor component implementation class
// ======================================================================

#include <Components/MotionSensor/MotionSensor.hpp>
#include <FpConfig.hpp>

namespace Components {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  MotionSensor ::
    MotionSensor(
        const char *const compName
    ) : MotionSensorComponentBase(compName)
#ifndef _BOARD_RPIPICO
                                        ,a{0.0, 0.0, 0.0},
                                        randomGenerator(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())),
                                        distribution(-1.1, 1.1)
#endif
  {
#ifndef _BOARD_RPIPICO
    update();
#endif

#ifdef _BOARD_RPIPICO
   this->mpu = new MPU6050();
#endif

}

void MotionSensor ::
    init(
        //const NATIVE_INT_TYPE queueDepth,
        const NATIVE_INT_TYPE instance
    )
  {
    MotionSensorComponentBase::init (instance);
  }

  MotionSensor ::
    ~MotionSensor()
  {
    #ifdef _BOARD_RPIPICO
      if (this->mpu != nullptr) {
          delete this->mpu;
      }
    #endif  
  }

  // ----------------------------------------------------------------------
  // Custom Functions
  // ----------------------------------------------------------------------

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void MotionSensor ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    if (i2c_status) {
   #ifdef _BOARD_RPIPICO 
    // Ενημέρωση δεδομένων από τον αισθητήρα
    this->mpu->update();
    this->mpu->getAccel(&a);
   #else
   update();
   getAccel(&a);
   #endif
    // Get accelerometer data
    acc_data[0] = a.accelX;
    acc_data[1] = a.accelY;
    acc_data[2] = a.accelZ;
    
    // Αποστολή Telemetry για την επιτάχυνση
    this->tlmWrite_accelerometer(acc_data);
    // Αποστολή Telemetry για τη θερμοκρασία
    #ifdef _BOARD_RPIPICO
    this->tlmWrite_tempC(mpu->getTemp());
    #endif
    this->MpuDataOut_out(0, acc_data);
   }
  }

  #ifdef _BOARD_RPIPICO
  void MotionSensor::init_i2c(void) {

    Wire.begin();             // join i2c bus
    Wire.setClock(400000UL);  // set speed to 400k

    // Init MPU6050: send telemetry and event
    if( mpu->init(calib, IMU_ADDRESS) != 0) {
      // Initialization failed
      this->log_WARNING_HI_MpuInitFail();
      this->tlmWrite_connected(false);
    } else {
      // Successfully initialized
      this->tlmWrite_connected(true);
      this->log_ACTIVITY_HI_MpuInitSucc();
    }
  }
#else
  void MotionSensor ::
      update()
  {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastSwitchTime).count();

    // Change dominant axis every 10 seconds
    if (elapsed >= 10) {
      dominantAxis = randomGenerator() % 3; // 0: X, 1: Y, 2: Z
      lastSwitchTime = now;
    }

    // Generate values
    dominantValue = std::uniform_real_distribution<F32>(0.9, 1.1)(randomGenerator);
    otherValue = std::uniform_real_distribution<F32>(-0.1, 0.1)(randomGenerator);

    a.accelX = (dominantAxis == 0) ? dominantValue : otherValue;
    a.accelY = (dominantAxis == 1) ? dominantValue : otherValue;
    a.accelZ = (dominantAxis == 2) ? dominantValue : otherValue;
  }
  
  void MotionSensor ::
      getAccel(
          AccelData *data)
  {
    if (data)
    {
      *data = a;
    }
  }
#endif

} // end namespace Components