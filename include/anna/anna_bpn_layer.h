// ANNA (Artificial Neural Network Architecture)
// This is a Back propagation artificial neural network which I wrote
// to fit the Fast Light Tool Kit (FLTK) library - www.fltk.org
//
// Copyright 2006 by Edmanuel Torres A. (eetorres@gmail.com)
//
// This library  is  free software;  you  can  redistribute  it and/or
// modify  it  under  the  terms  of  the   GNU Library General Public
// License  as  published  by  the   Free  Software Foundation; either
// version 2 of the License,  or  (at your option)  any  later version
// or much better FLTK license, which allow you static linking.
//
// This  library  is  distributed in the hope that  it will be useful,
// but  WITHOUT ANY WARRANTY;  without  even  the  implied warranty of
// MERCHANTABILITY  or  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received  a  copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Sent me suggestion, modification or bugs. Don't hesitate to contact
// me for any question, I will be very grateful with your feedbacks.
// Get the latest version at http://bpanna.sourceforge.net/
//

#ifndef _BPNLAYER_H_
#define _BPNLAYER_H_

#include <anna/anna_bpn_neuron.h>

//#define _MSG_

#define ACTIVATION_FUNCTION_SIGMOID 0
#define ACTIVATION_FUNCTION_TANSIG  1
#define ACTIVATION_FUNCTION_STEP    2

class anna_bpn_layer{

public:

  unsigned int num_inputs, activation_function;
  TVector<anna_bpn_neuron> layer;
  TVector<real> y;
  TVector<real> f;
  TVector<real> df;

  typedef real (anna_bpn_layer::*pt2func)(real);
  pt2func _mfun, _mdfun;

  anna_bpn_layer(){
    //_mfun=&anna_bpn_layer::tansig;
    //_mdfun=&anna_bpn_layer::dtansig;
  };

  ~anna_bpn_layer(){
    layer.resize(0);
    y.resize(0);
    f.resize(0);
    df.resize(0);
  }

  void resize(int _s){
    layer.resize(_s);
    //_u.resize(_s);
    f.resize(_s);
    df.resize(_s);
  }

  void set_activation(uint u){
    activation_function=u;
  }

  void activation(void){
    switch(activation_function){
    case ACTIVATION_FUNCTION_SIGMOID:
      _mfun=&anna_bpn_layer::sigmoid;
      _mdfun=&anna_bpn_layer::dsigmoid;
    break;
    case ACTIVATION_FUNCTION_TANSIG:
      _mfun=&anna_bpn_layer::tansig;
      _mdfun=&anna_bpn_layer::dtansig;
    break;
    default:
      _mfun=&anna_bpn_layer::tansig;
      _mdfun=&anna_bpn_layer::dtansig;
    }
  };

  unsigned int size(void){
    return layer.size();
  }

  unsigned int inputs(void){
    return num_inputs;
  }

  void inputs(unsigned int u){
    num_inputs = u;
    //resize(num_inputs);
    y.resize(num_inputs);
    for(unsigned int i=0; i<size(); i++){
      layer[i].resize(num_inputs);
    }
  }

  void wrand(void){
    for(unsigned int i=0; i<size(); i++){
      layer[i].wrand();
    }
  }

  void wsrand(unsigned int u){
    for(unsigned int i=0; i<size(); i++){
      layer[i].wsrand(u);
    }
  }

  //void wbias(void){
  //  for(unsigned int i=0; i<size(); i++)
  //    layer[i].wbias();
  //}

  real cb_func(real x, anna_bpn_layer& n){
    return (n.*_mfun)(x);
  }

  real cb_dfunc(real x, anna_bpn_layer& n){
    return (n.*_mdfun)(x);
  }

  anna_bpn_layer& operator= (const anna_bpn_layer& _l){
    layer = _l.layer;
    return *this;
  }

  //Forward
  friend TVector<real> operator * (TVector<real>& e, anna_bpn_layer& l){
    real u;
    l.y = e;
    TVector<real> v(l.layer.size());
    for(unsigned int i=0; i<l.layer.size(); i++){
      u = (e * l.layer[i]);
      l.df[i] = l.cb_dfunc(u,l);
      l.f[i]  = l.cb_func(u,l);
      v[i]    = l.cb_func(u,l);
    }
    return v;
  }

  //Backward
  friend TVector<real> operator * (anna_bpn_layer& l, TVector<real>& e){
    TVector<real> v(l.layer[0].size());
    for(unsigned int i=0; i<l.size(); i++){
      v = (v + e[i]*l.layer[i].w);
    }
    return v;
  }

  //Backward
  friend TMatrix<real> operator + (anna_bpn_layer& l, TVector<real>& e){
    TMatrix<real> m(l.size(),l.inputs());
    //cout<<"entradas"<<_l.inputs()<<endl;
    //cout<<"size"<<_e.size()<<endl;
    for(unsigned int i=0; i<l.size(); i++){
      m[i] = (l.layer[i].w*e[i]*l.df[i]);
    }
    return m;
  }

  friend std::ostream& operator<< (std::ostream& o, anna_bpn_layer& l){
    //cout<<"  < NAs "<<_l.size()<<" >"<<endl;
    for(unsigned int i=0; i<l.size(); i++){
      o<<l.layer[i];
    }
    return o;
  }

  // The sigmoid function.
  real sigmoid(real);
  real dsigmoid(real);// The sigmoid derivative function
  // The tansig function.
  real tansig(real);
  real dtansig(real);
  //
};

#endif

//
