// -*- C++ -*-

// HeterogenuousAffineTransformWeightPenalty.h
//
// Copyright (C) 2006 Hugo Larochelle
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

// Authors: Hugo Larochelle

/*! \file HeterogenuousAffineTransformWeightPenalty.h */


#ifndef HeterogenuousAffineTransformWeightPenalty_INC
#define HeterogenuousAffineTransformWeightPenalty_INC

#include <plearn/var/NaryVariable.h>

namespace PLearn {
using namespace std;

/*! * HeterogenuousAffineTransformWeightPenalty * */

/**
 * Penalty associated to an affine transform with continuous and discrete input
 */
class HeterogenuousAffineTransformWeightPenalty : public NaryVariable
{
    typedef NaryVariable inherited;

public:
    //#####  Public Build Options  ############################################

    //! Indication whether the input components are discrete
    TVec<bool> input_is_discrete;
    //! Weight decay parameter
    real weight_decay_;
    //! Bias decay parameter
    real bias_decay_;
    //! Type of weight decay penalty
    string penalty_type_;

public:
    //#####  Public Member Functions  #########################################

    //! Default constructor, usually does nothing
    HeterogenuousAffineTransformWeightPenalty();

    //! Constructor initializing from input variables

    // HeterogenuousAffineTransformWeightPenalty(const VarArray& vararray);
    HeterogenuousAffineTransformWeightPenalty(Var input, VarArray weights, TVec<bool> the_input_is_discrete, real weight_decay, real bias_decay=0, string penalty_type="L2_square");
    
    // Your other public member functions go here

    //#####  PLearn::Variable methods #########################################
    // (PLEASE IMPLEMENT IN .cc)
    virtual void recomputeSize(int& l, int& w) const;
    virtual void fprop();
    virtual void bprop();


    //#####  PLearn::Object Protocol  #########################################

    // Declares other standard object methods.
    PLEARN_DECLARE_OBJECT(HeterogenuousAffineTransformWeightPenalty);

    // Simply calls inherited::build() then build_()
    virtual void build();

    //! Transforms a shallow copy into a deep copy
    virtual void makeDeepCopyFromShallowCopy(CopiesMap& copies);

protected:
    //#####  Protected Member Functions  ######################################

    //! Declares the class options.
    // (PLEASE IMPLEMENT IN .cc)
    static void declareOptions(OptionList& ol);

private:
    //#####  Private Member Functions  ########################################

    //! This does the actual building.
    void build_();

private:
    //#####  Private Data Members  ############################################

};

// Declares a few other classes and functions related to this class
DECLARE_OBJECT_PTR(HeterogenuousAffineTransformWeightPenalty);

inline Var heterogenuous_affine_transform_weight_penalty(Var input, VarArray weights, TVec<bool> the_input_is_discrete, real weight_decay, real bias_decay=0, string penalty_type="L2_square")
{ return new HeterogenuousAffineTransformWeightPenalty(input, weights, the_input_is_discrete, weight_decay, bias_decay, penalty_type); }

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
