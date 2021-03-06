// -*- C++ -*-

// PLearn (A C++ Machine Learning Library)
// Copyright (C) 1998 Pascal Vincent
// Copyright (C) 1999-2002 Pascal Vincent, Yoshua Bengio and University of Montreal
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
 * $Id$
 * This file is part of the PLearn library.
 ******************************************************* */

#include "CompactVMatrixPolynomialKernel.h"

namespace PLearn {
using namespace std;

PLEARN_IMPLEMENT_OBJECT(CompactVMatrixPolynomialKernel, "ONE LINE DESCR", "NO HELP");

real CompactVMatrixPolynomialKernel::evaluate(const Vec& x1, const Vec& x2) const
{ // return ipow(beta*dot(x1,x2)+1.0, n);
    real dot_product=0;
    if (m)
        dot_product=m->dotProduct(int(x1[0]),int(x2[0]));
    else
        dot_product=dot(x1,x2);
    return ipow(beta*dot_product+1.0, n);
}

void CompactVMatrixPolynomialKernel::declareOptions(OptionList &ol)
{
    declareOption(ol, "n", &CompactVMatrixPolynomialKernel::n, OptionBase::buildoption,
                  "TODO: Some comments");
    declareOption(ol, "beta", &CompactVMatrixPolynomialKernel::beta, OptionBase::buildoption,
                  "TODO: Some comments");
    declareOption(ol, "m", &CompactVMatrixPolynomialKernel::m, OptionBase::buildoption, "");
    inherited::declareOptions(ol);
}


} // end of namespace PLearn


/*
  Local Variables:
  mode:c++
  c-basic-offset:4
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0))
  indent-tabs-mode:nil
  fill-column:79
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=79 :
