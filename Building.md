## Introduction ##

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


## Building the project : Generic ##

The prerequisite for doing these steps is an installation of 'cmake', and 'svn'.

  1. Check out the code (this will create a new sub-directory with the code in it) :
    * `svn checkout http://usb-scope.googlecode.com/svn/trunk/ usb-scope-read-only`
  1. Go to the build directory within the downloaded code : `cd build`
  1. To create a suitable Makefile  v. cool), execute : `cmake ..`
    * Did you type that in correctly?  The 'cmake' is followed by space and two dots...
  1. Finally, execute : `make`

On Linux, because of it's security model, you'll have to inform the usb device manager that ordinary users should have permission to write to the device (see [LinuxUSBdev](LinuxUSBdev.md)).

To run the software, just type one of the following (still in the build directory) :
  * `usb-programmer-config`
  * `SLOscope`


### Specific Instructions : Linux (Fedora) ###

To install cmake and svn initially just type : `yum install cmake svn`.

The `cmake ..` automatically detects the installed software, so that it can build the makefile appropriately.  Along the way it may request wxWidgets (which, on the Fedora machine means an installation of the platform-specific library wxGTK, plus its headers) :
  * `yum install wxGTK wxGTK-devel`

Somehow, the correct libusb was already installed.

Then, the rest of the build / run process works seamlessly (after applying the [LinuxUSBdev](LinuxUSBdev.md) magic).