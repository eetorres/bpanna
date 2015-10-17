// generated by Fast Light User Interface Designer (fluid) version 1.0300

#ifndef fl_anna_about_h
#define fl_anna_about_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Button.H>

class fl_anna_about {
public:
  fl_anna_about();
  Fl_Double_Window *about;
  Fl_Output *_inf;
private:
  void cb_Close_i(Fl_Button*, void*);
  static void cb_Close(Fl_Button*, void*);
public:
  void show();
  void hide();
};
#endif