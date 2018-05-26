/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 */

/* GStreamer 
* Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu> 
*                    2000 Wim Taymans <wtay@chello.be> 
*                    2002 Thomas Vander Stichele <thomas@apestaart.org>
* 
* gstutils.c: Utility functions: gtk_get_property stuff, etc. 
* 
* This library is free software; you can redistribute it and/or 
* modify it under the terms of the GNU Library General Public 
* License as published by the Free Software Foundation; either 
* version 2 of the License, or (at your option) any later version. 
* 
* This library is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
* Library General Public License for more details. 
* 
* You should have received a copy of the GNU Library General Public 
* License along with this library; if not, write to the 
* Free Software Foundation, Inc., 59 Temple Place - Suite 330, 
* Boston, MA 02111-1307, USA. 
*/ 


/** @file 
 *
 *  math functions for clock device
 *
 *  author		gaius.lee
 *  version		1.0 
 *  date		2010.03.09
 *  note		Additional information. 
 *
 *  @addtogroup lg1150_clock 
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
//#include <asm-generic/div64.h>

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	function prototypes
 *----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Global variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	Driver functions for CLOCK IOCTL calls
 *----------------------------------------------------------------------------------------*/

/** ------------------------------------------------------------------------------------------------------------
 *  Copy from gstutil.c, GStreamer
 * 
 * Start
 */

/* multiply two 64-bit unsigned ints into a 128-bit unsigned int.  the high
 * and low 64 bits of the product are placed in c1 and c0 respectively.
 * this operation cannot overflow. */
static void CLK_MATH_uint64_mul_uint64 (ClkUInt64 * c1, ClkUInt64 * c0, UINT64 arg1,
    UINT64 arg2)
{
  ClkUInt64 a1, b0;
  ClkUInt64 v, n;

  /* prepare input */
  v.ll = arg1;
  n.ll = arg2;

  /* do 128 bits multiply
   *                   nh   nl
   *                *  vh   vl
   *                ----------
   * a0 =              vl * nl
   * a1 =         vl * nh
   * b0 =         vh * nl
   * b1 =  + vh * nh
   *       -------------------
   *        c1h  c1l  c0h  c0l
   *
   * "a0" is optimized away, result is stored directly in c0.  "b1" is
   * optimized away, result is stored directly in c1.
   */
  c0->ll = (UINT64) v.l.low * n.l.low;
  a1.ll = (UINT64) v.l.low * n.l.high;
  b0.ll = (UINT64) v.l.high * n.l.low;

  /* add the high word of a0 to the low words of a1 and b0 using c1 as
   * scrach space to capture the carry.  the low word of the result becomes
   * the final high word of c0 */
  c1->ll = (UINT64) c0->l.high + a1.l.low + b0.l.low;
  c0->l.high = c1->l.low;

  /* add the carry from the result above (found in the high word of c1) and
   * the high words of a1 and b0 to b1, the result is c1. */
  c1->ll = (UINT64) v.l.high * n.l.high + c1->l.high + a1.l.high + b0.l.high;
}

/* count leading zeros */
static UINT32 CLK_MATH_clz (UINT32 val)
{
  UINT32 s;

  s = val | (val >> 1);
  s |= (s >> 2);
  s |= (s >> 4);
  s |= (s >> 8);
  s = ~(s | (s >> 16));
  s = s - ((s >> 1) & 0x55555555);
  s = (s & 0x33333333) + ((s >> 2) & 0x33333333);
  s = (s + (s >> 4)) & 0x0f0f0f0f;
  s += (s >> 8);
  s = (s + (s >> 16)) & 0x3f;

  return s;
}
static UINT64 CLK_MATH_div64_32(UINT64 n, UINT32 div)
{
	do_div(n, div);
	return n;
}
/* based on Hacker's Delight p152 */
static UINT64 CLK_MATH_div128_64 (ClkUInt64 c1, ClkUInt64 c0, UINT64 denom)
{
  ClkUInt64 q1, q0, rhat;
  ClkUInt64 v, cmp1, cmp2;
  UINT32 s;

  v.ll = denom;

  /* count number of leading zeroes, we know they must be in the high
   * part of denom since denom > G_MAXUINT32. */
  s = CLK_MATH_clz (v.l.high);

  if (s > 0) {
    /* normalize divisor and dividend */
    v.ll <<= s;
    c1.ll = (c1.ll << s) | (c0.l.high >> (32 - s));
    c0.ll <<= s;
  }

//  q1.ll = c1.ll / v.l.high;
  q1.ll = CLK_MATH_div64_32(c1.ll, v.l.high);
  
  rhat.ll = c1.ll - q1.ll * v.l.high;

  cmp1.l.high = rhat.l.low;
  cmp1.l.low = c0.l.high;
  cmp2.ll = q1.ll * v.l.low;

  while (q1.l.high || cmp2.ll > cmp1.ll) {
    q1.ll--;
    rhat.ll += v.l.high;
    if (rhat.l.high)
      break;
    cmp1.l.high = rhat.l.low;
    cmp2.ll -= v.l.low;
  }
  c1.l.high = c1.l.low;
  c1.l.low = c0.l.high;
  c1.ll -= q1.ll * v.ll;	
//  q0.ll = c1.ll / v.l.high;
  q0.ll = CLK_MATH_div64_32(c1.ll, v.l.high);
  rhat.ll = c1.ll - q0.ll * v.l.high;

  cmp1.l.high = rhat.l.low;
  cmp1.l.low = c0.l.low;
  cmp2.ll = q0.ll * v.l.low;

  while (q0.l.high || cmp2.ll > cmp1.ll) {
    q0.ll--;
    rhat.ll += v.l.high;
    if (rhat.l.high)
      break;
    cmp1.l.high = rhat.l.low;
    cmp2.ll -= v.l.low;
  }
  q0.l.high += q1.l.low;

  return q0.ll;
}


