// Cartesian.H,v 0.4
//
// Copyright 2000 by Roman Kantor.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public License
// version 2 as published by the Free Software Foundation.
//
// This library is distributed  WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY 
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.


#ifndef _Cartesian_h_
#define _Cartesian_h_

#include <msmvtl/libdef.h>
#include <FL/Fl_Box.H>

#define DEFAULT_POINT_SIZE 4

enum Ca_Damage {CA_DAMAGE_ALL=1, CA_DAMAGE_ADD=2};
enum Ca_When{CA_WHEN_MIN=1,CA_WHEN_MAX=2};
enum Ca_AxisAlign{CA_LEFT=0, CA_RIGHT=1, CA_TOP=CA_LEFT, CA_BOTTOM=CA_RIGHT, CA_CENTER=3,CA_ALIGNMENT=3,CA_LINE=128};
enum Ca_PointStyle{SIMPLE=0,ROUND=1,SQUARE=2,UP_TRIANGLE=3, DOWN_TRIANGLE=4, DIAMOND=5,NO_POINT=6, POINT_STYLE=127, BORDER=128};
enum Ca_GridVisible{CA_MINOR_GRID=1, CA_MAJOR_GRID=2, CA_LABEL_GRID=4};
enum Ca_Scale{CA_LIN=0, CA_LOG=1, CA_REV=2};

class Ca_Canvas;
class Ca_Axis_;
class Ca_Object_;
class Ca_ObjectChain;
class Cartesian;


///////////////////////////////////////////////////////////////////////////
class Ca_ObjectChain{
public:
    Ca_Object_ *object;
    Ca_ObjectChain *next;
};


///////////////////////////////////////////////////////////////////////////


class Ca_Canvas:public Fl_Box{

    friend class Ca_Axis_;
    friend class Ca_X_Axis;
    friend class Ca_Y_Axis;
    friend class Ca_Object_;

    static Ca_Canvas *current_;
    int replot_;
    Ca_Axis_ * last_axis_;
    int border_;
    Ca_Axis_ *current_x_;
    Ca_Axis_ *current_y_;
    Ca_ObjectChain * first_object_;
    Ca_ObjectChain * last_object_;
    Ca_ObjectChain * last_plotted_;
    void add_object(Ca_Object_ *object);
    
    /* No function body - prevents copy construction/assignment */
    Ca_Canvas(const Ca_Canvas &);    
    const Ca_Canvas & operator=(const Ca_Canvas &); 

protected:
    void draw();

public:
    static Ca_Canvas * current(){return current_;};
    static void current(Ca_Canvas * _current){current_=_current;}
    void current_x(Ca_Axis_ * axis){current_x_=axis;};
    void current_y(Ca_Axis_ * axis){current_y_=axis;};
    Ca_Axis_ * current_x(){return current_x_;};
    Ca_Axis_ * current_y(){return current_y_;};
    void clear();
    int border(){return border_;};
    void border(int border);
    Ca_Canvas(int x, int y, int w, int h, const char *label=0);
    ~Ca_Canvas();
    
};


///////////////////////////////////////////////////////////////////////////

class Ca_Axis_:public Fl_Box{

    friend class Ca_Canvas;

    Ca_Axis_ *previous_axis_;
	
	

protected:
	int scale_;
	int next_tick(int &tick_index, double &tick_value, int &tick_order, double &interval_);
	bool valid_;
	double k_;
    double q_;
    const char *label_format_;
    Fl_Color minor_grid_color_;
    Fl_Color major_grid_color_;
    Fl_Color label_grid_color_;
    int minor_grid_style_;
    int major_grid_style_;
    int label_grid_style_;
    int minor_grid_width_;
    int major_grid_width_;
    int label_grid_width_;
    char * minor_grid_dashes_;
    char * major_grid_dashes_;
    char * label_grid_dashes_;
    int grid_visible_;
    double tick_interval_;
    int tick_separation_;
    int tick_length_;
    int major_step_;
    int label_step_;
    Ca_Canvas * canvas_;
    unsigned char  axis_align_;
    Fl_Font label_font_face_;
    int label_font_size_;

    double min_;
    double max_;
    int min_pos_;
    int max_pos_;
    int border_;
    Fl_Color axis_color_;

    virtual int min_pos()=0;
    virtual int max_pos()=0;
    int update();
    virtual void draw_grid()=0;

public:
    virtual void current()=0;
    double position(double);
    double value(double);
    Ca_Canvas * canvas(){return canvas_;};
    int border(){return border_;};
    void border(int border){border_=border;damage(CA_DAMAGE_ALL);}
    double min_val(){return min_;};
    double max_val(){return max_;};
    void min_val(double x);
    void max_val(double x);

