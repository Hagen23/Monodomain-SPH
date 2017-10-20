/* Physiological constants used in the model,
   computational mesh and file output         */

#ifndef CARDIAC_CONSTANTS_H
#define CARDIAC_CONSTANTS_H

#include <string>

//stimulation time
const int tend=10;

//output
const int output_freq=100;
const std::string file_name="Vm.out.txt"; 

//conductivity
const double sigma_x= 1.0;
const double sigma_y= 1.0;
const double sigma = 1.0;

//grid dimensions and spacing
const double dx=0.01;
const double dy=0.01;
const double xlength=1.0;
const double ylength=1.0;

//electrode stimulus
const double radius=0.001;
// const double stim_strength=1000.0;
const double stim_strength=10000.0;
// const double stim_strength=1.0;
const double stim_duration=5.0;

//membrane model parameters
// const double FH_Vt=5.0;
// const double FH_Vp=100.0;

const double FH_Vt=-75.0;
const double FH_Vp=15.0;
const double FH_Vr=-85.0;

// const double C1 = 0.5;
// const double C2 = 0.02;
const double C1 = 0.175;
const double C2 = 0.03;

// const double C3 = 0.015;
// const double C4 = 0.005;
const double C3 = 0.011;
const double C4 = 0.55;

//membrane capacitance and surface to volume ratio 
const double Cm=1.0;
// const double Beta=2000;
const double Beta=140;

#endif

