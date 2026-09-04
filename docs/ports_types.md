# 4.2 Defining ports and types

In the [previous section](./mpu6050-intro.md) we saw an overview of the MPU6050 sensor.

The development of the component begins by making a model of the component using the [F Prime Prime](https://github.com/nasa/fpp/wiki) language. 


## MPU_Data

In F Prime, a type definition defines a kind of data that you can pass between components or use in commands and telemetry.

In the `Components` path we need to create the `MPU_data` directory:

```bash
# In: Components directory 
mkdir MPU_data 
cd MPU_data
```
The user defines types in an fpp (F prime prime) file. Use the the command below to create an empty fpp file to define the `MPU_data` port. 

```bash
# In: MPU_data
touch MPU_data.fpp
```
Use your favorite text editor, visual studios, nano, vim, etc…, and add the following to `MPU_data.fpp`.

```
module Components {

    @ 3-tuple type used for telemetry
    array Vector = [3] F32
    
    port MPU_data (
        acceleration: Vector
    )
}
```

The MPU will export the acceleration of the 3 axis (X, Y, Z). 

In the fpp file it is shown that there is a array of size 3 of type `F32` named `Vector`, that will be passed by the `acceleration` port.

**⚠️ This will be used as the output port Type in the Payload Component!**

Next we need to make a `CMakeLists.txt` and edit it in order to be included in the build. 

```bash
# In: MPU_data
touch CMakeLists.txt
```

Use a text editor to replace whatever is in CMakeLists.txt, most likely nothing, with the following.

```cmake
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/MPU_data.fpp"
)

register_fprime_module()
```

> Note: if you have not generated a build cache already, you may need to run `fprime-util generate` before you can build.

We also need to add the FPP to the projects build in `Components/CMakeLists.txt`

```cmake
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/MPU_data/")
```

And then build in project directory.

```bash
# In: project home dir
fprime-util build 
```

At this point you have successfully created the `MPU_data` type and added it to the project build. You can add more types here later if you feel so inclined.

---

**Next**: [MotionSensor Ports](./imu_ports.md)