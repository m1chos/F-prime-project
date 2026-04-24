# Software setup

### Outline

1. [Virtual Machine and F´ setup](#virtual-machine-and-f´-setup) 
2. [Arduino CLI Installation](#arduino-cli-installation)
3. [F´ demo](#fprime-demo)
4. [Raspberry Pi Pico](#raspberry-pi-pico)


## Virtual Machine and F´ setup

> For any troubleshooting refer to [official fprime installation guide](https://nasa.github.io/fprime/INSTALL.html#system-requirements)

F´ depends on several items before the user should attempt to install it. One of them is, that it runs in Linux-based systems.

If you are a Windows 10 (or above) user, you can install [Windows Subsystem for Linux](https://learn.microsoft.com/en-us/windows/wsl/install) or running [Ubuntu](https://releases.ubuntu.com/jammy/) in on a Virtual Manager like [VirtualBox](https://www.virtualbox.org/wiki/Downloads).

After installing it you need to update your system and download the following packages using:
```bash
sudo apt update && sudo apt upgrade -y
sudo apt install git cmake python3 python3-pip ipython3  python-is-python3 python3-venv build-essential unzip
```
Now setup your python virtual environment using:
```bash
python3 -m venv fprime
source fprime/bin/activate
```

By activating the `fprime` you can now install the fprime bootstrap tool
```bash
pip install fprime-bootstrap
```

Now we are ready to start an fprime project!

## Cloning the git repository

In order to download the exercise template we need to clone its repository in our local system. This can be done by using

```bash
git clone https://ci.software.aerospace.uoa.gr/aggelosmots/space-software-lab.git
```
And then get in the directory and activate the projects python virtual environment

``` bash
code space-software-lab # or cd space-software-lab
python -m venv "fprime-venv"
. fprime-venv/bin/activate
```

This allows to use the various tools that fprime provides

> ⚠️: Make sure to use the right virtual environment everytime you open a new shell!

## Arduino CLI Installation
After the set up, you will have to setup [arduino-cli](https://arduino.github.io/arduino-cli/0.35/) with the following command:

```bash
mkdir -p /home/$USER/bin
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=/home/$USER/bin sh
```

where `BINDIR` is the path that `arduino-cli` command will be installed. After that restart terminal session and `arduino-cli` command should be available

### Install arduino-cli-wrapper
```bash
python -m pip install --upgrade pip
pip install arduino-cli-cmake-wrapper
```

And then restart the bash session.

### Setup arduino-cli for Arduino boards
We will be doing this exercise using an Raspberry Pi Pico (RP2040), so we need to setup the BSP of the RP2040 using arduino-cli.

Initialize the arduino-cli configuration file.
```bash
arduino-cli config init
```
Board manager URL 
```bash
arduino-cli config add board_manager.additional_urls https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
```

Install the new board packages.
```bash
arduino-cli core update-index
arduino-cli core install rp2040:rp2040@3.9.5
```

For other development boards follow this [link](https://github.com/fprime-community/fprime-tutorial-arduino-blinker/blob/main/docs/arduino-cli-install.md)

### Adding MPU6050 library
Now we are ready to use fprime with the Arduino Framework Support.


Next we need to install the FastIMU library which we need for MPU6050
```bash
arduino-cli lib install FastIMU
```

Inspect and run the examples from [FastIMU github repository](https://github.com/LiquidCGS/FastIMU) in order to develop later your own code.  

## Fprime demo

### Configuration

In this project we will work with v3.4.1 of fprime. Run the following commands in terminal in order to setup. 

```bash
chmod +x setup.sh
./setup.sh
pip install -r ./fprime/requirements.txt
```

This will download fprime from archive of fprime repo, unzip it and rename it as `fprime` in your directory. Lastly, it will download the python packages required to begin developing an fprime project.

## Build template code

### Build demo
```bash
fprime-util generate rpipico
```

After build-cache generation we need to create the projects Deployment using `fprime-util new --deployment`

```bash
fprime-util new --deployment
```
Next, you will need to name your topology. The output will look like below:

> ⚠️: It is reccommended to use only latin characters e.g. `MissionDeployment`.

```bash
[INFO] Cookiecutter source: fprime-arduino-deployment-cookiecutter
  [1/1] deployment_name (fprime-arduino-deployment): MissionDeployment
[INFO] Found CMake file at 'fprime-space-sw-exercise-ast/project.cmake'
Add MissionDeployment to fprime-space-sw-exercise-ast/project.cmake at end of file? (yes/no) [yes]: 
[INFO] New deployment successfully created: /home/<username>/path/to/fprime-space-sw-exercise-ast/MissionDeployment
```

```bash
fprime-util build rpipico # if not specified in settings.ini
```

> You can add in `settings.ini` file the `default_toolchain` and add the proper toolchain. Example:

```
[fprime]
project_root: .
framework_path: ./fprime
library_locations: ./fprime-arduino
default_toolchain: rpipico
deployment_cookiecutter: fprime-arduino-deployment-cookiecutter

default_cmake_options:  FPRIME_ENABLE_FRAMEWORK_UTS=OFF
                        FPRIME_ENABLE_AUTOCODER_UTS=OFF
```

## Raspberry Pi Pico

### Upload binary to RPI Pico

Hold the BOOTSEL Button and plug in the USB connector. The file system of the RPI should be mounted.

Inside you will probably see 2 files named *INDEX.HTM* and *INFO_UF2.TXT*. The binary of the RPI Pico should have the extention name `<bin_name>.uf2`. After a successful build this file can be found in path `build-artifacts/<platform>/<Deployment>/bin/<bin_name>.uf2`. 

> ⚠️: For WSL users, first check the [section bellow](#wsl-only-to-run-gds).

In our case this is `build-artifacts/<toolchain>/<Deployment>/bin/<Deployment>.uf2`. Copy this file in the RPI's storage. After that, RPI Pico will be ejected from your filesystem and it will boot with your software.

### Run GDS
```bash
fprime-gds -n --dictionary ./build-artifacts/<toolchain>/<deployment>/dict/<deployment>TopologyAppDictionary.xml --comm-adapter uart --uart-device /dev/ttyACM0 --uart-baud 115200
```
where `/dev/ttyACM0` is the path of the Serial port of the Raspberry Pi Pico. In mnemonics you can find an example deployment with the LedControl commands.

---

### WSL Only to run GDS

For WSL you will have to install and use [USBIPD](https://learn.microsoft.com/en-us/windows/wsl/connect-usb). You will need to use the attach command, whenever the USB is removed from your computer. 

> ⚠️: Those commands need to be run in **Powershell** (**not Bash!!**)

```powershell
usbipd list
usbipd bind --busid <busid>
usbipd attach --wsl --busid <busid>
usbipd detach --busid <busid>
```

---

When GDS opens in your default browser, try the Led commands

> If connection can't be established make sure that you have no firewall enabled

[Back to the main page](../README.md)