// -*- C++ -*- generated by wxGlade 0.6.3 on Sat Sep 26 23:27:43 2009

#include <wx/wx.h>
// #include <wx/image.h>
// #include <wx/pen.h>

// begin wxGlade: ::dependencies
#include <wx/spinctrl.h>
// end wxGlade

#include "usb_utils.h"

#ifndef SLOSCOPE_H
#define SLOSCOPE_H

// 1 short buffer = 1ms // LONG_BUFFER_SIZE must be an exact multiple of SHORT_BUFFER_SIZE
#define LONG_BUFFER_SIZE (10000 * SHORT_BUFFER_SIZE)
#define THREAD_WAIT_SHORT (100 /* in ms */) 

#define DETAIL_SCOPE_MAX_POINTS (20000 /* this is 1 second of data */ )

class myReadScope : public wxThread {
public:
  myReadScope();
  ~myReadScope();

  virtual void *Entry();

  bool deviceFound();

  // These are so that (if necessary) MUTEXs can be used to avoid cross-thread contention
  usb_dev_handle *openHandle();
  void closeHandle(usb_dev_handle *handle);

  wxMutex s_mutexRawDataExtents;
//        s_mutexProtectingTheGlobalList->Lock();
//        s_data.Add(num);
//        s_mutexProtectingTheGlobalList->Unlock();

  // These extents are always [filled_up_from<=filled_up_next) - and are shifted down together once they're > raw_data_size
  int filled_up_from, filled_up_next;

  char *raw_data;

  // These are only for the outside world
  void setPleasePause(bool pause);
  bool getPaused();

  int getSLOmode();
  void setSLOmode(int _mode);
  
  float sample_frequency_Hz; // Rough : just # samples per second... (not adjusted for VARIABLE_SLOSCOPE_PERIOD )
  
 private :
  struct usb_device *dev; // Pointer to the (single) programmer 
  int slo_mode_saved; // so the device's initial state can be restored
  int slo_mode_current;
 
  void readBlock();
 
  bool please_pause;
  bool i_am_paused;

  // These are only for the reader 
  bool getPleasePause();
  void setPaused(bool paused);
};

// List all the event codes here, so that they can be tied up with the 
enum {
  menu_mode_2A,
  menu_mode_1A1D,
  osc_mode_change,
  long_scope_needs_attention,
  detail_scope_needs_attention,
};

enum {
  scope_mouse_left_click,
  scope_mouse_dragging,
  scope_mouse_drag_ended,
 
};


void coreRedrawData(wxDC &dc, wxPen *pen_a, wxPen *pen_b, wxCoord sy,
                    unsigned char *raw_data_buffer, bool interleaved, int raw_from, int raw_next, 
                    float pos_start, float pos_inc, float y_scale );


class myLongScope: public wxPanel {
public: 
  myLongScope(wxWindow *parent, int id);

protected:
  void OnSize(wxSizeEvent& event);
  void OnPaint(wxPaintEvent& event);

  void OnMouse(wxMouseEvent& event);
  void OnTimer(wxTimerEvent& event);

  void UpdateDisplay();

private:
  wxWindow *m_parent;
  int screen_showing_from, screen_showing_next;

  void RedrawData(wxDC &dc, int raw_from, int raw_next, float pos_from);

public:
  void setReader(myReadScope *reader);
  void stopPausing();

  wxTimer *refresh_timer;
  wxPen *pen_a, *pen_b;

  int drag_start_raw, drag_end_raw;  // These are indexes in the raw buffer (so that Frame can use them easily)

private:
  myReadScope *reader;

  bool highlight_onscreen, highlight;
  int drag_start, drag_end;  // These are screen locations
  int drag_start_onscreen, drag_end_onscreen;  // These are the locations that have been drawn on-screen so far
};



class myDetailScope: public wxPanel {
public: 
  myDetailScope(wxWindow *parent, int id);

  void setViewCurrent(int size);           // offset from end backwards
  void setViewExamine(int start, int end); // offsets from start of raw onwards

protected:
  void OnSize(wxSizeEvent& event);
  void OnPaint(wxPaintEvent& event);  

  void OnMouse(wxMouseEvent& event);
  void OnTimer(wxTimerEvent& event);

  void UpdateDisplay();

private:
  wxWindow *m_parent;
  
  bool examine_mode;
  int screen_offset_size;                        // for Current - unused : size defaults to # of pixels onscreen...
  int screen_offset_from, screen_offset_next;    // for examine - refers to raw data buffer

  bool screen_dirty;                             // does the screen need redrawing? 
  int screen_showing_from, screen_showing_next;  // Just so we can see where we are after OnPaint

  bool trigger_on;
  int trigger_channel;  // 0=>A 1=>B
  int trigger_level;
  bool trigger_rising;  // true=>rising, false=>falling

  void RedrawAxes(wxDC &dc, int raw_from, int raw_next);
  void RedrawData(wxDC &dc, int raw_from, int raw_next);

public:
  void setReader(myReadScope *reader);
  void setExamineHistory(int raw_from, int raw_next);
  void setExamineHistory();  // This sets raw_xyz from the Current(size)
  void setExamineCurrent(int size);

  wxTimer *refresh_timer;
  wxPen *pen_a, *pen_b;

private:
  myReadScope *reader;
  wxFont *font;

};



// begin wxGlade: ::extracode
// end wxGlade


class DetailScopeRightClick: public wxMenuBar {
public:
    // begin wxGlade: DetailScopeRightClick::ids
    // end wxGlade

    DetailScopeRightClick();

private:
    // begin wxGlade: DetailScopeRightClick::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: DetailScopeRightClick::attributes
    // end wxGlade
}; // wxGlade: end class





class MyFrame: public wxFrame {
public:
    // begin wxGlade: MyFrame::ids
    // end wxGlade

  MyFrame(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

  void test();

private:
    // begin wxGlade: MyFrame::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: MyFrame::attributes
    wxStaticBox* ChB_staticbox;
    wxStaticBox* ChA_staticbox;
    wxMenuBar* frame_1_menubar;
    wxRadioBox* radio_box_1;
    wxSpinCtrl* ChA_VDiv;
    wxRadioBox* ChA_InOut;
    wxSpinCtrl* ChB_VDiv;
    wxRadioBox* ChB_InOut;
    wxButton* SavePNG;
    myLongScope* LongScope;
    myDetailScope* DetailScope;
    wxStaticText* BottomLHS;
    wxStaticText* BottomRHS;
    // end wxGlade

  // added by mdda - look at http://www.math.uiuc.edu/~gfrancis/illimath/windows/aszgard_mini/bin/wxGlade-0.6/codegen/cpp_codegen.py
protected:
  DECLARE_EVENT_TABLE()

public:
    virtual void choose2A(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void choose1A1D(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void oscilloscope_mode_change(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnMouse(wxMouseEvent& event);
    void long_scope_event(wxCommandEvent &event);
    void detail_scope_event(wxCommandEvent &event);

public:
  void setReader(myReadScope *reader);
//  void refreshEvery(int ms);

  wxPen *pen_a, *pen_b;

private:
  myReadScope *reader;

  void coordinated_mode_change(bool mode_now_paused);

}; // wxGlade: end class


/*
class RightClick1: public wxFrame {
public:
// content of this block (ids) not found: did you rename this class?

  RightClick1(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

private:
    void set_properties();
    void do_layout();

protected:
// content of this block (attributes) not found: did you rename this class?
}; // wxGlade: end class
*/

class SLOscope: public wxApp {
public:
  bool OnInit();
  virtual int OnExit();

private:
  myReadScope *reader;
};

#endif // SLOSCOPE_H
