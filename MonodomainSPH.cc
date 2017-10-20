
#include <stdlib.h>
#include <iostream>
#include "MonodomainSPH.h"
#include "Types.h"
#include "Constants.h"

using namespace std;

monodomainSPH::monodomainSPH()
{
    kernel = 0.02f;

    Max_Number_Paticles = 50000;
	Number_Particles = 0;

    Cm = 1.0;
    Beta = 140;
	Beta_1 = 1.0f / Beta;
	isStimOn = false;

    sigma = 1.0f;

	World_Size = m3Vector(1.0f, 1.0f, 1.0f);

	Cell_Size = 0.04f;
	Grid_Size = World_Size / Cell_Size;
	Grid_Size.x = (int)Grid_Size.x;
	Grid_Size.y = (int)Grid_Size.y;
	Grid_Size.z = (int)Grid_Size.z;

	// Number_Cells = (int)Grid_Size.x * (int)Grid_Size.y * (int)Grid_Size.z;
	Number_Cells = (int)Grid_Size.x * (int)Grid_Size.y;
    
    Stand_Density = 100.0f;
    Time_Delta = 0.005; //0.4 * kernel / sqrt(max_vel.magnitudeSquared());

    Particles = new Particle[Max_Number_Paticles];
	Cells = new Cell[Number_Cells];

	Poly6_constant = 315.0f/(64.0f * m3Pi * pow(kernel, 9));
	Spiky_constant = 45.0f/(m3Pi * pow(kernel, 6));
	
	B_spline_constant = 10.0f / (7.0f*m3Pi*kernel*kernel);
    
    cout<<"SPHSystem"<<endl;
	cout<<"Grid_Size_X : " << Grid_Size.x << endl;
	cout<<"Grid_Size_Y : " << Grid_Size.y << endl;
	// cout<<"Grid_Size_Z : " << Grid_Size.z << endl;
	cout<<"Cell Number : "<<Number_Cells<<endl;
	cout<<"Time Delta : "<<Time_Delta<<endl;
}

monodomainSPH::~monodomainSPH()
{
	delete[] Particles;
	delete[] Cells;
}

void monodomainSPH::Init_SPH(std::vector<m3Vector> positions)
{
    m3Vector pos;
	m3Vector vel(0.0f, 0.0f, 0.0f);

	for(m3Vector pos : positions)
		Init_Particle(pos);

	set_stim(radius, stim_strength);

	cout<<"Number of Paticles : "<<Number_Particles<<endl;
}


void monodomainSPH::Init_Particle(m3Vector pos)
{
	if(Number_Particles + 1 > Max_Number_Paticles)
		return;

	Particle *p = &(Particles[Number_Particles]);
	
	p->pos = pos;
	p->Inter_Vm = 0.0f;
	p->Vm = 0.0f;
	p->Iion = 0.0f;
	p->stim = 0.0f;
	p->w = 0.0f;
	p->dens = 0.0f;
	p->mass = 0.2f;
	Number_Particles++;
}

m3Vector monodomainSPH::Calculate_Cell_Position(m3Vector pos)
{
	m3Vector cellpos = pos / Cell_Size;
	cellpos.x = (int)cellpos.x;
	cellpos.y = (int)cellpos.y;
	// cellpos.z = (int)cellpos.z;
	return cellpos;
}

int monodomainSPH::Calculate_Cell_Hash(m3Vector pos)
{
	if((pos.x < 0)||(pos.x >= Grid_Size.x)||
		(pos.y < 0)||(pos.y >= Grid_Size.y))
		// ||
		// (pos.z < 0)||(pos.z >= Grid_Size.z))
		return -1;

	// int hash = pos.x + Grid_Size.x * (pos.y + Grid_Size.y * pos.z);
	int hash = pos.x + Grid_Size.x * pos.y;
	if(hash > Number_Cells)
		cout<<"Error";
	return hash;
}

m3Real monodomainSPH::B_spline(m3Real r)
{
	m3Real q = r / kernel;
	if (q >= 0 && q < 1)
		return B_spline_constant * (1.0f - 1.5f * q * q + 0.75f * q * q * q);
	else if (q >= 1 && q < 2)
		return B_spline_constant * (0.25*pow(2 - q, 3));
	else
		return 0;
}

m3Real monodomainSPH::B_spline_1(m3Real r)
{
	m3Real q = r / kernel;
	if (q >= 0 && q < 1)
		return B_spline_constant * (-3.0f * q + 2.25f * q * q);
	else if (q >= 1 && q < 2)
		return B_spline_constant * (-0.75 * pow(2 - q, 2));
	else
		return 0;
}

m3Real monodomainSPH::B_spline_2(m3Real r)
{
	m3Real q = r / kernel;
	if (q >= 0 && q < 1)
		return B_spline_constant * (-3 + 4.5 * q);
	else if (q >= 1 && q < 2)
		return B_spline_constant * (1.5 * (2 - q));
	else
		return 0;
}

/// For density computation
m3Real monodomainSPH::Poly6(m3Real r2)
{
	return (r2 >= 0 && r2 <= kernel*kernel) ? Poly6_constant * pow(kernel * kernel - r2, 3) : 0;
}

/// For force of pressure computation
m3Real monodomainSPH::Spiky(m3Real r)
{
	return (r >= 0 && r <= kernel ) ? -Spiky_constant * (kernel - r) * (kernel - r) : 0;
}

/// For viscosity computation
m3Real monodomainSPH::Visco(m3Real r)
{
	return (r >= 0 && r <= kernel ) ? Spiky_constant * (kernel - r) : 0;
}

