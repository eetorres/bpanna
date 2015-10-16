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

#include <anna/anna_bpn_net.h>
#include <ctime>

// Many funny settings mainly for debuging
//#define  _INI_MSG_ 1  // initialization
//#define  _BWD_MSG_ 1  // backward propagation
//#define  _FWD_MSG_ 1  // fordward propagation
//#define  _TRN_MSG_ 1  // training cycle
//#define  _APP_MSG_ 1  // display the results
//#define  _ANNA_DEBUG_

// default values
anna_bpn_net::anna_bpn_net(){
    random_seed = 0;              // 0 = random generation
    training_algorithm=0; // Levenverg-Maquardt
    learning_rate_type = false; // static/dynamic
    dynamic_learning = false; // static learning
    beta = 0;
    alpha = 0;
    mu = 0;
}

anna_bpn_net::~anna_bpn_net(){
}

void anna_bpn_net::read_data_file(char* _fl,int _f, int _c){
    training_data.read_file(_fl,_f,_c);
    //cout<<"Data="<<training_data;
    columns = training_data.cols();
    rows = training_data.rows();
}

void anna_bpn_net::create(void){
    TVector<real> _v;
    ann_training_data = training_data;
    // get scaling parameters
    scaling_coefficients = m_get_intervals(ann_training_data);
#ifdef _ANNA_DEBUG_
    cout<<"scaling parameters="<<scaling_coefficients;
#endif
    // normalizing into the interval [-0.5,0.5]
    ann_training_data = m_normalize_ranges(ann_training_data,scaling_coefficients);
#ifdef _ANNA_DEBUG_
    cout<<"normalized="<<ann_training_data;
#endif
    columns = ann_training_data.cols();
    rows = ann_training_data.rows();
    // output pattern assignament
    output_scaling_coefficients.resize(0,2);
    ann_training_output.resize(ann_training_data.rows(),0);
    for(unsigned int _i=0; _i<get_output(); _i++){
      output_scaling_coefficients.add_row(scaling_coefficients[scaling_coefficients.rows()-1]);
      scaling_coefficients.del_row();
      _v = ann_training_data.get_col(ann_training_data.cols()-1);
      ann_training_data.del_col();
      ann_training_output.add_col(_v);
#ifdef _ANNA_DEBUG_
      cout<<" onet="<<ann_training_output;
#endif
    }
    error_size = ann_training_output.cols();
    //
#ifdef _INI_MSG_
    cout<<"initializing the neural network"<<endl;
#endif
    _net.resize(number_layers);
    //_nstr.resize(number_layers);
    set_neurons_in_layer(number_layers-1,output_size);
}

void anna_bpn_net::initialize(void){
#ifdef _INI_MSG_
    cout<<"initializing internal parameters"<<endl;
#endif
    unsigned int _nw=0, _nn=0;
    _net[0].inputs(input_size+1);
    _nw += (_net[0].inputs() * _net[0].size());
    _nn = _net[0].size();
    for(unsigned int i=1; i<_net.size(); i++){
      _net[i].inputs(_net[i-1].size()+1);
      _nw += (_net[i].inputs() * _net[i].size());
      _nn += _net[i].size();
    }
    for(unsigned int i=0; i<_net.size(); i++){
      _net[i].activation();
      if(random_seed > 0)
       _net[i].wsrand(random_seed);
      else{
        srand((unsigned)time(0));
        _net[i].wrand();
      }
#ifdef _ANNA_DEBUG_
    cout<<" layer["<<i<<"]="<<_net[i];
#endif
    }
    total_weights = _nw;
    total_neurons = _nn;
    _deltas.resize(total_neurons);
    m_jacobian.resize(ann_training_output.cols()*ann_training_output.rows(),total_weights);
    _g.resize(total_weights);
    //
    ann_output.resize(ann_training_output.rows(),ann_training_output.cols());
    ann_output_error.resize(ann_training_output.rows(),ann_training_output.cols());
    _dF.resize(ann_training_output.cols(),total_weights);
    m_hessian.resize(total_weights,total_weights);
    //
    _I.resize(total_weights,total_weights);
    _D.resize(ann_training_output.cols(),ann_training_output.cols());
    _e.resize(ann_training_output.cols()*ann_training_output.rows());
    _annerr.resize(ann_training_output.rows());
    _p.resize(ann_training_output.rows());
    //
    _I.main_diagonal(1.0);
    _D.main_diagonal(1.0);
    //
    initial_cg = false;
#ifdef _INI_MSG_
    cout<<"total neurons: "<<total_neurons<<endl;
    cout<<"total weights "<<total_weights<<endl;
#endif
#ifdef _ANNA_DEBUG_
    cout<<" net="<<_net;
#endif
}

