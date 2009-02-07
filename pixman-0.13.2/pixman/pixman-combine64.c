/* WARNING: This file is generated by combine.pl from combine.inc.
   Please edit one of those files rather than this one. */

#line 1 "combine.inc"
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>

#include "pixman-private.h"

#include "pixman-combine64.h"

/*
 * There are two ways of handling alpha -- either as a single unified value or
 * a separate value for each component, hence each macro must have two
 * versions.  The unified alpha version has a 'U' at the end of the name,
 * the component version has a 'C'.  Similarly, functions which deal with
 * this difference will have two versions using the same convention.
 */


/*
 * Combine src and mask
 */
FASTCALL static void
pixman_fbCombineMaskU (uint64_t *src, const uint64_t *mask, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        uint64_t a = *(mask + i) >> A_SHIFT;
        uint64_t s = *(src + i);
        FbByteMul(s, a);
        *(src + i) = s;
    }
}

/*
 * All of the composing functions
 */

FASTCALL static void
fbCombineClear (uint64_t *dest, const uint64_t *src, int width)
{
    memset(dest, 0, width*sizeof(uint64_t));
}

FASTCALL static void
fbCombineSrcU (uint64_t *dest, const uint64_t *src, int width)
{
    memcpy(dest, src, width*sizeof(uint64_t));
}

/* if the Src is opaque, call fbCombineSrcU */
FASTCALL static void
fbCombineOverU (uint64_t *dest, const uint64_t *src, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        uint64_t s = *(src + i);
        uint64_t d = *(dest + i);
        uint64_t ia = Alpha(~s);

        FbByteMulAdd(d, ia, s);
	*(dest + i) = d;
    }
}

/* if the Dst is opaque, this is a noop */
FASTCALL static void
fbCombineOverReverseU (uint64_t *dest, const uint64_t *src, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        uint64_t s = *(src + i);
        uint64_t d = *(dest + i);
        uint64_t ia = Alpha(~*(dest + i));
        FbByteMulAdd(s, ia, d);
	*(dest + i) = s;
    }
}

/* if the Dst is opaque, call fbCombineSrcU */
FASTCALL static void
fbCombineInU (uint64_t *dest, const uint64_t *src, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        uint64_t s = *(src + i);
        uint64_t a = Alpha(*(dest + i));
        FbByteMul(s, a);
	*(dest + i) = s;
    }
}

/* if the Src is opaque, this is a noop */
FASTCALL static void
fbCombineInReverseU (uint64_t *dest, const uint64_t *src, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        uint64_t d = *(dest + i);
        uint64_t a = Alpha(*(src + i));
        FbByteMul(d, a);
	*(dest + i) = d;
    }
}

/* if the Dst is opaque, call fbCombineClear */
FASTCALL static void
fbCombineOutU (uint64_t *dest, const uint64_t *src, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        uint64_t s = *(src + i);
        uint64_t a = Alpha(~*(dest + i));
        FbByteMul(s, a);
	*(dest + i) = s;
    }
}

/* if the Src is opaque, call fbCombineClear */
FASTCALL static void
fbCombineOutReverseU (uint64_t *dest, const uint64_t *src, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        uint64_t d = *(dest + i);
        uint64_t a = Alpha(~*(src + i));
        FbByteMul(d, a);
	*(dest + i) = d;
    }
}

/* if the Src is opaque, call fbCombineInU */
/* if the Dst is opaque, call fbCombineOverU */
/* if both the Src and Dst are opaque, call fbCombineSrcU */
FASTCALL static void
fbCombineAtopU (uint64_t *dest, const uint64_t *src, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        uint64_t s = *(src + i);
        uint64_t d = *(dest + i);
        uint64_t dest_a = Alpha(d);
        uint64_t src_ia = Alpha(~s);

        FbByteAddMul(s, dest_a, d, src_ia);
	*(dest + i) = s;
    }
}

/* if the Src is opaque, call fbCombineOverReverseU */
/* if the Dst is opaque, call fbCombineInReverseU */
/* if both the Src and Dst are opaque, call fbCombineDstU */
FASTCALL static void
fbCombineAtopReverseU (uint64_t *dest, const uint64_t *src, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        uint64_t s = *(src + i);
        uint64_t d = *(dest + i);
        uint64_t src_a = Alpha(s);
        uint64_t dest_ia = Alpha(~d);

        FbByteAddMul(s, dest_ia, d, src_a);
	*(dest + i) = s;
    }
}