static UINT64 CLK_MATH_Uint64_Scale_Uint64 (UINT64 val, UINT64 num, UINT64 denom, UINT64 correct)
{
  ClkUInt64 c1, c0;

  /* compute 128-bit numerator product */
  CLK_MATH_uint64_mul_uint64 (&c1, &c0, val, num);

  /* perform rounding correction */
  CORRECT (c0, c1, correct);

  /* high word as big as or bigger than denom --> overflow */
  if (c1.ll >= denom)
    return CLK_MAXUINT64;

  /* compute quotient, fits in 64 bits */
  return CLK_MATH_div128_64 (c1, c0, denom);
}


static void CLK_MATH_uint64_mul_uint32 (ClkUInt64 * c1, ClkUInt64 * c0, UINT64 arg1, UINT32 arg2)
{
  ClkUInt64 a;

  a.ll = arg1;

  c0->ll = (UINT64) a.l.low * arg2;
  c1->ll = (UINT64) a.l.high * arg2 + c0->l.high;
  c0->l.high = 0;
}

/* divide a 96-bit unsigned int by a 32-bit unsigned int when we know the
 * quotient fits into 64 bits.  the high 64 bits and low 32 bits of the
 * numerator are expected in c1 and c0 respectively. */
static  UINT64 CLK_MATH_div96_32 (UINT64 c1, UINT64 c0, UINT32 denom)
{
  UINT64 reminder = 0;

  reminder = c1;
  c0 += (UINT64)do_div(reminder , denom) << 32; 

  return (CLK_MATH_div64_32(c1 , denom) << 32) + CLK_MATH_div64_32(c0 , denom);
}

static UINT64 CLK_MATH_Uint64_Scale_Uint32(UINT64 val, UINT32 num, UINT32 denom, UINT32 correct)
{
  ClkUInt64 c1, c0;
  UINT64 aa;

  CLK_KDRV_LOG( CLK_INFO, "val = %llu num = %u denom = %u\n", val, num, denom);

  /* compute 96-bit numerator product */
  CLK_MATH_uint64_mul_uint32 (&c1, &c0, val, num);

  /* condition numerator based on rounding mode */
  CORRECT (c0, c1, correct);

  /* high 32 bits as big as or bigger than denom --> overflow */
  if (c1.l.high >= denom)
    return CLK_MAXUINT64;

  /* compute quotient, fits in 64 bits */
  aa = CLK_MATH_div96_32 (c1.ll, c0.ll, denom);

  return aa;
}

/* the guts of the gst_util_uint64_scale() variants */
static UINT64 CLK_MATH_Uint64_Scale (UINT64 val, UINT64 num, UINT64 denom, UINT64 correct)
{
	CLK_CHECK_CODE( ( (denom == 0) || (num == 0) )	, return 0, "denom[%llu]/num[%llu] is zero\n", num, denom);
	CLK_CHECK_CODE( ( num == denom )					, return val, "no need to scale\n");

  /* on 64bits we always use a full 128bits multipy/division */
  /* denom is low --> try to use 96 bit muldiv */
  if ( denom <= CLK_MAXUINT32 ) {
    /* num is low --> use 96 bit muldiv */
    if ( num <= CLK_MAXUINT32 )
      return CLK_MATH_Uint64_Scale_Uint32 (val, (UINT32) num,
          (UINT32) denom, correct);

    /* num is high but val is low --> swap and use 96-bit muldiv */
    if ( val <= CLK_MAXUINT32 )
      return CLK_MATH_Uint64_Scale_Uint32 (num, (UINT32) val,
          (UINT32) denom, correct);
  }

  return CLK_MATH_Uint64_Scale_Uint64 (num, val, denom, correct);
}

/**
 * Copy from gstutil.c, GStreamer
 * 
 * END
 *
 ------------------------------------------------------------------------------------------------------------*/

