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
   * $Id: GaussianKernel.cc,v 1.14 2004/12/24 07:25:36 chapados Exp $
   * This file is part of the PLearn library.
   ******************************************************* */

#include "GaussianKernel.h"
#include <plearn/math/TMat_maths_impl.h>

//#define GK_DEBUG

namespace PLearn {
using namespace std;

// ** GaussianKernel **

PLEARN_IMPLEMENT_OBJECT(GaussianKernel,
    "The good old Gaussian kernel.",
    "");

////////////////////
// GaussianKernel //
////////////////////
GaussianKernel::GaussianKernel()
: scale_by_sigma(false),
  sigma(1)
{
  build_();
}

GaussianKernel::GaussianKernel(real the_sigma)
: scale_by_sigma(false),
  sigma(the_sigma)
{
  build_();
}

////////////////////
// declareOptions //
////////////////////
void GaussianKernel::declareOptions(OptionList& ol)
{
  declareOption(ol, "sigma", &GaussianKernel::sigma, OptionBase::buildoption,
                "The width of the Gaussian.");

  declareOption(ol, "scale_by_sigma", &GaussianKernel::scale_by_sigma, OptionBase::buildoption,
                "If set to 1, the kernel will be scaled by sigma^2 / 2");

  inherited::declareOptions(ol);
}

///////////
// build //
///////////
void GaussianKernel::build()
{
  inherited::build();
  build_();
}

////////////
// build_ //
////////////
void GaussianKernel::build_()
{
  minus_one_over_sigmasquare = -1.0/square(sigma);
  sigmasquare_over_two = square(sigma) / 2.0;
}


void GaussianKernel::makeDeepCopyFromShallowCopy(CopiesMap& copies)
{
  inherited::makeDeepCopyFromShallowCopy(copies);
  deepCopyField(squarednorms,copies);
}


void GaussianKernel::addDataForKernelMatrix(const Vec& newRow)
{
  inherited::addDataForKernelMatrix(newRow);

  int dlen  = data.length();
  int sqlen = squarednorms.length();
  if(sqlen == dlen-1)
    squarednorms.resize(dlen);
  else if(sqlen == dlen)
    for(int s=1; s < sqlen; s++)
      squarednorms[s-1] = squarednorms[s];  
  else
    PLERROR("Only two scenarios are managed:\n"
            "Either the data matrix was only appended the new row or, under the windowed settings,\n"
            "newRow is the new last row and other rows were moved backward.\n"
            "However, sqlen = %d and dlen = %d excludes both!", sqlen, dlen);
  
  squarednorms.lastElement() = pownorm(newRow, 2); 
}

/////////////////////////////////////////
// evaluateFromSquaredNormOfDifference //
/////////////////////////////////////////
inline real GaussianKernel::evaluateFromSquaredNormOfDifference(real sqnorm_of_diff) const
{
  if (sqnorm_of_diff < 0) {
    // This could happen (especiallly in -opt) because of approximations, when
    // it should actually be 0.
    if (sqnorm_of_diff < -1e-3)
      // This should not happen.
      PLERROR("In GaussianKernel::evaluateFromSquaredNormOfDifference - The given 'sqnorm_of_diff' is (significantly) negative (%f)", sqnorm_of_diff);
    sqnorm_of_diff = 0;
  }
  if (scale_by_sigma) {
    return exp(sqnorm_of_diff*minus_one_over_sigmasquare) * sigmasquare_over_two;
  } else {
    return exp(sqnorm_of_diff*minus_one_over_sigmasquare);
  }
}


//////////////
// evaluate //
//////////////
real GaussianKernel::evaluate(const Vec& x1, const Vec& x2) const
{
#ifdef BOUNDCHECK
  if(x1.length()!=x2.length())
    PLERROR("IN GaussianKernel::evaluate x1 and x2 must have the same length");
#endif
  int l = x1.length();
  real* px1 = x1.data();
  real* px2 = x2.data();
  real sqnorm_of_diff = 0.;
  for(int i=0; i<l; i++)
    {
      real val = px1[i]-px2[i];
      sqnorm_of_diff += val*val;
    }
  return evaluateFromSquaredNormOfDifference(sqnorm_of_diff);
}


//////////////////
// evaluate_i_j //
//////////////////
real GaussianKernel::evaluate_i_j(int i, int j) const
{ 
#ifdef GK_DEBUG 
  if(i==0 && j==1){
    cout << "*** i==0 && j==1 ***" << endl;
    cout << "data(" << i << "): " << data(i) << endl << endl;
    cout << "data(" << j << "): " << data(j) << endl << endl;  
    
    real sqnorm_i = pownorm((Vec)data(i), 2);
    if(sqnorm_i != squarednorms[i])
      PLERROR("%f = sqnorm_i != squarednorms[%d] = %f", sqnorm_i, i, squarednorms[i]);
    
    real sqnorm_j = pownorm((Vec)data(j), 2);
    if(sqnorm_j != squarednorms[j])
      PLERROR("%f = sqnorm_j != squarednorms[%d] = %f", sqnorm_j, j, squarednorms[j]);
  }
#endif
  return evaluateFromDotAndSquaredNorm(squarednorms[i],data->dot(i,j,data_inputsize),squarednorms[j]); 
}

//////////////////
// evaluate_i_x //
//////////////////
real GaussianKernel::evaluate_i_x(int i, const Vec& x, real squared_norm_of_x) const 
{ 
  if(squared_norm_of_x<0.)
    squared_norm_of_x = pownorm(x);

#ifdef GK_DEBUG 
//   real dot_x1_x2 = data->dot(i,x);
//   cout << "data.row(" << i << "): " << data.row(i) << endl 
//        << "squarednorms[" << i << "]: " << squarednorms[i] << endl
//        << "data->dot(i,x): " << dot_x1_x2 << endl
//        << "x: " << x << endl
//        << "squared_norm_of_x: " << squared_norm_of_x << endl;
//   real sqnorm_of_diff = (squarednorms[i]+squared_norm_of_x)-(dot_x1_x2+dot_x1_x2);
//   cout << "a-> sqnorm_of_diff: " << sqnorm_of_diff << endl
//        << "b-> minus_one_over_sigmasquare: " << minus_one_over_sigmasquare << endl
//        << "a*b: " << sqnorm_of_diff*minus_one_over_sigmasquare << endl
//        << "res: " << exp(sqnorm_of_diff*minus_one_over_sigmasquare) << endl; 
#endif

  return evaluateFromDotAndSquaredNorm(squarednorms[i],data->dot(i,x),squared_norm_of_x); 
}


//////////////////
// evaluate_x_i //
//////////////////
real GaussianKernel::evaluate_x_i(const Vec& x, int i, real squared_norm_of_x) const
{ 
  if(squared_norm_of_x<0.)
    squared_norm_of_x = pownorm(x);
  return evaluateFromDotAndSquaredNorm(squared_norm_of_x,data->dot(i,x),squarednorms[i]); 
}

////////////////////////////
// setDataForKernelMatrix //
////////////////////////////
void GaussianKernel::setDataForKernelMatrix(VMat the_data)
{ 
  inherited::setDataForKernelMatrix(the_data);
  squarednorms.resize(data.length());
  for(int index=0; index<data.length(); index++)
    squarednorms[index] = data->dot(index,index, data_inputsize);
}

///////////////////
// setParameters //
///////////////////
void GaussianKernel::setParameters(Vec paramvec)
{ 
  PLWARNING("In GaussianKernel: setParameters is deprecated, use setOption instead");
  sigma = paramvec[0]; 
  minus_one_over_sigmasquare = -1.0/(sigma*sigma);
}


} // end of namespace PLearn