/* if the Src is opaque, call fbCombineOverU */
/* if the Dst is opaque, call fbCombineOverReverseU */
/* if both the Src and Dst are opaque, call fbCombineClear */
FASTCALL static void
fbCombineXorU (uint64_t *dest, const uint64_t *src, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        uint64_t s = *(src + i);
        uint64_t d = *(dest + i);
        uint64_t src_ia = Alpha(~s);
        uint64_t dest_ia = Alpha(~d);

        FbByteAddMul(s, dest_ia, d, src_ia);
	*(dest + i) = s;
    }
}

FASTCALL static void
fbCombineAddU (uint64_t *dest, const uint64_t *src, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        uint64_t s = *(src + i);
        uint64_t d = *(dest + i);
        FbByteAdd(d, s);
	*(dest + i) = d;
    }
}

/* if the Src is opaque, call fbCombineAddU */
/* if the Dst is opaque, call fbCombineAddU */
/* if both the Src and Dst are opaque, call fbCombineAddU */
FASTCALL static void
fbCombineSaturateU (uint64_t *dest, const uint64_t *src, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        uint64_t s = *(src + i);
        uint64_t d = *(dest + i);
        uint32_t sa, da;

        sa = s >> A_SHIFT;
        da = ~d >> A_SHIFT;
        if (sa > da)
        {
            sa = IntDiv(da, sa);
            FbByteMul(s, sa);
        };
        FbByteAdd(d, s);
	*(dest + i) = d;
    }
}


/*
 * All of the disjoint composing functions

 The four entries in the first column indicate what source contributions
 come from each of the four areas of the picture -- areas covered by neither
 A nor B, areas covered only by A, areas covered only by B and finally
 areas covered by both A and B.

 Disjoint			Conjoint
 Fa		Fb		Fa		Fb
 (0,0,0,0)	0		0		0		0
 (0,A,0,A)	1		0		1		0
 (0,0,B,B)	0		1		0		1
 (0,A,B,A)	1		min((1-a)/b,1)	1		max(1-a/b,0)
 (0,A,B,B)	min((1-b)/a,1)	1		max(1-b/a,0)	1
 (0,0,0,A)	max(1-(1-b)/a,0) 0		min(1,b/a)	0
 (0,0,0,B)	0		max(1-(1-a)/b,0) 0		min(a/b,1)
 (0,A,0,0)	min(1,(1-b)/a)	0		max(1-b/a,0)	0
 (0,0,B,0)	0		min(1,(1-a)/b)	0		max(1-a/b,0)
 (0,0,B,A)	max(1-(1-b)/a,0) min(1,(1-a)/b)	 min(1,b/a)	max(1-a/b,0)
 (0,A,0,B)	min(1,(1-b)/a)	max(1-(1-a)/b,0) max(1-b/a,0)	min(1,a/b)
 (0,A,B,0)	min(1,(1-b)/a)	min(1,(1-a)/b)	max(1-b/a,0)	max(1-a/b,0)

*/

#define CombineAOut 1
#define CombineAIn  2
#define CombineBOut 4
#define CombineBIn  8

#define CombineClear	0
#define CombineA	(CombineAOut|CombineAIn)
#define CombineB	(CombineBOut|CombineBIn)
#define CombineAOver	(CombineAOut|CombineBOut|CombineAIn)
#define CombineBOver	(CombineAOut|CombineBOut|CombineBIn)
#define CombineAAtop	(CombineBOut|CombineAIn)
#define CombineBAtop	(CombineAOut|CombineBIn)
#define CombineXor	(CombineAOut|CombineBOut)

/* portion covered by a but not b */
FASTCALL static uint16_t
fbCombineDisjointOutPart (uint16_t a, uint16_t b)
{
    /* min (1, (1-b) / a) */

    b = ~b;		    /* 1 - b */
    if (b >= a)		    /* 1 - b >= a -> (1-b)/a >= 1 */
	return MASK;	    /* 1 */
    return IntDiv(b,a);     /* (1-b) / a */
}

/* portion covered by both a and b */
FASTCALL static uint16_t
fbCombineDisjointInPart (uint16_t a, uint16_t b)
{
    /* max (1-(1-b)/a,0) */
    /*  = - min ((1-b)/a - 1, 0) */
    /*  = 1 - min (1, (1-b)/a) */

    b = ~b;		    /* 1 - b */
    if (b >= a)		    /* 1 - b >= a -> (1-b)/a >= 1 */
	return 0;	    /* 1 - 1 */
    return ~IntDiv(b,a);    /* 1 - (1-b) / a */
}

/* portion covered by a but not b */
FASTCALL static uint16_t
fbCombineConjointOutPart (uint16_t a, uint16_t b)
{
    /* max (1-b/a,0) */
    /* = 1-min(b/a,1) */

    /* min (1, (1-b) / a) */

    if (b >= a)		    /* b >= a -> b/a >= 1 */
	return 0x00;	    /* 0 */
    return ~IntDiv(b,a);    /* 1 - b/a */
}

