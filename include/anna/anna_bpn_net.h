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

#ifndef _anna_BPN_NET_H_
#define _anna_BPN_NET_H_

#include <msmvtl/tmmanip.h>
#include <anna/anna_bpn_layer.h>

//------------------------------------------------------------------------------------------------
//#define _FW_MSG_ 1
//#define _BW_MSG_ 1

class anna_bpn_net : anna_bpn_layer{

public:
  //
  anna_bpn_net();
  ~anna_bpn_net();
  //
  // built functions
  void create(void);

  void initialize(void);
  //
  // input data from a file
  void read_data_file(char*,int,int);

  void read_data_file(char* f){
    read_data_file(f,0,0);
  }
  //
  void select_learning_rate(bool b){
    learning_rate_type=b;
  }

  void select_training_algorithm(int i){
    training_algorithm=i;
  }
  //
  // set functions
  void set_input(unsigned int u){
    input_size = u;
  }

  void set_output(unsigned int u){
    output_size = u;
  }

  void set_layers(unsigned int u){
    number_layers = u+1;
  }
  //
  void set_function_in_layer(unsigned int u1, unsigned int u2){
    _net[u1].set_activation(u2);
  }
  //
  void set_neurons_in_layer(unsigned int u1, unsigned int u2){
    _net[u1].resize(u2);
  }

  void set_weights_seed(unsigned int u){
    random_seed=u; srand(u);
  }

  void set_mu(real r){
    mu = r;
  }

  void set_alpha(real r){
    alpha = r;
  }

  void set_beta(real r){
    beta = r;
  }

  void set_gamma(real r){
    gamma = r;
  }

  // get function
  unsigned int get_input(void){
    return input_size;
  }

  unsigned int get_output(void){
    return output_size;
  }

  unsigned int get_number_layers(void){
    return number_layers;
  }

  unsigned int get_neuron_in_layer(unsigned int _n){
    return _net[_n].size();
  }
  //
  unsigned int get_rows(void){
    return rows;
  }

  unsigned int get_cols(void){
    return columns;
  }
  //TVector<int>& get_structure(void){return _nstr;};
  //
  // training functions
  void training(void);
  TMatrix<real> evalue(void);
  //
  TMatrix<real>& get_input_patterns(void){
    return ann_training_data;
  }

  TMatrix<real>& get_output_patterns(void){
    return ann_training_output;
  }

  TMatrix<real>& get_ann_outputs(void){
    return ann_output;
  }

  TMatrix<real>& get_data(void){
    return training_data;
  }

  real get_error(void){
    return total_error/ann_training_data.rows();
  }

  void open_bp_anna(const char* f){
    std::ifstream open_ann(f);
    //save_ann<<(*this);
    open_ann.close();
  }

  void save_bp_anna(const char* f){
    std::ofstream save_ann(f);
    save_ann<<(*this);
    save_ann.close();
  }

private:
  //
  // internal settings
  bool anna_sw_rs, learning_rate_type, dynamic_learning, initial_cg;
  // internal data
  unsigned int _di, _wi;
  unsigned int rows, columns, error_size;
  int total_weights, total_neurons, number_layers;
  int random_seed, input_size, output_size;
  int training_algorithm;
  real beta, alpha, mu, gamma, last_error, total_error;
  //
  TMatrix<real> training_data, ann_training_data, ann_training_output;
  TMatrix<real> ann_output, ann_output_error;
  TMatrix<real> _dF, m_hessian, invH, m_jacobian, m_jacobian_transpose;
  TMatrix<real> _D2,_D, _I;
  TMatrix<real> scaling_coefficients,  output_scaling_coefficients;
  //
  TVector<anna_bpn_layer> _net, _cnet;
  TVector<real> _annerr, _deltas, _g, _e, _w, _p;
  //
  // Training functions
  void steepest_descent_(void);
  void conjugate_gradient_(void);
  void levenverg_maquardt_(void);
  //
  inline TVector<real> forward_(TVector<real>& _v){
    return (_v*(*this));
  }

  inline TMatrix<real> forward_(TMatrix<real>& _m){
    TMatrix<real> _m1;
    _m1 = (_m*(*this));
    return _m1;
  }

