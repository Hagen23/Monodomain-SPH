/* FitzHugh-Nagumo membrane model */

#ifndef CARDIAC_MEMFH_H
#define CARDIAC_MEMFH_H

#include "Types.h"

class memFH
{
  public:
   void explicitTS(ARRAY2D& Vm, ARRAY2D& iion, ARRAY2D& w, double dt, int rows, int cols, double FH_Vt, double FH_Vp, double C1, double C2, double C3, double C4);
};

#endif