/* portion covered by both a and b */
FASTCALL static uint16_t
fbCombineConjointInPart (uint16_t a, uint16_t b)
{
    /* min (1,b/a) */

    if (b >= a)		    /* b >= a -> b/a >= 1 */
	return MASK;	    /* 1 */
    return IntDiv(b,a);     /* b/a */
}

FASTCALL static void
fbCombineDisjointGeneralU (uint64_t *dest, const uint64_t *src, int width, uint16_t combine)
{
    int i;
    for (i = 0; i < width; ++i) {
        uint64_t s = *(src + i);
        uint64_t d = *(dest + i);
        uint64_t m,n,o,p;
        uint32_t Fa, Fb, t, u, v;
        uint16_t sa = s >> A_SHIFT;
        uint16_t da = d >> A_SHIFT;

        switch (combine & CombineA) {
        default:
            Fa = 0;
            break;
        case CombineAOut:
            Fa = fbCombineDisjointOutPart (sa, da);
            break;
        case CombineAIn:
            Fa = fbCombineDisjointInPart (sa, da);
            break;
        case CombineA:
            Fa = MASK;
            break;
        }

        switch (combine & CombineB) {
        default:
            Fb = 0;
            break;
        case CombineBOut:
            Fb = fbCombineDisjointOutPart (da, sa);
            break;
        case CombineBIn:
            Fb = fbCombineDisjointInPart (da, sa);
            break;
        case CombineB:
            Fb = MASK;
            break;
        }
        m = FbGen (s,d,0,Fa,Fb,t, u, v);
        n = FbGen (s,d,G_SHIFT,Fa,Fb,t, u, v);
        o = FbGen (s,d,R_SHIFT,Fa,Fb,t, u, v);
        p = FbGen (s,d,A_SHIFT,Fa,Fb,t, u, v);
        s = m|n|o|p;
	*(dest + i) = s;
    }
}

FASTCALL static void
fbCombineDisjointOverU (uint64_t *dest, const uint64_t *src, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        uint64_t s = *(src + i);
        uint32_t a = s >> A_SHIFT;

        if (a != 0x00)
        {
            if (a != MASK)
            {
                uint64_t d = *(dest + i);
                a = fbCombineDisjointOutPart (d >> A_SHIFT, a);
                FbByteMulAdd(d, a, s);
                s = d;
            }
	    *(dest + i) = s;
        }
    }
}

FASTCALL static void
fbCombineDisjointInU (uint64_t *dest, const uint64_t *src, int width)
{
    fbCombineDisjointGeneralU (dest, src, width, CombineAIn);
}

FASTCALL static void
fbCombineDisjointInReverseU (uint64_t *dest, const uint64_t *src, int width)
{
    fbCombineDisjointGeneralU (dest, src, width, CombineBIn);
}

FASTCALL static void
fbCombineDisjointOutU (uint64_t *dest, const uint64_t *src, int width)
{
    fbCombineDisjointGeneralU (dest, src, width, CombineAOut);
}

FASTCALL static void
fbCombineDisjointOutReverseU (uint64_t *dest, const uint64_t *src, int width)
{
    fbCombineDisjointGeneralU (dest, src, width, CombineBOut);
}

FASTCALL static void
fbCombineDisjointAtopU (uint64_t *dest, const uint64_t *src, int width)
{
    fbCombineDisjointGeneralU (dest, src, width, CombineAAtop);
}

FASTCALL static void
fbCombineDisjointAtopReverseU (uint64_t *dest, const uint64_t *src, int width)
{
    fbCombineDisjointGeneralU (dest, src, width, CombineBAtop);
}

FASTCALL static void
fbCombineDisjointXorU (uint64_t *dest, const uint64_t *src, int width)
{
    fbCombineDisjointGeneralU (dest, src, width, CombineXor);
}

FASTCALL static void
fbCombineConjointGeneralU (uint64_t *dest, const uint64_t *src, int width, uint16_t combine)
{
    int i;
    for (i = 0; i < width; ++i) {
        uint64_t s = *(src + i);
        uint64_t d = *(dest + i);
        uint64_t m,n,o,p;
        uint32_t Fa, Fb, t, u, v;
        uint16_t sa = s >> A_SHIFT;
        uint16_t da = d >> A_SHIFT;

        switch (combine & CombineA) {
        default:
            Fa = 0;
            break;
        case CombineAOut:
            Fa = fbCombineConjointOutPart (sa, da);
            break;
        case CombineAIn:
            Fa = fbCombineConjointInPart (sa, da);
            break;
        case CombineA:
            Fa = MASK;
            break;
        }

        switch (combine & CombineB) {
        default:
            Fb = 0;
            break;
        case CombineBOut:
            Fb = fbCombineConjointOutPart (da, sa);
            break;
        case CombineBIn:
            Fb = fbCombineConjointInPart (da, sa);
            break;
        case CombineB:
            Fb = MASK;
            break;
        }
        m = FbGen (s,d,0,Fa,Fb,t, u, v);
        n = FbGen (s,d,G_SHIFT,Fa,Fb,t, u, v);
        o = FbGen (s,d,R_SHIFT,Fa,Fb,t, u, v);
        p = FbGen (s,d,A_SHIFT,Fa,Fb,t, u, v);
        s = m|n|o|p;
	*(dest + i) = s;
    }
}

