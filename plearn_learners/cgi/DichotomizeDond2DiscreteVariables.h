// -*- C++ -*-

// DichotomizeDond2DiscreteVariables.h
//
// Copyright (C) 2006 Dan Popovici
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

// Authors: Dan Popovici

/*! \file DichotomizeDond2DiscreteVariables.h */


#ifndef DichotomizeDond2DiscreteVariables_INC
#define DichotomizeDond2DiscreteVariables_INC

#include <plearn_learners/generic/PLearner.h>
#include <plearn/vmat/FileVMatrix.h>

namespace PLearn {

/**
 * Generate samples from a mixture of two gaussians
 *
 */
class DichotomizeDond2DiscreteVariables : public PLearner
{
    typedef PLearner inherited;

public:

    //#####  Public Build Options  ############################################

    //! ### declare public option fields (such as build options) here
    //! Start your comments with Doxygen-compatible comments such as //!

    //! The instructions to fix the binary variables in the form of field_name : instruction.
    //! Supported instructions are 9_is_one, not_0_is_one, not_missing_is_one, not_1000_is_one.
    //! Variables with no specification will be kept as_is.
    TVec< pair<string, TVec< pair<real, real> > > > discrete_variable_instructions;
    
    //! The file path for the fixed output file.
    string output_path;

public:
    //#####  Public Member Functions  #########################################

    //! Default constructor
    // ### Make sure the implementation in the .cc
    // ### initializes all fields to reasonable default values.
    DichotomizeDond2DiscreteVariables();
    int outputsize() const;
    void train();
    void computeOutput(const Vec&, Vec&) const;
    void computeCostsFromOutputs(const Vec&, const Vec&, const Vec&, Vec&) const;
    TVec<string> getTestCostNames() const;
    TVec<string> getTrainCostNames() const;
    VMat getOutputFile();


    //#####  PLearn::Object Protocol  #########################################

    // Declares other standard object methods.
    // ### If your class is not instantiatable (it has pure virtual methods)
    // ### you should replace this by PLEARN_DECLARE_ABSTRACT_OBJECT_METHODS
    PLEARN_DECLARE_OBJECT(DichotomizeDond2DiscreteVariables);

    // Simply calls inherited::build() then build_()
    virtual void build();

    //! Transforms a shallow copy into a deep copy
    // (PLEASE IMPLEMENT IN .cc)
    virtual void makeDeepCopyFromShallowCopy(CopiesMap& copies);    

protected:
    //#####  Protected Member Functions  ######################################

    //! Declares the class options.
    static void declareOptions(OptionList& ol);

private:
    //#####  Private Member Functions  ########################################

    //! This does the actual building.
    void build_();
    void dichotomizeDiscreteVariables();

private:
    //#####  Private Data Members  ############################################

    // The rest of the private stuff goes here
    
    // output dataset variables
    VMat output_file;
};

// Declares a few other classes and functions related to this class
DECLARE_OBJECT_PTR(DichotomizeDond2DiscreteVariables);

} // end of namespace PLearn

#endif


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
