// -*- C++ -*-

// TorchKernelFromKernel.h
//
// Copyright (C) 2005 Olivier Delalleau 
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
   * $Id: TorchKernelFromKernel.h,v 1.1 2005/02/24 15:33:33 tihocan Exp $ 
   ******************************************************* */

// Authors: Olivier Delalleau

/*! \file TorchKernelFromKernel.h */


#ifndef TorchKernelFromKernel_INC
#define TorchKernelFromKernel_INC

#include <plearn/ker/Kernel.h>
#include <torch/Kernel.h>

namespace Torch {

/** Turns a PLearn kernel into a Torch kernel **/
class TorchKernelFromKernel : public Kernel
{

protected:

  //! PLearn vectors to store inputs to the kernel.
  PLearn::Vec x_vec;
  PLearn::Vec y_vec;

public:

  //! The underlying PLearn kernel.
  PLearn::Ker kernel;

  //! Build a Torch::Kernel object from a PLearn::Kernel.
  TorchKernelFromKernel(PLearn::Ker ker);

  virtual real eval(Sequence *x, Sequence *y);

  virtual ~TorchKernelFromKernel();
};

} // end of namespace Torch

#endif
