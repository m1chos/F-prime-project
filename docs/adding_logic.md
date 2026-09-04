# 4.7 Developing logic in component

In the [previous section](./tlm_events.md) we added telemetry and events. Now we need to use those, the same way that the Arduino example code intends. 

## Development method of the component
In the source code we need now to make the same code functionality as in the Arduino example.

### `Setup()`
For the `setup()` we will create a new function called `init_i2c()`. The function should look like this:

```cpp
void MotionSensor::init_i2c(void) {

  Wire.begin();             // join i2c bus
  Wire.setClock(400000UL);  // set speed to 400k

  // Init MPU6050: send telemetry and event
  if( myMPU6050->init(calib, IMU_ADDRESS) != 0) {
    // Initialization failed
    this->log_WARNING_HI_MpuInitFail();
    this->tlmWrite_connected(false);
  } else {
    // Successfully initialized
    this->tlmWrite_connected(true);
    this->log_ACTIVITY_HI_MpuInitSucc();
  }
}
```

**Predefine the function in the header file as `public` and build the component to check for compiler errors.**

The code first joins the I2C bus and sets the clock speed to 400 kHz. Then, it attempts to initialize the MPU6050 sensor using the init() function, passing the calibration data and the I2C address of the sensor. If the initialization fails, the code logs a warning message and writes a telemetry event indicating that the sensor is not connected. If the initialization succeeds, it writes a telemetry event indicating that the sensor is connected.

### `Loop()`

The `loop()` function in the *Arduino Fw* is basically a function that runs forever. Same code executes every time the function reaches its end like a `while(1)` loop. A similar logic has the `run_handler` using the rate groups 

```cpp
void MotionSensor ::
  run_handler(
      NATIVE_INT_TYPE portNum,
      NATIVE_UINT_TYPE context
  )
{
  myMPU6050->update();
  myMPU6050->getAccel(&a);

  // Get accelerometer data
  acc_data[0] = a.accelX;
  acc_data[1] = a.accelY;
  acc_data[2] = a.accelZ;
  this->tlmWrite_accelerometer(acc_data);

  this->tlmWrite_tempC(myMPU6050->getTemp());
  /* Use this later to connect this port with Payload component */
  // this->MpuDataOut_out(0, acc_data);
}
```

The function updates the MPU6050 motion sensor data and writes the accelerometer data and temperature to telemetry. It first calls the `update()` function to refresh the sensor data. Then, it gets the accelerometer data from the sensor and stores it in the `acc_data` array. Finally, it writes the accelerometer data and temperature to telemetry using the `tlmWrite_accelerometer()` and `tlmWrite_tempC()` functions, and also outputs the accelerometer data using the `MpuDataOut_out()` function, which is corresponds to the output port.

Build the component again to check for compiler errors. The component now is ready to add in the Deployment!

---

**Next**: [Full System Integration](./full_system.md)
