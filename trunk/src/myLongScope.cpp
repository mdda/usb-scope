#include "SLOscope.h"

myLongScope::myLongScope(wxWindow *parent, int id) : wxPanel(parent, id)
//       : wxFrame(NULL, id, wxT("LongScope"), wxDefaultPosition, wxDefaultSize)
//      : wxPanel(parent, id, wxDefaultPosition, wxSize(-1, 30), wxSUNKEN_BORDER)
{
//  m_frame = frame;
  m_parent = parent;

  Connect(wxEVT_PAINT, wxPaintEventHandler(myLongScope::OnPaint));
  Connect(wxEVT_SIZE, wxSizeEventHandler(myLongScope::OnSize));

  highlight=false;    // Are we currently highlighting stuff ?
  highlight_onscreen=false;  // Is there a highlight currently showing on the screen ?
  
  Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(myLongScope::OnMouse));
  Connect(wxEVT_LEFT_UP, wxMouseEventHandler(myLongScope::OnMouse));
  Connect(wxEVT_MOTION, wxMouseEventHandler(myLongScope::OnMouse));
 
  screen_showing_from=0;
  screen_showing_next=0;
 
  Connect(wxEVT_TIMER, wxTimerEventHandler(myLongScope::OnTimer));
 
  // See : http://zetcode.com/tutorials/wxwidgetstutorial/thetetrisgame/
  // And for another example : http://wiki.wxwidgets.org/Making_a_render_loop
  refresh_timer = new wxTimer(this, 1);
}

void myLongScope::setReader(myReadScope *_reader) {
  reader=_reader;
}

void myLongScope::stopPausing() {
  highlight=false;
}

void myLongScope::OnPaint(wxPaintEvent& event) { // This repaints the whole thing
  // Repaint the whole thing : i.e. declare that nothing is on-screen before 'UpdateDisplay'
  screen_showing_from=0;
  screen_showing_next=0;
 
  UpdateDisplay();
  return;
 
  wxPaintDC dc(this);
  printf("Painting myLongScope\n");
  
//  wxFont font(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier 10 Pitch"));
//  dc.SetFont(font);
//  return;
 
  if( dc.IsOk() ) {
//    RedrawData( dc, 0, LONG_BUFFER_SIZE, 0.0 ); // Whole thing
  }
  return;
}

void myLongScope::OnSize(wxSizeEvent& event) {
  printf("Refreshing myLongScope\n");
  Refresh();  // This calls OnPaint, apparently
}


void myLongScope::OnMouse(wxMouseEvent& event) { // Do something when there's a mouse event
  if(event.Moving()) {  // This is NOT a drag - see below
    // Ignore motions, unless the buttons are pressed
    return;
  }
  
  printf("Mousing in myLongScope\n");
  wxPoint mouse(event.GetX(), event.GetY());
  
  wxCoord sx, sy;
  GetSize(&sx, &sy);
  
  float screen_pct = (float)mouse.x/(float)sx;
  int raw_version =  screen_showing_next - (float)LONG_BUFFER_SIZE * (float)(1.0 - screen_pct);
  
//  printf("Mouse position (before) : %d (=%6.4f) :translates to %d)\n", mouse.x, screen_pct, raw_version);
  if(raw_version<screen_showing_from) {
    raw_version=screen_showing_from;
  }
  if(raw_version>screen_showing_next) {
    raw_version=screen_showing_next;
  }
  printf("Mouse position (after ) : %d (=%6.4f) :translates to %d)\n", mouse.x, screen_pct, raw_version);
  
  // Prepare a command event to send to parent...
  wxCommandEvent parent_event(wxEVT_COMMAND_TOOL_CLICKED, long_scope_needs_attention);
  
  if(event.LeftDown()) {
    printf("LongScope : OnMouse : LeftDown() @ %d\n", mouse.x);
    parent_event.SetInt(scope_mouse_left_click);
    RedrawDataAll();
    
    // This is for OnPaint
    drag_start=mouse.x;
    drag_end=mouse.x;
    highlight=true;
   
    // This is for the parent frame to inform DetailScope
    drag_start_raw=raw_version;
    drag_end_raw=raw_version; 
  }
  else if(event.LeftUp()) {
    printf("LongScope : OnMouse : LeftUp() @ %d\n", mouse.x);
    parent_event.SetInt(scope_mouse_drag_ended);
    drag_end=mouse.x;
    drag_end_raw=raw_version; 
    // Don't stop the 'highlighting' here : let the Frame do this, since it's the one setting the reader going...
  }
  else if(event.Dragging()) {
    printf("LongScope : OnMouse : Dragging() now @ %d\n", mouse.x);
    parent_event.SetInt(scope_mouse_dragging);
    drag_end=mouse.x;
    drag_end_raw=raw_version; 
  }

  ProcessEvent(parent_event);
//  printf("Highlighting in OnMouse - before:'%s' next:'%s'\n", highlight_onscreen?"Y":"N", highlight?"Y":"N");
}


void myLongScope::OnTimer(wxTimerEvent& event) { // Do something when there's a timer event
  // printf("Timer Event in myLongScope\n");
  UpdateDisplay();
  return;
}



