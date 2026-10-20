
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
| Canonicalizes an extFloat80 value whose exponent and sign are stored in the
| packed 'signExp' layout used by the non-FAST_INT64 comparison functions.
| The packed 16-bit field '*signExpPtr' holds the sign in bit 15 and the
| 15-bit biased exponent in bits 14:0.  This function extracts the exponent,
| passes it along with '*sigPtr' to 'softfloat_canonicalizeExtF80M' (see
| s_canonicalizeExtF80.c for the full canonicalization rules), and repacks
| the possibly-modified exponent back into '*signExpPtr' with the original
| sign bit preserved.
*----------------------------------------------------------------------------*/
void
 softfloat_canonicalizeExtF80MUI(
     uint_fast16_t *signExpPtr, uint64_t *sigPtr )
{
    int32_t exp = expExtF80UI64( *signExpPtr );

    softfloat_canonicalizeExtF80M( &exp, sigPtr );
    *signExpPtr = packToExtF80UI64( signExtF80UI64( *signExpPtr ), exp );

}
