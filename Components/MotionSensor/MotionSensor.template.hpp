// ======================================================================
// \title  MotionSensor.hpp
// \author michos
// \brief  hpp file for MotionSensor component implementation class
// ======================================================================

#ifndef Components_MotionSensor_HPP
#define Components_MotionSensor_HPP

#include "Components/MotionSensor/MotionSensorComponentAc.hpp"

namespace Components {

  class MotionSensor :
    public MotionSensorComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct MotionSensor object
      MotionSensor(
          const char* const compName //!< The component name
      );

      //! Destroy MotionSensor object
      ~MotionSensor();

  };

}

#endif
