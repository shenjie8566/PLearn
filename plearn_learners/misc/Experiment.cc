// -*- C++ -*-

// Experiment.cc
// 
// Copyright (C) 2002 Pascal Vincent, Frederic Morin
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
   * $Id: Experiment.cc,v 1.6 2002/12/02 08:46:51 plearner Exp $ 
   ******************************************************* */

/*! \file Experiment.cc */
#include "Experiment.h"
#include "pl_io.h"
#include "VecStatsCollector.h"

namespace PLearn <%
using namespace std;

Experiment::Experiment() 
  :save_models(true)
  {}

  IMPLEMENT_NAME_AND_DEEPCOPY(Experiment);

  void Experiment::declareOptions(OptionList& ol)
  {
    declareOption(ol, "expdir", &Experiment::expdir, OptionBase::buildoption,
                  "Path of this experiment's directory in which to save all experiment results (will be created if it does not already exist)");
    declareOption(ol, "learner", &Experiment::learner, OptionBase::buildoption,
                  "The learner to train/test");
    declareOption(ol, "dataset", &Experiment::dataset, OptionBase::buildoption,
                  "The dataset to use for training/testing (will be split according to what is specified in the testmethod)\n"
                  "You may omit this only if your splitter is an ExplicitSplitter");
    declareOption(ol, "splitter", &Experiment::splitter, OptionBase::buildoption,
                  "The splitter to use to generate one or several train/test pairs from the dataset.");
    declareOption(ol, "save_models", &Experiment::save_models, OptionBase::buildoption,
                  "If false, the models won't be saved.");
    inherited::declareOptions(ol);
  }

  string Experiment::help() const
  {
    return 
      "The Experiment class allows you to describe a typical learning experiment that you wish to perform, \n"
      "as a training/testing of a learning algorithm on a particular dataset.\n"
      "Detailed results for each split #i will be saved in sub-directory Splits/#i of the experiment directory. \n"
      "Final results for each split and basic statistics across all splits will be saved in the results.summary \n"
      "file in the experiment directory.\n"
      + optionHelp();
  }

  void Experiment::build_()
  {
    splitter->setDataSet(dataset);
  }

  // ### Nothing to add here, simply calls build_
  void Experiment::build()
  {
    inherited::build();
    build_();
  }

void Experiment::run()
{
  if(expdir=="")
    PLERROR("No expdir specified for Experiment.");
  if(!learner)
    PLERROR("No leaner specified for Experiment.");
  if(!splitter)
    PLERROR("No splitter specified for Experiment");

  if(PLMPI::rank==0)
    {
      if(pathexists(expdir))
        PLERROR("Directory (or file) %s already exists. First move it out of the way.",expdir.c_str());

      if(!force_mkdir(expdir))
        PLERROR("Could not create experiment directory %s", expdir.c_str());

      // Save this experiment description in the expdir (buildoptions only)
      PLearn::save(append_slash(expdir)+"experiment.plearn", *this, OptionBase::buildoption);
    }

  int nsplits = splitter->nsplits();
  Array<string> testresnames = learner->testResultsNames();
  int ntestcosts = testresnames.size();
  VecStatsCollector teststats;

  if(PLMPI::rank==0)
    {
      POFStream resout(append_slash(expdir)+"results.summary");
      resout.setMode(PStream::raw_ascii);
      resout << "#: result_type ";
      for(int k=0; k<ntestcosts; k++)
        resout << testresnames[k] << "  ";
      resout << endl;
    }

  for(int k=0; k<nsplits; k++)
    {
      Array<VMat> train_test = splitter->getSplit(k);
      if(train_test.size()!=2) 
        PLERROR("Splitter returned a split with %d subsets, instead of the expected 2: train&test",train_test.size());
      VMat trainset = train_test[0];
      VMat testset = train_test[1];
      string learner_expdir = append_slash(expdir)+"Split"+tostring(k);
      learner->setExperimentDirectory(learner_expdir);
      learner->forget();
      learner->train(trainset);
      Vec testres = learner->test(testset);
      if(PLMPI::rank==0)
        {
          POFStream resout(append_slash(expdir)+"results.summary", ios::out | ios::app);
          resout << "Split_" << k << " ";
          resout << testres << endl;
        }
      teststats.update(testres);
    }

  if(PLMPI::rank==0)
    {
      POFStream resout(append_slash(expdir)+"results.summary", ios::out | ios::app);
      resout << "STDDEV   " << teststats.getStdDev() << endl;
      resout << "STDERROR " << teststats.getStdError() << endl;
      resout << "MEAN     " << teststats.getMean() << endl;
    }
}

%> // end of namespace PLearn
