// -*- C++ -*-

// HTMLHelpCommand.h
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
   * $Id: HTMLHelpCommand.h,v 1.1 2004/11/27 08:54:11 chapados Exp $ 
   ******************************************************* */

// Authors: Nicolas Chapados

/*! \file HTMLHelpCommand.h */


#ifndef HTMLHelpCommand_INC
#define HTMLHelpCommand_INC

#include "PLearnCommand.h"
#include "PLearnCommandRegistry.h"
#include <iostream>

namespace PLearn {
using namespace std;

class HTMLHelpConfig;                        //!< forward-declare
  
class HTMLHelpCommand: public PLearnCommand
{
  PP<HTMLHelpConfig> config;
  
protected:
  //! Generate a top-level index.html
  virtual void helpIndex();
  
  //! Generate list of registered commands
  virtual void helpCommands();

  //! Generate list of registered classes
  virtual void helpClasses();

  //! Generate documentation for a given command; called by helpCommands
  virtual void helpOnCommand(const string& theCommand);
  
  //! Generate documentation for a given class; called by helpClasses
  virtual void helpOnClass(const string& theClass);

  //! Copy the document snippet in filename into the ostream
  virtual void copySnippet(ostream& os, const string& document);

  //! Basic HTML quoting of: < > # &
  virtual string quote(string s) const;

  //! Return the list of parent classes of a given classname
  virtual TVec<string> parent_classes(string s) const;

  //! Make link to known classes embedded within type name
  virtual string highlight_known_classes(string typestr) const;

  //! DWIM for free text
  virtual string format_free_text(string text) const;
  
public:
  HTMLHelpCommand();

  //! Sole argument should be a filename containing an HTMLHelpConfig object
  virtual void run(const vector<string>& args);

protected:
  static PLearnCommandRegistry reg_;
};

//#####  Configuration Object for HTMLHelpCommand  ############################

class HTMLHelpConfig: public Object
{
  typedef Object inherited;
    
public:
  //! Directory where the .html files should be generated
  string output_dir;

  //! Filename containing the "top" of the HTML document (including an
  //! opening <body> tag)
  string html_prolog_document;

  //! Filename containing the "bottom" of the HTML document (including the
  //! closing </body> tag)
  string html_epilog_document;


protected: 
  //! Declares this class' options.
  // (PLEASE IMPLEMENT IN .cc)
  static void declareOptions(OptionList& ol);

public:
  PLEARN_DECLARE_OBJECT(HTMLHelpConfig);
};

  
} // end of namespace PLearn

#endif