FASTCALL static void
fbCombineConjointOverU (uint64_t *dest, const uint64_t *src, int width)
{
    fbCombineConjointGeneralU (dest, src, width, CombineAOver);
}


FASTCALL static void
fbCombineConjointOverReverseU (uint64_t *dest, const uint64_t *src, int width)
{
    fbCombineConjointGeneralU (dest, src, width, CombineBOver);
}


FASTCALL static void
fbCombineConjointInU (uint64_t *dest, const uint64_t *src, int width)
{
    fbCombineConjointGeneralU (dest, src, width, CombineAIn);
}


FASTCALL static void
fbCombineConjointInReverseU (uint64_t *dest, const uint64_t *src, int width)
{
    fbCombineConjointGeneralU (dest, src, width, CombineBIn);
}

FASTCALL static void
fbCombineConjointOutU (uint64_t *dest, const uint64_t *src, int width)
{
    fbCombineConjointGeneralU (dest, src, width, CombineAOut);
}

FASTCALL static void
fbCombineConjointOutReverseU (uint64_t *dest, const uint64_t *src, int width)
{
    fbCombineConjointGeneralU (dest, src, width, CombineBOut);
}

FASTCALL static void
fbCombineConjointAtopU (uint64_t *dest, const uint64_t *src, int width)
{
    fbCombineConjointGeneralU (dest, src, width, CombineAAtop);
}

FASTCALL static void
fbCombineConjointAtopReverseU (uint64_t *dest, const uint64_t *src, int width)
{
    fbCombineConjointGeneralU (dest, src, width, CombineBAtop);
}

FASTCALL static void
fbCombineConjointXorU (uint64_t *dest, const uint64_t *src, int width)
{
    fbCombineConjointGeneralU (dest, src, width, CombineXor);
}

/********************************************************************************/
/*************************** Per Channel functions ******************************/
/********************************************************************************/

FASTCALL static void
fbCombineMaskC (uint64_t *src, uint64_t *mask)
{
    uint64_t a = *mask;

    uint64_t	x;
    uint32_t	xa;

    if (!a)
    {
	*(src) = 0;
	return;
    }

    x = *(src);
    if (a == ~0)
    {
	x = x >> A_SHIFT;
	x |= x << G_SHIFT;
	x |= x << R_SHIFT;
	*(mask) = x;
	return;
    }

    xa = x >> A_SHIFT;
    FbByteMulC(x, a);
    *(src) = x;
    FbByteMul(a, xa);
    *(mask) = a;
}

FASTCALL static void
fbCombineMaskValueC (uint64_t *src, const uint64_t *mask)
{
    uint64_t a = *mask;
    uint64_t	x;

    if (!a)
    {
	*(src) = 0;
	return;
    }

    if (a == ~0)
	return;

    x = *(src);
    FbByteMulC(x, a);
    *(src) =x;
}

FASTCALL static void
fbCombineMaskAlphaC (const uint64_t *src, uint64_t *mask)
{
    uint64_t a = *(mask);
    uint64_t	x;

    if (!a)
	return;

    x = *(src) >> A_SHIFT;
    if (x == MASK)
	return;
    if (a == ~0)
    {
	x = x >> A_SHIFT;
	x |= x << G_SHIFT;
	x |= x << R_SHIFT;
	*(mask) = x;
	return;
    }

    FbByteMul(a, x);
    *(mask) = a;
}



FASTCALL static void
fbCombineClearC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    memset(dest, 0, width*sizeof(uint64_t));
}

FASTCALL static void
fbCombineSrcC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
	uint64_t s = *(src + i);
	uint64_t m = *(mask + i);

	fbCombineMaskValueC (&s, &m);

	*(dest) = s;
    }
}

FASTCALL static void
fbCombineOverC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
	uint64_t s = *(src + i);
	uint64_t m = *(mask + i);
	uint64_t a;

	fbCombineMaskC (&s, &m);

	a = ~m;
        if (a != ~0)
        {
            if (a)
            {
                uint64_t d = *(dest + i);
                FbByteMulAddC(d, a, s);
                s = d;
            }
	    *(dest + i) = s;
        }
    }
}

