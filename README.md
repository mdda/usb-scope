# usb-scope

( Automatically exported from code.google.com/p/usb-scope )

The Pololu USB Programmer device includes two extra ports that can sample analog data at 10KHz.

Pololu has created a Windows-only oscilloscope utility. However, now that robotics is attracting more and more 'software types', a cross-platform version SLOscope is essential.

This project provides both a configuration utility for the USB device, and a 'wxWidgets-based' SLOscope oscilloscope GUI. While Pololu have provided some help (like hardware specs, and moral support), this is a totally independent project.

It's all cross-platform, easily buildable, and open source. Have fun!
Code Coming Soon !

## Building the SLOscope code

### Introduction

The SLOscope source was developed on a Linux box running Fedora.  However, the tools used means that it should be seamlessly buildable on each of these platforms : 

  * Linux
    * Fedora
    * Ubuntu
    * etc...
  * MacOS
  * Windows
    * Microsoft compilers
    * GNU compilers
    * etc...

Once you've managed to build the project on your platform (especially if it's not Linux), please let us know your experiences!  Ideally, we'll be able to host installable binaries here too, so that _regular users_ of the Windows and Mac platforms can install it quickly too.


###  Building the project : Generic 

The prerequisite for doing these steps is an installation of 'cmake', and 'svn'.

 # Check out the code (this will create a new sub-directory with the code in it) : 
   * `svn checkout http://usb-scope.googlecode.com/svn/trunk/ usb-scope-read-only`
 # Go to the build directory within the downloaded code : `cd build`
 # To create a suitable Makefile  v. cool), execute : `cmake ..`
   * Did you type that in correctly?  The 'cmake' is followed by space and two dots...
 # Finally, execute : `make`

On Linux, because of it's security model, you'll have to inform the usb device manager that ordinary users should have permission to write to the device (see [LinuxUSBdev]).

To run the software, just type one of the following (still in the build directory) :
  * `usb-programmer-config`
  * `SLOscope`


### Specific Instructions : Linux (Fedora) 

To install cmake and svn initially just type : `yum install cmake svn`.

The `cmake ..` automatically detects the installed software, so that it can build the makefile appropriately.  Along the way it may request wxWidgets (which, on the Fedora machine means an installation of the platform-specific library wxGTK, plus its headers) :
  * `yum install wxGTK wxGTK-devel`

Somehow, the correct libusb was already installed.

Then, the rest of the build / run process works seamlessly (after applying the [LinuxUSBdev] magic).


##  Modifying (and hopefully improving) the software

### Modifying the GUI : Generic 
For modifying the GUI, you'll need wxglade installed.  This has the capability of modifying the C++ source code in-place, without destroying the customisation that was developed 'on top'.

*  Launch 'wxglade src/USBconfig.wxg' or 'wxglade src/SLOscope.wxg' 
*  Generate code (don't overwrite existing code - let it update the existing)
*  Rebuild


### Developing on Linux

On Fedora, to install wxGlade : `yum install wxGlade`

To build and (if successful) run in the ./build directory : 
`make && ./SLOscope`


## Command-line options for the configuration tool

### Introduction

```
$ ./usb-programmer-config --help
Usage: usb-programmer-config [-h] [-g] [-q] [-d] [-s <num>] [-p <num>] [-a <str>] [-b <str>]
  -h, --help        	displays help on the command line parameters
  -g, --gui         	enables the GUI (not yet)
  -q, --quiet       	don't show device settings
  -d, --dump        	view current settings in parsable format
  -s, --slo=<num>   	set SLO scope state: 0=off, 1=2A, 2=1A.1D
  -p, --period=<num>	set SLO scope period (new reading every (period+1)/12 microseconds, normal=539)
  -a, --line_a=<str>	set line A state : { i|input, 0|low, 1|high }
  -b, --line_b=<str>	set line B state : { i|input, 0|low, 1|high }
```

NB: If the usb-programmer-config complains : 'USB error: error sending control message: Operation not permitted', then try rerunning it as root - something funky was happening on one of my machines without that.  This may be a 'udev' problem (to do with the permissions that linux gives the device nodes when it finds them).  More info soon.

```
# ./usb-programmer-config --slo=1 --line_a=i --line_b=0
usb_set_debug: Setting debugging level to 2 (on)
Pololu USB programmer : HW version 15, SW version 2.10
  SLOscope mode : 2 analog inputs, 10KHz
    Sampling every 45.000 us
    Reference Voltage (1.024V) registers : 14400
    Direction of external lines :
      Line A : Input
      Line B : Ouput Low
```


## Running the SLOscope Oscilloscope GUI 


## Linux USBDev :   Getting user access to the USB device

### Problem

Since the Linux kernel doesn't 'know' the Pololu device, it doesn't give regular users privileges to write to it.

### Details - and quick fix 

```
$ usb-programmer-config
Gives meaning-less, and inconsistent results for the HW version (for instance)
```

while (doing the same as root) :
```
# usb-programmer-config
Look different (and correct)
```

Check the device permissions :

```
# lsusb 
Bus 005 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
Bus 004 Device 003: ID 046d:c03d Logitech, Inc. M-BT96a Pilot Optical Mouse
Bus 004 Device 002: ID 413c:2003 Dell Computer Corp. Keyboard
Bus 004 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
Bus 008 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
Bus 002 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 006 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
Bus 007 Device 011: ID 1ffb:0081  
Bus 007 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 003 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub

[root@madison build]# ls -l /dev/bus/usb/007/011 
crw-rw-r-- 1 root root 189, 778 2009-10-13 20:59 /dev/bus/usb/007/011

[root@madison build]# chmod 666 /dev/bus/usb/007/011 
```

Now : 
```
$ usb-programmer-config
Works beautifully...
```

### Details - and permanent fix  *work in progress* 

Need to create a 'udev rule' to assign the permissions when the device is plugged in.

Create a file called `/etc/udev/rules.d/65-pololu.rules` that contains :


(for Linux Ubuntu 7.10 - based on what I've read) :
```
# udev rules file for Pololu SLOscope USB device (for udev 0.98 version)

SUBSYSTEM!="usb_device", GOTO="pololu_rules_end"
ACTION!="add", GOTO="pololu_rules_end"

ATTRS{idVendor}=="1ffb", ATTRS{idProduct}=="0081", MODE="0666"

LABEL="pololu_rules_end" 
```

(for Fedora 11 - this is tested, and really works) :
```
# udev rules file for Pololu SLOscope USB device (for udev 0.98 version)

SUBSYSTEM!="usb", GOTO="pololu_rules_end"
ACTION!="add", GOTO="pololu_rules_end"

SYSFS{idVendor}=="1ffb", SYSFS{idProduct}=="0081", MODE="0666"

LABEL="pololu_rules_end" 
```

Test this with : `udevtest /bus/usb/devices/7-1`, if the device is under `/dev/bus/usb/007` ...


###  Complaint 

And, yes, I'm aware that this should be done with !ConsoleKit and HAL (and / or !DeviceKit).  But don't you think I would have been able to figure it out if it were documented properly ??  YMMV.
