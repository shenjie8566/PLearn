// -*- C++ -*-

// PLearn (A C++ Machine Learning Library)
// Copyright (C) 1998 Pascal Vincent
// Copyright (C) 1999-2002 Pascal Vincent, Yoshua Bengio and University of Montreal
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
   * $Id: SampleVariable.h,v 1.2 2002/10/23 23:32:33 dorionc Exp $
   * AUTHORS: Pascal Vincent & Yoshua Bengio
   * This file is part of the PLearn library.
   ******************************************************* */


/*! \file PLearnLibrary/PLearnCore/SampleVariable.h */

#ifndef SampleVariable_INC
#define SampleVariable_INC

#include "Var_all.h"
#include "random.h"
#include "VarArray.h"

namespace PLearn <%
using namespace std;


class SourceSampleVariable: public SourceVariable
{
 public:
  SourceSampleVariable(int length, int width)
    :SourceVariable(length, width) {}
  virtual SourceSampleVariable* deepCopy(map<const void*, void*>& copies) const =0;
  virtual string classname() const;
  virtual void bprop() { }
  VarArray random_sources();
};

class UnarySampleVariable: public UnaryVariable
{
 public:
  UnarySampleVariable(Variable* input, int length, int width)
    :UnaryVariable(input,length,width) {}
  virtual string classname() const;
  virtual void bprop() { }
  VarArray random_sources();
};

class BinarySampleVariable: public BinaryVariable
{
 public:
  BinarySampleVariable(Variable* input1, Variable* input2, int length, int width)
    :BinaryVariable(input1,input2,length,width) { }
  virtual string classname() const;
  virtual void bprop() { }
  VarArray random_sources();
};

class UniformSampleVariable: public SourceSampleVariable
{
 protected:
  real min_value, max_value;
  char name[50];

 public:
  UniformSampleVariable(int length=1, int width=1,
                        real minvalue=0, real maxvalue=1);
  virtual UniformSampleVariable* deepCopy(map<const void*, void*>& copies) const;
  virtual string classname() const;
  virtual void fprop();
};

class MultinomialSampleVariable: public UnarySampleVariable
{
 protected:
  char name[50];

 public:
  MultinomialSampleVariable(Variable* probabilities, int length=1, int width=1);
  virtual MultinomialSampleVariable* deepCopy(map<const void*, void*>& copies) const;
  virtual string classname() const;
  virtual void fprop();
};

class DiagonalNormalSampleVariable: public BinarySampleVariable
{
 public:
  DiagonalNormalSampleVariable(Variable* mu, Variable* sigma);
  virtual DiagonalNormalSampleVariable* deepCopy(map<const void*, void*>& copies) const;
  virtual string classname() const;
  virtual void fprop();
};

%> // end of namespace PLearn

#endif
