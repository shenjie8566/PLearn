// -*- C++ -*-

// OnlineLearningModule.cc
//
// Copyright (C) 2005 Yoshua Bengio 
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
   * $Id: .pyskeleton_header 544 2003-09-01 00:05:31Z plearner $ 
   ******************************************************* */

// Authors: Yoshua Bengio

/*! \file OnlineLearningModule.cc */


#include "OnlineLearningModule.h"

namespace PLearn {
using namespace std;

PLEARN_IMPLEMENT_ABSTRACT_OBJECT(
    OnlineLearningModule,
    "Learn to map inputs to outputs, online, using caller-provided gradients.",
    "This pure virtual class (i.e. an interface) can basically do two things:\n"
    "  * map an input to an output\n"
    "  * modify itself when told in what direction the output should have \n"
    "    changed (i.e. output gradient),  while optionally giving back the \n"
    "    information about how the input should also have changed (i.e. input gradient)\n"
    );

OnlineLearningModule::OnlineLearningModule() :
    input_size(-1),
    output_size(-1)
{
}

//! Default method for bbpropUpdate functions, so that it compiles but crashes
//! if not implemented but used.
void OnlineLearningModule::bbpropUpdate(const Vec input, const Vec output,
                                        const Vec output_gradient,
                                        const Vec output_diag_hessian)
{
    PLERROR("In OnlineLearningModule.cc: method 'bbpropUpdate' not"
            "implemented.\n"
            "Please implement it in your derived class, or use"
            "'bpropUpdate'.\n");
}

void OnlineLearningModule::bbpropUpdate(const Vec input, const Vec output,
                                        const Vec output_gradient,
                                        Vec& input_gradient,
                                        const Vec output_diag_hessian,
                                        Vec& input_diag_hessian)
{
    PLERROR("In OnlineLearningModule.cc: method 'bbpropUpdate' not"
            "implemented.\n"
            "Please implement it in your derived class, or use"
            "'bpropUpdate'.\n");
}


void OnlineLearningModule::build()
{
    inherited::build();
    build_();
}

void OnlineLearningModule::makeDeepCopyFromShallowCopy(CopiesMap& copies)
{
    inherited::makeDeepCopyFromShallowCopy(copies);
}

void OnlineLearningModule::declareOptions(OptionList& ol)
{
    declareOption(ol, "input_size", &OnlineLearningModule::input_size,
                  OptionBase::buildoption,
                  "Size of the input Hessian)");

    declareOption(ol, "output_size", &OnlineLearningModule::output_size,
                  OptionBase::buildoption,
                  "Size of the output");

    inherited::declareOptions(ol);
}

void OnlineLearningModule::build_()
{
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
