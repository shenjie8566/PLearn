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
   * $Id: AffineTransformWeightPenalty.cc,v 1.8 2004/04/28 18:42:20 tihocan Exp $
   * This file is part of the PLearn library.
   ******************************************************* */

#include "AffineTransformWeightPenalty.h"
#include "Var_utils.h"

namespace PLearn {
using namespace std;


PLEARN_IMPLEMENT_OBJECT(AffineTransformWeightPenalty, "Affine transformation with Weight decay terms", "NO HELP");

void AffineTransformWeightPenalty::recomputeSize(int& l, int& w) const
{ l=1; w=1; }

void
AffineTransformWeightPenalty::declareOptions(OptionList &ol)
{
    declareOption(ol, "weight_decay_", &AffineTransformWeightPenalty::weight_decay_, OptionBase::buildoption, "");
    declareOption(ol, "bias_decay_", &AffineTransformWeightPenalty::bias_decay_, OptionBase::buildoption, "");
    declareOption(ol, "L1_penalty_", &AffineTransformWeightPenalty::L1_penalty_, OptionBase::buildoption, "");
    inherited::declareOptions(ol);
}

void AffineTransformWeightPenalty::fprop()
{
  if (L1_penalty_)
  {
    if (input->length()>1)
      valuedata[0] = weight_decay_*sumabs(input->matValue.subMatRows(1,input->length()-1));
    else 
      valuedata[0] = 0;
    if(bias_decay_!=0)
      valuedata[0] += bias_decay_*sumabs(input->matValue(0));
  }
  else
  {
    if (input->length()>1)
      valuedata[0] = weight_decay_*sumsquare(input->matValue.subMatRows(1,input->length()-1));
    else 
      valuedata[0] = 0;
    if(bias_decay_!=0)
      valuedata[0] += bias_decay_*sumsquare(input->matValue(0));
  }
}

    
void AffineTransformWeightPenalty::bprop()
{
  int l = input->length() - 1;
  if (L1_penalty_)
  {
    if (!input->matGradient.isCompact())
      PLERROR("AffineTransformWeightPenalty::bprop, L1 penalty currently not handling non-compact weight matrix");
    int n=input->width();
    if (weight_decay_!=0)
    {
      real delta = weight_decay_ * gradientdata[0];
      real* w = input->matValue[n];
      real* d_w = input->matGradient[n];
      int tot = l * n; // Number of weights to update.
      for (int i = 0; i < tot; i++) {
        if (w[i] > 0)
          d_w[i] += delta;
        else if (w[i] < 0)
          d_w[i] -= delta;
      }
    }
    if(bias_decay_!=0)
    {
      real* d_biases = input->matGradient[0];
      real* biases = input->matValue[0];
      for (int i=0;i<n;i++)
        if (biases[i]>0)
          d_biases[i] += bias_decay_*gradientdata[0];
        else if (biases[i]<0)
          d_biases[i] -= bias_decay_*gradientdata[0];
    }
  }
  else
  {
    multiplyAcc(input->matGradient.subMatRows(1,l), input->matValue.subMatRows(1,l), two(weight_decay_)*gradientdata[0]);
    if(bias_decay_!=0)
      multiplyAcc(input->matGradient(0), input->matValue(0), two(bias_decay_)*gradientdata[0]);
  }
}



} // end of namespace PLearn


