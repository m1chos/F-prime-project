# 4.5 Motion Sensor Implementation
In the [previous section](./arduino.md) we analyzed the functionallity of the Arduino example.

Also, in the [MotionSensor Ports](./imu_ports.md) section we constructed the ports of the component.

## A quick look inside the component
The result of this created a new template block of code in the implementation code of the component

```c++
  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void MotionSensor ::
    run_handler(
        NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    // TODO
  }
```

This block of code corresponds to the `run` port, which means that it will execute every time it gets called.

**But what about the output, where is it?**

The output port does indeed exist in the project! It exists in the `build-fprime-automatic-rpipico/Components/MotionSensor/MotionSensorComponentAc.cpp`

> Do not write code inside `*Ac.*` files because it is auto-generated code from CMake tool. Any code will be overwritten on the next *build*. Just inspect!

```c++
  // ----------------------------------------------------------------------
  // Invocation functions for typed output ports
  // ----------------------------------------------------------------------

  void MotionSensorComponentBase ::
    MpuDataOut_out(
        NATIVE_INT_TYPE portNum,
        const Components::Vector& acceleration
    )
  {
    FW_ASSERT(
      portNum < this->getNum_MpuDataOut_OutputPorts(),
      static_cast<FwAssertArgType>(portNum)
    );
    this->m_MpuDataOut_OutputPort[portNum].invoke(
      acceleration
    );
  }
```

This function  is **ready-to-use** in the component's class.

## I2C Communication using Arduino library

In order to develop fprime components, while using the *Arduino Fw* libraries, we need to expose the libraries.

> Make sure that `library_locations: ./fprime-arduino` is in `settings.ini` file.

Firstly, we need to include in the CMake the `FastIMU` library. This can be done by adding the `target_use_arduino_libraries("FastIMU")`. The `CMakeLists.txt` should look like this:

```cmake
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/MotionSensor.fpp"
  "${CMAKE_CURRENT_LIST_DIR}/MotionSensor.cpp"
)

target_use_arduino_libraries("FastIMU")

register_fprime_module()
```

> Build to make sure it is included correctly.

The `FastIMU` library can be seen by the build system. In the header file inlcude the library and define the I2C port.

```cpp
#include "Components/MotionSensor/MotionSensorComponentAc.hpp"
#include "FastIMU.h"
#include <Wire.h>

#define IMU_ADDRESS 0x68
```

And secondly we need to add the essential variables, that we saw that are used previously in the Arduino example

```cpp
    // Below `run_handler`
    calData calib = { 0 };
    AccelData a;
    Components::Vector acc_data;
    MPU6050* myMPU6050 = new MPU6050();
```

Now, we need to make the corresponding `setup()` and `loop()` functions. 

In the setup functions we need to make every initialization needed to establish the I2C connection with our board.

```cpp
void setup() {
  Wire.begin();
  Wire.setClock(400000); //400khz clock
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  int err = IMU.init(calib, IMU_ADDRESS);
  if (err != 0) {
    Serial.print("Error initializing IMU: ");
    Serial.println(err);
    while (true) {
      ;
    }
  }
```

**But how will we know the status of the satellite, when it is flying? We can't debug it through USB to get its status.**

That's why we need to use telemetry and events!

---

**Next**: [Defining telemetry and events](./tlm_events.md)