FASTCALL static void
fbCombineOverReverseC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        uint64_t d = *(dest + i);
        uint64_t a = ~d >> A_SHIFT;

        if (a)
        {
            uint64_t s = *(src + i);
	    uint64_t m = *(mask + i);

	    fbCombineMaskValueC (&s, &m);

            if (a != MASK)
            {
                FbByteMulAdd(s, a, d);
            }
	    *(dest + i) = s;
        }
    }
}

FASTCALL static void
fbCombineInC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        uint64_t d = *(dest + i);
        uint32_t a = d >> A_SHIFT;
        uint64_t s = 0;
        if (a)
        {
	    uint64_t m = *(mask + i);

	    s = *(src + i);
	    fbCombineMaskValueC (&s, &m);
            if (a != MASK)
            {
                FbByteMul(s, a);
            }
        }
	*(dest + i) = s;
    }
}

FASTCALL static void
fbCombineInReverseC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        uint64_t s = *(src + i);
        uint64_t m = *(mask + i);
        uint64_t a;

	fbCombineMaskAlphaC (&s, &m);

	a = m;
        if (a != ~0)
        {
            uint64_t d = 0;
            if (a)
            {
                d = *(dest + i);
                FbByteMulC(d, a);
            }
	    *(dest + i) = d;
        }
    }
}

FASTCALL static void
fbCombineOutC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        uint64_t d = *(dest + i);
        uint32_t a = ~d >> A_SHIFT;
        uint64_t s = 0;
        if (a)
        {
	    uint64_t m = *(mask + i);

	    s = *(src + i);
	    fbCombineMaskValueC (&s, &m);

            if (a != MASK)
            {
                FbByteMul(s, a);
            }
        }
	*(dest + i) = s;
    }
}

FASTCALL static void
fbCombineOutReverseC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
	uint64_t s = *(src + i);
	uint64_t m = *(mask + i);
	uint64_t a;

	fbCombineMaskAlphaC (&s, &m);

        a = ~m;
        if (a != ~0)
        {
            uint64_t d = 0;
            if (a)
            {
                d = *(dest + i);
                FbByteMulC(d, a);
            }
	    *(dest + i) = d;
        }
    }
}

FASTCALL static void
fbCombineAtopC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        uint64_t d = *(dest + i);
        uint64_t s = *(src + i);
        uint64_t m = *(mask + i);
        uint64_t ad;
        uint32_t as = d >> A_SHIFT;

	fbCombineMaskC (&s, &m);

        ad = ~m;

        FbByteAddMulC(d, ad, s, as);
	*(dest + i) = d;
    }
}

FASTCALL static void
fbCombineAtopReverseC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {

        uint64_t d = *(dest + i);
        uint64_t s = *(src + i);
        uint64_t m = *(mask + i);
        uint64_t ad;
        uint32_t as = ~d >> A_SHIFT;

	fbCombineMaskC (&s, &m);

	ad = m;

        FbByteAddMulC(d, ad, s, as);
	*(dest + i) = d;
    }
}

FASTCALL static void
fbCombineXorC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        uint64_t d = *(dest + i);
        uint64_t s = *(src + i);
        uint64_t m = *(mask + i);
        uint64_t ad;
        uint32_t as = ~d >> A_SHIFT;

	fbCombineMaskC (&s, &m);

	ad = ~m;

        FbByteAddMulC(d, ad, s, as);
	*(dest + i) = d;
    }
}

FASTCALL static void
fbCombineAddC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        uint64_t s = *(src + i);
        uint64_t m = *(mask + i);
        uint64_t d = *(dest + i);

	fbCombineMaskValueC (&s, &m);

        FbByteAdd(d, s);
	*(dest + i) = d;
    }
}

FASTCALL static void
fbCombineSaturateC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        uint64_t s, d;
        uint32_t sa, sr, sg, sb, da;
        uint32_t t, u, v;
        uint64_t m,n,o,p;

        d = *(dest + i);
        s = *(src + i);
	m = *(mask + i);

	fbCombineMaskC (&s, &m);

        sa = (m >> A_SHIFT);
        sr = (m >> R_SHIFT) & MASK;
        sg = (m >> G_SHIFT) & MASK;
        sb =  m             & MASK;
        da = ~d >> A_SHIFT;

        if (sb <= da)
            m = Add(s,d,0,t);
        else
            m = FbGen (s, d, 0, (da << G_SHIFT) / sb, MASK, t, u, v);

        if (sg <= da)
            n = Add(s,d,G_SHIFT,t);
        else
            n = FbGen (s, d, G_SHIFT, (da << G_SHIFT) / sg, MASK, t, u, v);

        if (sr <= da)
            o = Add(s,d,R_SHIFT,t);
        else
            o = FbGen (s, d, R_SHIFT, (da << G_SHIFT) / sr, MASK, t, u, v);

        if (sa <= da)
            p = Add(s,d,A_SHIFT,t);
        else
            p = FbGen (s, d, A_SHIFT, (da << G_SHIFT) / sa, MASK, t, u, v);

	*(dest + i) = m|n|o|p;
    }
}

