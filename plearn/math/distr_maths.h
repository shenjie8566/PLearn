// -*- C++ -*-4 1999/10/29 20:41:34 dugas

// distr_math.h
// Copyright (C) 2002 Pascal Vincent
//
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
   * $Id: distr_maths.h,v 1.2 2003/10/29 16:55:49 plearner Exp $
   * This file is part of the PLearn library.
   ******************************************************* */


/*! \file PLearnLibrary/PLearnAlgo/distr_maths.h */

#ifndef distr_maths_INC
#define distr_maths_INC

#include "TMat.h"

namespace PLearn <%
using namespace std;

real logOfNormal(const Vec& x, const Vec& mu, const Mat& C);

//! Fits a gaussian to the points in X (computing its mean and covariance
//! matrix, and adding lambda to the diagonal of that covariance matrix)
//! Then calls logOfNormal to return log(p(x | the_gaussian))
real logPFittedGaussian(const Vec& x, const Mat& X, real lambda);

//! returns log P(x|gaussian) with a gaussian represented compactly 
//! by the first few eigenvalues and eigenvectors of its covariance matrix.
//! gamma is the "variance" used for all other directions.
//! Eigenvalues need not be in decreasing order, but as soon as we meet a 0 eigenvalue,
//! this and all subsequent ones are considered to be equal to gamma.
//! In addition if add_gamma_to_eigenval is true, the used eigenvalues will be eigenvalues+gamma.
real logOfCompactGaussian(const Vec& x, const Vec& mu, 
                          const Vec& eigenvalues, const Mat& eigenvectors, 
                          real gamma=1e-6, bool add_gamma_to_eigenval=false);


%> // end of namespace PLearn

#endif
