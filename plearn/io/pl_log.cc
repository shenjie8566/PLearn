// -*- C++ -*-

// pl_log.cc
//
// Copyright (C) 2004 Nicolas Chapados 
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
   * $Id: pl_log.cc,v 1.1 2004/10/27 04:29:34 chapados Exp $ 
   ******************************************************* */

// Authors: Nicolas Chapados

/*! \file pl_log.cc */

#include <string>
#include <plearn/base/stringutils.h>
#include "pl_log.h"

namespace PLearn {
using namespace std;

PL_Log::PL_Log()
  : runtime_verbosity(PL_LOG_MAXVERBOSITY),
    output_stream(&cout, false),
    null_stream(new ofstream("/dev/null"), true),
    logger_count(0)
{ }

PStream& PL_Log::logger(int requested_verbosity)
{
  logger_count++;
  if (requested_verbosity <= runtime_verbosity)
    return output_stream;
  else
    return null_stream;
}

PL_Log& PL_Log::instance()
{
  static PL_Log global_logger;
  return global_logger;
}

PStream& plsep(PStream& p)
{
  return p << plhead("");
}

PStream& operator<<(PStream& p, PL_Log::Heading heading)
{
  string msg = "#####  "+tostring(PL_Log::instance().loggerCount())
    + (heading.h.size() > 0? (": "+heading.h) : string("")) + "  ";
  string rest(max(75-int(msg.size()),5),'#');
  return p << endl << (msg + rest) << endl;
}

} // end of namespace PLearn