void anna_bpn_net::training(void){
  if(learning_rate_type) _cnet = _net;
#ifdef _TRN_MSG_
  cout<<"Train"<<endl;
#endif
  switch(training_algorithm){
    case 0:
    levenverg_maquardt_();
    break;
  case 1:
    steepest_descent_();
    break;
  case 2:
    conjugate_gradient_();
    break;
  }
#ifdef _ANNA_DEBUG_
  cout<<" trained net="<<_net;
#endif
}

void anna_bpn_net::levenverg_maquardt_(void){
#ifdef _TRN_MSG_
  cout<<" levenverg_maquardt"<<endl;
#endif
  total_error = 0;
  int _cnt = 0;
  for(unsigned int i=0; i<ann_training_data.rows(); i++){
  ann_output[i] = forward_(ann_training_data[i]);
  ann_output_error[i] = (ann_training_output[i]-ann_output[i]);
  backward_(); // derivatives of the nodes
  // build the Jacobian matrix
  for(unsigned int _j=0; _j<error_size; _j++)
    m_jacobian[(i*error_size)+_j] = _dF.get_col(_j);
  }

  for(unsigned int _i=0; _i<ann_output_error.rows(); _i++){
    for(unsigned int _j=0; _j<ann_output_error.cols(); _j++){
      _e[_cnt] = ann_output_error[_i][_j];
      _cnt++;
    }
  }
  m_jacobian_transpose = m_jacobian;            // making a copy of J
  m_jacobian_transpose.transpose();     // transpose of the Jacobian
  _g = (m_jacobian_transpose*_e);       // gradient
  m_hessian = (m_jacobian_transpose*m_jacobian);       // the Hessian matrix
  m_hessian = (m_hessian+mu*_I);    // applying a perturbation
  invH =m_hessian.inverse();  // modified Hessian matrix inversion
  _w = (invH*_g);      // correction vector
  _annerr = ann_output_error.norm_cols();
  total_error = _annerr.sum();
  learn();             // updating weights

}

void anna_bpn_net::steepest_descent_(void){
#ifdef _TRN_MSG_
  cout<<" steepest_descent"<<endl;
#endif
  TVector<real> _oe;
  total_error = 0;
  for(unsigned int i=0; i<ann_training_data.rows(); i++){
    ann_output[i] = forward_(ann_training_data[i]);
    ann_output_error[i] = (ann_training_output[i]-ann_output[i]);
    backward_();  // derivatives of the nodes
    _oe = ann_output_error[i];
    _w = alpha*(_dF*_oe);  // compute the gradient
    learn();  // updating weights
  }
  _annerr = ann_output_error.norm_cols();
  total_error = _annerr.sum();
}

void anna_bpn_net::conjugate_gradient_(void){
#ifdef _TRN_MSG_
  cout<<" conjugate_gradient"<<endl;
#endif
  TVector<real> e, _g_1, _dg;
  total_error = 0;
  for(unsigned int i=0; i<ann_training_data.rows(); i++){
    ann_output[i] = forward_(ann_training_data[i]);
    ann_output_error[i] = (ann_training_output[i]-ann_output[i]);
    backward_();  // derivatives of the nodes
    e = ann_output_error[i];
    _g = _dF*e;  // compute the gradient
    if( initial_cg ){
      _p = (_g+beta*_p);  // correction vector
    }else{
      _p = _g;  // correction vector
      initial_cg = true;
    }
   _g_1=_g;
   _w = (alpha*_p);  // correction vector
   learn();  // updating weights
  }
  _annerr = ann_output_error.norm_cols();
  total_error = _annerr.sum();
}

TMatrix<real> anna_bpn_net::evalue(void){
    TVector<real> _v1;
    TMatrix<real> _m1;
    ann_output = forward_(ann_training_data);
    _m1 = m_rescale(ann_output,output_scaling_coefficients);  // rescaling back
#ifdef _APP_MSG_
    cout<<"Resultado="<<_m1;
#endif
  //save_bp_net((char*)"structure.ann");
  return _m1;
}

//
