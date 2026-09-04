# 4.8 Full System Integration

In the [previous section](./adding_logic.md) we completed the implmentation of the MotionSensor component. Now that the component is ready to be used, its time to integrate it into the **Topology** of the system.

## Adding MotionSensor Component to the Topology

In order to add a component into the **Topology**, we need to instantiate it. This can be done by adding this line into the deployment in `MissionDeployment/Top/instances.fpp` into the **Passive component instances** in the end of the list.

```
# MPU
instance mpu: Components.MotionSensor base id 0x11000
```

Then we need to add in `MissionDeployment/Top/topology.fpp` in the **Instances used in the topology**, **Rate Groups** and **connections MissionDeployment** the following lines.

```
topology MissionDeployment {
    
    # ----------------------------------------------------------------------
    # Instances used in the topology
    # ----------------------------------------------------------------------

    
    ...
    instance mpu
    ...
}
...

# ----------------------------------------------------------------------
# Direct graph specifiers
# ----------------------------------------------------------------------


connections RateGroups {
    ...
    rateGroup1.RateGroupMemberOut[3] -> mpu.run
    ...
}
```

We set up the `mpu` instance in the `RateGroup1`. This rate group is 100 ms and it can be viewed in [MissionDeploymentTopology.cpp](../MissionDeployment/Top/MissionDeploymentTopology.cpp) in the lines.

```cpp
Svc::RateGroupDriver::DividerSet rateGroupDivisors{{{100, 0}, {1000, 0}}};

...

NATIVE_INT_TYPE rateGroup1Context[FppConstant_PassiveRateGroupOutputPorts::PassiveRateGroupOutputPorts] = {};
NATIVE_INT_TYPE rateGroup2Context[FppConstant_PassiveRateGroupOutputPorts::PassiveRateGroupOutputPorts] = {};

...

rateGroup1.configure(rateGroup1Context, FW_NUM_ARRAY_ELEMENTS(rateGroup1Context));
rateGroup2.configure(rateGroup2Context, FW_NUM_ARRAY_ELEMENTS(rateGroup2Context));

```
> `RateGroup2` as we can see it is set up in 1000 ms.

# Configuring the MotionSensor Component
So far the MotionSensor has been instantiated and wired, but has not been told to actually initialize in the code!

This can be set up in [MissionDeploymentTopology.cpp](../MissionDeployment/Top/MissionDeploymentTopology.cpp) by adding the following line to the `setupTopology` function.

```cpp
void setupTopology(const TopologyState& state) {
    ...
    // Initialize MPU instances
    mpu.init_i2c();
    ...
}

```

This code tells to run the `init_i2c()` that we created in [Developing logic in component](./imu_impl.md) section.

## Packetization

Lastly, we need to add the telemetry and events we created into the `MissionDeploymentPackets.xml` with the following lines above **Ignored packets**.

```xml
<packet name="MPU" id="10" level="1">
    <channel name="mpu.accelerometer"/>
    <channel name="mpu.connected"/>
    <channel name="mpu.tempC"/>
</packet>
```

---

Congratulations! You've completed the MotionSensor component to the rate group driver. It is time to run it in GDS as we have discussed in the [Software Setup](./sw_setup.md) section.

### End of tutorial 
[Return to home page](../README.md)