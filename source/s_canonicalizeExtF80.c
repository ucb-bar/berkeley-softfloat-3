
/*============================================================================

This C source file is part of the SoftFloat IEEE Floating-Point Arithmetic
Package, Release 3e, by John R. Hauser.  

Copyright 2011, 2012, 2013, 2014 The Regents of the University of California.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
    this list of conditions, and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions, and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

 3. Neither the name of the University nor the names of its contributors may
    be used to endorse or promote products derived from this software without
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS "AS IS", AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ARE
DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=============================================================================*/

/* This particular file was written by John Byrd. */

#include <stdint.h>
#include "platform.h"
#include "internals.h"

/*----------------------------------------------------------------------------
| The 80-bit extended double format (extFloat80) is unique among IEEE 754
| types in that it stores the integer bit of the significand explicitly as
| bit 63 of the 64-bit significand field, commonly called the 'J' bit.  In
| all other IEEE 754 binary formats, the integer bit is implicit and
| determined solely by the exponent.  Because the J bit is stored
| independently from the exponent in extFloat80, it is possible to construct
| encodings where the two are inconsistent.  These inconsistent encodings
| are called "non-canonical" or, on the 8087 through 80287, "unnormal".
| Intel no longer generates non-canonical encodings, but the 80-bit format
| is still defined to accept them.
|   This function rewrites the exponent '*expPtr' and significand '*sigPtr'
| of an extFloat80 value in place so that the encoding is canonical, i.e.,
| the J bit is consistent with the exponent.  The mathematical value
| represented by the encoding is preserved.  If the encoding is already
| canonical, the values are unchanged.
|   There are four classes of non-canonical encoding, each resolved as
| follows:
|
|   1. Unnormals (0 < exp < 0x7FFF, J = 0).  The significand is nonzero
|      but lacks its integer bit.  If the significand can be left-shifted
|      to set bit 63 without driving the biased exponent below 1, the value
|      is renormalized.  If the shift would require a biased exponent less
|      than 1, the value is flushed to a denormal (exp = 0) with whatever
|      significand bits survive the limited shift.  If the significand is
|      entirely zero, the exponent is set to 0 (encoding positive or
|      negative zero, depending on the sign bit, which is not touched here).
|
|   2. Pseudo-denormals (exp = 0, J = 1).  These represent the same value
|      as a biased exponent of 1, so the exponent is set to 1 and the
|      significand is left unchanged.
|
|   3. Pseudo-infinities (exp = 0x7FFF, J = 0, significand = 0).  The
|      J bit is forced to 1, producing the canonical infinity encoding.
|
|   4. Pseudo-NaNs (exp = 0x7FFF, J = 0, significand != 0).  The J bit
|      is forced to 1.  The remaining significand bits (the NaN payload)
|      are preserved, so quiet vs. signaling status is maintained.
|
|   Cases 3 and 4 are handled together: when the biased exponent is
|   0x7FFF, the J bit is unconditionally set.
|
|   This function does not modify the sign bit, which is not part of the
| exponent or significand fields passed here.  It also does not raise any
| floating-point exception flags; canonicalization is a silent rewrite of
| the encoding only.
*----------------------------------------------------------------------------*/
void
 softfloat_canonicalizeExtF80(
     int_fast32_t *expPtr, uint_fast64_t *sigPtr )
{
    int_fast32_t exp;
    uint_fast64_t sig;
    struct exp32_sig64 normExpSig;

    exp = *expPtr;
    sig = *sigPtr;
    if ( exp == 0x7FFF ) {
        *sigPtr = sig | UINT64_C( 0x8000000000000000 );
    } else if ( exp ) {
        if ( ! (sig & UINT64_C( 0x8000000000000000 )) ) {
            if ( ! sig ) {
                *expPtr = 0;
            } else {
                normExpSig = softfloat_normSubnormalExtF80Sig( sig );
                if ( exp + normExpSig.exp >= 1 ) {
                    *expPtr = exp + normExpSig.exp;
                    *sigPtr = normExpSig.sig;
                } else {
                    if ( exp > 1 ) *sigPtr = sig << (exp - 1);
                    *expPtr = 0;
                }
            }
        }
    } else {
        if ( sig & UINT64_C( 0x8000000000000000 ) )
            *expPtr = 1;
    }

}