void monodomainSPH::Find_neighbors()
{
	int hash;
	Particle *p;

	for(int i = 0; i < Number_Cells; i++)
		Cells[i].contained_particles.clear();

	for(int i = 0; i < Number_Particles; i++)
	{
		p = &Particles[i];
		hash = Calculate_Cell_Hash(Calculate_Cell_Position(p->pos));
		Cells[hash].contained_particles.push_back(p);
	}

	// for(int i = 0; i < Number_Cells; i++)
	// {
	// 	int count = 0;
	// 	cout << "Cell "<< i ;
	// 	for(Particle* p : Cells[i].contained_particles)
	// 		count++;
	// 	cout << " count " << count << endl;
	// }

	// int test = 0;
}

void monodomainSPH::Compute_Density_SingPressure()
{
	Particle *p;
	m3Vector CellPos;
	m3Vector NeighborPos;
	int hash;

	for(int k = 0; k < Number_Particles; k++)
	{
		p = &Particles[k];
		p->dens = 0;
		CellPos = Calculate_Cell_Position(p->pos);

		// for(int k = -1; k <= 1; k++)
		for(int j = -1; j <= 1; j++)
		for(int i = -1; i <= 1; i++)
		{
			// NeighborPos = CellPos + m3Vector(i, j, k);
			NeighborPos = CellPos + m3Vector(i, j, 0);
			hash = Calculate_Cell_Hash(NeighborPos);

			if(hash == -1)
				continue;

			for(Particle* np : Cells[hash].contained_particles)
			{
				m3Vector Distance;
				Distance = p->pos - np->pos;
				// m3Real dis2 = (m3Real)Distance.magnitudeSquared();
				// p->dens += np->mass * Poly6(dis2);
				m3Real r = (m3Real)Distance.magnitude();
				p->dens += np->mass * B_spline(r);
			}
		}

		// p->dens += p->mass * Poly6(0.0f);
	}
}

void monodomainSPH::calculate_voltage()
{
    Particle *p;
	m3Vector CellPos, NeighborPos;
	
	int hash;

	for(int k = 0; k < Number_Particles; k++)
	{
		p = &Particles[k];
		// p->Vm = 0.0f;
		p->Inter_Vm = 0.0f;
        CellPos = Calculate_Cell_Position(p->pos);

        // for(int k = -1; k <= 1; k++)
		for(int j = -1; j <= 1; j++)
		for(int i = -1; i <= 1; i++)
		{
			// NeighborPos = CellPos + m3Vector(i, j, k);
			NeighborPos = CellPos + m3Vector(i, j, 0);
			hash = Calculate_Cell_Hash(NeighborPos);
			if(hash == -1)
				continue;

			for(Particle* np : Cells[hash].contained_particles)
			{
				m3Vector Distance;
				Distance = p->pos - np->pos;
				m3Real dis2 = (m3Real)Distance.magnitudeSquared();

				if(dis2 > INF)
				{
					m3Real dis = sqrt(dis2);
					float Volume = np->mass / np->dens;
					// p->Vm += (np->Vm) * Volume * Visco(dis);
					p->Inter_Vm += (np->Vm - p->Vm) * Volume * B_spline_2(dis);
					// p->Inter_Vm += (np->Vm - p->Vm) * Volume * Visco(dis);
				}
			}
		}
		
		// float Cm_1 = 1.0f / Cm;

		p->Inter_Vm += (sigma / (Beta * Cm)) * p->Inter_Vm - ((p->Iion - p->stim * Time_Delta / p->mass) / Cm);
		
		// p->Vm = (p->Vm * Beta_1 * sigma + p->Iion + p->stim) * Cm_1 * Time_Delta;
		// p->Vm /= p->mass;
    }
}

void monodomainSPH::calculate_cell_model()
{
	Particle *p;

	m3Real denom = (FH_Vp - FH_Vr);
	m3Real asd = (FH_Vt - FH_Vr)/denom;
	m3Real u = 0.0;

	for(int k = 0; k < Number_Particles; k++)
	{
		p = &Particles[k];

		u = (p->Vm - FH_Vr) / denom;

		p->Iion += Time_Delta * (C1*u*(u - asd)*(u - 1.0) + C2* p->w) / p->mass ;
		// p->Iion /= p->mass;
		// p->Iion /= p->mass != 0.0f ? p->mass : 1;
		p->w += Time_Delta * C3*(u - C4*p->w) / p->mass;
		// p->w/=p->mass;
	}
}

void monodomainSPH::update_voltage()
{
	Particle *p;
	
	for(int i=0; i < Number_Particles; i++)
	{
		p = &Particles[i];
		p->Vm += p->Inter_Vm * Time_Delta / p->mass;
	}
}

void monodomainSPH::set_stim(m3Real radius, m3Real stim_strength)
{
	Particle *p;
	isStimOn=true;

	for(int k = 0; k < Number_Particles; k++)
	{
		p = &Particles[k];
		m3Vector position = p->pos;
		if (((position.x-0.5)*(position.x-0.5)+(position.y-0.5)*(position.y-0.5)) <= radius)
		{
			p->stim = stim_strength;
		}
	}
}

void monodomainSPH::turnOffStim(m3Real time, m3Real stim_duration)
{
	if ((time>=stim_duration)&&(isStimOn==true))
	{
		Particle *p;
		
		for(int k = 0; k < Number_Particles; k++)
		{
			p = &Particles[k];
			p->stim = 0.0f;
	   	}
	   	isStimOn=false;
	}
}

void monodomainSPH::compute_monodomain()
{
	Find_neighbors();
	Compute_Density_SingPressure();

	calculate_cell_model();
	calculate_voltage();

	update_voltage();
}