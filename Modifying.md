## Modifying the GUI : Generic ##
For modifying the GUI, you'll need wxglade installed.  This has the capability of modifying the C++ source code in-place, without destroying the customisation that was developed 'on top'.

  1. Launch 'wxglade src/USBconfig.wxg' or 'wxglade src/SLOscope.wxg'
  1. Generate code (don't overwrite existing code - let it update the existing)
  1. Rebuild


### Developing on Linux ###

On Fedora, to install wxGlade : `yum install wxGlade`

To build and (if successful) run in the ./build directory :
`make && ./SLOscope`