void myLongScope::UpdateDisplay() {
  wxPaintDC dc(this);
//  printf("Updating myLongScope\n");
  
  if( dc.IsOk() ) {
    wxCoord sx, sy;
    dc.GetSize(&sx, &sy);

    if(highlight || highlight_onscreen) {
//      printf("Highlighting - before:'%s' next:'%s'\n", highlight_onscreen?"Y":"N", highlight?"Y":"N");
     
//    This is for a rubber-band box
 /*  
      dc.SetPen(*wxBLACK_DASHED_PEN);
      dc.SetBrush(*wxTRANSPARENT_BRUSH);
*/     
//    This is for a filled-in inverted box
      dc.SetPen(wxNullPen);
      dc.SetBrush(*wxBLACK_BRUSH);
     
      dc.SetLogicalFunction(wxINVERT);
     
      if(highlight_onscreen) { // There's already something on-screen, remove it first
        dc.DrawRectangle(drag_start_onscreen,0, (drag_end_onscreen - drag_start_onscreen), sy);
        highlight_onscreen=false;
      }
      
      if(highlight) {
        // update the onscreen internals...
        drag_start_onscreen=drag_start;
        drag_end_onscreen=drag_end;
        dc.DrawRectangle(drag_start_onscreen,0, (drag_end_onscreen - drag_start_onscreen), sy);
        highlight_onscreen=true;
        // Now, since we're showing a highlight, we must be paused...  Don't bother with blit, etc...
        return;
      }
      dc.SetLogicalFunction(wxCOPY);
    }
   
    // Grab the raw data variables (using MUTEX)
    reader->s_mutexRawDataExtents.Lock();
    int raw_from = reader->filled_up_from;
    int raw_next = reader->filled_up_next;
    reader->s_mutexRawDataExtents.Unlock();

    // Determine the 'new data' - this principally concerns the difference in the 'next' values
    int new_amount = raw_next - screen_showing_next;
    if(new_amount < 0 ) {
      new_amount += LONG_BUFFER_SIZE;  // Since there must have been a wrap-around
    }
    // printf("myLongScope Raw available  : [%d, %d)\n", raw_from, raw_next);
    // printf("myLongScope onscreen now   : [%d, %d)\n", screen_showing_from, screen_showing_next);
    // printf("myLongScope New Amount     : [%d])\n", new_amount);
    
    float shift_pct = (float)(new_amount)/(float)LONG_BUFFER_SIZE;
    
    // Shift 'old data' to the Left
    wxCoord shift_x = (wxCoord) (sx * shift_pct); 
    
    if(shift_x <= 1) {
      // printf("myLongScope SHIFT : [%d]\n", shift_x);
      return;  // Not enough to bother with updating...
    }
    
    if(1) {  // Blit the old stuff across : very efficient
      wxPoint dest(0,0);
      wxSize  blit_size(sx - shift_x, sy);
      wxPoint src(shift_x, 0);
    
/*
bool wxDC::Blit  	(  	wxCoord   	 xdest, wxCoord  	ydest,
                      wxCoord  	width, wxCoord  	height,
                      wxDC *  	source,
                      wxCoord  	xsrc, wxCoord  	ysrc,
                      wxRasterOperationMode  	logicalFunc = wxCOPY,
                      bool  	useMask = false,
                      wxCoord  	xsrcMask = wxDefaultCoord, wxCoord  	ysrcMask = wxDefaultCoord	 
) 			
*/    
//      wxPoint dummy_point(0,0);
      dc.Blit(dest, blit_size, &dc, src);  // , wxCOPY, false, dummy_point
    }
    else {  // This is/was just for testing
      wxColour color_background(0,0,0); // black
      wxBrush  brush_background(color_background);
      dc.SetBackground(brush_background);
      dc.Clear();
    }
    
    // TODO : Really : Should adjust extents of data to be displayed to be pixel-accurate, otherwise
    //  there's a sub-pixel drift built into this process 
    // (fix at the moment is to redraw whole LongScope when it gets clicked for examination)
    
    // Ok, so lets paint up the new data ...
    RedrawData( dc, screen_showing_next, screen_showing_next + new_amount, 1.0 - shift_pct); 

    // update extents in on_screen variables
    screen_showing_next += new_amount;
    if(screen_showing_next - screen_showing_from > LONG_BUFFER_SIZE) {  // We're showing all the data now
      screen_showing_from = screen_showing_next - LONG_BUFFER_SIZE;
    }
    if(screen_showing_from > LONG_BUFFER_SIZE) {
      screen_showing_from -= LONG_BUFFER_SIZE;
      screen_showing_next -= LONG_BUFFER_SIZE;
      printf("myLongScope Reset to : [%d, %d)\n", screen_showing_from, screen_showing_next);
    }
  }
  return;
}

void myLongScope::RedrawDataAll() {
  wxPaintDC dc(this);
  float pos_start = (float)(screen_showing_next-screen_showing_from)/(float)LONG_BUFFER_SIZE;
  RedrawData(dc, screen_showing_from, screen_showing_next, 1.0-pos_start);
}
 
