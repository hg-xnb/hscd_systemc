/* DIGIT.C - digit arithmetic routines
 */

/* Copyright (C) RSA Laboratories, a division of RSA Data Security,
      Inc., created 1991. All rights reserved.
 */

#include "global.h"
#include "rsaref.h"
#include "nn.h"
#include "digit.h"


/* Computes a = b * c, where b and c are digits.

   Lengths: a[2].
 */
void NN_DigitMult (NN_DIGIT a[2],NN_DIGIT b,NN_DIGIT c)
{
	  //Call the "Extern" Function to link up with the hardware
	  //implementation instead of doing the operation in software
	  
}

/* Sets a = b / c, where a and c are digits.

   Lengths: b[2].
   Assumes b[1] < c and HIGH_HALF (c) > 0. For efficiency, c should be
   normalized.
 */
void NN_DigitDiv (a, b, c)
NN_DIGIT *a, b[2], c;
{
  NN_DIGIT t[2], u, v;
  NN_HALF_DIGIT aHigh, aLow, cHigh, cLow;

  cHigh = (NN_HALF_DIGIT)HIGH_HALF (c);
  cLow = (NN_HALF_DIGIT)LOW_HALF (c);

  t[0] = b[0];
  t[1] = b[1];

  /* Underestimate high half of quotient and subtract.
   */
  if (cHigh == MAX_NN_HALF_DIGIT)
    aHigh = (NN_HALF_DIGIT)HIGH_HALF (t[1]);
  else
    aHigh = (NN_HALF_DIGIT)(t[1] / (cHigh + 1));
  u = (NN_DIGIT)aHigh * (NN_DIGIT)cLow;
  v = (NN_DIGIT)aHigh * (NN_DIGIT)cHigh;
  if ((t[0] -= TO_HIGH_HALF (u)) > (MAX_NN_DIGIT - TO_HIGH_HALF (u)))
    t[1]--;
  t[1] -= HIGH_HALF (u);
  t[1] -= v;

  /* Correct estimate.
   */
  while ((t[1] > cHigh) ||
         ((t[1] == cHigh) && (t[0] >= TO_HIGH_HALF (cLow)))) {
    if ((t[0] -= TO_HIGH_HALF (cLow)) > MAX_NN_DIGIT - TO_HIGH_HALF (cLow))
      t[1]--;
    t[1] -= cHigh;
    aHigh++;
  }

  /* Underestimate low half of quotient and subtract.
   */
  if (cHigh == MAX_NN_HALF_DIGIT)
    aLow = (NN_HALF_DIGIT)LOW_HALF (t[1]);
  else
    aLow = 
      (NN_HALF_DIGIT)((TO_HIGH_HALF (t[1]) + HIGH_HALF (t[0])) / (cHigh + 1));
  u = (NN_DIGIT)aLow * (NN_DIGIT)cLow;
  v = (NN_DIGIT)aLow * (NN_DIGIT)cHigh;
  if ((t[0] -= u) > (MAX_NN_DIGIT - u))
    t[1]--;
  if ((t[0] -= TO_HIGH_HALF (v)) > (MAX_NN_DIGIT - TO_HIGH_HALF (v)))
    t[1]--;
  t[1] -= HIGH_HALF (v);

  /* Correct estimate.
   */
  while ((t[1] > 0) || ((t[1] == 0) && t[0] >= c)) {
    if ((t[0] -= c) > (MAX_NN_DIGIT - c))
      t[1]--;
    aLow++;
  }
  
  *a = TO_HIGH_HALF (aHigh) + aLow;
}
