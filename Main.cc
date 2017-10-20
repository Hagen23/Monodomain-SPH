/*
    Monodomain: finite difference and explicit time stepping
    Leon Graham, 2014
*/

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <chrono>
#include "Constants.h"
#include "Types.h"
#include "MemFH.h"
#include "MonodomainFD.h"
#include "Output.h"
#include "PointStim.h"

using namespace std;


int main()
{
    double t=0;
    int tstep=1;

    double dx2=dx*dx;
    double dy2=dy*dy;

    //dt = dx2*dy2/(2*K*(dx2+dy2))
    double dt=0.005; //0.04 for anisotropy=1=alpha   

    int cols=static_cast<int>(xlength/dx);
    int rows=static_cast<int>(ylength/dy);

    // Transmembrane potential
    ARRAY2D Vm(rows, vector<double>(cols));

    // Transmembrane ionic current per unit area
    ARRAY2D iion(rows, vector<double>(cols));

    ARRAY2D w(rows, vector<double>(cols));

    // Current stimulus
    ARRAY2D stim(rows, vector<double>(cols));
 
    memFH membrane_model;
    monodomainFD diffusion_model;
    Output Vm_output, Vm_output_sph;
    PointStim stimElectrode;

    monodomainSPH *mono_sph = new monodomainSPH();

    vector<m3Vector> Positions;

    for(int row = 0; row < rows; row+=1)
    for(int col = 0; col < cols; col+=1)
        Positions.push_back(m3Vector((m3Real)col*dx, (m3Real)row*dy, 0.0f));

    mono_sph->Init_SPH(Positions);
    
    //set current stimulus
    stimElectrode.setStim(stim, radius, stim_strength);
 
    //start timer
    std::chrono::system_clock::time_point tstart = std::chrono::system_clock::now();

    //Vm_output.open_file("binary");
    Vm_output.open_file("ascii");
    Vm_output_sph.open_file("ascii", "Vm.out.sph.txt");

    //finite difference calculation
    while (t<=tend)
    {
          //calculate membrane potential from membrane model
          membrane_model.explicitTS(Vm, iion, w, dt, rows, cols, FH_Vt, FH_Vp, C1, C2, C3, C4);

          /*
          Boundary Conditions:
          each boundary is set to zero.
          results from how the finite difference stencil is implemented.
          */

          //calculate membrane potential from diffusion
          diffusion_model.explicitTS(Vm, iion, stim, dt, Cm, Beta, rows, cols, dx2, dy2);
        
          mono_sph->compute_monodomain();
          //write potentials to file
        Vm_output.write_file(Vm, tstep, output_freq);
        Vm_output_sph.write_file(mono_sph->Get_Paticles(), mono_sph->Get_Particle_Number(), tstep, output_freq);
          
        mono_sph->turnOffStim(t, stim_duration);

          //turn off stimulus when duration has been met
          stimElectrode.turnOffStim(stim, t, stim_duration);

        //   printf("tstep= %d, dt= %f, t= %f \n", tstep, dt, t);    
          t=t+dt;
          tstep=tstep+1;
    }
    Vm_output.close_file();

    //get elapsed time
    std::chrono::duration<double> sec = std::chrono::system_clock::now() - tstart;

    printf("Total time: %.6f s\n", sec.count());
    printf("Average time per time-step: %.6f s.\n", (sec.count()/tstep));

   return 0;
}
