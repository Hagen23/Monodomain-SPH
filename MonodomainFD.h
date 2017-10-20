/* Finite difference for solving diffusion */

#ifndef CARDIAC_MONODOMAINFD_H
#define CARDIAC_MONODOMAINFD_H

#include "Types.h"

class monodomainFD
{
  public:
	void explicitTS(ARRAY2D& Vm, ARRAY2D& iion, ARRAY2D& stim, double dt, double Cm, double Beta, int rows, int cols, double dx2, double dy2);
};

#endif
