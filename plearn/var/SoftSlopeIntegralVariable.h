// -*- C++ -*-

// PLearn (A C++ Machine Learning Library)
// Copyright (C) 1998 Pascal Vincent
// Copyright (C) 1999-2002 Pascal Vincent, Yoshua Bengio, Rejean Ducharme and University of Montreal
// Copyright (C) 2001-2002 Nicolas Chapados, Ichiro Takeuchi, Jean-Sebastien Senecal
// Copyright (C) 2002 Xiangdong Wang, Christian Dorion

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
// 
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
// 
//  3. The name of the authors may not be used to endorse or promote
//     products derived from this software without specific prior written
//     permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
// NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// This file is part of the PLearn library. For more information on the PLearn
// library, go to the PLearn Web site at www.plearn.org


/* *******************************************************      
   * $Id$
   * This file is part of the PLearn library.
   ******************************************************* */

#ifndef SoftSlopeIntegralVariable_INC
#define SoftSlopeIntegralVariable_INC

#include "NaryVariable.h"
#include <plearn/math/pl_math.h>
#include "Var_utils.h"

namespace PLearn {
using namespace std;


// Integral wrt x, from a to b, of SoftSlopeVariable(x,smoothness,left,right)
class SoftSlopeIntegralVariable: public NaryVariable
{
  typedef NaryVariable inherited;

public:
//protected:
  real a,b;
  bool tabulated;

public:
  //!  Default constructor for persistence
  SoftSlopeIntegralVariable();
  SoftSlopeIntegralVariable(Variable* smoothness, Variable* left, Variable* right, real a_=0, real b_=1, bool tabulated=true);

  PLEARN_DECLARE_OBJECT(SoftSlopeIntegralVariable);
  static void declareOptions(OptionList &ol);

  virtual void recomputeSize(int& l, int& w) const;
  virtual void fprop();
  virtual void bprop();
  virtual void symbolicBprop();
};

DECLARE_OBJECT_PTR(SoftSlopeIntegralVariable);

inline Var soft_slope_integral(Var smoothness, Var left, Var right, real a=0, real b=1)
{
  return new SoftSlopeIntegralVariable(smoothness,left,right,a,b);
}

} // end of namespace PLearn

#endif 
