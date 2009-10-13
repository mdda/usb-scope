#include "SLOscope.h"

myReadScope::myReadScope() {
  raw_data=new char[LONG_BUFFER_SIZE];
  dev = lib_find_usb_device(SLOSCOPE_VENDOR, SLOSCOPE_PRODUCT);
  
  filled_up_from=0;
  filled_up_next=0;
 
  sample_frequency_Hz=20000.0; // Rough estimate for now TODO
 
  please_pause=false; // Only time this is written 'this end'
  setPaused(false);
  
  if(dev) {
    usb_dev_handle *handle = usb_open(dev);
    slo_mode_saved = getset_slo_scope_state(handle, NULL);
    slo_mode_current=slo_mode_saved; // Just so that it's a known-good value
   
    // Here's an opportunity to save the state of the device...

    usb_close(handle);
  }
}


myReadScope::~myReadScope() {
  // delete s_mutexRawDataExtents;
  // delete raw_data;
 
  printf("Cleaning up USB\n");
  if(dev) {
    usb_dev_handle *handle = usb_open(dev);
    getset_slo_scope_state(handle, &slo_mode_saved);

    // Here's an opportunity to restore the state of the device...

    usb_close(handle);
  }
}

bool myReadScope::deviceFound() {
  return (dev != NULL);
}

// This is so that (if necessary) MUTEXs can be used to avoid cross-thread contention
usb_dev_handle *myReadScope::openHandle() {
 return usb_open(dev);
}

// This is so that (if necessary) MUTEXs can be used to avoid cross-thread contention
void myReadScope::closeHandle(usb_dev_handle *handle) {
  usb_close(handle);
}

void myReadScope::setPleasePause(bool pause) {
  please_pause = pause;
}
bool myReadScope::getPleasePause() {
  return please_pause;
}

bool myReadScope::getPaused() {
  return i_am_paused;
}
void myReadScope::setPaused(bool paused) {
  i_am_paused=paused;
}

void myReadScope::setSLOmode(int _mode) {
  usb_dev_handle *handle = openHandle();
  int v = _mode;
  getset_slo_scope_state(handle, &v);
  closeHandle(handle);
 
  slo_mode_current=_mode;
  printf("myReadScope::setSLOmode(%d)\n", slo_mode_current);
  return;
}

int myReadScope::getSLOmode() {
  return slo_mode_current;
}

void *myReadScope::Entry() {
  while(1) {
    if( getPaused() ) {  // I am in a paused state
      if( ! getPleasePause() ) {
        printf("In thread - stop pausing : switch to running\n");
        setPaused(false);  // Out of paused state
      }
      else {  // Remain paused
        // printf("In thread - continuing to pause\n");
        wxThread::Sleep(THREAD_WAIT_SHORT);
      }
    } 
    else {  // I am not paused at the moment
      if( getPleasePause() ) {
        printf("In thread - switch to pausing\n");
        setPaused(true);  // Declare that I am paused
      }
      else {
        // Do the data collection here
        // printf("In thread - running\n");
        readBlock();
      }
    }
  }
  return NULL;
}

void myReadScope::readBlock() {
  // printf("readBlock : [%d, %d)\n", filled_up_from, filled_up_next);
 
  // First, move the trailing 'from edge' if we're wrapping around into it
  s_mutexRawDataExtents.Lock();
  if(filled_up_next + SHORT_BUFFER_SIZE - filled_up_from > LONG_BUFFER_SIZE) {
    filled_up_from += SHORT_BUFFER_SIZE;
  }
  //  Now, if filled_up_from has gone beyond the buffer, move both back
  if(filled_up_from >= LONG_BUFFER_SIZE) {
    filled_up_from -= LONG_BUFFER_SIZE;
    filled_up_next -= LONG_BUFFER_SIZE;
    printf("readBlock Reset to : [%d, %d)\n", filled_up_from, filled_up_next);
  }
  s_mutexRawDataExtents.Unlock();

  
  if(true) { // Here is the real data read
    lib_read_slo_data(dev, raw_data);
  }
  else {  // This is just for debugging...
    // Now, fill in the data from 'filled_in_next' for SHORT_BUFFER_SIZE
    char *p = raw_data + (filled_up_next % LONG_BUFFER_SIZE);
    for(int i=0; i<SHORT_BUFFER_SIZE; i++) {
  //    p[i]=(char) ( rand() & 0xFF );
      p[i]=(char) ( (i+filled_up_next) & 0xFF );
    }
    wxThread::Sleep(1); // This is a bogus delay
  }  
  
  
  // Last, move the declared filled location to cover the filled in data
  s_mutexRawDataExtents.Lock();
  filled_up_next += SHORT_BUFFER_SIZE;
  s_mutexRawDataExtents.Unlock();
  
  return;
}
