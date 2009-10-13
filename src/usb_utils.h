#include "usb.h"
#include "usbprogrammer.h"

// See : /usr/include/usb.h
// See : perldoc Device::USB
// See : [ location of Device::USB... ]

#define USBPROGRAMMER_TIMEOUT 100 
#define SHORT_BUFFER_SIZE (SLOSCOPE_DATA_PACKETSIZE - 2)

void lib_usb_test();
struct usb_device *lib_find_usb_device( int vendor, int product );

int getset_slo_scope_state(usb_dev_handle *handle, int *state_new);
int getset_slo_scope_period(usb_dev_handle *handle, int *period_new);
int get_slo_scope_reference_voltage(usb_dev_handle *handle);

int getset_slo_scope_output_state(usb_dev_handle *handle, int *state_new);
int getset_slo_scope_output_state_a(usb_dev_handle *handle, int *state_new);
int getset_slo_scope_output_state_b(usb_dev_handle *handle, int *state_new);

int get_hw_version(usb_dev_handle *handle);
int get_sw_version_major(usb_dev_handle *handle);
int get_sw_version_minor(usb_dev_handle *handle);

//void lib_set_slo_scope_state( struct usb_device *dev, int state );
//void lib_read_slo_data(struct usb_device *dev);

void lib_read_slo_data(struct usb_device *dev, char *raw);
