/* 
This class implements the SPH method to solve the monodomain equations.

Solves the elliptical equation (diffusion) with the SPH method.

FitzHugh-Nagumo membrane model solves the two ODEs explicitly.

Uses the forward Euler method for time integration.

@author Octavio Navarro
@version 1.0

*/

#ifndef CARDIAC_MONODOMAINSPH_H
#define CARDIAC_MONODOMAINSPH_H

#include "Types.h"
#include "Constants.h"
#include "Math3D/m3Vector.h"
#include "Math3D/m3Real.h"

#define PI 3.141592f
#define INF 1E-12f

/// Definition of each particle for the SPH method
class Particle
{
  public:
	m3Real    Vm;     		// Voltage
	m3Real	  Inter_Vm;		// Intermediate Voltage for time integration
    m3Real    Iion;  		// Ionic current
    m3Real    stim;   		// Stimulation
    m3Real    w;      		// Recovery variable
    m3Vector  pos;    		// Position of the particle
    m3Real    mass;			// Mass of each particle
    m3Real    dens;   		// Density
};

/// Cells that divide the computation space, and contain all the particles in the simulation. Basically, it is an octree.
class Cell
{
public:
  std::vector<Particle*> contained_particles;
};

class monodomainSPH
{
  private:
	
		m3Real 		kernel;					    // kernel or h in kernel function
		int 		Max_Number_Paticles;		// initial array for particles
		int 	  	Number_Particles;		  	// paticle number

		m3Vector 	Grid_Size;				    // Size of a size of each grid voxel
		m3Vector 	World_Size;				    // screen size
		m3Real 		Cell_Size;				    // Size of the divisions in the grid; used to determine the cell position for the has grid; kernel or h
    	int 		Number_Cells;			    // Number of cells in the hash grid
    	m3Real    	Cm;                   		// Membrane capacitance per unit area
		m3Real    	Beta;                 		// Surface volume ratio
		
		m3Real		sigma;						// Value for the conductivity tensor (a constant in this case)
		bool		isStimOn;					// Checks if the stimulation current is still enabled

		m3Real 		Time_Delta;

		// Smoothing kernel constants for SPH
		m3Real 		Poly6_constant, Spiky_constant, Visco_Constant;
		// Smoothing kernel constants for SPH; this constant has to change depending on the dimension of the simulation. B-spline kernel as in J.J. Monaghan, Smoothed particle hydrodynamics, Annual Review of Astronomy and Astrophysics 30 (1992).
		m3Real 		B_spline_constant;			

		Particle*	Particles;
		Cell*		Cells;

	public:
		monodomainSPH();
		~monodomainSPH();

		void Init_SPH(std::vector<m3Vector> positions);		// initialize 
		void Init_Particle(m3Vector pos);					// initialize particle system
		m3Vector Calculate_Cell_Position(m3Vector pos);		// get cell position
		int Calculate_Cell_Hash(m3Vector pos);				// get cell hash number

		/// Kernel functions
		m3Real B_spline(m3Real r);				// B-spline kernel function
		m3Real B_spline_1(m3Real r);			// B-spline kernel first derivative
		m3Real B_spline_2(m3Real r);			// B-spline kernel second derivative 

		/// Hash the particles into a grid
		void Find_neighbors();
		
		/// SPH methods
		void Compute_Density_SingPressure();	// Computes the density of each particle
		void calculate_voltage();				// Calculates the intermediate voltage
		void calculate_cell_model();			// Updates the ionic current and recovery variable with the cell model
		void update_voltage();					// Applies the time integration to the voltage

		void set_stim(m3Real radius, m3Real stim_strength);		// Turns the stimulation on at a specific point (0.5, 0.5), around a given radius
		void turnOffStim(m3Real time, m3Real stim_duration);	// Turns the stimulation off for all particles
		
		void compute_monodomain();				// Calculates all the SPH steps

		inline int Get_Particle_Number() { return Number_Particles; }
		inline m3Vector Get_World_Size() { return World_Size; }
		inline Particle* Get_Paticles()	 { return Particles; }
		inline Cell* Get_Cells()		 { return Cells; }
};

#endif