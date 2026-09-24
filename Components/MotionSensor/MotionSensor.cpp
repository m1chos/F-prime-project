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
    ) : MotionSensorComponentBase(compName),
        accelerationSample{0.0, 0.0, 0.0},
        randomGenerator(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())),
        distribution(-1.1, 1.1)

  {

    update();

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
    if (this->m_usesSimulation) {
        this->update();
        this->getAccel(&accelerationSample);
    }
    else {
        const Drv::I2cStatus status =
            this->readAcceleration(accelerationSample);

        if (status != Drv::I2cStatus::I2C_OK) {
            this->tlmWrite_connected(false);
            return;
        }

        this->tlmWrite_connected(true);
    }

    acc_data[0] = accelerationSample.accelX;
    acc_data[1] = accelerationSample.accelY;
    acc_data[2] = accelerationSample.accelZ;

    this->tlmWrite_accelerometer(acc_data);

    this->MpuDataOut_out(
        0,
        acc_data
    );
}
  Drv::I2cStatus MotionSensor ::
    configure(
        U32 i2cAddress
    )
{
    this->m_i2cAddress = i2cAddress;

    U8 whoAmI = 0;

    Drv::I2cStatus status =
        this->readRegister(
            QMI8658_WHO_AM_I_REG,
            whoAmI
        );

    if (
        status != Drv::I2cStatus::I2C_OK ||
        whoAmI != QMI8658_WHO_AM_I_VALUE
    ) {
        this->m_usesSimulation = true;

        this->tlmWrite_connected(false);
        this->log_WARNING_HI_SimulationEnabled();

        if (status != Drv::I2cStatus::I2C_OK) {
            return status;
        }

        return Drv::I2cStatus::I2C_OTHER_ERR;
    }

    status = this->configureAccelerometer();

    if (status != Drv::I2cStatus::I2C_OK) {
        this->m_usesSimulation = true;

        this->tlmWrite_connected(false);
        this->log_WARNING_HI_SimulationEnabled();

        return status;
    }

    this->m_usesSimulation = false;

    this->tlmWrite_connected(true);
    this->log_ACTIVITY_HI_SensorDetected();

    return Drv::I2cStatus::I2C_OK;
}

  Drv::I2cStatus MotionSensor ::
    readRegisterBlock(
        U8 startRegister,
        U8* data,
        U32 size
    )
{
    Fw::Buffer writeBuffer(
        &startRegister,
        sizeof(startRegister)
    );

    Fw::Buffer readBuffer(
        data,
        size
    );

    return this->busWriteRead_out(
        0,
        this->m_i2cAddress,
        writeBuffer,
        readBuffer
    );
}

    Drv::I2cStatus MotionSensor ::
      configureAccelerometer()
{
    const U8 accelConfig =
        QMI8658_ACCEL_RANGE_2G |
        QMI8658_ACCEL_ODR_125HZ;

    Drv::I2cStatus status =
        this->writeRegister(
            QMI8658_CTRL2_REG,
            accelConfig
        );

    if (status != Drv::I2cStatus::I2C_OK) {
        return status;
    }

    return this->writeRegister(
        QMI8658_CTRL7_REG,
        QMI8658_ACCEL_ENABLE
    );
}    



    Drv::I2cStatus MotionSensor ::
      readAcceleration(
        AccelData& acceleration
    )
{
    U8 data[QMI8658_ACCEL_DATA_SIZE] = {};

    const Drv::I2cStatus status =
        this->readRegisterBlock(
            QMI8658_ACCEL_X_L_REG,
            data,
            sizeof(data)
        );

    if (status != Drv::I2cStatus::I2C_OK) {
        return status;
    }

    const I16 rawX =
        static_cast<I16>(
            (static_cast<U16>(data[1]) << 8) |
             static_cast<U16>(data[0])
        );

    const I16 rawY =
        static_cast<I16>(
            (static_cast<U16>(data[3]) << 8) |
             static_cast<U16>(data[2])
        );

    const I16 rawZ =
        static_cast<I16>(
            (static_cast<U16>(data[5]) << 8) |
             static_cast<U16>(data[4])
        );

    acceleration.accelX =
        static_cast<F32>(rawX) / QMI8658_ACCEL_SCALE;

    acceleration.accelY =
        static_cast<F32>(rawY) / QMI8658_ACCEL_SCALE;

    acceleration.accelZ =
        static_cast<F32>(rawZ) / QMI8658_ACCEL_SCALE;

    return Drv::I2cStatus::I2C_OK;
}

void MotionSensor ::
    update()
{
    auto now = std::chrono::steady_clock::now();

    auto elapsed =
        std::chrono::duration_cast<std::chrono::seconds>(
            now - lastSwitchTime
        ).count();

    if (elapsed >= 10) {
        dominantAxis = randomGenerator() % 3;
        lastSwitchTime = now;
    }

    dominantValue =
        std::uniform_real_distribution<F32>(
            0.9,
            1.1
        )(randomGenerator);

    otherValue =
        std::uniform_real_distribution<F32>(
            -0.1,
            0.1
        )(randomGenerator);

    accelerationSample.accelX =
        (dominantAxis == 0) ? dominantValue : otherValue;

    accelerationSample.accelY =
        (dominantAxis == 1) ? dominantValue : otherValue;

    accelerationSample.accelZ =
        (dominantAxis == 2) ? dominantValue : otherValue;
}


void MotionSensor ::
    getAccel(
        AccelData* data
    )
{
    if (data != nullptr) {
        *data = accelerationSample;
    }
}


}