FASTCALL static void
fbCombineDisjointGeneralC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width, uint16_t combine)
{
    int i;

    for (i = 0; i < width; ++i) {
        uint64_t s, d;
        uint64_t m,n,o,p;
        uint64_t Fa, Fb;
        uint32_t t, u, v;
        uint64_t sa;
        uint16_t da;

        s = *(src + i);
        m = *(mask + i);
        d = *(dest + i);
        da = d >> A_SHIFT;

	fbCombineMaskC (&s, &m);

	sa = m;

        switch (combine & CombineA) {
        default:
            Fa = 0;
            break;
        case CombineAOut:
            m = (uint64_t)fbCombineDisjointOutPart ((uint16_t) (sa >> 0), da);
            n = (uint64_t)fbCombineDisjointOutPart ((uint16_t) (sa >> G_SHIFT), da) << G_SHIFT;
            o = (uint64_t)fbCombineDisjointOutPart ((uint16_t) (sa >> R_SHIFT), da) << R_SHIFT;
            p = (uint64_t)fbCombineDisjointOutPart ((uint16_t) (sa >> A_SHIFT), da) << A_SHIFT;
            Fa = m|n|o|p;
            break;
        case CombineAIn:
            m = (uint64_t)fbCombineDisjointInPart ((uint16_t) (sa >> 0), da);
            n = (uint64_t)fbCombineDisjointInPart ((uint16_t) (sa >> G_SHIFT), da) << G_SHIFT;
            o = (uint64_t)fbCombineDisjointInPart ((uint16_t) (sa >> R_SHIFT), da) << R_SHIFT;
            p = (uint64_t)fbCombineDisjointInPart ((uint16_t) (sa >> A_SHIFT), da) << A_SHIFT;
            Fa = m|n|o|p;
            break;
        case CombineA:
            Fa = ~0;
            break;
        }

        switch (combine & CombineB) {
        default:
            Fb = 0;
            break;
        case CombineBOut:
            m = (uint64_t)fbCombineDisjointOutPart (da, (uint16_t) (sa >> 0));
            n = (uint64_t)fbCombineDisjointOutPart (da, (uint16_t) (sa >> G_SHIFT)) << G_SHIFT;
            o = (uint64_t)fbCombineDisjointOutPart (da, (uint16_t) (sa >> R_SHIFT)) << R_SHIFT;
            p = (uint64_t)fbCombineDisjointOutPart (da, (uint16_t) (sa >> A_SHIFT)) << A_SHIFT;
            Fb = m|n|o|p;
            break;
        case CombineBIn:
            m = (uint64_t)fbCombineDisjointInPart (da, (uint16_t) (sa >> 0));
            n = (uint64_t)fbCombineDisjointInPart (da, (uint16_t) (sa >> G_SHIFT)) << G_SHIFT;
            o = (uint64_t)fbCombineDisjointInPart (da, (uint16_t) (sa >> R_SHIFT)) << R_SHIFT;
            p = (uint64_t)fbCombineDisjointInPart (da, (uint16_t) (sa >> A_SHIFT)) << A_SHIFT;
            Fb = m|n|o|p;
            break;
        case CombineB:
            Fb = ~0;
            break;
        }
        m = FbGen (s,d,0,GetComp(Fa,0),GetComp(Fb,0),t, u, v);
        n = FbGen (s,d,G_SHIFT,GetComp(Fa,G_SHIFT),GetComp(Fb,G_SHIFT),t, u, v);
        o = FbGen (s,d,R_SHIFT,GetComp(Fa,R_SHIFT),GetComp(Fb,R_SHIFT),t, u, v);
        p = FbGen (s,d,A_SHIFT,GetComp(Fa,A_SHIFT),GetComp(Fb,A_SHIFT),t, u, v);
        s = m|n|o|p;
	*(dest + i) = s;
    }
}

FASTCALL static void
fbCombineDisjointOverC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    fbCombineDisjointGeneralC (dest, src, mask, width, CombineAOver);
}

FASTCALL static void
fbCombineDisjointInC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    fbCombineDisjointGeneralC (dest, src, mask, width, CombineAIn);
}

