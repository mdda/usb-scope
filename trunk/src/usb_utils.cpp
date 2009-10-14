#include <stdio.h>
#include <string.h> // for memcpy
#include "usb_utils.h"

/*

// PROBLEM : 
    // Hmm : on Linux madison 2.6.30.8-64.fc11.x86_64 #1 SMP Fri Sep 25 04:43:32 EDT 2009 x86_64 x86_64 x86_64 GNU/Linux
    //   - needs to be root to read device descriptors properly : What is fix ??
   
    // And : on 
    //   - everything is hunky-dory.

// RESOLUTION : 

The following file does the trick for Linux Ubuntu 7.10:

# udev rules file for atmel usb devices (for udev 0.98 version)
#
SUBSYSTEM!="usb_device", GOTO="pololu_rules_end"
ACTION!="add", GOTO="atmel_rules_end"

ATTRS{idVendor}=="1ffb", ATTRS{idProduct}=="0081", MODE="0660", GROUP="plugdev"

LABEL="pololu_rules_end" 

*/



/*
 * Find a particular device
 *
 *  vendor  - the vendor id
 *  product - product id for that vendor
 *
 * returns a pointer to the device if it is found, NULL otherwise.
 */
struct usb_device *lib_find_usb_device( int vendor, int product ) {
 struct usb_bus *busses;
   
 usb_init();
 usb_find_busses();
 usb_find_devices();
    
 busses = usb_get_busses();

	for (struct usb_bus *bus = busses; bus; bus = bus->next) {
  for (struct usb_device *dev = bus->devices; dev; dev = dev->next) {
   //printf("lib_find_usb_device : (vendor=%04x, product=%04x) \n", dev->descriptor.idVendor, dev->descriptor.idProduct);
   if((dev->descriptor.idVendor == vendor) && (dev->descriptor.idProduct == product)) {
    return dev; // This appears to be a pointer into a global structure - no need to 'free'
   }
  }
 }
 return NULL;
}


//int usb_control_msg(usb_dev_handle *dev, int bRequestType, 
//                   int bRequest, int wValue, int wIndex, char *bytes, int wLength, int timeout);
int getset_usb_programmer_variable(usb_dev_handle *handle, int key, int *value_new, int size) {
 char bytes[4]; 
 
 //  0b01000000
 int bytes_read = usb_control_msg(handle, (USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN),   
                                  REQUEST_GET_VARIABLE, 0, key, &bytes[0], size, USBPROGRAMMER_TIMEOUT);
 //printf("%d bytes read from programmer\n", bytes_read);
 
 int value_old = bytes[0];
 if(size>1) {
  value_old += bytes[1]<<8; 
 }
 
 if(value_new) {
  // Very strange that it needs 'USB_ENDPOINT_IN'
  int bytes_written = usb_control_msg(handle, (USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN), 
                                      REQUEST_SET_VARIABLE, *value_new, key, NULL, 0, USBPROGRAMMER_TIMEOUT);
  //printf("%d bytes written to programmer\n", bytes_written);
 }
 
 return value_old;
}

// state = SLOSCOPE_STATE_OFF | SLOSCOPE_STATE_2ANALOG | SLOSCOPE_STATE_1ANALOG_1DIGITAL
int getset_slo_scope_state(usb_dev_handle *handle, int *state_new) {
 return getset_usb_programmer_variable(handle, VARIABLE_SLOSCOPE_STATE, state_new, 1);
}

// The slo-scope will take a new reading every (Period+1)/12 microseconds.  Default 539.
int getset_slo_scope_period(usb_dev_handle *handle, int *period_new) {
 return getset_usb_programmer_variable(handle, VARIABLE_SLOSCOPE_PERIOD, period_new, 2);
}

int get_slo_scope_reference_voltage(usb_dev_handle *handle) {
 return getset_usb_programmer_variable(handle, VARIABLE_FVR_ADC, NULL, 2);
}

// state = {0=>input, 1=>output lo, 3=>output hi};
int getset_slo_scope_output_state_a(usb_dev_handle *handle, int *state_new) {
 int state_new_both=0xFFFF;
 if(state_new) {
   state_new_both &= (*state_new & 0x00FF);
 }
 int state_old_both=getset_slo_scope_output_state(handle, &state_new_both);
 return (state_old_both>>0) & 0x00FF;
}

