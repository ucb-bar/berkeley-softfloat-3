
/*============================================================================

This C source file is part of the SoftFloat IEEE Floating-Point Arithmetic
Package, Release 3, by John R. Hauser.

Copyright 2011, 2012, 2013, 2014, 2015 The Regents of the University of
California (Regents).  All Rights Reserved.  Redistribution and use in source
and binary forms, with or without modification, are permitted provided that
the following conditions are met:

Redistributions of source code must retain the above copyright notice,
this list of conditions, and the following two paragraphs of disclaimer.
Redistributions in binary form must reproduce the above copyright notice,
this list of conditions, and the following two paragraphs of disclaimer in the
documentation and/or other materials provided with the distribution.  Neither
the name of the Regents nor the names of its contributors may be used to
endorse or promote products derived from this software without specific prior
written permission.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE.  THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
HEREUNDER IS PROVIDED "AS IS".  REGENTS HAS NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

=============================================================================*/

#include <stdint.h>
#include "platform.h"
#include "primitiveTypes.h"
#include "specialize.h"
#include "softfloat.h"

/*----------------------------------------------------------------------------
| Assuming at least one of the two 80-bit extended floating-point values
| pointed to by `aSPtr' and `bSPtr' is a NaN, stores the combined NaN result
| at the location pointed to by `zSPtr'.  If either original floating-point
| value is a signaling NaN, the invalid exception is raised.
*----------------------------------------------------------------------------*/
void
 softfloat_propagateNaNExtF80M(
     const struct extFloat80M *aSPtr,
     const struct extFloat80M *bSPtr,
     struct extFloat80M *zSPtr
 )
{
    uint_fast16_t ui64;
    uint_fast64_t ui0;

    ui64 = aSPtr->signExp;
    ui0  = aSPtr->signif;
    if (
        softfloat_isSigNaNExtF80UI( ui64, ui0 )
            || (bSPtr
                    && (ui64 = bSPtr->signExp,
                        ui0  = bSPtr->signif,
                        softfloat_isSigNaNExtF80UI( ui64, ui0 )))
    ) {
        softfloat_raiseFlags( softfloat_flag_invalid );
    }
    zSPtr->signExp = defaultNaNExtF80UI64;
    zSPtr->signif  = defaultNaNExtF80UI0;

}