FASTCALL static void
fbCombineDisjointInReverseC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    fbCombineDisjointGeneralC (dest, src, mask, width, CombineBIn);
}

FASTCALL static void
fbCombineDisjointOutC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    fbCombineDisjointGeneralC (dest, src, mask, width, CombineAOut);
}

FASTCALL static void
fbCombineDisjointOutReverseC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    fbCombineDisjointGeneralC (dest, src, mask, width, CombineBOut);
}

FASTCALL static void
fbCombineDisjointAtopC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    fbCombineDisjointGeneralC (dest, src, mask, width, CombineAAtop);
}

FASTCALL static void
fbCombineDisjointAtopReverseC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    fbCombineDisjointGeneralC (dest, src, mask, width, CombineBAtop);
}

FASTCALL static void
fbCombineDisjointXorC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    fbCombineDisjointGeneralC (dest, src, mask, width, CombineXor);
}

FASTCALL static void
fbCombineConjointGeneralC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width, uint16_t combine)
{
    int i;

    for (i = 0; i < width; ++i) {
        uint64_t s, d;
        uint64_t m,n,o,p;
        uint64_t Fa, Fb;
        uint32_t t, u, v;
        uint64_t sa;
        uint16_t da;

        s = *(src + i);
        m = *(mask + i);
        d = *(dest + i);
        da = d >> A_SHIFT;

	fbCombineMaskC (&s, &m);

        sa = m;

        switch (combine & CombineA) {
        default:
            Fa = 0;
            break;
        case CombineAOut:
            m = (uint64_t)fbCombineConjointOutPart ((uint16_t) (sa >> 0), da);
            n = (uint64_t)fbCombineConjointOutPart ((uint16_t) (sa >> G_SHIFT), da) << G_SHIFT;
            o = (uint64_t)fbCombineConjointOutPart ((uint16_t) (sa >> R_SHIFT), da) << R_SHIFT;
            p = (uint64_t)fbCombineConjointOutPart ((uint16_t) (sa >> A_SHIFT), da) << A_SHIFT;
            Fa = m|n|o|p;
            break;
        case CombineAIn:
            m = (uint64_t)fbCombineConjointInPart ((uint16_t) (sa >> 0), da);
            n = (uint64_t)fbCombineConjointInPart ((uint16_t) (sa >> G_SHIFT), da) << G_SHIFT;
            o = (uint64_t)fbCombineConjointInPart ((uint16_t) (sa >> R_SHIFT), da) << R_SHIFT;
            p = (uint64_t)fbCombineConjointInPart ((uint16_t) (sa >> A_SHIFT), da) << A_SHIFT;
            Fa = m|n|o|p;
            break;
        case CombineA:
            Fa = ~0;
            break;
        }

        switch (combine & CombineB) {
        default:
            Fb = 0;
            break;
        case CombineBOut:
            m = (uint64_t)fbCombineConjointOutPart (da, (uint16_t) (sa >> 0));
            n = (uint64_t)fbCombineConjointOutPart (da, (uint16_t) (sa >> G_SHIFT)) << G_SHIFT;
            o = (uint64_t)fbCombineConjointOutPart (da, (uint16_t) (sa >> R_SHIFT)) << R_SHIFT;
            p = (uint64_t)fbCombineConjointOutPart (da, (uint16_t) (sa >> A_SHIFT)) << A_SHIFT;
            Fb = m|n|o|p;
            break;
        case CombineBIn:
            m = (uint64_t)fbCombineConjointInPart (da, (uint16_t) (sa >> 0));
            n = (uint64_t)fbCombineConjointInPart (da, (uint16_t) (sa >> G_SHIFT)) << G_SHIFT;
            o = (uint64_t)fbCombineConjointInPart (da, (uint16_t) (sa >> R_SHIFT)) << R_SHIFT;
            p = (uint64_t)fbCombineConjointInPart (da, (uint16_t) (sa >> A_SHIFT)) << A_SHIFT;
            Fb = m|n|o|p;
            break;
        case CombineB:
            Fb = ~0;
            break;
        }
        m = FbGen (s,d,0,GetComp(Fa,0),GetComp(Fb,0),t, u, v);
        n = FbGen (s,d,G_SHIFT,GetComp(Fa,G_SHIFT),GetComp(Fb,G_SHIFT),t, u, v);
        o = FbGen (s,d,R_SHIFT,GetComp(Fa,R_SHIFT),GetComp(Fb,R_SHIFT),t, u, v);
        p = FbGen (s,d,A_SHIFT,GetComp(Fa,A_SHIFT),GetComp(Fb,A_SHIFT),t, u, v);
        s = m|n|o|p;
	*(dest + i) = s;
    }
}

