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
   * $Id: general.h,v 1.5 2002/12/02 08:46:51 plearner Exp $
   * AUTHORS: Pascal Vincent & Yoshua Bengio
   * This file is part of the PLearn library.
   ******************************************************* */


/*! \file PLearnLibrary/PLearnCore/general.h */

#ifndef GENERAL_INC
#define GENERAL_INC

#if !(!defined(_MSC_VER) && !defined(_MINGW_))
#include <io.h>
#include <float.h>
#else
#include <unistd.h>
#endif

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cstdarg>
#include <cstddef>
#include <cfloat>
#include <cctype>
#include <iostream>
#include <fstream>
#include <strstream>
#include <iomanip>
#include <cerrno>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <functional>

#ifdef SPARC
#include <ieeefp.h>
#endif

#include "plerror.h"
#include "pl_math.h"
#include "pl_io.h"
#include "pl_io_deprecated.h"
//#include "plstreams.h"
#include "stringutils.h"
#include "pl_hash_fun.h"
#include "TypeTraits.h"
#include "CopiesMap.h"

//!  Size of header (in bytes) for native PLearn 
//!  data files (.pmat and .pvec)
#define DATAFILE_HEADERLENGTH 64

namespace std <%
  
  //! efficient specialisations of std::copy for built-in types

  inline float* copy(float* first, float* last, float* dest)
  { size_t n = last-first; memcpy(dest, first, n*sizeof(float)); return dest+n; }

  inline double* copy(double* first, double* last, double* dest)
  { size_t n = last-first; memcpy(dest, first, n*sizeof(double)); return dest+n; }

  inline bool* copy(bool* first, bool* last, bool* dest)
  { size_t n = last-first; memcpy(dest, first, n*sizeof(bool)); return dest+n; }

  inline char* copy(char* first, char* last, char* dest)
  { size_t n = last-first; memcpy(dest, first, n*sizeof(char)); return dest+n; }

  inline unsigned char* copy(unsigned char* first, unsigned char* last, unsigned char* dest)
  { size_t n = last-first; memcpy(dest, first, n*sizeof(unsigned char)); return dest+n; }

  inline short* copy(short* first, short* last, short* dest)
  { size_t n = last-first; memcpy(dest, first, n*sizeof(short)); return dest+n; }

  inline unsigned short* copy(unsigned short* first, unsigned short* last, unsigned short* dest)
  { size_t n = last-first; memcpy(dest, first, n*sizeof(unsigned short)); return dest+n; }

  inline int* copy(int* first, int* last, int* dest)
  { size_t n = last-first; memcpy(dest, first, n*sizeof(int)); return dest+n; }

  inline unsigned int* copy(unsigned int* first, unsigned int* last, unsigned int* dest)
  { size_t n = last-first; memcpy(dest, first, n*sizeof(unsigned int)); return dest+n; }

  inline long* copy(long* first, long* last, long* dest)
  { size_t n = last-first; memcpy(dest, first, n*sizeof(long)); return dest+n; }

  inline unsigned long* copy(unsigned long* first, unsigned long* last, unsigned long* dest)
  { size_t n = last-first; memcpy(dest, first, n*sizeof(unsigned long)); return dest+n; }

%> // end of namespace std


namespace PLearn <%
using namespace std;

using std::min;
using std::max;

  //! Like std::copy, but with an explicit cast to the destination type
  template<class In, class Out>
  inline Out copy_cast(In first, In last, Out res)
  {
    typedef typename iterator_traits<Out>::value_type out_t;
    for(; first!=last; ++first, ++res)
      *res = out_t(*first);
    return res;
  }

  //! clears the range
  template<class For>
  inline void clear_n(For begin, int n)
  {
    static typename iterator_traits<For>::value_type zero;
    fill_n(begin, n, zero);
  }

  //! efficient specialisation for built-in types
  inline void clear_n(float* begin, int n)
  { memset(begin,0,n*sizeof(float)); }

  inline void clear_n(double* begin, int n)
  { memset(begin,0,n*sizeof(double)); }

  inline void clear_n(bool* begin, int n)
  { memset(begin,0,n*sizeof(bool)); }

  inline void clear_n(char* begin, int n)
  { memset(begin,0,n*sizeof(char)); }

  inline void clear_n(unsigned char* begin, int n)
  { memset(begin,0,n*sizeof(unsigned char)); }

  inline void clear_n(short* begin, int n)
  { memset(begin,0,n*sizeof(short)); }

  inline void clear_n(unsigned short* begin, int n)
  { memset(begin,0,n*sizeof(unsigned short)); }

  inline void clear_n(int* begin, int n)
  { memset(begin,0,n*sizeof(int)); }

  inline void clear_n(unsigned int* begin, int n)
  { memset(begin,0,n*sizeof(unsigned int)); }

  inline void clear_n(long* begin, int n)
  { memset(begin,0,n*sizeof(long)); }  

  inline void clear_n(unsigned long* begin, int n)
  { memset(begin,0,n*sizeof(unsigned long)); }  

  typedef int (*compare_function)(const void *, const void *);

  template<class T>
  inline void swap(T& a, T& b)
    { 
      T tmp; 
      tmp = a;
      a = b;
      b = tmp;
    }

//!  make a copy of a C string and return it
  char* strcopy(char* s);

//!  print a number without unnecessary trailing zero's, into buffer
  void pretty_print_number(char* buffer, real number);

//!  Simple file info
  int   file_size(const string& filename);
  bool file_exists(const string& filename);

	
  //!  check that all keys of the map are int values
  bool isMapKeysAreInt(map<real,int>& m);

// return the name of host, e.g. with getenv("HOSTNAME") or getenv("HOST")
string hostname();



%> // end of namespace PLearn

#endif
