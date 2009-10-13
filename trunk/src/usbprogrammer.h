//int usb_control_msg(usb_dev_handle *handle, int bRequestType, int bRequest, int wValue, int wIndex, char *bytes, int wLength, int timeout);

#define REQUEST_GET_VARIABLE 0x81
/* To get the value of a variable from the device, make a request like this:
 *   bRequestType = 0b01000000  Direction=Device-to-host, Type=Vendor, Recipient=Device
 *   bRequest = REQUEST_GET_VARIABLE
 *   wValue = 0
 *   wIndex = The id of the variable (see VARIABLE_* defines below)
 *   wLength = [length of variable, in bytes]
 */
 
#define REQUEST_SET_VARIABLE 0x82
/* To set the value of a variable, make a request like this:
 *   bRequestType = 0b01000000  Direction=Device-to-host, Type=Vendor, Recipient=Device
 *   bRequest = REQUEST_SET_VARIABLE
 *   wValue = [value of variable]
 *   wIndex = The id of the variable (see VARIABLE_* defines below)
 *   wLength = 0
 */
 
#define VARIABLE_SLOSCOPE_STATE  0x42 // 1 byte
#define SLOSCOPE_STATE_OFF              0
#define SLOSCOPE_STATE_2ANALOG          1
#define SLOSCOPE_STATE_1ANALOG_1DIGITAL 2
 
#define VARIABLE_SLOSCOPE_PERIOD 0x40 // 2 bytes.  The slo-scope will take a new reading every (Period+1)/12 microseconds.  Default 539.
 
#define VARIABLE_FVR_ADC         0x41 // 2 bytes, read only.  A value of 1 indicates an error.  Otherwise it's a measurement of the 1.024 V internal voltage reference, as a number from 0 to 65472.
 
#define VARIABLE_SLOSCOPE_OUTPUT_STATE 0x43 // 2 bytes, first is line A and second is line B.
#define SLOSCOPE_OUTPUT_OFF       0
#define SLOSCOPE_OUTPUT_LOW       1
#define SLOSCOPE_OUTPUT_HIGH      3
#define SLOSCOPE_OUTPUT_NO_CHANGE 0xFF

#define SLOSCOPE_PERIOD_10K    (100*12 - 1)
#define SLOSCOPE_PERIOD_20K    ( 50*12 - 1)

#define SLOSCOPE_DATA_ENDPOINT     5
#define SLOSCOPE_DATA_PACKETSIZE  22

#define SLOSCOPE_VENDOR  (0x1ffb)
#define SLOSCOPE_PRODUCT (0x0081)


enum variableIds {
  VARIABLE_TARGET_VCC_ALLOWED_MINIMUM        = 0x01, // 1 byte, 
  VARIABLE_TARGET_VCC_ALLOWED_MAXIMUM_RANGE  = 0x02, // 1 byte, 
  VARIABLE_TARGET_VCC_MEASURED_MINIMUM       = 0x03, // 1 byte, read only
  VARIABLE_TARGET_VCC_MEASURED_MAXIMUM       = 0x04, // 1 byte, read only
  VARIABLE_PROGRAMMING_ERROR                 = 0x08, // 1 byte, read only
  VARIABLE_LINE_A_IDENTITY                   = 0x20, // 1 byte
  VARIABLE_LINE_B_IDENTITY                   = 0x21, // 1 byte
  // 0x40 - 0x4F are reserved for oscillsocope variables.  See oscilloscope_protocol.h
  VARIABLE_SW_MINOR                          = 0x90, // 1 byte
  VARIABLE_SW_MAJOR                          = 0x91, // 1 byte
  VARIABLE_HW_VER                            = 0x92, // 1 byte
  VARIABLE_SCK_DURATION                      = 0x98, // 1 byte
};
 
/* TARGET_VCC_UNITS is the conversion factor between all the TARGET_VCC variables above
 * and millivolts.  So (value in millivolts) = TARGET_VCC_UNITS * (value of VARIABLE_TARGET_VCC_MINIMUM).
 */
#define TARGET_VCC_UNITS 32
 
// LINE IDENTITY BYTES:
enum lineIds {
  LINE_IS_NOTHING = 0x00, // Line is not used for anything.
  LINE_IS_DTR = 0x04, // DB9 Pin 4 - OUT - Data Terminal Ready
  LINE_IS_RTS = 0x07, // DB9 Pin 7 - OUT - Request to Send
  LINE_IS_CD  = 0x81, // DB9 Pin 1 - IN - Carrier Detect
  LINE_IS_DSR = 0x86, // DB9 Pin 6 - IN - Data Set Ready
  LINE_IS_RI  = 0x89, // DB9 Pin 9 - IN - Ring Indicator
};
 
#define REQUEST_START_BOOTLOADER 0xFF
/* To enter bootloading mode, send this request:
 *   bRequestType = 0b01000000  Direction=Host-to-device, Type=Vendor, Recipient=Device
 *   bRequest = REQUEST_START_BOOTLOADER
 *   wValue = 0
 *   wIndex = 0
 *   wLength = 0
 */
 
enum programmingErrors {
  PROGRAMMING_ERROR_TARGET_VCC_BAD      = 0x01,
  PROGRAMMING_ERROR_SYNCH               = 0x02,
  PROGRAMMING_ERROR_IDLE_FOR_TOO_LONG   = 0x03,
  PROGRAMMING_ERROR_USB_NOT_CONFIGURED  = 0x04,
  PROGRAMMING_ERROR_USB_SUSPEND         = 0x05,
};
