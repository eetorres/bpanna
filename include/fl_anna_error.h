// generated by Fast Light User Interface Designer (fluid) version 1.0300

#ifndef fl_anna_error_h
#define fl_anna_error_h
#include <FL/Fl.H>
#include <fl_cartesian.h>
#include <msmvtl/tmatrix.h>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Button.H>

class fl_anna_error {
  int counter; 
  Ca_LinePoint * GP; 
public:
  fl_anna_error();
  Fl_Double_Window *werror;
  Ca_Canvas *graph;
  Ca_Y_Axis *error;
  Ca_X_Axis *epoch;
  Fl_Value_Output *_error;
  Fl_Progress *_prog;
  Fl_Button *close;
private:
  inline void cb_close_i(Fl_Button*, void*);
  static void cb_close(Fl_Button*, void*);
public:
  void show();
private:
  void esetxaxis();
  void esetyaxis();
public:
  void grapherror(real err);
  void last_error(real r);
  void num_epochs(real r);
};
#endif
