# 4.6 Defining telemetry and events

In the [previous section](./imu_impl.md) we completed the setup to use the `FastIMU` library. Now we need to see their results using telemetry and events.

## Back to the FPP Model

In order to create telemetry and events for a component, we need to write it inside the `.fpp` file.

The `MotionSensor.fpp` should look like this

```
sync input port run: Svc.Sched
output port MpuDataOut: [3] MPU_data

@ Failed to initialize I2C
event MpuInitFail severity warning high \
    format "MPU not found"

@ Successful initializion of I2C
event MpuInitSucc severity activity high \
    format "MPU found"

@ X, Y, Z acceleration from accelerometer
telemetry accelerometer: Vector id 0 update always format "{} g"

@ I2C current status
telemetry connected: bool id 1 update always format "{}"

@ Temperaure of the sensor in Celsius scale
telemetry tempC: F32 id 2 update always format "{} degC"
```

### Events
* `MpuInitFail`: Failed to initialize I2C with a `warning high` type of log.
* `MpuInitSucc`: Successful initializion of I2C with an `activity high` type of log.

### Telemetry
* `accelerometer`: X, Y, Z acceleration from accelerometer. It updates in every call of the run port. Type of `MPU_data Vector`.
* `connected`: I2C current status. It updates in initialization. Type of `bool`.
* `tempC`: Temperaure of the sensor in Celsius scale. Type of `F32`.

Now we need to build again the component to auto-generate in the `build-fprime-automatic-rpipico/Components/MotionSensor/MotionSensorComponentAc` as we have shown before.

In this section we learned how to add telemetry and events in our fpp model. In the next section we will learn how to use those and add logic to our component.

---

**Next**: [Developing logic in component](./adding_logic.md)