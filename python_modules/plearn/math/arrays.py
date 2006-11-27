# arrays.py
# Copyright (C) 2005,2006 Christian Dorion
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#
#   1. Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#
#   2. Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#
#   3. The name of the authors may not be used to endorse or promote
#      products derived from this software without specific prior written
#      permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
#  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
#  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
#  NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
#  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#  This file is part of the PLearn library. For more information on the PLearn
#  library, go to the PLearn Web site at www.plearn.org

# Author: Christian Dorion
from math import *
from numarray import *
from numarray.linear_algebra import inverse as __numarray_inverse

def inverse(a):
    """Built over numarray's inverse() function while handling scalars."""
    if isinstance(a, (int, long, float, complex)):
        return 1.0/a
    return __numarray_inverse(a)

def lag(series, k, fill=zeros):
    """Returns a lagged version of 'series'.

    Default behavior is to return an array of the same length than 'series'
    with last 'k' elements equal to 0 (read first 'k' elements if 'k' is
    negative). This behavior can be modified by providing a user-defined
    'fill' function which accepts a shape tuple as single argument.
    """
    assert len(shape(series)) == 1, "Manages row vector only"
    if k > 0:
        return concatenate([series[k:], fill((k,))])
    elif k < 0:
        return concatenate([fill((-k,)), series[:k]])
    else:
        return series

def matrix_distance(m1, m2):
    return maximum.reduce( fabs(ravel(m1-m2)) )

def mmult(*args):
    """Shorthand for matrix multiplication

    Numarray's 'matrixmultiply' function takes only two matrices at a time,
    which can make statements quite heavy when multiply many matrices. This
    function accepts as many matrices as one ones, processing multiplication
    'from left to right'.
    """
    return reduce(matrixmultiply, args)

def rrange(start, stop, step, include_stop=False):
    """Behaves like the builtin range() function but with real arguments.

    Note that since 'step' is here mandatory since there is no obvious
    default choice. The 'include_stop' is False, so that the default
    behaviour is similar to all Python range* functions, but can be set
    True for convenience.
    """
    npoints = (stop-start) / step
    if include_stop:
        npoints += 1
    return [ start+step*d for d in range(npoints) ]

def grid_around(center, width, step):        
    start = center-(width/2.0)
    npoints = int(width/step) + 1
    return array([ start + step*d for d in range(npoints) ])
        
def sign(m):
    m = asarray(m)
    return zeros(shape(m))-ufunc.less(m,0)+ufunc.greater(m,0)

def fast_softmax( x ):
    s = 0
    res = []
    for r in x:
        e = exp(r)
        s += e
        res.append( e )

    return [ r/s for r in res ]

def floats_are_equal(a, b, numtol=1e-6):
    # print "Comparing floats ",a,b,numtol
    minabs = min(abs(a),abs(b))
    if minabs<1.0:
        return abs(a-b) <= numtol
    return abs(a-b) <= numtol*minabs

def hasNaN(f):
    has = sum(isNaN(f))
    while not isinstance(has, int):
        has = sum(has)
    return has
    
def isNaN(f):
    """Return 1 where f contains NaN values, 0 elsewhere."""
    from numarray import ieeespecial
    return ieeespecial.mask(f, ieeespecial.NAN)

def isNotNaN(f):
    """Return 0 where f contains NaN values, 1 elsewhere."""
    return ufunc.equal(isNaN(f), 0.0)

def replace_nans(a, with=0.0):
    return choose(isNotNaN(a), (with, a))

if __name__ == '__main__':
    print fast_softmax([ 0, 0, 100 ])
    print
    print isNaN(float('NaN'))
    print isNaN(2.0)
    print isNaN([1.0, float('NaN'), 3.0])
    print hasNaN([1.0, float('NaN'), 3.0])
    print isNaN([1.0, 2.0, 3.0])
    print hasNaN([1.0, 2.0, 3.0])
    print

    print "2d"
    print hasNaN([[1.0, 2.0, 3.0],
                  [1.0, 2.0, 3.0]])
    print hasNaN([[1.0, 2.0,          3.0],
                  [1.0, float('NaN'), 3.0]])
    print hasNaN([[1.0, float('NaN'), 3.0],
                  [1.0, 2.0,          3.0]])
    print

    
    a = [1.0, float('NaN'), 3.0, float('NaN')]
    print a
    print replace_nans(a)