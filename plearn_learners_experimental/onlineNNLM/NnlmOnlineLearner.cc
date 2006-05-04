// -*- C++ -*-

// NnlmOnlineLearner.cc
//
// Copyright (C) 2006 Pierre-Antoine Manzagol
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

// Authors: Pierre-Antoine Manzagol

/*! \file NnlmOnlineLearner.cc */


#include "NnlmOnlineLearner.h"

namespace PLearn {
using namespace std;

PLEARN_IMPLEMENT_OBJECT(
    NnlmOnlineLearner,
    "Trains a Naive Bayes Neural Network Language Model (NBNNLM).",
    "MULTI-LINE \nHELP");

NnlmOnlineLearner::NnlmOnlineLearner()
    :   PLearner()
{
    // ### You may (or not) want to call build_() to finish building the object
    // ### (doing so assumes the parent classes' build_() have been called too
    // ### in the parent classes' constructors, something that you must ensure)

    // ### If this learner needs to generate random numbers, uncomment the
    // ### line below to enable the use of the inherited PRandom object.
    // random_gen = new PRandom();
}

void NnlmOnlineLearner::declareOptions(OptionList& ol)
{

    declareOption(ol, "modules", &NnlmOnlineLearner::modules,
                  OptionBase::buildoption,
                  "Layers of the learner");

    declareOption(ol, "output_module", &NnlmOnlineLearner::output_module,
                  OptionBase::buildoption,
                  "Output layer");


    // ### Declare all of this object's options here.
    // ### For the "flags" of each option, you should typically specify
    // ### one of OptionBase::buildoption, OptionBase::learntoption or
    // ### OptionBase::tuningoption. If you don't provide one of these three,
    // ### this option will be ignored when loading values from a script.
    // ### You can also combine flags, for example with OptionBase::nosave:
    // ### (OptionBase::buildoption | OptionBase::nosave)

    // Now call the parent class' declareOptions
    inherited::declareOptions(ol);
}

void NnlmOnlineLearner::build_()
{
    // ### This method should do the real building of the object,
    // ### according to set 'options', in *any* situation.
    // ### Typical situations include:
    // ###  - Initial building of an object from a few user-specified options
    // ###  - Building of a "reloaded" object: i.e. from the complete set of
    // ###    all serialised options.
    // ###  - Updating or "re-building" of an object after a few "tuning"
    // ###    options have been modified.
    // ### You should assume that the parent class' build_() has already been
    // ### called.

    inherited::build();
    build_();
}

// ### Nothing to add here, simply calls build_
void NnlmOnlineLearner::build()
{
    inherited::build();
    build_();
}


void NnlmOnlineLearner::makeDeepCopyFromShallowCopy(CopiesMap& copies)
{
    inherited::makeDeepCopyFromShallowCopy(copies);

    // ### Call deepCopyField on all "pointer-like" fields
    // ### that you wish to be deepCopied rather than
    // ### shallow-copied.
    // ### ex:
    // deepCopyField(trainvec, copies);

    // ### Remove this line when you have fully implemented this method.
    PLERROR("NnlmOnlineLearner::makeDeepCopyFromShallowCopy not fully (correctly) implemented yet!");
}


int NnlmOnlineLearner::outputsize() const
{
    // Compute and return the size of this learner's output (which typically
    // may depend on its inputsize(), targetsize() and set options).
}

void NnlmOnlineLearner::forget()
{
    //! (Re-)initialize the PLearner in its fresh state (that state may depend
    //! on the 'seed' option) and sets 'stage' back to 0 (this is the stage of
    //! a fresh learner!)
    /*!
      A typical forget() method should do the following:
      - call inherited::forget() to initialize its random number generator
        with the 'seed' option
      - initialize the learner's parameters, using this random generator
      - stage = 0
    */
    inherited::forget();
}

void NnlmOnlineLearner::train()
{
    // The role of the train method is to bring the learner up to
    // stage==nstages, updating train_stats with training costs measured
    // on-line in the process.

    /* TYPICAL CODE:

    static Vec input;  // static so we don't reallocate memory each time...
    static Vec target; // (but be careful that static means shared!)
    input.resize(inputsize());    // the train_set's inputsize()
    target.resize(targetsize());  // the train_set's targetsize()
    real weight;

    // This generic PLearner method does a number of standard stuff useful for
    // (almost) any learner, and return 'false' if no training should take
    // place. See PLearner.h for more details.
    if (!initTrain())
        return;

    while(stage<nstages)
    {
        // clear statistics of previous epoch
        train_stats->forget();

        //... train for 1 stage, and update train_stats,
        // using train_set->getExample(input, target, weight)
        // and train_stats->update(train_costs)

        ++stage;
        train_stats->finalize(); // finalize statistics for this epoch
    }
    */
}


void NnlmOnlineLearner::computeOutput(const Vec& input, Vec& output) const
{
    // Compute the output from the input.
    // int nout = outputsize();
    // output.resize(nout);
    // ...
}

void NnlmOnlineLearner::computeCostsFromOutputs(const Vec& input, const Vec& output,
                                           const Vec& target, Vec& costs) const
{
// Compute the costs from *already* computed output.
// ...
}

TVec<string> NnlmOnlineLearner::getTestCostNames() const
{
    // Return the names of the costs computed by computeCostsFromOutputs
    // (these may or may not be exactly the same as what's returned by
    // getTrainCostNames).
    // ...
}

TVec<string> NnlmOnlineLearner::getTrainCostNames() const
{
    // Return the names of the objective costs that the train method computes
    // and for which it updates the VecStatsCollector train_stats
    // (these may or may not be exactly the same as what's returned by
    // getTestCostNames).
    // ...
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
