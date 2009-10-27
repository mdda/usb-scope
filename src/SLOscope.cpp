// -*- C++ -*- generated by wxGlade 0.6.3 on Sat Sep 26 23:27:43 2009

#include "SLOscope.h"
#include <wx/colordlg.h>

// begin wxGlade: ::extracode
// end wxGlade

// added by mdda - look at http://www.math.uiuc.edu/~gfrancis/illimath/windows/aszgard_mini/bin/wxGlade-0.6/codegen/cpp_codegen.py
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    // begin wxGlade: MyFrame::event_table
    EVT_MENU(menu_mode_2A, MyFrame::choose2A)
    EVT_MENU(menu_mode_1A1D, MyFrame::choose1A1D)
    EVT_MENU(menu_view_a_color, MyFrame::chooseAcolor)
    EVT_MENU(menu_view_b_color, MyFrame::chooseBcolor)
    EVT_RADIOBOX(osc_mode_change, MyFrame::oscilloscope_mode_change)
    EVT_RADIOBOX(radio_ch_a_change, MyFrame::ChannelIO_changed)
    EVT_RADIOBOX(radio_ch_b_change, MyFrame::ChannelIO_changed)
    // end wxGlade
    EVT_TOOL(long_scope_needs_attention, MyFrame::long_scope_event)
    EVT_TOOL(detail_scope_needs_attention, MyFrame::detail_scope_event)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(DetailScopeRightClick, wxMenuBar)
// content of this block not found: did you rename this class?
/*
    EVT_MENU(rc_no_triggers, MyFrame::RightClickMenuChange)
    EVT_MENU(rc_a_up, MyFrame::RightClickMenuChange)
    EVT_MENU(rc_a_down, MyFrame::RightClickMenuChange)
    EVT_MENU(rc_b_up, MyFrame::RightClickMenuChange)
    EVT_MENU(rc_b_down, MyFrame::RightClickMenuChange)
*/
    EVT_MENU(rc_no_triggers, DetailScopeRightClick::RightClickMenuChange)
    EVT_MENU(rc_a_up, DetailScopeRightClick::RightClickMenuChange)
    EVT_MENU(rc_a_down, DetailScopeRightClick::RightClickMenuChange)
    EVT_MENU(rc_b_up, DetailScopeRightClick::RightClickMenuChange)
    EVT_MENU(rc_b_down, DetailScopeRightClick::RightClickMenuChange)
END_EVENT_TABLE()