// state = {0=>input, 1=>output lo, 3=>output hi};
int getset_slo_scope_output_state_b(usb_dev_handle *handle, int *state_new) {
 int state_new_both=0xFFFF;
 if(state_new) {
   state_new_both &= (*state_new & 0x00FF)<<8;
 }
 int state_old_both=getset_slo_scope_output_state(handle, &state_new_both);
 return (state_old_both>>8) & 0x00FF;
}

// This is a two-byte version : A=low byte, B=high byte (0xFF => no change to selected variable)
int getset_slo_scope_output_state(usb_dev_handle *handle, int *state_new) {
 return getset_usb_programmer_variable(handle, VARIABLE_SLOSCOPE_OUTPUT_STATE, state_new, 2);
}

int get_hw_version(usb_dev_handle *handle) {
 return getset_usb_programmer_variable(handle, VARIABLE_HW_VER, NULL, 1);
}

int get_sw_version_major(usb_dev_handle *handle) {
 return getset_usb_programmer_variable(handle, VARIABLE_SW_MAJOR, NULL, 1);
}
int get_sw_version_minor(usb_dev_handle *handle) {
 return getset_usb_programmer_variable(handle, VARIABLE_SW_MINOR, NULL, 1);
}



/*
// state = SLOSCOPE_STATE_OFF | SLOSCOPE_STATE_2ANALOG | SLOSCOPE_STATE_1ANALOG_1DIGITAL
void lib_set_slo_scope_state( struct usb_device *dev, int state ) {  //
 usb_dev_handle *handle=usb_open(dev);
 
// int retval = usb_control_msg(handle, requesttype, request, value, index, (char *)data, size, timeout (ms) );
 int retval = usb_control_msg(handle, 0x40, REQUEST_SET_VARIABLE, state, VARIABLE_SLOSCOPE_STATE, NULL, 0, USBPROGRAMMER_TIMEOUT);
// If you succeed (with !OFF), then the yellow LED of the programmer should be on solid.
 printf("set:VARIABLE_SLOSCOPE_STATE -> %d\n", retval);

 usb_close(handle);
 return;
}
*/

void lib_explain_slo_data(char *data) {
 printf("Missing = %d, Frame = %d\n  Data : ", data[0], data[1]);
 for(int i=2; i<SLOSCOPE_DATA_PACKETSIZE; i++) {
  printf("%02x ", data[i]);
 }
 printf("\n");
}

void lib_read_slo_data_TEST(struct usb_device *dev) {
 char data1[SLOSCOPE_DATA_PACKETSIZE];
 char data2[SLOSCOPE_DATA_PACKETSIZE];
 char data3[SLOSCOPE_DATA_PACKETSIZE];
 
 usb_dev_handle *handle=usb_open(dev);
 
// The slo-scope will take a new reading every (Period+1)/12 microseconds.  Default 539.

// int retval = usb_control_msg(handle, requesttype, request, value, index, (char *)data, size, timeout (ms) );
 int retval = usb_control_msg(handle, 0x40, REQUEST_SET_VARIABLE, SLOSCOPE_PERIOD_10K, VARIABLE_SLOSCOPE_PERIOD, NULL, 0, USBPROGRAMMER_TIMEOUT);
 printf("set:VARIABLE_SLOSCOPE_PERIOD -> %d\n", retval);
 
/*
 To read data from the slo-scope, you must use the REQUEST_SET_VARIABLE request to set VARIABLE_SLOSCOPE_STATE, as above.  
 Then you can read data from  Endpoint 5 IN.  This is an interrupt endpoint.  
 
 A new packet of slo-scope data will be available every millisecond.
 
 Each packet is 22 bytes long.  
 
 The first byte is the number of missed readings (readings that had to be discarded because there was no USB buffer available for them) between the time the last packet was finished and the time this packet was started.  It's good for this number to be positive because a value of zero means the slo-scope might be sampling too slowly.  
 
 The second byte is the low 8 bits of the USB frame number when the packet was started (basically a milli-second timer).  If the SLO-scope is sampling fast enough and the computer is reading fast enough, then the USB frame number should increase by exactly one each time you read a new packet from the slo-scope, which should be every millisecond.
*/
 
// int retval=usb_interrupt_read(usb_dev_handle *dev, int ep, char *bytes, int size, int timeout);
 int retval1=usb_interrupt_read(handle, SLOSCOPE_DATA_ENDPOINT, data1, SLOSCOPE_DATA_PACKETSIZE, USBPROGRAMMER_TIMEOUT);
 int retval2=usb_interrupt_read(handle, SLOSCOPE_DATA_ENDPOINT, data2, SLOSCOPE_DATA_PACKETSIZE, USBPROGRAMMER_TIMEOUT);
// int retval3=usb_interrupt_read(handle, SLOSCOPE_DATA_ENDPOINT, data3, SLOSCOPE_DATA_PACKETSIZE, USBPROGRAMMER_TIMEOUT);

 usb_close(handle);
 
 printf("get:SLOSCOPE_DATA -> %d\n", retval1);
 lib_explain_slo_data(data1);
 
 printf("get:SLOSCOPE_DATA -> %d\n", retval2);
 lib_explain_slo_data(data2);
 
// printf("get:SLOSCOPE_DATA -> %d\n", retval3);
// lib_explain_slo_data(data3);
 
 return;
}

