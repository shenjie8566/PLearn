// -*- C++ -*-

// SquaredExponentialARDKernel.h
//
// Copyright (C) 2006 Nicolas Chapados
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

// Authors: Nicolas Chapados

/*! \file SquaredExponentialARDKernel.h */


#ifndef SquaredExponentialARDKernel_INC
#define SquaredExponentialARDKernel_INC

#include <plearn/ker/ARDBaseKernel.h>

namespace PLearn {

/**
 *  Squared-Exponential kernel that can be used for Automatic Relevance
 *  Determination
 *
 *  This is a variant of the GaussianKernel (a.k.a. Radial Basis Function)
 *  that provides a different length-scale parameter for each input variable.
 *  When used in conjunction with GaussianProcessRegressor, this kernel may be
 *  used for Automatic Relevance Determination (ARD), a procedure wherein the
 *  significance of each input variable for the prediction task is found
 *  automatically through numerical optimization.
 *
 *  Similar to C.E. Rasmussen's GPML code (see http://www.gaussianprocess.org),
 *  this kernel function is specified as:
 *
 *    k(x,y) = sf2 * exp(- 0.5 * (sum_i (x_i - y_i)^2 / w_i)) + delta_x,y*sn2
 *
 *  where sf2 is the exp of the 'log_signal_sigma' option, sn2 is the exp of
 *  the 'log_noise_sigma' option (added only if x==y), and w_i is
 *  exp(log_global_sigma + log_input_sigma[i]).
 *
 *  Note that to make its operations more robust when used with unconstrained
 *  optimization of hyperparameters, all hyperparameters of this kernel are
 *  specified in the log-domain.
 */
class SquaredExponentialARDKernel : public ARDBaseKernel
{
    typedef ARDBaseKernel inherited;

public:
    //#####  Public Build Options  ############################################

    // (No new options other than those inherited)
    
public:
    //#####  Public Member Functions  #########################################

    //! Default constructor
    SquaredExponentialARDKernel();


    //#####  Kernel Member Functions  #########################################

    //! Compute K(x1,x2).
    virtual real evaluate(const Vec& x1, const Vec& x2) const;

    //! Directly compute the derivative with respect to hyperparameters
    //! (Faster than finite differences...)
    // virtual void computeGramMatrixDerivative(Mat& KD, const string& kernel_param,
    //                                          real epsilon=1e-6) const;
    

    //#####  PLearn::Object Protocol  #########################################

    // Declares other standard object methods.
    PLEARN_DECLARE_OBJECT(SquaredExponentialARDKernel);

    // Simply calls inherited::build() then build_()
    virtual void build();

    //! Transforms a shallow copy into a deep copy
    virtual void makeDeepCopyFromShallowCopy(CopiesMap& copies);

protected:
    //! Declares the class options.
    static void declareOptions(OptionList& ol);

private:
    //! This does the actual building.
    void build_();
};

// Declares a few other classes and functions related to this class
DECLARE_OBJECT_PTR(SquaredExponentialARDKernel);

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