MyFrame::MyFrame(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxFrame(parent, id, title, pos, size, wxDEFAULT_FRAME_STYLE)
{
    // begin wxGlade: MyFrame::MyFrame
    ChB_staticbox = new wxStaticBox(this, -1, wxT("Channel B"));
    ChA_staticbox = new wxStaticBox(this, -1, wxT("Channel A"));
    frame_1_menubar = new wxMenuBar();
    wxMenu* menu_mode_radio = new wxMenu();
    menu_mode_radio->Append(menu_mode_2A, wxT("2 8-bit Analog, 10 KHz"), wxEmptyString, wxITEM_RADIO);
    menu_mode_radio->Append(menu_mode_1A1D, wxT("1 7-bit Analog, 1 Digital, 20 KHz"), wxEmptyString, wxITEM_RADIO);
    frame_1_menubar->Append(menu_mode_radio, wxT("Mode"));
    wxMenu* wxglade_tmp_menu_1 = new wxMenu();
    wxglade_tmp_menu_1->Append(wxID_ANY, wxT("Save as PNG"), wxEmptyString, wxITEM_NORMAL);
    wxglade_tmp_menu_1->Append(menu_view_a_color, wxT("Channel A Color"), wxEmptyString, wxITEM_NORMAL);
    wxglade_tmp_menu_1->Append(menu_view_b_color, wxT("Channel B Color"), wxEmptyString, wxITEM_NORMAL);
    frame_1_menubar->Append(wxglade_tmp_menu_1, wxT("View"));
    wxMenu* menu_persistence_radio = new wxMenu();
    menu_persistence_radio->Append(wxID_ANY, wxT("Off"), wxEmptyString, wxITEM_RADIO);
    menu_persistence_radio->Append(wxID_ANY, wxT("1"), wxEmptyString, wxITEM_RADIO);
    menu_persistence_radio->Append(wxID_ANY, wxT("2"), wxEmptyString, wxITEM_RADIO);
    menu_persistence_radio->Append(wxID_ANY, wxT("3"), wxEmptyString, wxITEM_RADIO);
    menu_persistence_radio->Append(wxID_ANY, wxT("4"), wxEmptyString, wxITEM_RADIO);
    frame_1_menubar->Append(menu_persistence_radio, wxT("Persistence"));
    SetMenuBar(frame_1_menubar);
    const wxString radio_box_1_choices[] = {
        wxT("Real-time Display"),
        wxT("Review History")
    };
    radio_box_1 = new wxRadioBox(this, osc_mode_change, wxT("Oscilloscope Mode"), wxDefaultPosition, wxDefaultSize, 2, radio_box_1_choices, 0, wxRA_SPECIFY_ROWS);
    ChA_VDiv = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
    const wxString ChA_InOut_choices[] = {
        wxT("In"),
        wxT("=0"),
        wxT("=1")
    };
    ChA_InOut = new wxRadioBox(this, radio_ch_a_change, wxT("Input / Output"), wxDefaultPosition, wxDefaultSize, 3, ChA_InOut_choices, 3, wxRA_SPECIFY_COLS);
    ChB_VDiv = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
    const wxString ChB_InOut_choices[] = {
        wxT("In"),
        wxT("=0"),
        wxT("=1")
    };
    ChB_InOut = new wxRadioBox(this, radio_ch_b_change, wxT("Input / Output"), wxDefaultPosition, wxDefaultSize, 3, ChB_InOut_choices, 3, wxRA_SPECIFY_COLS);
    SavePNG = new wxButton(this, wxID_ANY, wxT("Save as PNG"));
    LongScope = new myLongScope(this, wxID_ANY);
    DetailScope = new myDetailScope(this, wxID_ANY);
    BottomLHS = new wxStaticText(this, wxID_ANY, wxT("BottomLHS"));
    BottomRHS = new wxStaticText(this, wxID_ANY, wxT("BottomRHS"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);

    set_properties();
    do_layout();
    // end wxGlade
    
    // Set up all the defaults here...
    
    // Set the mode (default is first item on list)
    menu_mode_radio->FindItemByPosition(0)->Check(true);
    
    // Disable all the persistence>0 entries
    for(int i=1; i<menu_persistence_radio->GetMenuItemCount() ;i++) {
      menu_persistence_radio->FindItemByPosition(i)->Enable(false);
    }
    
    
    
//    Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MyFrame::OnMouse));
    
    //long_scope_needs_attention
    
//  See : http://docs.wxwidgets.org/trunk/overview_events.html  'exactly as expected'
//    LongScope->Bind(wxEVT_LEFT_DOWN, wxMouseEventHandler(MyFrame::OnMouse), this);
//    LongScope->Bind(wxEVT_LEFT_DOWN, &MyFrame::OnMouse, this);
    //printf("Set up mouse event\n");
}


void MyFrame::set_properties() {
    // begin wxGlade: MyFrame::set_properties
    SetTitle(wxT("SLOscope - Open Source"));
    radio_box_1->SetSelection(0);
    ChA_InOut->SetBackgroundColour(wxColour(255, 255, 128));
    ChA_InOut->SetSelection(0);
    ChB_InOut->SetBackgroundColour(wxColour(192, 192, 255));
    ChB_InOut->SetSelection(0);
    // end wxGlade
 
   wxColour color_a(255,255,128); // yellow
   pen_a = new wxPen(color_a, 1, wxSOLID );  
   ChA_InOut->SetBackgroundColour(color_a);
   LongScope->pen_a=pen_a;
   DetailScope->pen_a=pen_a;
   
   wxColour color_b(192,192,255); // blue
   pen_b = new wxPen(color_b, 1, wxSOLID );  
   ChB_InOut->SetBackgroundColour(color_b);
   LongScope->pen_b=pen_b;
   DetailScope->pen_b=pen_b;
}


void MyFrame::do_layout() {
    // begin wxGlade: MyFrame::do_layout
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* TopControls = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_8 = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* ChB = new wxStaticBoxSizer(ChB_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_6_copy = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* ChA = new wxStaticBoxSizer(ChA_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_6 = new wxBoxSizer(wxHORIZONTAL);
    TopControls->Add(radio_box_1, 2, wxEXPAND|wxADJUST_MINSIZE, 0);
    sizer_6->Add(ChA_VDiv, 0, wxALIGN_CENTER_VERTICAL|wxADJUST_MINSIZE, 0);
    wxStaticText* label_4 = new wxStaticText(this, wxID_ANY, wxT("V/Div"));
    sizer_6->Add(label_4, 0, wxALIGN_CENTER_VERTICAL|wxADJUST_MINSIZE, 0);
    ChA->Add(sizer_6, 1, wxEXPAND, 0);
    ChA->Add(ChA_InOut, 0, wxEXPAND|wxADJUST_MINSIZE, 0);
    TopControls->Add(ChA, 0, wxEXPAND|wxADJUST_MINSIZE, 0);
    sizer_6_copy->Add(ChB_VDiv, 0, wxALIGN_CENTER_VERTICAL|wxADJUST_MINSIZE, 0);
    wxStaticText* label_4_copy = new wxStaticText(this, wxID_ANY, wxT("V/Div"));
    sizer_6_copy->Add(label_4_copy, 0, wxALIGN_CENTER_VERTICAL|wxADJUST_MINSIZE, 0);
    ChB->Add(sizer_6_copy, 1, wxEXPAND, 0);
    ChB->Add(ChB_InOut, 0, wxEXPAND|wxADJUST_MINSIZE, 0);
    TopControls->Add(ChB, 0, wxEXPAND|wxADJUST_MINSIZE, 0);
    sizer_8->Add(SavePNG, 1, wxEXPAND|wxADJUST_MINSIZE|wxFIXED_MINSIZE, 0);
    TopControls->Add(sizer_8, 1, wxEXPAND|wxADJUST_MINSIZE, 0);
    sizer_1->Add(TopControls, 0, wxEXPAND|wxADJUST_MINSIZE, 3);
    sizer_1->Add(LongScope, 1, wxALL|wxEXPAND, 1);
    sizer_1->Add(DetailScope, 3, wxALL|wxEXPAND, 1);
    sizer_2->Add(BottomLHS, 0, wxALIGN_BOTTOM, 0);
    sizer_2->Add(20, 10, 1, wxEXPAND|wxADJUST_MINSIZE, 0);
    sizer_2->Add(BottomRHS, 0, wxALIGN_BOTTOM, 0);
    sizer_1->Add(sizer_2, 0, wxEXPAND, 0);
    SetSizer(sizer_1);
    sizer_1->Fit(this);
    Layout();
    // end wxGlade
  
//  this->SetMinSize(wxSize(5,500));
}

void MyFrame::setReader(myReadScope *_reader) {
  reader=_reader;
  LongScope->setReader(reader);
  DetailScope->setReader(reader);
}

//void MyFrame::refreshEvery(int ms) {
//  LongScope->UpdateDisplay(); SOMETHING
//}


void MyFrame::test() {
  BottomRHS->SetLabel(wxT("Banana"));
  LongScope->refresh_timer->Start(20); // in ms
  DetailScope->refresh_timer->Start(20); // in ms

/* 
  wxClientDC dc(LongScope);

  if( dc.IsOk() ) {
    wxSize size=dc.GetSize();
    printf("LongScan size=(%d,%d)\n", size.GetWidth(), size.GetHeight() );
    wxSize client=GetClientSize();
    printf("Client size=(%d,%d)\n", client.GetWidth(), client.GetHeight() );
  }

  wxColour color_background(0,0,0); // black
  wxBrush  brush_background(color_background);
  dc.SetBackground(brush_background);
  dc.Clear();
  //wxPaintDC dc(LongScan->getDC());
*/

}

IMPLEMENT_APP(SLOscope)

bool SLOscope::OnInit() {
  wxInitAllImageHandlers();

  MyFrame* frame_1 = new MyFrame(NULL, wxID_ANY, wxEmptyString);
  SetTopWindow(frame_1);

  reader = new myReadScope();
  if( !reader->deviceFound() ) {
    int answer = wxMessageBox(wxT("Cannot find Pololu USB programmer\nConnect the device and try again"), wxT("Cannot find Device"), wxOK, NULL);
    return false;
  }

  frame_1->setReader(reader);
  
  frame_1->test(); // The frame can be updated prior to display here
  frame_1->Show();
  
  wxCommandEvent dummy=wxCommandEvent();  // Can't we get the event from the MenuItem somehow.?.  this is ugly
  frame_1->choose2A(dummy);
  
  //wxMouseEvent mouse=wxMouseEvent();  // Testing
  //ProcessEvent(mouse);
    
  // Prepare the thread for the USB reader
  if( reader->Create() != wxTHREAD_NO_ERROR ) {  // Prepare the thread
    wxLogError(wxT("Can't create Scope Reader thread"));
  }
  reader->Run();  // Start the thread
  
  // Now loop from within wxWidgets
  return true;
}

int SLOscope::OnExit() {
  reader->setPleasePause(true);
  while(! reader->getPaused() ) {  // Wait for the thread to stop collecting data
    wxThread::Sleep(THREAD_WAIT_SHORT);
  }
  reader->Delete(); // Remove the thread
  delete reader;    // remove the object's data
  return 1;
}

// wxGlade: add MyFrame event handlers

void MyFrame::choose2A(wxCommandEvent &event) {
  reader->setSLOmode(SLOSCOPE_STATE_2ANALOG);
  // event.Skip();
  //  wxLogDebug(wxT("Event handler (MyFrame::choose2A) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void MyFrame::choose1A1D(wxCommandEvent &event) {
  reader->setSLOmode(SLOSCOPE_STATE_1ANALOG_1DIGITAL);
}


void MyFrame::choosePenColor(wxPen *p, wxRadioBox *rb) {
  wxColourData cdata;
  cdata.SetColour(p->GetColour());
 
  wxColourDialog dialog(this, &cdata);
  if(dialog.ShowModal() == wxID_OK) {
    wxColourData retData = dialog.GetColourData();
    wxColour col = retData.GetColour();
    p->SetColour(col);
    rb->SetBackgroundColour(col);
  }
  return;
}


void MyFrame::chooseAcolor(wxCommandEvent &event) {
  choosePenColor(pen_a, ChA_InOut);
}

void MyFrame::chooseBcolor(wxCommandEvent &event) {
  choosePenColor(pen_b, ChB_InOut);
}

void MyFrame::ChannelIO_update_controls() {
  // This reads the current IO control positions, and updates the radio buttons to suit
  printf("ChannelIO_update_controls\n"); 
  int state =  reader->getIOstate();   

// state = {0=>input, 1=>output lo, 3=>output hi};
  int ch_a = state & 0xFF;
  int ch_b = ( state >> 8 ) & 0xFF;
  
  ChA_InOut->SetSelection( (ch_a == 0)?(0):((ch_a == 1)?(1):(2)) );
  ChB_InOut->SetSelection( (ch_b == 0)?(0):((ch_b == 1)?(1):(2)) );
}
 
void MyFrame::ChannelIO_changed(wxCommandEvent &event) {
  // Get the IO control positions, update the IO setup, and then re-read the IO setup back to the controls
  printf("ChannelIO_changed\n"); 
  int state_new=0;
 
  int ch_a = ChA_InOut->GetSelection();
  state_new =    (ch_a == 0)?(0):((ch_a == 1)?(1):(3));
  int ch_b = ChB_InOut->GetSelection();
  state_new += ( (ch_b == 0)?(0):((ch_b == 1)?(1):(3)) ) << 8;
 
  reader->setIOstate(state_new);   
 
  ChannelIO_update_controls();
}

void MyFrame::oscilloscope_mode_change(wxCommandEvent &event) {
  coordinated_mode_change(event.GetSelection() == 1);
}

void MyFrame::coordinated_mode_change(bool mode_now_paused) {
  reader->setPleasePause(mode_now_paused);
  if(mode_now_paused) {
    radio_box_1->SetSelection(1);
    DetailScope->setExamineHistory(); // The DetailScope will determine window based on 'Current' size and position
  }
  else {
    radio_box_1->SetSelection(0);
    DetailScope->setExamineCurrent(LongScope->drag_end_raw - LongScope->drag_start_raw); // Use the length of period in the dragged space
    LongScope->stopPausing();
  }
}

void MyFrame::long_scope_event(wxCommandEvent &event) {
  printf("Long Scope Event '%d' in MyFrame\n", event.GetInt()); 
  switch(event.GetInt()) {
   case scope_mouse_left_click: 
    // Clicking on the LongScope pauses the display
    coordinated_mode_change(true);
    break;
   case scope_mouse_dragging:
    // update the DetailScope with the new bounds
    DetailScope->setExamineHistory(LongScope->drag_start_raw, LongScope->drag_end_raw);  // The DetailScope will 'zoom in as required'
    break;
   case scope_mouse_drag_ended:
    // update the DetailScope with the new bounds 
    DetailScope->setExamineHistory(LongScope->drag_start_raw, LongScope->drag_end_raw);  // The DetailScope will 'zoom in as required'
    break;
   default : 
    printf("Unknown LongScope Action !\n");
    break;
  }
  return;
}

void MyFrame::detail_scope_event(wxCommandEvent &event) {
  printf("Detail Scope Event '%d' in MyFrame\n", event.GetInt()); 
 
  switch(event.GetInt()) {
   case scope_mouse_left_click: 
    // Clicking on the DetailScope unpauses the display : Maybe there are better things to think about...
    coordinated_mode_change(false);
    break;
   default : 
    printf("Unknown DetailScope Action !\n");
    break;
  }
  return;
}


DetailScopeRightClick::DetailScopeRightClick():wxMenuBar() {
    // begin wxGlade: DetailScopeRightClick::DetailScopeRightClick

    set_properties();
    do_layout();
    // end wxGlade
}

void DetailScopeRightClick::set_properties() {
    // begin wxGlade: DetailScopeRightClick::set_properties
    wxMenu* wxglade_tmp_menu_1 = new wxMenu();
    wxglade_tmp_menu_1->Append(rc_no_triggers, wxT("Remove Triggers"), wxEmptyString, wxITEM_RADIO);
    wxglade_tmp_menu_1->AppendSeparator();
    wxglade_tmp_menu_1->Append(rc_a_up, wxT("Ch A Up"), wxEmptyString, wxITEM_RADIO);
    wxglade_tmp_menu_1->Append(rc_a_down, wxT("Ch A Down"), wxEmptyString, wxITEM_RADIO);
    wxglade_tmp_menu_1->AppendSeparator();
    wxglade_tmp_menu_1->Append(rc_b_up, wxT("Ch B Up"), wxEmptyString, wxITEM_RADIO);
    wxglade_tmp_menu_1->Append(rc_b_down, wxT("Ch B Down"), wxEmptyString, wxITEM_RADIO);
    Append(wxglade_tmp_menu_1, wxT("RightClickMenu is first item"));
    // end wxGlade
}

void DetailScopeRightClick::pop_up() {
  wxMenu *menu=this->GetMenu(0);
  for(int i=0; i<menu->GetMenuItemCount(); i++) {
    wxMenuItem *item = menu->FindItemByPosition(i);
    item->Check(item->GetId() == selected_id);
  }
  PopupMenu( menu );
}

void DetailScopeRightClick::SetSelectedId(int _sel) {
  selected_id=_sel;
}

int DetailScopeRightClick::GetSelectedId() {
  return selected_id;
}

void DetailScopeRightClick::do_layout() {
    // begin wxGlade: DetailScopeRightClick::do_layout
    // end wxGlade
}

void DetailScopeRightClick::RightClickMenuChange(wxCommandEvent &event) {
  printf("RightClickMenuChange to '%d' in DetailScopeRightClick\n", event.GetSelection()); 
  // What was clicked ??
  selected_id=-1;
  for(int i=0; i<this->GetMenu(0)->GetMenuItemCount(); i++) {
    wxMenuItem * item=this->GetMenu(0)->FindItemByPosition(i);
    bool sel = item->IsChecked();
    // printf("Menu Item %d is %s\n", i, (sel)?"Selected":"--");
    if(sel) {
      selected_id=item->GetId();
    }
  }
  
  return;
}

