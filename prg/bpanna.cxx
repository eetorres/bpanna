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

#include <anna/anna_bpn_net.h>

TMatrix<real> data2;

int main(void) {
  //
  unsigned int NUM_OUTPUTS=1;//, NUM_INPUTS=2;
  unsigned int NUM_LAYERS=3, NUM_NODES1=4, NUM_NODES2=7, NUM_NODES3=5;
  //
  anna_bpn_net *anna = new anna_bpn_net();
  anna->read_data_file((char*)"nor.txt");
  data2 = anna->get_data();
  cout<<data2;
  anna->set_output(NUM_OUTPUTS);
  anna->set_input((unsigned int)anna->get_cols()-(unsigned int)NUM_OUTPUTS);
  //
  anna->set_layers(NUM_LAYERS);
  anna->create();
  //
  anna->set_neurons_in_layer(0,NUM_NODES1);
  anna->set_neurons_in_layer(1,NUM_NODES2);
  anna->set_neurons_in_layer(2,NUM_NODES3);
  //
  anna->select_training_algorithm(true);
  //
  anna->initialize();
  //
  anna->set_mu(0.1);
  anna->set_alpha(0.2);
  // print anna
  //cout<<*anna;
  // training nested loop
  for(unsigned int epoch=0; epoch<10; epoch++){
    anna->training();
  }
  // evalue anna and print the output
  data2=anna->evalue();
  cout<<data2;
  // print anna
  //cout<<*anna;
  return 0;
}
