// ======================================================================
// \title  HumSensor.cpp
// \author michos
// \brief  cpp file for HumSensor component implementation class
// ======================================================================

#include "Components/HumSensor/HumSensor.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  HumSensor ::
    HumSensor(const char* const compName) :
      HumSensorComponentBase(compName)
#ifndef _BOARD_RPIPICO
                  , i2c_status(true),
                  h{0},
                  randomGenerator(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())),
                  distribution(0, 100)
#endif
{
#ifndef _BOARD_RPIPICO
    update();
#endif
}

  HumSensor ::
    ~HumSensor()
  {

  }

  Drv::I2cStatus HumSensor ::
    writeCommand(
        U16 command
      )
  {
    U8 data[2];

    data[0] = static_cast<U8>((command >> 8) & 0xFF);
    data[1] = static_cast<U8>(command & 0xFF);

    Fw::Buffer buffer(
        data,
        sizeof(data)
    );

    return this->busWrite_out(
        0,
        this->m_i2cAddress,
        buffer
    );
  }


   Drv::I2cStatus HumSensor ::
    readData(
        U8* data,
        U32 size
      )
   {
    Fw::Buffer buffer(
        data,
        size
    );

    return this->busRead_out(
        0,
        this->m_i2cAddress,
        buffer
    );
}
      U8 HumSensor ::
      calculateCrc(
          const U8* data,
          U32 size
      )
    {
      U8 crc = 0xFF;

      for (U32 i = 0; i < size; i++) {
          crc ^= data[i];

          for (U8 bit = 0; bit < 8; bit++) {
              if (crc & 0x80) {
                  crc = static_cast<U8>((crc << 1) ^ 0x31);
              }
              else {
                  crc <<= 1;
              }
          }
      }

      return crc;
    }

    Drv::I2cStatus HumSensor ::
    readSensorId(
        U16& sensorId
    )
  {
    Drv::I2cStatus status =
        this->writeCommand(SHTC3_READ_ID_COMMAND);

    if (status != Drv::I2cStatus::I2C_OK) {
        return status;
    }

    U8 data[SHTC3_ID_DATA_SIZE] = {};

    status = this->readData(
        data,
        sizeof(data)
    );

    if (status != Drv::I2cStatus::I2C_OK) {
        return status;
    }

    if (this->calculateCrc(data, 2) != data[2]) {
        return Drv::I2cStatus::I2C_OTHER_ERR;
    }

    sensorId =
        static_cast<U16>(
            (static_cast<U16>(data[0]) << 8) |
            static_cast<U16>(data[1])
        );

    constexpr U16 SHTC3_ID_MASK = 0x083F;
    constexpr U16 SHTC3_ID_VALUE = 0x0807;

    if ((sensorId & SHTC3_ID_MASK) != SHTC3_ID_VALUE) {
        return Drv::I2cStatus::I2C_OTHER_ERR;
    }

    return Drv::I2cStatus::I2C_OK;
  }

  Drv::I2cStatus HumSensor ::
    configure(
        U32 i2cAddress
    )
  {
    this->m_i2cAddress = i2cAddress;

    U16 sensorId = 0;

    Drv::I2cStatus status =
        this->readSensorId(sensorId);

    if (status != Drv::I2cStatus::I2C_OK) {
        this->m_usesSimulation = true;
        this->log_WARNING_HI_SimulationEnabled();

        return status;
    }

    status =
        this->writeCommand(SHTC3_SLEEP_COMMAND);

    if (status != Drv::I2cStatus::I2C_OK) {
        this->m_usesSimulation = true;
        this->log_WARNING_HI_SimulationEnabled();

        return status;
    }

    this->m_usesSimulation = false;
    this->log_ACTIVITY_HI_SensorDetected();

    return Drv::I2cStatus::I2C_OK;
  }


  Drv::I2cStatus HumSensor ::
    readMeasurement(
        HumData& measurement
    )
  {

    Drv::I2cStatus status =
        this->writeCommand(SHTC3_WAKEUP_COMMAND);

    if (status != Drv::I2cStatus::I2C_OK) {
        return status;
    }

    Os::Task::delay(1);

    status =
        this->writeCommand(SHTC3_MEASURE_COMMAND);

    if (status != Drv::I2cStatus::I2C_OK) {
        
        this->writeCommand(SHTC3_SLEEP_COMMAND);
        return status;
    }

    Os::Task::delay(13);

    U8 data[SHTC3_MEASUREMENT_DATA_SIZE] = {};

    status =
        this->readData(
            data,
            sizeof(data)
        );

    const Drv::I2cStatus sleepStatus =
        this->writeCommand(SHTC3_SLEEP_COMMAND);

    if (status != Drv::I2cStatus::I2C_OK) {
        return status;
    }

    if (sleepStatus != Drv::I2cStatus::I2C_OK) {
        return sleepStatus;
    }

    if (this->calculateCrc(data, 2) != data[2]) {
        return Drv::I2cStatus::I2C_OTHER_ERR;
    }

    if (this->calculateCrc(&data[3], 2) != data[5]) {
        return Drv::I2cStatus::I2C_OTHER_ERR;
    }

    const U16 rawHumidity =
        static_cast<U16>(
            (static_cast<U16>(data[3]) << 8) |
            static_cast<U16>(data[4])
        );

    measurement.hum =
        100.0f *
        static_cast<F32>(rawHumidity) /
        65536.0f;

    return Drv::I2cStatus::I2C_OK;
  }
  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void HumSensor ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    if(i2c_status){
     #ifdef _BOARD_RPIPICO
          hum->update();
          hum->getHum(&h);
     #else 
          update();
          getHum(&h);
     #endif
          hum_data[0] = h.hum;

          this->tlmWrite_humidity(hum_data);
          this->HumDataOut_out(0, hum_data);
    }
  }

#ifdef _BOARD_RPIPICO
  void HumSensor ::
      init_i2c()
  {
    Wire.begin();
    Wire.setClock(400000L);

    if (hum->init(calib, HUM_ADDRESS) != 0)
    {
      i2c_status = false;
      this->log_WARNING_HI_HumInitFail();
    }
    else
    {
      i2c_status = true;
      this->log_ACTIVITY_HI_HumInitSucc();
    }
  }
#else
  void HumSensor ::
      update()
  {
    
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastSwitchTime).count();

    if (elapsed >= 15) {
      targetHumidity = std::uniform_real_distribution<F32>(10.0, 90.0)(randomGenerator);
      lastSwitchTime = now;
    }

    currentHumidity += (targetHumidity - currentHumidity) * 0.1f;

    F32 noise = std::uniform_real_distribution<F32>(-0.3f, 0.3f)(randomGenerator);

    h.hum = currentHumidity + noise;
  }
    
    
  void HumSensor ::
      getHum(
          HumData *data)
  {
    if (data)
    {
      *data = h;
    }
  }
#endif
}


