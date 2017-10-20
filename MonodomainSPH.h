#ifndef CARDIAC_MONODOMAINSPH_H
#define CARDIAC_MONODOMAINSPH_H

#include "Types.h"
#include "Constants.h"
#include "Math3D/m3Vector.h"
#include "Math3D/m3Real.h"

#define PI 3.141592f
#define INF 1E-12f

class Particle
{
  public:
	m3Real    Vm;     // Voltage
	m3Real	  Inter_Vm;
    m3Real    Iion;   // Ionic current
    m3Real    stim;   // Stimulation
    m3Real    w;      // Recovery variable
    m3Vector  pos;    // Position of the particle
    m3Real    mass;
    m3Real    dens;   // Density
};

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
		
		m3Real		Beta_1;
		m3Real		sigma;
		bool		isStimOn;

		m3Real		Stand_Density;
		m3Real 		Time_Delta;

		// Smoothing kernel constants for SPH
		m3Real 		Poly6_constant, Spiky_constant, Visco_Constant;
		m3Real 		B_spline_constant;			// Smoothing kernel constants for SPH; this constant has to change depending on the dimension of the simulation. B-spline kernel as in J.J. Monaghan, Smoothed particle hydrodynamics, Annual Review of Astronomy and Astrophysics 30 (1992).

		Particle*	Particles;
		Cell*		Cells;

	public:
		monodomainSPH();
		~monodomainSPH();

		void Init_SPH(std::vector<m3Vector> positions);		// initialize 
		void Init_Particle(m3Vector pos);					// initialize particle system
		m3Vector Calculate_Cell_Position(m3Vector pos);		// get cell position
		int Calculate_Cell_Hash(m3Vector pos);				// get cell hash number

		// Kernel function
		m3Real Poly6(m3Real r2);		// for density
		m3Real Spiky(m3Real r);			// for pressure
		m3Real Visco(m3Real);			// for viscosity

		m3Real B_spline(m3Real r);				// B-spline kernel function
		m3Real B_spline_1(m3Real r);			// B-spline kernel first derivative
		m3Real B_spline_2(m3Real r);			// B-spline kernel second derivative 

		/// Hashed the particles into a grid
		void Find_neighbors();
		
		void Compute_Density_SingPressure();
		void calculate_voltage();
		void calculate_cell_model();
		void update_voltage();

		void set_stim(m3Real radius, m3Real stim_strength);
		void turnOffStim(m3Real time, m3Real stim_duration);
		
		void compute_monodomain();

		inline int Get_Particle_Number() { return Number_Particles; }
		inline m3Vector Get_World_Size() { return World_Size; }
		inline Particle* Get_Paticles()	 { return Particles; }
		inline Cell* Get_Cells()		 { return Cells; }
};

#endif