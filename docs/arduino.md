# 4.4 Arduino Example

In the [previous section](./imu_ports.md) we discussed about the  creation of ports in a component. As we made the skeleton of this component, it is now time to analyze the Arduino Example code that we want to implement in the F´ Framework.

## Arduino Fw x F´ Fw

Arduino is often used as a starting point for electronics and programming due to its simplicity and ease of use. By experimenting with Arduino examples, developers can grasp fundamental concepts like hardware interfacing, sensor readings, and basic control logic.

### Testing Functionality
Before integrating a feature into a complex system like F´, it's often beneficial to create a simplified proof of concept using Arduino.

### Hardware Compatibility

If a developer is unsure about the compatibility of a specific sensor or actuator with F´, they can first test it using Arduino. This helps ensure that the hardware functions correctly and that the necessary drivers or libraries are available.

### Code Reuse

In some cases, the code from an Arduino example can be adapted and reused in F´. This can save development time and effort, especially for common tasks like sensor readings or actuator control.

## Example

Now lets focus to our target. Here is a brief explanation of the code that will be used as the base code for the implementation of the `MotionSensor` Component. 

**Header Inclusion:**

```cpp
#include "FastIMU.h"
#include <Wire.h>
```

- These lines include the necessary headers for the FastIMU library and the I2C communication interface, respectively.

> Don't forget to install the library as it was previously shown

**Constant Definitions:**

```cpp
#define IMU_ADDRESS 0x68    //Change to the address of the IMU
#define PERFORM_CALIBRATION //Comment to disable startup calibration
MPU6050 IMU;                //Change to the name of any supported IMU! 
```

- `IMU_ADDRESS`: Defines the I2C address of the IMU sensor.
- `PERFORM_CALIBRATION`: A preprocessor directive that controls whether the IMU calibration is performed during setup.
- `IMU`: Creates an instance of the `MPU6050` class to represent the IMU sensor.

**Variable Declarations:**

```cpp
calData calib = { 0 };  //Calibration data
AccelData accelData;    //Sensor data
```

- `calib`: Stores the calibration data for the IMU.
- `accelData`: Stores the accelerometer data from the IMU.

**Setup Function:**

```cpp
void setup() {
  // Initialize I2C communication
  Wire.begin();
  Wire.setClock(400000); // Set I2C clock speed to 400kHz

  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  // Initialize the IMU
  int err = IMU.init(calib, IMU_ADDRESS);
  if (err != 0) {
    // Handle error if initialization fails
    Serial.print("Error initializing IMU: ");
    Serial.println(err);
    while (true) {
      ;
    }
  }

  // Perform calibration if enabled
#ifdef PERFORM_CALIBRATION
  Serial.println("FastIMU calibration & data example");

  delay(5000);
  Serial.println("Keep IMU level.");
  delay(5000);
  IMU.calibrateAccelGyro(&calib);
  Serial.println("Calibration done!");
  Serial.println("Accel biases X/Y/Z: ");
  Serial.print(calib.accelBias[0]);
  Serial.print(", ");
  Serial.print(calib.accelBias[1]);
  Serial.print(", ");
  Serial.println(calib.accelBias[2]);

  delay(5000);
  IMU.init(calib, IMU_ADDRESS);
#endif
}
```

- Initializes I2C communication and serial communication.
- Initializes the IMU using the `IMU.init()` function.
- If calibration is enabled, it prompts the user to keep the IMU level, performs calibration, and prints the calibration data.
- Re-initializes the IMU with the calibrated data.

**Loop Function:**

```cpp
void loop() {
  // Update IMU data
  IMU.update();

  // Get accelerometer data
  IMU.getAccel(&accelData);

  // Print accelerometer data
  Serial.print(accelData.accelX);
  Serial.print("\t");
  Serial.print(accelData.accelY);
  Serial.print("\t");
  Serial.print(accelData.accelZ);
  Serial.print("\t");

  // Print temperature if available
  if (IMU.hasTemperature()) {
    Serial.print("\t");
    Serial.println(IMU.getTemp());
  }

  // Delay for 100 milliseconds
  delay(100);
}
```

- Continuously updates the IMU data.
- Reads the accelerometer data and prints it to the serial monitor.
- Prints the temperature if available.

We can run this example by running the following command:
```bash
arduino-cli compile --fqbn rp2040:rp2040:rpipico ./read_accelereration_arduino/read_accelereration_arduino.ino
arduino-cli upload -p /dev/ttyACM0 --fqbn rp2040:rp2040:rpipico ./read_accelereration_arduino/read_accelereration_arduino.ino
```

We can see the results from a serial monitor using a baud rate of 115200 in `/dev/ttyACM0`.

---

**Next**: [Motion Sensor Implementation](./imu_impl.md)