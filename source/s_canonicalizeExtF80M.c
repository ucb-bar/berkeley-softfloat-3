
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
| Equivalent to 'softfloat_canonicalizeExtF80' (see s_canonicalizeExtF80.c
| for the full description of extFloat80 non-canonical encodings and the
| canonicalization rules), but using 'int32_t' and 'uint64_t' parameter
| types to match the non-FAST_INT64 code paths.  The algorithm and behavior
| are identical.
*----------------------------------------------------------------------------*/
void
 softfloat_canonicalizeExtF80M( int32_t *expPtr, uint64_t *sigPtr )
{
    int32_t exp;
    uint64_t sig;
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
