// -*- C++ -*-

// PairwiseDiffsCommand.cc
//
// Copyright (C) 2005 Nicolas Chapados 
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
   * $Id: PairwiseDiffsCommand.cc,v 1.1 2005/02/17 07:37:09 chapados Exp $ 
   ******************************************************* */

// Authors: Nicolas Chapados

/*! \file PairwiseDiffsCommand.cc */

// From C++ stdlib
#include <deque>
#include <iomanip>

// From Boost
#include <boost/format.hpp>

// From PLearn
#include "PairwiseDiffsCommand.h"
#include <plearn/base/stringutils.h>
#include <plearn/math/StatsCollector.h>
#include <plearn/db/getDataSet.h>

namespace PLearn {
using namespace std;
using boost::format;

//! This allows to register the 'PairwiseDiffsCommand' command in the command registry
PLearnCommandRegistry PairwiseDiffsCommand::reg_(new PairwiseDiffsCommand);

PairwiseDiffsCommand::PairwiseDiffsCommand():
  PLearnCommand(
    "pairdiff",
    "Compute a set of statistics on the pairwise differences between cost matrices",
    " usage: pairdiff [--stats=E,PZ2t,...] costname costfile1.pmat costfile2.pmat ...\n"
    "\n"
    "This command computes a set of statistics (user-specified) on the\n"
    "pairwise differences between a given column of a list of matrices.  The\n"
    "typical usage is to give a list of test cost matrices (e.g. such as the\n"
    "'test_costs.pmat' files generated by PTester), and specify the cost of\n"
    "interest (column name) in the matrix.  For all of the matrices taken\n"
    "pairwise, the command will accumulate the DIFFERENCE of the specified\n"
    "columns in a stats collector, and then will output a set of\n"
    "user-specified statistics.  The default statistics are 'E' and 'PZ2t'\n"
    "(see the supported statistics in StatsCollector), meaning that the mean\n"
    "difference is computed, and the p-value that this difference is zero."
    )
{ }

// Return a statscollector with the data accumulated from two matrices
static PP<StatsCollector>
accumInStatsCol(string costname, string file1, string file2)
{
  VMat data1 = getDataSet(file1);
  VMat data2 = getDataSet(file2);
  int index1 = data1->getFieldIndex(costname);
  int index2 = data2->getFieldIndex(costname);
  if (index1 < 0)
    PLERROR("PairwiseDiffsCommand: fieldname '%s' does not exist in file '%s'",
            costname.c_str(), file1.c_str());
  if (index2 < 0)
    PLERROR("PairwiseDiffsCommand: fieldname '%s' does not exist in file '%s'",
            costname.c_str(), file2.c_str());

  int len = min(data1.length(), data2.length());
  if (len != data1.length() || len != data2.length())
    PLWARNING("PairwiseDiffsCommand: files '%s' and '%s' do not contain the same "
              "number of rows; only comparing the first %d rows",
              file1.c_str(), file2.c_str(), len);

  PP<StatsCollector> sc = new StatsCollector;
  for (int i=0; i<len; ++i) {
    real value1 = data1(i,index1);
    real value2 = data2(i,index2);
    sc->update(value1-value2);
  }
  return sc;
}
  
//! The actual implementation of the 'PairwiseDiffsCommand' command 
void PairwiseDiffsCommand::run(const vector<string>& origargs)
{
  if (origargs.size() < 3)
    return;

  // Default arguments
  vector<string> stats;
  stats.push_back("E");
  stats.push_back("PZ2t");

  // Parse arguments
  deque<string> args(origargs.begin(), origargs.end());
  while (args[0].substr(0,2) == "--") {
    if (args[0].substr(0,8) == "--stats=") {
      string commastats = args[0].substr(8);
      stats = split(commastats,',');
    }
    args.pop_front();
  }

  string costname = args.front();
  args.pop_front();

  // Find maximum-length filename
  int maxlen = 0;
  for (int i=0, n=args.size() ; i<n ; ++i)
    maxlen = max(maxlen, int(args[i].size()));

  // Print out heading row
  const int width = 15;
  cout.setf(ios_base::left);
  cout << setw(maxlen) << "Filename1" << ' '
       << setw(maxlen) << "Filename2" << ' ';
  cout.setf(ios_base::right);
  for (int i=0, n=stats.size() ; i<n ; ++i)
    cout << setw(width) << stats[i] << ' ';
  cout << endl;
  cout.setf(ios_base::left);
  
  // What we have in args is a list of filenames.  Go over them.
  for (int i=0, n=args.size() ; i < n-1 ; ++i) {
    for (int j=i+1 ; j < n ; ++j) {
      PP<StatsCollector> sc = accumInStatsCol(costname, args[i], args[j]);

      // Print out result
      cout << setw(maxlen) << args[i] << ' '
           << setw(maxlen) << args[j] << ' ';
      for (int k=0, m=stats.size() ; k<m ; ++k) {
        real stat = sc->getStat(stats[k]);
        cout << format("%+"+tostring(width)+".7f ") % stat;
      }
      cout << endl;
    }
  }
}

} // end of namespace PLearn

