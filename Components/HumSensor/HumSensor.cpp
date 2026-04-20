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
    
    
    
    
    /*
    {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastSwitchTime).count();

    if (elapsed >= 10) {
      humarea = randomGenerator() % 10; 
      lastSwitchTime = now;
    }
    h.hum = std::uniform_real_distribution<F32>(0.0, 100.0)(randomGenerator);
  }*/

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


