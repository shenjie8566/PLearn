// -*- C++ -*-

// RBMConv2DLLParameters.h
//
// Copyright (C) 2006 Pascal Lamblin
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

// Authors: Pascal Lamblin

/*! \file PLearn/plearn_learners/online/DEPRECATED/RBMConv2DLLParameters.h */


#ifndef RBMConv2DLLParameters_INC
#define RBMConv2DLLParameters_INC

#include "RBMParameters.h"

namespace PLearn {
using namespace std;


/**
 * Filter between two linear layers of a 2D convolutional RBM.
 *
 * @todo: yes
 * @deprecated Use ../RBMConv2DConnection.h instead
 */
class RBMConv2DLLParameters: public RBMParameters
{
    typedef RBMParameters inherited;

public:
    //#####  Public Build Options  ############################################

    //! Momentum factor
    real momentum;

    //! Length of the down image
    int down_image_length;

    //! Width of the down image
    int down_image_width;

    //! Length of the up image
    int up_image_length;

    //! Width of the up image
    int up_image_width;

    //! "Vertical" step
    int kernel_step1;

    //! "Horizontal" step
    int kernel_step2;

    //#####  Learned Options  #################################################

    //! Matrix containing the convolution kernel (filter)
    Mat kernel;

    //! Element i contains the bias of up unit i
    Vec up_units_bias;

    //! Element i contains the bias of down unit i
    Vec down_units_bias;

    //#####  Not Options  #####################################################

    //! Length of the kernel
    int kernel_length;

    //! Width of the kernel
    int kernel_width;

    //! Accumulates positive contribution to the weights' gradient
    Mat kernel_pos_stats;

    //! Accumulates negative contribution to the weights' gradient
    Mat kernel_neg_stats;

    //! Accumulates positive contribution to the gradient of up_units_bias
    Vec up_units_bias_pos_stats;
    //! Accumulates negative contribution to the gradient of up_units_bias
    Vec up_units_bias_neg_stats;
    //! Accumulates positive contribution to the gradient of down_units_bias
    Vec down_units_bias_pos_stats;
    //! Accumulates negative contribution to the gradient of down_units_bias
    Vec down_units_bias_neg_stats;

    //! Used if momentum != 0.
    Mat kernel_inc;
    Vec down_units_bias_inc;
    Vec up_units_bias_inc;

public:
    //#####  Public Member Functions  #########################################

    //! Default constructor
    RBMConv2DLLParameters( real the_learning_rate=0 );

    //! Constructor from two string prototymes
    RBMConv2DLLParameters( string down_types, string up_types,
                     real the_learning_rate=0 );

    // Your other public member functions go here

    //! Accumulates positive phase statistics to *_pos_stats
    virtual void accumulatePosStats( const Vec& down_values,
                                     const Vec& up_values );

    //! Accumulates negative phase statistics to *_neg_stats
    virtual void accumulateNegStats( const Vec& down_values,
                                     const Vec& up_values );

    //! Updates parameters according to contrastive divergence gradient
    virtual void update();

    //! Updates parameters according to contrastive divergence gradient,
    //! not using the statistics but the explicit values passed
    virtual void update( const Vec& pos_down_values,
                         const Vec& pos_up_values,
                         const Vec& neg_down_values,
                         const Vec& neg_up_values );

    //! Clear all information accumulated during stats
    virtual void clearStats();

    //! Computes the vectors of activation of "length" units,
    //! starting from "start", and concatenates them into "activations".
    //! "start" indexes an up unit if "going_up", else a down unit.
    virtual void computeUnitActivations( int start, int length,
                                         const Vec& activations ) const;

    //! Adapt based on the output gradient: this method should only
    //! be called just after a corresponding fprop; it should be
    //! called with the same arguments as fprop for the first two arguments
    //! (and output should not have been modified since then).
    //! Since sub-classes are supposed to learn ONLINE, the object
    //! is 'ready-to-be-used' just after any bpropUpdate.
    //! N.B. A DEFAULT IMPLEMENTATION IS PROVIDED IN THE SUPER-CLASS, WHICH
    //! JUST CALLS
    //!     bpropUpdate(input, output, input_gradient, output_gradient)
    //! AND IGNORES INPUT GRADIENT.
    // virtual void bpropUpdate(const Vec& input, const Vec& output,
    //                          const Vec& output_gradient);

    //! this version allows to obtain the input gradient as well
    //! N.B. THE DEFAULT IMPLEMENTATION IN SUPER-CLASS JUST RAISES A PLERROR.
    virtual void bpropUpdate(const Vec& input, const Vec& output,
                             Vec& input_gradient,
                             const Vec& output_gradient);

    //! reset the parameters to the state they would be BEFORE starting
    //! training.  Note that this method is necessarily called from
    //! build().
    virtual void forget();

    //! optionally perform some processing after training, or after a
    //! series of fprop/bpropUpdate calls to prepare the model for truly
    //! out-of-sample operation.  THE DEFAULT IMPLEMENTATION PROVIDED IN
    //! THE SUPER-CLASS DOES NOT DO ANYTHING.
    // virtual void finalize();

    //! return the number of parameters
    virtual int nParameters(bool share_up_params, bool share_down_params) const;

    //! Make the parameters data be sub-vectors of the given global_parameters.
    //! The argument should have size >= nParameters. The result is a Vec
    //! that starts just after this object's parameters end, i.e.
    //!    result = global_parameters.subVec(nParameters(),global_parameters.size()-nParameters());
    //! This allows to easily chain calls of this method on multiple RBMParameters.
    virtual Vec makeParametersPointHere(const Vec& global_parameters, bool share_up_params, bool share_down_params);

    //#####  PLearn::Object Protocol  #########################################

    // Declares other standard object methods.
    PLEARN_DECLARE_OBJECT(RBMConv2DLLParameters);

    // Simply calls inherited::build() then build_()
    virtual void build();

    //! Transforms a shallow copy into a deep copy
    virtual void makeDeepCopyFromShallowCopy(CopiesMap& copies);


protected:
    //#####  Protected Member Functions  ######################################
    //! Declares the class options.
    static void declareOptions(OptionList& ol);

private:
    //#####  Private Member Functions  ########################################

    //! This does the actual building.
    void build_();

private:
    //#####  Private Data Members  ############################################

    // The rest of the private stuff goes here
    mutable Mat down_image;
    mutable Mat up_image;
    mutable Mat down_image_gradient;
    mutable Mat up_image_gradient;
    Mat kernel_gradient;
};

// Declares a few other classes and functions related to this class
DECLARE_OBJECT_PTR(RBMConv2DLLParameters);

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