    void rescale(int when, double x);
    void rescale_move(int when, double  x);
    void tick_interval(double interval) {tick_interval_=interval; damage(CA_DAMAGE_ALL);};
    double tick_interval(){return tick_interval_;};
    void tick_separation(int separation) {tick_separation_=separation; damage(CA_DAMAGE_ALL);};
    int tick_separation(){return tick_separation_;};
    void tick_length(int length){tick_length_=length; damage(CA_DAMAGE_ALL);};
    int tick_length(){return tick_length_;};
    void major_step(int step){major_step_=step;damage(CA_DAMAGE_ALL);};
    int major_step(){return major_step_;};
    void label_step(int step){label_step_=step;damage(CA_DAMAGE_ALL);};
    int label_step(){return label_step_;};
    void label_format(const char *format){label_format_=format; damage(CA_DAMAGE_ALL);};
    const char* label_format(){return label_format_;};
    void label_font(Fl_Font face){label_font_face_=face; damage(CA_DAMAGE_ALL);};
    Fl_Font label_font(){return label_font_face_;};
    void label_size(int size){label_font_size_=size; damage(CA_DAMAGE_ALL);};
    int label_size(){return label_font_size_;};
    void axis_color(Fl_Color _axis_color){axis_color_=_axis_color; damage(CA_DAMAGE_ALL);};
    void minor_grid_color(Fl_Color color){minor_grid_color_=color; canvas_->damage(CA_DAMAGE_ALL);};
    Fl_Color minor_grid_color(){return minor_grid_color_;};
    void minor_grid_style(int style){minor_grid_style_=style; canvas_->damage(CA_DAMAGE_ALL);};
    int minor_grid_style(){return minor_grid_style_;};
    void minor_grid_width(int width){minor_grid_width_=width; canvas_->damage(CA_DAMAGE_ALL);};
    int minor_grid_width(){return minor_grid_width_;};
    void minor_grid_dashes(char *dashes){minor_grid_dashes_=dashes; canvas_->damage(CA_DAMAGE_ALL);};
    char *minor_grid_dashes(){return minor_grid_dashes_;};
    void major_grid_color(Fl_Color color){major_grid_color_=color;  canvas_->damage(CA_DAMAGE_ALL);};
    Fl_Color major_grid_color(){return major_grid_color_;};
    void major_grid_style(int style){major_grid_style_=style; canvas_->damage(CA_DAMAGE_ALL);};
    int major_grid_style(){return major_grid_style_;};
    void major_grid_width(int width){major_grid_width_=width; canvas_->damage(CA_DAMAGE_ALL);};
    int major_grid_width(){return major_grid_width_;};
    void major_grid_dashes(char *dashes){major_grid_dashes_=dashes; canvas_->damage(CA_DAMAGE_ALL);};
    char *major_grid_dashes(){return major_grid_dashes_;};
    void label_grid_color(Fl_Color color){label_grid_color_=color;  canvas_->damage(CA_DAMAGE_ALL);};
    Fl_Color label_grid_color(){return label_grid_color_;};
    void label_grid_style(int style){label_grid_style_=style; canvas_->damage(CA_DAMAGE_ALL);};
    int label_grid_style(){return label_grid_style_;};
    void label_grid_width(int width){label_grid_width_=width; canvas_->damage(CA_DAMAGE_ALL);};
    int label_grid_width(){return label_grid_width_;};
    void label_grid_dashes(char *dashes){label_grid_dashes_=dashes; canvas_->damage(CA_DAMAGE_ALL);};
    char *label_grid_dashes(){return label_grid_dashes_;};
    void grid_visible(int visible){grid_visible_=visible;canvas_->damage(CA_DAMAGE_ALL);};
    int grid_visible(){return grid_visible_;};
    void axis_align(unsigned char align){axis_align_=align; damage(CA_DAMAGE_ALL);};
    unsigned char axis_align(){return axis_align_;};
	void scale(int s){if(s!=scale_){redraw(); canvas_->redraw();} scale_=s;};
	int scale(){return scale_;};
    
    Ca_Axis_(int x, int y, int w, int h, const char * label=0);
    ~Ca_Axis_();
};


//////////////////////////////////////////////////////////////////////
class Ca_X_Axis: public Ca_Axis_{

protected:
    int min_pos();
    int max_pos();
    void draw();
    void draw_grid();
public:
    void current();
    Ca_X_Axis(int x, int y, int w, int h, const char * label=0);
	~Ca_X_Axis();

};

class Ca_Y_Axis: public Ca_Axis_{
    int min_pos();
    int max_pos();
    void draw();
    void draw_grid();
public:
    void current();
    Ca_Y_Axis(int x, int y, int w, int h, const char * label=0);
	~Ca_Y_Axis();
    
};

class Ca_Object_{
    
	friend class Ca_Canvas;
    friend class Ca_Axis_;
    friend class Ca_Y_Axis;
    friend class Ca_X_Axis;

    // No function body - prevents copy construction/assignment 
    Ca_Object_(const Ca_Object_ &);    
    const Ca_Object_ & operator=(const Ca_Object_ &);
    
protected:
    Ca_Canvas *canvas_;
    Ca_Axis_ *x_axis_;
    Ca_Axis_ *y_axis_;
    virtual void draw()=0;
        
public:
    Ca_Object_(Ca_Canvas * canvas=0);
    virtual ~Ca_Object_();

};

class Ca_Point:public Ca_Object_{

protected:
    void draw();

public:
    double x;
    double y;
    int style;
    int size;
    Fl_Color color;
    Fl_Color border_color;
    Ca_Point(double _x, double _y, Fl_Color color=FL_BLACK, Fl_Color border_color=FL_BLACK, int style=SIMPLE, int size=DEFAULT_POINT_SIZE);
};

///////////////////////////////////////////////////////////////////////////

class Ca_LinePoint:public Ca_Point{

protected:
    void draw();

public:
    Ca_LinePoint *previous;
    Ca_LinePoint(Ca_LinePoint *_previous, double _x, double _y, Fl_Color color, Fl_Color border_color=FL_BLACK, int style=0, int size=DEFAULT_POINT_SIZE);
    Ca_LinePoint(Ca_LinePoint *_previous, double _x, double _y);
};

class Ca_PolyLine:public Ca_LinePoint{
	
protected:
	void draw();
public:
	int line_size;
	int line_style;
	Ca_PolyLine * next;
	Ca_PolyLine(Ca_PolyLine *_previous, double _x, double _y,int _line_style, int _line_size=0, Fl_Color color=FL_BLACK, Fl_Color border_color=FL_BLACK, int style=0, int size=DEFAULT_POINT_SIZE);
	Ca_PolyLine(Ca_PolyLine *_previous, double _x, double _y);
};




#endif
