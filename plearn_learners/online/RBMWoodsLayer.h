// -*- C++ -*-

// RBMWoodsLayer.h
//
// Copyright (C) 2008 Hugo Larochelle
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

/*! \file RBMWoodsLayer.h */


#ifndef RBMWoodsLayer_INC
#define RBMWoodsLayer_INC

#include "RBMLayer.h"

namespace PLearn {
using namespace std;

/**
 * RBM layer with tree-structured groups of units
 *
 */
class RBMWoodsLayer: public RBMLayer
{
    typedef RBMLayer inherited;

public:
    //#####  Public Build Options  ############################################

    // Number of trees in the woods
    int n_trees;

    // Depth of the trees in the woods (1 gives the ordinary RBMBinomialLayer)
    int tree_depth;

public:
    //#####  Public Member Functions  #########################################

    //! Default constructor
    RBMWoodsLayer( real the_learning_rate=0. );

    //! generate a sample, and update the sample field
    virtual void generateSample() ;

    //! Inherited.
    virtual void generateSamples();

    virtual void computeProbabilisticClustering(Vec& prob_clusters);

    //! Compute marginal expectations of all units
    virtual void computeExpectation() ;

    //! Compute marginal mini-batch expectations of all units.
    virtual void computeExpectations();

    //! forward propagation
    virtual void fprop( const Vec& input, Vec& output ) const;

    //! Batch forward propagation
    virtual void fprop( const Mat& inputs, Mat& outputs ) const;

    //! forward propagation with provided bias
    virtual void fprop( const Vec& input, const Vec& rbm_bias,
                        Vec& output ) const;

    //! back-propagates the output gradient to the input
    virtual void bpropUpdate(const Vec& input, const Vec& output,
                             Vec& input_gradient, const Vec& output_gradient,
                             bool accumulate=false);

    //! back-propagates the output gradient to the input and the bias
    virtual void bpropUpdate(const Vec& input, const Vec& rbm_bias,
                             const Vec& output,
                             Vec& input_gradient, Vec& rbm_bias_gradient,
                             const Vec& output_gradient) ;

    //! Back-propagate the output gradient to the input, and update parameters.
    virtual void bpropUpdate(const Mat& inputs, const Mat& outputs,
                             Mat& input_gradients,
                             const Mat& output_gradients,
                             bool accumulate = false);

    //! Computes the negative log-likelihood of target given the
    //! internal activations of the layer
    virtual real fpropNLL(const Vec& target);
    virtual void fpropNLL(const Mat& targets, const Mat& costs_column);

    //! Computes the gradient of the negative log-likelihood of target
    //! with respect to the layer's bias, given the internal activations
    virtual void bpropNLL(const Vec& target, real nll, Vec& bias_gradient);
    virtual void bpropNLL(const Mat& targets, const Mat& costs_column,
                          Mat& bias_gradients);

    //! compute -bias' unit_values
    virtual real energy(const Vec& unit_values) const;

    //! Computes -log(\sum_{possible values of h} exp(h' unit_activations))
    //! This quantity is used for computing the free energy of a sample x in
    //! the OTHER layer of an RBM, from which unit_activations was computed.
    virtual real freeEnergyContribution(const Vec& unit_activations) const;

    virtual int getConfigurationCount();

    virtual void getConfiguration(int conf_index, Vec& output);

    //#####  PLearn::Object Protocol  #########################################

    // Declares other standard object methods.
    PLEARN_DECLARE_OBJECT(RBMWoodsLayer);

    // Simply calls inherited::build() then build_()
    virtual void build();

    //! Transforms a shallow copy into a deep copy
    virtual void makeDeepCopyFromShallowCopy(CopiesMap& copies);

protected:
    //#####  Not Options  #####################################################

    // Probability that sampling reaches a node but samples h=0 (expectation is for h=1)
    Vec off_expectation;
    // Ordinary RBMBinomialLayer expectation
    Vec local_node_expectation;

    // Computations of the local_node_expectation free energies for h = 1
    Vec on_free_energy;
    // Computations of the local_node_expectation free energies for h = 0
    Vec off_free_energy;

    // Gradient through the local_node_expectations (after sigmoid)
    Vec local_node_expectation_gradient;
    // Gradient through the tree structure
    Vec on_tree_gradient;
    Vec off_tree_gradient;
    Vec on_free_energy_gradient;
    Vec off_free_energy_gradient;

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
};

// Declares a few other classes and functions related to this class
DECLARE_OBJECT_PTR(RBMWoodsLayer);

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