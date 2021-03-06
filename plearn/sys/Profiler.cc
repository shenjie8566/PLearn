// -*- C++ -*-

// PLearn (A C++ Machine Learning Library)
// Copyright (C) 2001 Yoshua Bengio and University of Montreal
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
 * $Id$
 * This file is part of the PLearn library.
 ******************************************************* */

#include "Profiler.h"
#include <time.h>

#ifdef _OPENMP
#include<omp.h>
#include<plearn/base/tostring.h>
#endif

namespace PLearn {
using namespace std;

// initialize static variables
map<string,Profiler::Stats> Profiler::codes_statistics;
#ifdef PROFILE
struct tms Profiler::t;
#endif
bool Profiler::active  = false;


string Profiler::get_omp_save_name(const string name_of_piece_of_code){
#ifdef _OPENMP
    return name_of_piece_of_code+tostring(omp_get_thread_num());
#else
    return name_of_piece_of_code;
#endif
}

#ifdef PROFILE
// start recording time for named piece of code
void Profiler::start(const string& name_of_piece_of_code_, const int max_nb_going)
{
    string name_of_piece_of_code=get_omp_save_name(name_of_piece_of_code_);
#ifdef _OPENMP
#pragma omp critical (codes_statistics)
#endif
    if (active)
    {
        map<string,Profiler::Stats>::iterator it = 
            codes_statistics.find(name_of_piece_of_code);
        if (it == codes_statistics.end())
        {
            Stats stats;
            stats.nb_going = 1;
            stats.wall_last_start   = times(&t);
            stats.user_last_start   = t.tms_utime;
            stats.system_last_start = t.tms_stime;
            codes_statistics[name_of_piece_of_code] = stats;
        }
        else
        {
            Profiler::Stats& stats = it->second;
            if (stats.nb_going >= max_nb_going)
                PLERROR("Profiler::start(%s) called while previous %d starts had not ended and we allowed only %d starts",
                        name_of_piece_of_code.c_str(),stats.nb_going, max_nb_going);
            if (stats.nb_going==0){
                stats.wall_last_start   = times(&t);
                stats.user_last_start   = t.tms_utime;
                stats.system_last_start = t.tms_stime;
            }
            stats.nb_going++;
        }
    }
}

  
// end recording time for named piece of code, and increment
// frequency of occurence and total duration of this piece of code.
void Profiler::end(const string& name_of_piece_of_code_)
{
    string name_of_piece_of_code=get_omp_save_name(name_of_piece_of_code_);
#ifdef _OPENMP
#pragma omp critical (codes_statistics)
#endif
    if (active)
    {
        clock_t end_time = times(&t);
        map<string,Profiler::Stats>::iterator it = 
            codes_statistics.find(name_of_piece_of_code);
        if (it == codes_statistics.end())
            PLERROR("Profiler::end(%s) called before any call to start(%s)",
                    name_of_piece_of_code.c_str(),name_of_piece_of_code.c_str());
        Profiler::Stats& stats = it->second;
        if (stats.nb_going == 0)
            PLERROR("Profiler::end(%s) called before previous start was called",
                    name_of_piece_of_code.c_str());

        stats.nb_going--;
        stats.frequency_of_occurence++;
        if (stats.nb_going==0){
            long wall_duration   = end_time    - stats.wall_last_start;
            long user_duration   = t.tms_utime - stats.user_last_start;
            long system_duration = t.tms_stime - stats.system_last_start;
            stats.wall_duration   += wall_duration;
            stats.user_duration   += user_duration;
            stats.system_duration += system_duration;
            if (wall_duration < 0) {
                wall_duration = user_duration = system_duration = 1;
                PLWARNING("Profiler: negative duration measured with times!");
            }
        }
    }
}

#ifdef PL_PROFILE
// call Profiler::start if PL_PROFILE is set
void Profiler::pl_profile_start(const string& name_of_piece_of_code, const int max_nb_going){
    Profiler::start(name_of_piece_of_code, max_nb_going);}
// call Profiler::end if PL_PROFILE is set
void Profiler::pl_profile_end(const string& name_of_piece_of_code){
    Profiler::end(name_of_piece_of_code);}
// call Profiler::activate if PL_PROFILE is set
void Profiler::pl_profile_activate(){
    Profiler::activate();}
// call Profiler::deactivate if PL_PROFILE is set
void Profiler::pl_profile_deactivate(){
    Profiler::deactivate();}
// call Profiler::report if PL_PROFILE is set
void Profiler::pl_profile_report(ostream& out){
    Profiler::report(out);}
void Profiler::pl_profile_report(PStream out){
    Profiler::report(out);}
// call Profiler::reportwall if PL_PROFILE is set
void Profiler::pl_profile_reportwall(ostream& out){
    Profiler::reportwall(out);}
void Profiler::pl_profile_reportwall(PStream out){
    Profiler::reportwall(out);}
#endif

#endif

//! Return the statistics related to a piece of code.  This is useful
//! for aggregators that collect and report a number of statistics
const Profiler::Stats& Profiler::getStats(const string& name_of_piece_of_code_)
{
    string name_of_piece_of_code=get_omp_save_name(name_of_piece_of_code_);
    Stats* s;
#ifdef _OPENMP
#pragma omp critical (codes_statistics)
#endif
{
    map<string,Stats>::iterator it = codes_statistics.find(name_of_piece_of_code);
    if (it == codes_statistics.end())
        PLERROR("Profiler::getStats: cannot find statistics for '%s'. the active variable is at: %d",
                name_of_piece_of_code.c_str(),active);
    s = &(it->second);
}
return *s;
}


//! Reset the statistics associated with a piece of code.  The piece of
//! code may not yet exist, this is fine.
void Profiler::reset(const string& name_of_piece_of_code_)
{
    string name_of_piece_of_code=get_omp_save_name(name_of_piece_of_code_);

    Stats empty;
#ifdef _OPENMP
#pragma omp critical (codes_statistics)
#endif
    codes_statistics[name_of_piece_of_code] = empty;
}


// output a report on the output stream, giving
// the statistics recorded for each of the named pieces of codes.
void Profiler::report(ostream& out)
{
    report(PStream(&out));
}
void Profiler::report(PStream out)
{
#ifdef _OPENMP
#pragma omp critical (codes_statistics)
#endif
{
    map<string,Profiler::Stats>::iterator it =  
        codes_statistics.begin(), end =  codes_statistics.end();

    out << "*** PLearn::Profiler Report ***" << endl;
    out << "Ticks per second : " << ticksPerSecond() <<endl;
    for ( ; it!=end ; ++it)
    {
        out << endl << "For " << it->first << " :" << endl;
        Profiler::Stats& stats = it->second;
        out << "Frequency of occurence   = " << stats.frequency_of_occurence << endl;
        out << "Wall duration   (ticks)  = " << stats.wall_duration << endl
            << "User duration   (ticks)  = " << stats.user_duration << endl
            << "System duration (ticks)  = " << stats.system_duration << endl;

        double avg_wall = (double)stats.wall_duration/stats.frequency_of_occurence;
        double avg_user = (double)stats.user_duration/stats.frequency_of_occurence;
        double avg_sys  = (double)stats.system_duration/stats.frequency_of_occurence;
        out << "Average wall   duration  = " << avg_wall << endl
            << "Average user   duration  = " << avg_user << endl
            << "Average system duration  = " << avg_sys  << endl;
    }
}
}

// output a report on the output stream, giving
// the wall time statistics recorded for each of the named pieces of code
void Profiler::reportwall(ostream& out)
{
    reportwall(PStream(&out));
}
void Profiler::reportwall(PStream out)
{
#ifdef _OPENMP
#pragma omp critical (codes_statistics)
#endif
{
    map<string,Profiler::Stats>::iterator it =  
        codes_statistics.begin(), end =  codes_statistics.end();

    out << "*** PLearn::Profiler Wall Report ***" << endl;
    out << "Ticks per second : " << ticksPerSecond() <<endl;
    for ( ; it!=end ; ++it)
    {
        out << endl << "For " << it->first << " :" << endl;
        Profiler::Stats& stats = it->second;
        out << "Frequency of occurence   = " << stats.frequency_of_occurence << endl;
        out << "Wall duration   (ticks)  = " << stats.wall_duration << endl;

        double avg_wall = (double)stats.wall_duration/stats.frequency_of_occurence;
        out << "Average wall   duration  = " << avg_wall << endl;
    }
}
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