  inline void forward_(void){
#ifdef _FWD_MSG_
    cout<<"forward"<<endl;
#endif
    ann_output = forward_(ann_training_data);
    ann_output_error = (ann_training_output-ann_output);
  }

  inline void backward_(TVector<real>& _v){
#ifdef _BWD_MSG_
    cout<<"backward"<<endl;
#endif
    ((*this)*_v);
  }

  inline void backward_(void){
#ifdef _BWD_MSG_
    cout<<"backward"<<endl;
#endif
    _dF= ((*this)*_D);
  }

  // updating weights
  inline void learn(){
    _wi=0;
    for(int _i2=(int)(get_number_layers()-1); _i2>=0; _i2--){
      for(unsigned int _jj2=0; _jj2<_net[_i2].size();_jj2++){
        for(unsigned int _w12=0; _w12<_net[_i2].inputs(); _w12++){
         _net[_i2].layer[_jj2].w[_w12]+=_w[_wi];
         _wi++;
        }
      }
    }
  }

  inline anna_bpn_net& operator= (const anna_bpn_net& _n){
    _net = _n._net;
    return *this;
  }

  // forward propagator matrix operator
  friend inline TMatrix<real> operator* (TMatrix<real>& _m, anna_bpn_net& _net){
    TMatrix<real> _m1(_m.rows(),_net.output_size);
    TVector<real> _v1;
    for(unsigned int _i=0; _i<_m.rows(); _i++){
      _v1 = _m[_i];
      _m1[_i] = _v1*_net;
    }
    return _m1;
  }

  // forward propagation vector operator
  friend inline TVector<real> operator* (TVector<real>& _v, anna_bpn_net& _net){
#ifdef _FW_MSG_
    std::cout<<endl<<"//********** begin forward **********//"<<std::endl;
#endif
    TVector<real> _tmp=_v, _tmp1;
    for(unsigned int _i=0; _i<_net.get_number_layers(); _i++){
      _tmp1 = (_tmp*_net._net[_i]);
      _tmp = _tmp1;
    }
#ifdef _FW_MSG_
    std::cout<<"//********** finished forward **********//"<<std::endl;
#endif
    return _tmp;
  }

  // backward propagation matrix operator
  friend inline TMatrix<real> operator* (anna_bpn_net& _n, TMatrix<real>& _m){
    TVector<real> _v1;
    TMatrix<real> _m1(_n.total_weights,0);
    for(unsigned int _e=0; _e<_m.cols(); _e++){
      _v1 = _m.get_col(_e);
      _v1 = _n*_v1;
      _m1.add_col(_v1);
    }
    return _m1;
  }

  // backward propagation vector operator
  friend inline TVector<real> operator* (anna_bpn_net& _n, TVector<real>& _v){
    TVector<real> d=_v;
    unsigned int _di=0, _wi=0;
    for(int _i=(int)_n.get_number_layers()-1; _i>=0; _i--){
      if(_i<(int)(_n.get_number_layers()-1)){
        d = (_n._net[_i+1]*d);
      }
      for(unsigned int _jj=0; _jj<_n._net[_i].size();_jj++){
      //if(_i!=_n.get_number_layers()-1)
        if(_i<(int)(_n.get_number_layers()-1))
          d[_jj] *= _n._net[_i].df[_jj];
        _n._deltas[_di] = d[_jj];
        for(unsigned int _w1=0; _w1<_n._net[_i].inputs(); _w1++){
          if(_w1<(_n._net[_i].inputs()-1)){
            _n._g[_wi] = (d[_jj] * _n._net[_i].y[_w1]);
          }else{
            _n._g[_wi] = -d[_jj];
          }
          _wi++;
        }
        _di++;
      }
    }
    return _n._g;
  }

  friend std::ostream& operator<< (std::ostream& _o, anna_bpn_net& _n){
    //cout<<"{ NEURAL NETWORK }"<<endl;
    _o<<_n.get_number_layers()<<std::endl;
    for(unsigned int _i=0; _i<_n.get_number_layers(); _i++){
      _o<<_n._net[_i].inputs()<<std::endl;
      _o<<_n._net[_i].size()<<std::endl;
      _o<<_n._net[_i]<<std::endl;
    }
    return _o;
  }
};

#endif

// END

