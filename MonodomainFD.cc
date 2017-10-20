/* Solves the elliptical equation (diffusion)
   by finite difference. Uses a central difference 
   stencil and explicit time-stepping scheme.      */

#include <stdlib.h>
#include "MonodomainFD.h"
#include "Types.h"
#include "Constants.h"

void monodomainFD::explicitTS(ARRAY2D& Vm, ARRAY2D& iion, ARRAY2D& stim, double dt, double Cm, double Beta, int rows, int cols, double dx2, double dy2)
{
    for (unsigned int i=1; i < (Vm.size()-1); ++i)
    {
        for (unsigned int j=1; j < (Vm[i].size()-1); ++j)
        {
            // Vm[i][j] = Vm[i][j]+(dt/Cm)*((1/Beta)*( sigma_x*((Vm[i+1][j] - 2*Vm[i][j] + Vm[i-1][j])/dx2) +
            // sigma_y*((Vm[i][j+1] - 2*Vm[i][j] + Vm[i][j-1])/dy2)) - iion[i][j] + stim[i][j]);

            Vm[i][j] += (dt/(Beta * Cm)) * (sigma_x*((Vm[i+1][j] - 2*Vm[i][j] + Vm[i-1][j])/dx2) +
            sigma_y*((Vm[i][j+1] - 2*Vm[i][j] + Vm[i][j-1])/dy2)) - (dt/Cm) * (iion[i][j] - stim[i][j]);

            // Vm[i][j] = Vm[i][j] + (dt/Cm)*((1/Beta)*(sigma_x*((Vm[i+1][j] - 2*Vm[i][j] + Vm[i-1][j])/dx2) +
            // sigma_y*((Vm[i][j+1] - 2*Vm[i][j] + Vm[i][j-1])/dy2) + stim[i][j])-iion[i][j]);
        }
    }
}
