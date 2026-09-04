// ======================================================================
// \title  PressureSensor.cpp
// \author michos
// \brief  cpp file for PressureSensor component implementation class
// ======================================================================


#include "Components/PressureSensor/PressureSensor.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  PressureSensor ::
    PressureSensor(const char* const compName) :
      PressureSensorComponentBase(compName)
#ifndef _BOARD_RPIPICO
                  , i2c_status(true),
                  b{0},
                  randomGenerator(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())),
                  distribution(0, 100)
#endif
{
#ifndef _BOARD_RPIPICO
    update();
#endif
}

  PressureSensor ::
    ~PressureSensor()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void PressureSensor ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    if(i2c_status){
     #ifdef _BOARD_RPIPICO
          bar->update();
          bar->getBar(&b);
     #else 
          update();
          getBar(&b);
     #endif
          bar_data[0] = b.bar;

          this->tlmWrite_pressure(bar_data);
          this->BarDataOut_out(0, bar_data);
    }
  }

#ifdef _BOARD_RPIPICO
  void PressureSensor ::
      init_i2c()
  {
    Wire.begin();
    Wire.setClock(400000L);

    if (bar->init(calib, BAR_ADDRESS) != 0)
    {
      i2c_status = false;
      this->log_WARNING_HI_BarInitFail();
    }
    else
    {
      i2c_status = true;
      this->log_ACTIVITY_HI_BarInitSucc();
    }
  }
#else
  void PressureSensor ::
      update()
  {
    
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastSwitchTime).count();

    if (elapsed >= 15) {
      targetPressure = std::uniform_real_distribution<F32>(10.0, 90.0)(randomGenerator);
      lastSwitchTime = now;
    }

    currentPressure += (targetPressure - currentPressure) * 0.1f;
    F32 noise = std::uniform_real_distribution<F32>(-0.3f, 0.3f)(randomGenerator);

    b.bar = currentPressure + noise;
  }
    
    
  void PressureSensor ::
      getBar(
          BarData *data)
  {
    if (data)
    {
      *data = b;
    }
  }
#endif
}