// Reads (SLOSCOPE_DATA_PACKETSIZE-2) bytes into the raw buffer - space is already allocated
void lib_read_slo_data(struct usb_device *dev, char *raw) { 
 char data[SLOSCOPE_DATA_PACKETSIZE];
 
 usb_dev_handle *handle=usb_open(dev);
 
// The slo-scope will take a new reading every (Period+1)/12 microseconds.  Default 539.

/*
 To read data from the slo-scope, you must use the REQUEST_SET_VARIABLE request to set VARIABLE_SLOSCOPE_STATE, as above.  
 Then you can read data from  Endpoint 5 IN.  This is an interrupt endpoint.  
 
 A new packet of slo-scope data will be available every millisecond.
 
 Each packet is 22 bytes long.  
 
 The first byte is the number of missed readings (readings that had to be discarded because there was no USB buffer available for them) between the time the last packet was finished and the time this packet was started.  It's good for this number to be positive because a value of zero means the slo-scope might be sampling too slowly.  
 
 The second byte is the low 8 bits of the USB frame number when the packet was started (basically a milli-second timer).  If the SLO-scope is sampling fast enough and the computer is reading fast enough, then the USB frame number should increase by exactly one each time you read a new packet from the slo-scope, which should be every millisecond.
*/
 
// int retval=usb_interrupt_read(usb_dev_handle *dev, int ep, char *bytes, int size, int timeout);
 int retval=usb_interrupt_read(handle, SLOSCOPE_DATA_ENDPOINT, data, SLOSCOPE_DATA_PACKETSIZE, USBPROGRAMMER_TIMEOUT);

 usb_close(handle);

 // BLAM !
 memcpy(raw, &data[2], (SLOSCOPE_DATA_PACKETSIZE-2) );
 
 return;
}




void UNUSED_lib_usb_TEST() {
	struct usb_bus *busses;
   
 usb_init();
 usb_find_busses();
 usb_find_devices();
    
 busses = usb_get_busses();

	for (struct usb_bus *bus = busses; bus; bus = bus->next) {
  for (struct usb_device *dev = bus->devices; dev; dev = dev->next) {
   printf("find_usbprogrammer : bDeviceClass = %d\n", dev->descriptor.bDeviceClass);
   
   /* Check if this device is a printer */
   if (dev->descriptor.bDeviceClass == 7) {
    /* Open the device, claim the interface and do your processing */
   }
   
   for (int c = 0; c < dev->descriptor.bNumConfigurations; c++) {           /* Loop through all of the configurations */
    for (int i = 0; i < dev->config[c].bNumInterfaces; i++) {               /* Loop through all of the interfaces */
     for (int a = 0; a < dev->config[c].interface[i].num_altsetting; a++) { /* Loop through all of the alternate settings */
      printf("find_usbprogrammer : alt - bDeviceClass = %d\n", dev->descriptor.bDeviceClass);
      
      /* Check if this interface is a printer */
      if (dev->config[c].interface[i].altsetting[a].bInterfaceClass == 7) {
       /* Open the device, set the alternate setting, claim the interface and do your processing */
      }
      
     }
    }
   }
  }
 }
}

