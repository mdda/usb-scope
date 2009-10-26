#include "SLOscope.h"

myDetailScope::myDetailScope(wxWindow *parent, int id) : wxPanel(parent, id) {
  m_parent = parent;
 
  font = new wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier 10 Pitch"));
  setExamineCurrent(-1);  // This sets the extents automatically to be the last pixels # of samples...

  // Not implemented yet... 
  trigger_on=false;
 
  Connect(wxEVT_PAINT, wxPaintEventHandler(myDetailScope::OnPaint));
  Connect(wxEVT_SIZE, wxSizeEventHandler(myDetailScope::OnSize));
 
  Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(myDetailScope::OnMouse));
 
  Connect(wxEVT_TIMER, wxTimerEventHandler(myDetailScope::OnTimer));
 
//  EVT_CONTEXT_MENU(MyFrame::right_click) 
 
  refresh_timer = new wxTimer(this, 1);
  screen_dirty = true;
}

void myDetailScope::setReader(myReadScope *_reader) {
  reader=_reader;
}

void myDetailScope::setExamineHistory(int raw_from, int raw_next) {
  if(raw_from < raw_next) {  // Make sure the coordinates are in order : required below
    screen_offset_from=raw_from;
    screen_offset_next=raw_next;
  }
  else {
    screen_offset_from=raw_next;
    screen_offset_next=raw_from;
  }
  
  examine_mode = true;
  screen_dirty = true;
}
void myDetailScope::setExamineHistory() {
  setExamineHistory(screen_showing_from,screen_showing_next);  // Assumes that 1 OnPaint has occurred...
}
void myDetailScope::setExamineCurrent(int size) { 
  printf("screen_offset_size set to %d\n", size);
  screen_offset_size=size;

  examine_mode = false;
  screen_dirty = true;
}

void myDetailScope::OnPaint(wxPaintEvent& event) {
  UpdateDisplay();
}

void myDetailScope::OnSize(wxSizeEvent& event) {
  printf("Refreshing myDetailScope\n");
  screen_dirty = true;
  Refresh();
}

void myDetailScope::OnMouse(wxMouseEvent& event) { // Do something when there's a mouse event
  printf("Mousing in myDetailScope\n");
 
  // Send an event to parent...
  wxCommandEvent parent_event(wxEVT_COMMAND_TOOL_CLICKED, detail_scope_needs_attention);
  ProcessEvent(parent_event);
}


void myDetailScope::OnTimer(wxTimerEvent& event) { // Do something when there's a timer event
  // printf("Timer Event in myDetailScope\n");
  UpdateDisplay();
  return;
}

void myDetailScope::UpdateDisplay() {
  if(!examine_mode) { // This mode doesn't need the screen to be continuously redrawn
    screen_dirty = true;
  }

  if(!screen_dirty) {
    return;  // Nothing to do
  }

  // This should be done with wxAutoBufferedPaintDC 
  // - but do that once other issues are sorted out (better to see ugliness than have it auto-hidden)
  wxPaintDC dc(this); 
  
  if( dc.IsOk() ) {
    // Grab the raw data variables (using MUTEX)
    reader->s_mutexRawDataExtents.Lock();
    int raw_from = reader->filled_up_from;
    int raw_next = reader->filled_up_next;
    reader->s_mutexRawDataExtents.Unlock();

    wxCoord sx,sy;
    dc.GetSize(&sx, &sy);
   
    bool not_enough_data=false;
   
    int show_raw_from, show_raw_next;  // These versions of the variables are bound-checked
   
    if(examine_mode) { // Use the offsets from start of raw window
      show_raw_from = screen_offset_from;
      if(show_raw_from < raw_from || show_raw_from > raw_next) {
        not_enough_data=true;
      }
      show_raw_next = screen_offset_next;
      if(show_raw_next - show_raw_from < sx) {
        show_raw_next = show_raw_from + sx;  // Show at least 1 pixel per raw unit
      }
      if(show_raw_next < raw_from || show_raw_next > raw_next) {
        not_enough_data=true;
      }
    }
    else { // Use size offset from end of raw window
      int size_from_end = screen_offset_size;
      if(size_from_end < sx) {  // This implies that we should be using the screen size to tell us
        size_from_end=sx;
      }
      if(size_from_end > DETAIL_SCOPE_MAX_POINTS) {
        size_from_end = DETAIL_SCOPE_MAX_POINTS; // Hard limit on the size for refreshing here
      }
      if(trigger_on) {  // Search for trigger locations here...
        // STUFF for Trigger detection
      }
      else {
        show_raw_next = raw_next;
        show_raw_from = show_raw_next - size_from_end;
        if(show_raw_from < raw_from) {
          not_enough_data=true;
        }
      }
    }
    if( not_enough_data ) { // Don't bother doing anything
      printf("Not enough Data!\n");
      return; 
    }
    
    // Channel A is always on Even bytes (forget whether we're interleaved or not : resolution not that important)
    show_raw_from = show_raw_from & 0xFFFFFFFFFE; // Clear off lowest bit 
    
    // Save the extents of what we're showing - used by setExamineHistory() when we just 'pause' the display
    screen_showing_from=show_raw_from;
    screen_showing_next=show_raw_next;
    
    // Put up a black background and axes ( ... persistence ? )
    RedrawAxes(dc, show_raw_from, show_raw_next); 
    
    // Ok, so lets paint up the data ...
    RedrawData(dc, show_raw_from, show_raw_next); 
    
    screen_dirty=false;
  }
  return;
}

