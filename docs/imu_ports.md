# 4.3 MotionSensor Ports

In the [previous section](./ports_types.md) we saw the method to make a port Type. In this section we will show the method to create components, but more specifically the creation and development of the MotionSensor (IMU) Component. 

## Creating Component

In order to use the ports we created, we need to make a `MotionSensor` component.

```bash
# In: Components
fprime-util new --component 
```

This command will prompt you for some inputs. Answer the prompts as shown below:
> When input is blank the default selection is in parenthesis

```bash
$ fprime-util new --component
[INFO] Cookiecutter source: using builtin
  [1/8] Component name (MyComponent): MotionSensor
  [2/8] Component short description (Component for F Prime FSW framework.): Component for MPU6050
  [3/8] Component namespace (Components): 
  [4/8] Select component kind
    1 - active
    2 - passive
    3 - queued
    Choose from [1/2/3] (1): 2
  [5/8] Enable Commands?
    1 - yes
    2 - no
    Choose from [1/2] (1): 
  [6/8] Enable Telemetry?
    1 - yes
    2 - no
    Choose from [1/2] (1): 
  [7/8] Enable Events?
    1 - yes
    2 - no
    Choose from [1/2] (1): 
  [8/8] Enable Parameters?
    1 - yes
    2 - no
    Choose from [1/2] (1): 
[INFO] Found CMake file at 'fprime-space-sw-exercise-ast/Components/CMakeLists.txt'
Add MotionSensor to fprime-space-sw-exercise-ast/Components/CMakeLists.txt at end of file? (yes/no) [yes]: 
Generate implementation files? (yes/no) [yes]: 
Refreshing cache and generating implementation files...
[INFO] Created new component and generated initial implementations.
```

> Inspect the files inside the directory that we created.

By inspecting the `fpp` file, we can see that we model a [passive component](https://nasa.github.io/fprime/UsersGuide/user/port-comp-top.html), which means that it will execute whenever it is being invoked by a port.

> Those types of components can also be `queue` type components, but for simplicity we will develop it as passive component. 

Edit the `MotionSensor.fpp` like this bellow:

```
module Components {
    @ Component for MPU6050
    passive component MotionSensor {

        sync input port run: Svc.Sched
        output port MpuDataOut: [3] MPU_data

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}
```

### Ports
* `run` : Synchronous input port that will be executed based on the `rateGroup`.
* `MpuDataOut` : Output port that will export the data from the MPU6050.

## Generate the Implementation Files

Now you have written the FPP code for the component, but the cpp and hpp files do not yet reflect the changes you have made to the fpp file. To get the cpp and hpp to reflect the specs you have defined in the fpp, you need to use the implement command as shown below:

```bash
# In: MotionSensor
fprime-util impl
```

```bash
# In: MotionSensor
mv MotionSensor.template.cpp MotionSensor.cpp
mv MotionSensor.template.hpp MotionSensor.hpp
```

Build `MotionSensor` to make sure everything worked as expected.

```bash
# In: MotionSensor
fprime-util build 
```
---

**Next**: [Back to Arduino...](./arduino.md)