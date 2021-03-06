// -*- C++ -*-

// StatefulLearner.cc
//
// Copyright (C) 2004 R�jean Ducharme 
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
 ******************************************************* */

// Authors: R�jean Ducharme

/*! \file StatefulLearner.cc */


#include "StatefulLearner.h"

namespace PLearn {
using namespace std;

StatefulLearner::StatefulLearner()
    : current_test_t(-1), test_start_time(-1)
{}

PLEARN_IMPLEMENT_ABSTRACT_OBJECT(
    StatefulLearner,
    "PLearner with an internal state",
    "PLearner with an internal state.\n"
    "It replaces, for efficiency and compatibility reasons, SequentialLearner.");

void StatefulLearner::declareOptions(OptionList& ol)
{
    declareOption(
        ol, "test_start_time", &StatefulLearner::test_start_time,
        OptionBase::buildoption,
        "Initial time at which testing should be started.");

    inherited::declareOptions(ol);
}

void StatefulLearner::build_()
{}

// ### Nothing to add here, simply calls build_
void StatefulLearner::build()
{
    inherited::build();
    build_();
}

void StatefulLearner::makeDeepCopyFromShallowCopy(CopiesMap& copies)
{
    inherited::makeDeepCopyFromShallowCopy(copies);
}

void StatefulLearner::forget()
{ /* empty */ }

void StatefulLearner::resetInternalState()
{
    current_test_t = test_start_time;
}

void StatefulLearner::computeOutput(const Vec& input, Vec& output) const
{
    // PLWARNING("You called StatefulLearner::computeOutput(...), are you sure you don't want to use computeOutputAndCosts(...) instead???");

    // These cannot be static because of potential re-entrancy problem upon
    // recursive calls
    Vec tmp_target(targetsize());
    Vec tmp_costs(nTestCosts());
    tmp_target.resize(targetsize());
    tmp_costs.resize(nTestCosts());
    computeOutputAndCosts(input, tmp_target, output, tmp_costs);
}

void StatefulLearner::computeCostsFromOutputs(const Vec& input, const Vec& output, 
                                              const Vec& target, Vec& costs) const
{
    PLERROR("The method computeCostsFromOutputs is not defined and has no meaning for a StatefulLearner");
}                                

void StatefulLearner::computeCostsOnly(const Vec& input, const Vec& target, Vec& costs) const
{
    PLWARNING("You called StatefulLearner::computeCostsOnly(...), are you sure you don't want to use computeOutputAndCosts(...) instead???");

    static Vec tmp_output;
    tmp_output.resize(outputsize());
    computeOutputAndCosts(input, target, tmp_output, costs);
}

void StatefulLearner::setTrainingSet(VMat training_set, bool call_forget)
{
    train_set   = training_set;
    inputsize_  = train_set->inputsize();
    targetsize_ = train_set->targetsize();
    weightsize_ = train_set->weightsize();

    if ( call_forget )
        forget();
}

void StatefulLearner::setTestSet(VMat testset)
{}

bool StatefulLearner::isStatefulLearner() const
{
    return true;
}

void StatefulLearner::setTestStartTime(int t)
{
    test_start_time = t;
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
