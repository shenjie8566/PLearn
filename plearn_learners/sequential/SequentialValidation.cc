// -*- C++ -*-

// SequentialValidation.cc
//
// Copyright (C) 2003 Rejean Ducharme, Yoshua Bengio
// Copyright (C) 2003 Pascal Vincent
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



#include "SequentialValidation.h"
#include "VecStatsCollector.h"
#include "AsciiVMatrix.h"
#include "FileVMatrix.h"
#include "PTester.h"  // for using class StatSpec

namespace PLearn <%
using namespace std;


PLEARN_IMPLEMENT_OBJECT(SequentialValidation, "ONE LINE DESCR", "NO HELP");

SequentialValidation::SequentialValidation()
  : init_train_size(1),
    expdir(""),
    report_stats(true),
    save_final_model(true),
    save_initial_model(false),
    save_initial_seqval(true),
    save_data_sets(false),
    save_test_outputs(false),
    save_test_costs(false),
    save_stat_collectors(false),
    provide_learner_expdir(true)
{}

void SequentialValidation::build_()
{
  if (expdir != "")
  {
    if(pathexists(expdir))
      PLERROR("Directory (or file) %s already exists. First move it out of the way.", expdir.c_str());
    if(!force_mkdir(expdir))
      PLERROR("Could not create experiment directory %s", expdir.c_str());
  }
}

void SequentialValidation::build()
{
  inherited::build();
  build_();
}

void SequentialValidation::declareOptions(OptionList& ol)
{
  declareOption(ol, "report_stats", &SequentialValidation::report_stats, OptionBase::buildoption, "If true, the computed global statistics specified in statnames will be saved in global_stats.pmat \n"
                  "and the corresponding per-split statistics will be saved in split_stats.pmat \n"
                  "For reference, all cost names (as given by the learner's getTrainCostNames() and getTestCostNames() ) \n"
                  "will be reported in files train_cost_names.txt and test_cost_names.txt");

  declareOption(ol, "statnames", &SequentialValidation::statnames, OptionBase::buildoption,
                  "A list of global statistics we are interested in.\n"
                  "These are strings of the form S1[S2[dataset.cost_name]] where:\n"
                  "  - dataset is train or test1 or test2 ... (train being \n"                  "    the first dataset in a split, test1 the second, ...) \n"
                  "  - cost_name is one of the training or test cost names (depending on dataset) understood \n"
                  "    by the underlying learner (see its getTrainCostNames and getTestCostNames methods) \n"
                  "  - S1 and S2 are a statistic, i.e. one of: E (expectation), V(variance), MIN, MAX, STDDEV, ... \n"
                  "    S2 is computed over the samples of a given dataset split. S1 is over the splits. \n");

  declareOption(ol, "expdir", &SequentialValidation::expdir,
    OptionBase::buildoption, "Path of this experiment's directory in which to save all experiment results (will be created if it does not already exist). \n");

  declareOption(ol, "learner", &SequentialValidation::learner,
    OptionBase::buildoption, "The SequentialLearner to train/test. \n");

  declareOption(ol, "dataset", &SequentialValidation::dataset,
    OptionBase::buildoption, "The dataset to use for training/testing. \n");

  declareOption(ol, "init_train_size", &SequentialValidation::init_train_size,
    OptionBase::buildoption, "Size of the first training set. \n");

  declareOption(ol, "save_final_model", &SequentialValidation::save_final_model,
    OptionBase::buildoption, "If true, the final model will be saved in model.psave \n");

  declareOption(ol, "save_initial_model", &SequentialValidation::save_initial_model,
    OptionBase::buildoption, "If true, the initial model will be saved in initial_model.psave. \n");

  declareOption(ol, "save_initial_seqval", &SequentialValidation::save_initial_seqval,
    OptionBase::buildoption, "If true, this SequentialValidation object will be saved in sequential_validation.psave. \n");

  declareOption(ol, "save_data_sets", &SequentialValidation::save_data_sets,
    OptionBase::buildoption, "If true, the data sets (train/test) for each split will be saved. \n");

  declareOption(ol, "save_test_outputs", &SequentialValidation::save_test_outputs,
    OptionBase::buildoption, "If true, the outputs of the tests will be saved in test_outputs.pmat \n");

  declareOption(ol, "save_test_costs", &SequentialValidation::save_test_costs,
    OptionBase::buildoption, "If true, the costs of the tests will be saved in test_costs.pmat \n");

  declareOption(ol, "save_stat_collectors", &SequentialValidation::save_stat_collectors, OptionBase::buildoption, "If true, stat collectors of each data sets (train/test) will be saved for each split. \n");

  inherited::declareOptions(ol);
}

string SequentialValidation::help()
{
  return
    "The SequentialValidation class allows you to describe a typical sequential \n"
    "validation experiment that you wish to perform........."
    + optionHelp();
}