void myDetailScope::RedrawAxes(wxDC &dc, int raw_from, int raw_next) {
  // Clear it all out
  wxColour color_background(0,0,0); // black
  wxBrush  brush_background(color_background);
  dc.SetBackground(brush_background);
  dc.Clear();
 
  dc.SetFont(*font);
 
  wxColour color_grid(128,128,128); // grey
//  wxBrush  brush_grid(color_grid, wxBRUSHSTYLE_CROSS_HATCH);
  wxPen pen_grid(color_grid, 1, wxLONG_DASH );  // wxPENSTYLE_DOT  wxDOT 
  dc.SetPen(pen_grid);
  
  dc.SetTextBackground(wxColour(0,0,0)); // black
  dc.SetTextForeground(wxColour(255,255,255)); // white
  
  wxCoord sx,sy;
  dc.GetSize(&sx, &sy);
 
  // Time Axis
  bool interleaved = (reader->getSLOmode() == SLOSCOPE_STATE_2ANALOG);
  float time_displayed = (float)(raw_next - raw_from) / reader->sample_frequency_Hz * (interleaved?2.0:1.0);
  if(time_displayed > 0.0) {
    float pos_t, time_unit=1.0; // This is in seconds (and is a starter value) - pos_t_inc === time_unit
    float pos_x, pos_x_inc;
   
    float number_of_units_desired = (float)((int)sx / (int)75);  // Assume that we'd like units less than 75px apart
   
    while( time_displayed/number_of_units_desired < time_unit  ) { // Scale time_unit until we've got (#/10 .. #) ...
      time_unit *= 0.1;
    }
    
    // So now there are between time_units on the display...
    if(time_displayed/number_of_units_desired > time_unit * 4.0 ) {  // If there are more , then * 5
      time_unit *= 5.0;
    }
    else if(time_displayed/number_of_units_desired > time_unit * 1.5 ) { // If there are more , then * 2
      time_unit *= 2.0;
    }
    
    // Ok, so now there are a sensible # of rules - let's draw them : start at zero for now (can be improved TODO)
    pos_t = 0.0;
    pos_x = 0.0; 
    
    //  translate time_unit to pixels...
    pos_x_inc = (time_unit/time_displayed) * (float)sx;
    
    // Now fix up the formatting
    wxString fmt=_T("%4.0fms"); // like 1ms 
    float mult=1000.0;
    if(time_unit >= 0.1) {
      fmt=_T("%4.1fs"); // like 1.1s
      mult=1.0;
    }

//    printf("Now time_displayed=%6.4f, time_unit=%6.4f, pos_x_inc=%6.2f\n", time_displayed, time_unit, pos_x_inc);
    
    for(; (int)pos_x < sx; pos_x += pos_x_inc, pos_t += time_unit ) {
      dc.DrawLine((int)pos_x,0, (int)pos_x,  sy);
      wxString label=wxString::Format(fmt, mult * pos_t);
      dc.DrawText(label, (int)pos_x-20, sy-20);  // Rough-and-ready
    }
  
  }
 
  // Voltage Axis
  for(int j=0; j<sy; j+=50) {
    dc.DrawLine(0, sy-j,  sx, sy-j); // Start at y=0 (flip coordinates vertically)
  }
  return;
}


// raw_xyz corresponds to the data in the raw buffer
void myDetailScope::RedrawData(wxDC &dc, int raw_from, int raw_next) {  
  // X Axis : Assume that [raw_from, raw_next) -> 0 .. dc.GetSize().GetWidth()
  // Y Axis : Assume that [0, 256) -> 0 .. dc.GetSize().GetHeight()
  
  wxCoord sx,sy;
  dc.GetSize(&sx, &sy);
  
  float pos_inc = ((float)sx)/(float)(raw_next-raw_from);
  float y_scale = sy/256.0;

  bool interleaved = (reader->getSLOmode() == SLOSCOPE_STATE_2ANALOG);
  // printf("myLongScope : mode = %s\n", interleaved?"interleaved":"bit-packed");
  
  coreRedrawData(dc, pen_a, pen_b, sy,
                 (unsigned char *)reader->raw_data, interleaved, raw_from, raw_next, 
                 0.0, pos_inc, y_scale );
  
  return;
}

