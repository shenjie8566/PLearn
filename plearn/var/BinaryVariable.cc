// -*- C++ -*-

// PLearn (A C++ Machine Learning Library)
// Copyright (C) 1998 Pascal Vincent
// Copyright (C) 1999-2002 Pascal Vincent, Yoshua Bengio, Rejean Ducharme and University of Montreal
// Copyright (C) 2001-2002 Nicolas Chapados, Ichiro Takeuchi, Jean-Sebastien Senecal
// Copyright (C) 2002 Xiangdong Wang, Christian Dorion

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
 * This file is part of the PLearn library.
 ******************************************************* */

#include "BinaryVariable.h"

namespace PLearn {

using namespace std;

PLEARN_IMPLEMENT_ABSTRACT_OBJECT(
    BinaryVariable,
    "Variable that takes two other variables as input.",
    ""
);

////////////////////
// BinaryVariable //
////////////////////
BinaryVariable::BinaryVariable(Variable* v1, Variable* v2, int thelength,
                               int thewidth, bool call_build_):
    inherited(thelength, thewidth, call_build_), 
    input1(v1),
    input2(v2) 
{
    input1->disallowPartialUpdates();
    input2->disallowPartialUpdates();
    if (call_build_)
        build_();
}

////////////////////
// declareOptions //
////////////////////
void BinaryVariable::declareOptions(OptionList& ol)
{
    declareOption(ol, "input1", &BinaryVariable::input1, OptionBase::buildoption, 
                  "The first parent variable that this one depends on\n");

    declareOption(ol, "input2", &BinaryVariable::input2, OptionBase::buildoption, 
                  "The second parent variable that this one depends on\n");

    inherited::declareOptions(ol);
}

///////////
// build //
///////////
void BinaryVariable::build()
{
    inherited::build();
    build_();
}

////////////
// build_ //
////////////
void BinaryVariable::build_() {}

void BinaryVariable::setParents(const VarArray& parents)
{
    if(parents.length() != 2)
        PLERROR("In BinaryVariable::setParents  VarArray length must be 2;"
                " you are trying to set %d parents for this BinaryVariable...", parents.length());

    input1= parents[0];
    input2= parents[1];
  
    int dummy_l, dummy_w;
    recomputeSize(dummy_l, dummy_w);
}

#ifdef __INTEL_COMPILER
#pragma warning(disable:1419)  // Get rid of compiler warning.
#endif
extern void varDeepCopyField(Var& field, CopiesMap& copies);
#ifdef __INTEL_COMPILER
#pragma warning(default:1419)
#endif

void BinaryVariable::makeDeepCopyFromShallowCopy(CopiesMap& copies)
{
    Variable::makeDeepCopyFromShallowCopy(copies);
    //deepCopyField(input1, copies);
    varDeepCopyField(input1, copies);
    //deepCopyField(input2, copies);
    varDeepCopyField(input2, copies);
}


bool BinaryVariable::markPath()
{
    if(!marked)
        marked = input1->markPath() | input2->markPath();
    return marked;
}


void BinaryVariable::buildPath(VarArray& proppath)
{
    if(marked)
    {
        input1->buildPath(proppath);
        input2->buildPath(proppath);
        proppath &= Var(this);
        //cout<<"proppath="<<this->getName()<<endl;
        clearMark();
    }
}


VarArray BinaryVariable::sources() 
{ 
    if (marked)
        return VarArray(0,0);
    marked = true;
    return input1->sources() & input2->sources(); 
}


VarArray BinaryVariable::random_sources() 
{ 
    if (marked)
        return VarArray(0,0);
    marked = true;
    return input1->random_sources() & input2->random_sources(); 
}


VarArray BinaryVariable::ancestors() 
{ 
    if (marked)
        return VarArray(0,0);
    marked = true;
    return input1->ancestors() & input2->ancestors() & Var(this) ;
}


void BinaryVariable::unmarkAncestors()
{ 
    if (marked)
    {
        marked = false;
        input1->unmarkAncestors();
        input2->unmarkAncestors();
    }
}


VarArray BinaryVariable::parents() 
{ 
    VarArray unmarked_parents;
    if (!input1->marked)
        unmarked_parents.append(input1);
    if (!input2->marked)
        unmarked_parents.append(input2);
    return unmarked_parents;
}


void BinaryVariable::resizeRValue()
{
    inherited::resizeRValue();
    if (!input1->rvaluedata) input1->resizeRValue();
    if (!input2->rvaluedata) input2->resizeRValue();
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
