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

#include "SubsampleVariable.h"

namespace PLearn {
using namespace std;


/** SubsampleVariable **/

PLEARN_IMPLEMENT_OBJECT(SubsampleVariable,
                        "A subsample var; equals subsample(input, the_subsamplefactor)",
                        "NO HELP");

SubsampleVariable::SubsampleVariable(Variable* input, int the_subsamplefactor) 
    : inherited(input, input->length()/the_subsamplefactor, input->width()/the_subsamplefactor), 
      subsamplefactor(the_subsamplefactor) 
{
    build_();
}

void
SubsampleVariable::build()
{
    inherited::build();
    build_();
}

void
SubsampleVariable::build_()
{
    if (input) {
        if (input->length() % subsamplefactor != 0 || input->width() % subsamplefactor != 0)
            PLERROR("In SubsampleVariable constructor: Dimensions of input are not dividable by subsamplefactor");
    }
}

void
SubsampleVariable::declareOptions(OptionList &ol)
{
    declareOption(ol, "subsamplefactor", &SubsampleVariable::subsamplefactor, OptionBase::buildoption, "");
    inherited::declareOptions(ol);
}

void SubsampleVariable::recomputeSize(int& l, int& w) const
{
    if (input) {
        l = input->length() / subsamplefactor;
        w = input->width()/subsamplefactor;
    } else
        l = w = 0;
}

void SubsampleVariable::fprop()
{
    subsample(input->matValue, subsamplefactor, matValue);
}


void SubsampleVariable::bprop()
{
    int norm = subsamplefactor * subsamplefactor;
    for(int i=0; i<length(); i++)
        for(int j=0; j<width(); j++)
        {
            real* inputgradientptr = input->matGradient[subsamplefactor*i]+subsamplefactor*j;
            real thisgradient = matGradient(i,j);
            for(int l=0; l<subsamplefactor; l++, inputgradientptr += input->matGradient.mod())
                for(int c=0; c<subsamplefactor; c++)
                {
                    inputgradientptr[c] = thisgradient/norm;
                }
        }
}


void SubsampleVariable::symbolicBprop()
{ PLERROR("SubsampleVariable::symbolicBprop() not yet implemented"); }



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