// raw_xyz corresponds to the data in the raw buffer
// pos_xyz corresponds to the graphic on the screen // pos_next=1.0 is always the far-right of display, =0.0 far-left
void myLongScope::RedrawData(wxDC &dc, int raw_from, int raw_next, float pos_from) {  
  // X Axis : Assume that [0, LONG_BUFFER_SIZE) -> 0 .. dc.GetSize().GetWidth()
  // Y Axis : Assume that [0, 256) -> 0 .. dc.GetSize().GetHeight()
  
  if(raw_next == raw_from) {
    return; // nothing to do
  }
 
  wxCoord sx,sy;
  dc.GetSize(&sx, &sy);
//  wxCoord y_spacer=0;  // Adding a 1px border around LongScope removes the need
//  sy -= y_spacer;
  
//  dc.SetAxisOrientation(true,true);  // Origin in bottom LHS
  
  float pos_start = pos_from * (float)sx;
  float pos_inc = ((float)sx - pos_start)/(float)(raw_next-raw_from);
  float y_scale = sy/256.0;

  // printf("myLongScope : (raw_from, raw_next) (sx,sy) = (%d,%d),(%d,%d)\n", raw_from, raw_next, sx,sy);
  // printf("myLongScope : start, inc, y_scale = (%6.4f,%6.4f,%6.4f)\n", pos_start, pos_inc, y_scale);

  wxColour color_background(0,0,0); // black
  wxBrush  brush_background(color_background);
  dc.SetBrush(brush_background); 
  dc.SetPen(wxNullPen); // No outline
  
  // The +1 is here since if pos_start = 50.9, rounding causes it to blank a line that's mostly got data in already
  //   and that causes black bars to appear in the output... 
  //   (we can assume that what's already there cleared to background, last time around)
  dc.DrawRectangle(pos_start+1, 0, sx - pos_start, sy );  // Clear just the necessary background (x,y,width,height)
  
  bool interleaved = (reader->getSLOmode() == SLOSCOPE_STATE_2ANALOG);
  // printf("myLongScope : mode = %s\n", interleaved?"interleaved":"bit-packed");
  
  coreRedrawData(dc, pen_a, pen_b, sy,
                 (unsigned char *)reader->raw_data, interleaved, raw_from, raw_next, 
                 pos_start, pos_inc, y_scale );
  return; 
}


// Centralize this function : it's used by myDetailScope too : only need one optimized version...

void coreRedrawData(wxDC &dc, wxPen *pen_a, wxPen *pen_b, wxCoord sy,
                    unsigned char *raw_data_buffer, bool interleaved, int raw_from, int raw_next, 
                    float pos_start, float pos_inc, float y_scale ) { 
                     
  int i = raw_from;       // This one counts through the raw buffer
  int j;                  // This one goes through each SHORT_BUFFER_SIZE of data quickly
  int k=0;                // This one tracks how must data we've blasted
  float pos_now=pos_start;  // This one paces across the screen
  
  float pos_this;         // This is used to temporarily store the pos_ of the start of each SHORT_BUFFER worth of data
  unsigned char *p;
  
  if(interleaved) {
    pos_inc *= 2.0;
  }
  
  for(k=0; k<(raw_next-raw_from); k+=SHORT_BUFFER_SIZE ) {  // Go through the raw buffer, SHORT_BUFFER_SIZE at a time
    if(i>LONG_BUFFER_SIZE) {
      i = i % LONG_BUFFER_SIZE; // Wrap around
    }
    
    p=&raw_data_buffer[i];
   
    if(interleaved) {
      pos_this=pos_now;
      dc.SetPen(*pen_a); 
      for(j=0; j<SHORT_BUFFER_SIZE; j+=2, pos_this+=pos_inc) {
        dc.DrawPoint(pos_this, sy - y_scale * (unsigned char) p[j]);
      }
      
      pos_this=pos_now;
      dc.SetPen(*pen_b); 
      for(j=1; j<SHORT_BUFFER_SIZE; j+=2, pos_this+=pos_inc) {
        dc.DrawPoint(pos_this, sy - y_scale * (unsigned char) p[j]);
      }
    }
    else {
      pos_this=pos_now;
      dc.SetPen(*pen_a); 
      for(j=0; j<SHORT_BUFFER_SIZE; j++, pos_this+=pos_inc) {
        dc.DrawPoint(pos_this, sy - y_scale * ((unsigned char) p[j] & 0xFE));
      }
      
      pos_this=pos_now;
      dc.SetPen(*pen_b); 
      for(j=0; j<SHORT_BUFFER_SIZE; j++, pos_this+=pos_inc) {
        dc.DrawPoint(pos_this, sy - y_scale * (( ((unsigned char) p[j] & 0x01)<<7) + 64) );
      }
    }
    
    i+=SHORT_BUFFER_SIZE;
    //pos_now += pos_inc * SHORT_BUFFER_SIZE;  // This is what we mean (but need to /2 for interleaved)
    pos_now=pos_this; // Rather naughty - relies on loop leaving value in pos_this correctly...
  }
  return;
}
