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

  Drv::I2cStatus MotionSensor ::
    readRegister(
        U8 reg,
        U8& value
    )
{
    U8 registerAddress = reg;

    Fw::Buffer writeBuffer(
        &registerAddress,
        sizeof(registerAddress)
    );

    Fw::Buffer readBuffer(
        &value,
        sizeof(value)
    );

    return this->busWriteRead_out(
        0,
        this->m_i2cAddress,
        writeBuffer,
        readBuffer
    );
}

Drv::I2cStatus MotionSensor ::
    writeRegister(
        U8 reg,
        U8 value
    )
{
    U8 data[2] = {
        reg,
        value
    };

    Fw::Buffer writeBuffer(
        data,
        sizeof(data)
    );

    return this->busWrite_out(
        0,
        this->m_i2cAddress,
        writeBuffer
    );
}
   void MotionSensor ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {

   if (i2c_status) {
   #ifdef _BOARD_RPIPICO 
  
    this->mpu->update();
    this->mpu->getAccel(&a);
   #else
   update();
   getAccel(&a);
   #endif

    acc_data[0] = a.accelX;
    acc_data[1] = a.accelY;
    acc_data[2] = a.accelZ;
    
    
    this->tlmWrite_accelerometer(acc_data);
    
    #ifdef _BOARD_RPIPICO
    this->tlmWrite_tempC(mpu->getTemp());
    #endif
    this->MpuDataOut_out(0, acc_data);
   }
  }

  Drv::I2cStatus MotionSensor :: configure(
        U32 i2cAddress
    )
  {
    this->m_i2cAddress = i2cAddress;

    U8 whoAmI = 0;

    const Drv::I2cStatus status =
        this->readRegister(
            QMI8658_WHO_AM_I_REG,
            whoAmI
        );

    if (
        status == Drv::I2cStatus::I2C_OK &&
        whoAmI == QMI8658_WHO_AM_I_VALUE
    ) {
        this->m_usesSimulation = false;

        this->tlmWrite_connected(true);
        this->log_ACTIVITY_HI_SensorDetected();
    } else {
        this->m_usesSimulation = true;

        this->tlmWrite_connected(false);
        this->log_WARNING_HI_SimulationEnabled();

         if (status != Drv::I2cStatus::I2C_OK) {
        return status;
        }

        return Drv::I2cStatus::I2C_OTHER_ERR;
        }
  }

  #ifdef _BOARD_RPIPICO
  void MotionSensor::init_i2c(void) {

    Wire.begin();             
    Wire.setClock(400000UL); 

    if (mpu->init(calib, IMU_ADDRESS) != 0) {
    this->i2c_status = false;
    this->log_WARNING_HI_MpuInitFail();
    this->tlmWrite_connected(false);
} else {
    this->i2c_status = true;
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

  
    if (elapsed >= 10) {
      dominantAxis = randomGenerator() % 3;
      lastSwitchTime = now;
    }

    
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

}