void SequentialValidation::run()
{
  if (expdir=="")
    PLERROR("No expdir specified for SequentialValidation.");
  if (!learner)
    PLERROR("No learner specified for SequentialValidation.");

  if(provide_learner_expdir)
    learner->setExperimentDirectory(append_slash(expdir)+"Model");

  // This is to set inputsize() and targetsize()
  learner->setTrainingSet(dataset, false);

  string dir = append_slash(expdir);

  // Save this experiment description in the expdir (buildoptions only)
  if (save_initial_seqval)
    PLearn::save(dir+"sequential_validation.psave", *this);

  TVec<string> testcostnames = learner->getTestCostNames();
  TVec<string> traincostnames = learner->getTrainCostNames();
 
  // int traincostsize = traincostnames.size();
  int testcostsize = testcostnames.size();
  int outputsize = learner->outputsize();
  int nstats = statnames.length();

  TVec< PP<VecStatsCollector> > stcol(2);  // one for train and one for test

  // stats for a train on one split
  PP<VecStatsCollector> train_stats = new VecStatsCollector();
  train_stats->setFieldNames(traincostnames);
  learner->setTrainStatsCollector(train_stats);
  stcol[0] = train_stats;

  // stats for a test on one split
  PP<VecStatsCollector> test_stats = new VecStatsCollector();
  test_stats->setFieldNames(testcostnames);
  stcol[1] = test_stats;

  // stats over all sequence
  PP<VecStatsCollector> sequence_stats = new VecStatsCollector();

  // Stat specs
  TVec<StatSpec> statspecs(nstats);
  for (int k=0; k<nstats; k++)
    statspecs[k].init(statnames[k]);

  VMat global_stats_vm;  // the vmat in which to save global result stats specified in statnames
  VMat split_stats_vm;  // the vmat in which to save per split result stats

  if (report_stats)
  {
    saveStringInFile(dir+"train_cost_names.txt", join(traincostnames,"\n")+"\n");
    saveStringInFile(dir+"test_cost_names.txt", join(testcostnames,"\n")+"\n");

    global_stats_vm = new FileVMatrix(dir+"global_stats.pmat", 0, nstats);
    for(int k=0; k<nstats; k++)
      global_stats_vm->declareField(k,statspecs[k].statName());
    global_stats_vm->saveFieldInfos();

    split_stats_vm = new FileVMatrix(dir+"sequence_stats.pmat", 0, 1+nstats);
    split_stats_vm->declareField(0,"splitnum");
    for(int k=0; k<nstats; k++)
      split_stats_vm->declareField(k+1,statspecs[k].setname + "." + statspecs[k].intstatname);
    split_stats_vm->saveFieldInfos();
  }

  // the learner horizon
  int horizon = learner->horizon;
 
  int splitnum = 0;
  for (int t=init_train_size; t<=dataset.length()-horizon; t++, splitnum++)
  {
#ifdef DEBUG
    cout << "SequentialValidation::run() -- sub_train.length = " << t << " et sub_test.length = " << t+horizon << endl;
#endif
    VMat sub_train = dataset.subMatRows(0,t); // excludes t, last training pair is (t-1-horizon,t-1)
    VMat sub_test = dataset.subMatRows(0, t+horizon);

    string splitdir = dir+"train_t="+tostring(t)+"/";
    if (save_data_sets)
      PLearn::save(splitdir+"training_set.psave", sub_train);
    if (save_initial_model)
      PLearn::save(splitdir+"initial_learner.psave",learner);

    // Train
    //learner->forget(); // PAS CERTAIN!  Doit-on faire un forget a chaque t?
    train_stats->forget();
    learner->setTrainingSet(sub_train, false);
    learner->train();
    train_stats->finalize();

    if (save_stat_collectors)
      PLearn::save(splitdir+"train_stats.psave",train_stats);
    if (save_final_model)
      PLearn::save(splitdir+"final_learner.psave",learner);

    // Test
    VMat test_outputs;
    VMat test_costs;
    if (save_test_outputs)
      test_outputs = new FileVMatrix(splitdir+"test_outputs.pmat",0,outputsize);
    if (save_test_costs)
      test_costs = new FileVMatrix(splitdir+"test_costs.pmat",0,testcostsize);

    if (save_data_sets)
      PLearn::save(splitdir+"test_set.psave", sub_test);

    test_stats->forget();
    learner->test(sub_test, test_stats, test_outputs, test_costs);
    test_stats->finalize();
    if (save_stat_collectors)
      PLearn::save(splitdir+"test_stats.psave",test_stats);

    Vec splitres(1+nstats);
    splitres[0] = splitnum;

    for(int k=0; k<nstats; k++)
    {
      StatSpec& sp = statspecs[k];
      if (sp.setnum>=stcol.length())
        PLERROR("SequentialValidation::run, trying to access a test set (test%d) beyond the last one (test%d)",
            sp.setnum, stcol.length()-1);
      splitres[k+1] = stcol[sp.setnum]->getStat(sp.intstatname);
      //splitres[k+1] = stcol[sp.setnum]->stats ? stcol[sp.setnum]->getStats(sp.costindex).getStat(sp.intstat) : MISSING_VALUE;
    }

    if (split_stats_vm)
      split_stats_vm->appendRow(splitres);

    sequence_stats->update(splitres.subVec(1,nstats));
  }

  sequence_stats->finalize();

  Vec global_result(nstats);
  for (int k=0; k<nstats; k++)
    global_result[k] = sequence_stats->getStats(k).getStat(statspecs[k].extstat);

  if (global_stats_vm)
    global_stats_vm->appendRow(global_result);

  if (report_stats)
  {
    PLearn::save(dir+"global_result.avec", global_result);
    PLearn::save(dir+"predictions.amat", learner->predictions);
    PLearn::save(dir+"errors.amat", learner->errors);
  }

}

%> // end of namespace PLearn

