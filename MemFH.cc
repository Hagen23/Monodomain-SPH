/* FitzHugh-Nagumo membrane model
   Solves the two ODEs explicitly
*/

//#include <stdlib.h>
#include "MemFH.h"
#include "Types.h"
#include "Constants.h"

void memFH::explicitTS(ARRAY2D& Vm, ARRAY2D& iion, ARRAY2D& w, double dt, int rows, int cols, double FH_Vt, double FH_Vp, double C1, double C2, double C3, double C4)
{
    double denom = (FH_Vp - FH_Vr);
    double asd = (FH_Vt - FH_Vr)/denom;
    double u = 0.0;

     for (size_t i=0; i<Vm.size(); ++i)
     {
         for (size_t j=0; j<Vm[i].size(); ++j)
         {
            //  iion[i][j] = (C1*Vm[i][j]*(Vm[i][j]/FH_Vt-1.0)*(1.0-Vm[i][j]/FH_Vp) - C2*Vm[i][j]*w[i][j]);
            
            u = (Vm[i][j] - FH_Vr) / denom;

            iion[i][j] = iion[i][j] + dt*(C1*u*(u - asd)*(u - 1.0) + C2*w[i][j]);
             
            // w[i][j] = w[i][j] + dt*C3*(Vm[i][j] - C4*w[i][j]);
            w[i][j] = w[i][j] + dt*C3*(u - C4*w[i][j]);
         }
     }
}
