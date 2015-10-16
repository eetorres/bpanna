// ANNA (Artificial Neural Network Architecture)
// This is a Back propagation artificial neural network which I wrote
// to fit the Fast Light Tool Kit (FLTK) library - www.fltk.org
//
// Copyright 2006 by Edmanuel Torres (eetorres@gmail.com)
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

#ifndef _BPNEURON_H_
#define _BPNEURON_H_

#include <cmath>
#include <iostream>
#include <msmvtl/tmatrix.h>
#include <msmvtl/tvmath.h>
#include <msmvtl/tmmath.h>

using namespace std;
//--------------------------------------------------------


class anna_bpn_neuron{
public:
  TVector<real> w;

  anna_bpn_neuron(){};

  ~anna_bpn_neuron(){
    w.resize(0);
  }

  // neuron Weights
  void resize(unsigned int);
  unsigned int size(void);
  void wrand(void);
  void wsrand(unsigned int);

  //void wbias(void){w[w.size()-1]=1.0;};
  anna_bpn_neuron& operator=(const anna_bpn_neuron& _n){
    w = _n.w;
    return *this;
  }

  friend real operator*(TVector<real>& _e, anna_bpn_neuron& _n){
    real res, b;
    b = _n.w[_n.w.size()-1];
    res = (_e*_n.w) - b;
    return res;
  }

  friend ::ostream& operator<<(::ostream& _o, anna_bpn_neuron& _n){
    _o<<_n.w;
    return _o;
  }
};

//------------------------------------------------------------------------------------------------
#endif

//
