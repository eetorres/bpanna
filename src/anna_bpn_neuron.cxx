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

#include <anna/anna_bpn_neuron.h>

// neuron Weights
void anna_bpn_neuron::resize(unsigned int u){
  w.resize(u);
}

unsigned int anna_bpn_neuron::size(void){
  return w.size();
}

void anna_bpn_neuron::wrand(void){
  w.vrand();
  cout<<" w="<<w;
}

void anna_bpn_neuron::wsrand(unsigned int u){
  w.svrand(u);
}

//
//