FASTCALL static void
fbCombineConjointOverC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    fbCombineConjointGeneralC (dest, src, mask, width, CombineAOver);
}

FASTCALL static void
fbCombineConjointOverReverseC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    fbCombineConjointGeneralC (dest, src, mask, width, CombineBOver);
}

FASTCALL static void
fbCombineConjointInC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    fbCombineConjointGeneralC (dest, src, mask, width, CombineAIn);
}

FASTCALL static void
fbCombineConjointInReverseC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    fbCombineConjointGeneralC (dest, src, mask, width, CombineBIn);
}

FASTCALL static void
fbCombineConjointOutC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    fbCombineConjointGeneralC (dest, src, mask, width, CombineAOut);
}

FASTCALL static void
fbCombineConjointOutReverseC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    fbCombineConjointGeneralC (dest, src, mask, width, CombineBOut);
}

FASTCALL static void
fbCombineConjointAtopC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    fbCombineConjointGeneralC (dest, src, mask, width, CombineAAtop);
}

FASTCALL static void
fbCombineConjointAtopReverseC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    fbCombineConjointGeneralC (dest, src, mask, width, CombineBAtop);
}

FASTCALL static void
fbCombineConjointXorC (uint64_t *dest, uint64_t *src, uint64_t *mask, int width)
{
    fbCombineConjointGeneralC (dest, src, mask, width, CombineXor);
}

static const CombineFuncU64 pixman_fbCombineFuncU[] = {
    fbCombineClear,
    fbCombineSrcU,
    NULL, /* CombineDst */
    fbCombineOverU,
    fbCombineOverReverseU,
    fbCombineInU,
    fbCombineInReverseU,
    fbCombineOutU,
    fbCombineOutReverseU,
    fbCombineAtopU,
    fbCombineAtopReverseU,
    fbCombineXorU,
    fbCombineAddU,
    fbCombineSaturateU,
    NULL,
    NULL,
    fbCombineClear,
    fbCombineSrcU,
    NULL, /* CombineDst */
    fbCombineDisjointOverU,
    fbCombineSaturateU, /* DisjointOverReverse */
    fbCombineDisjointInU,
    fbCombineDisjointInReverseU,
    fbCombineDisjointOutU,
    fbCombineDisjointOutReverseU,
    fbCombineDisjointAtopU,
    fbCombineDisjointAtopReverseU,
    fbCombineDisjointXorU,
    NULL,
    NULL,
    NULL,
    NULL,
    fbCombineClear,
    fbCombineSrcU,
    NULL, /* CombineDst */
    fbCombineConjointOverU,
    fbCombineConjointOverReverseU,
    fbCombineConjointInU,
    fbCombineConjointInReverseU,
    fbCombineConjointOutU,
    fbCombineConjointOutReverseU,
    fbCombineConjointAtopU,
    fbCombineConjointAtopReverseU,
    fbCombineConjointXorU,
};

static const CombineFuncC64 pixman_fbCombineFuncC[] = {
    fbCombineClearC,
    fbCombineSrcC,
    NULL, /* Dest */
    fbCombineOverC,
    fbCombineOverReverseC,
    fbCombineInC,
    fbCombineInReverseC,
    fbCombineOutC,
    fbCombineOutReverseC,
    fbCombineAtopC,
    fbCombineAtopReverseC,
    fbCombineXorC,
    fbCombineAddC,
    fbCombineSaturateC,
    NULL,
    NULL,
    fbCombineClearC,	    /* 0x10 */
    fbCombineSrcC,
    NULL, /* Dest */
    fbCombineDisjointOverC,
    fbCombineSaturateC, /* DisjointOverReverse */
    fbCombineDisjointInC,
    fbCombineDisjointInReverseC,
    fbCombineDisjointOutC,
    fbCombineDisjointOutReverseC,
    fbCombineDisjointAtopC,
    fbCombineDisjointAtopReverseC,
    fbCombineDisjointXorC,  /* 0x1b */
    NULL,
    NULL,
    NULL,
    NULL,
    fbCombineClearC,
    fbCombineSrcC,
    NULL, /* Dest */
    fbCombineConjointOverC,
    fbCombineConjointOverReverseC,
    fbCombineConjointInC,
    fbCombineConjointInReverseC,
    fbCombineConjointOutC,
    fbCombineConjointOutReverseC,
    fbCombineConjointAtopC,
    fbCombineConjointAtopReverseC,
    fbCombineConjointXorC,
};

const FbComposeFunctions64 pixman_composeFunctions64 = {
    pixman_fbCombineFuncU,
    pixman_fbCombineFuncC,
    pixman_fbCombineMaskU
};
