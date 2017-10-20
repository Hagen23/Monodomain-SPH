/* Model output written to file 
   Binary and ascii format      */

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "Output.h"
#include "Types.h"
#include "Constants.h"

using namespace std;

void Output::open_file(string format, string fileName)
{
  if(fileName == "")
    fileName = file_name;

   if (format == "binary")
   {
      outputFile.open(fileName, ios::out | ios::binary);
      outputFile.setf(ios::fixed, ios::floatfield);
      outputFile.setf(ios::showpoint);
      isAscii=false;
     
      cout << "Output file in binary format" << endl;

   }
   else if ((format=="ascii")||(format=="text"))
   {
      outputFile.open(fileName);
      outputFile.setf(ios::fixed, ios::floatfield);
      outputFile.setf(ios::showpoint);
      isAscii=true;

      cout << "Output file in ascii format" << endl;

   }
   else
   {
      outputFile.open(fileName);
      outputFile.setf(ios::fixed, ios::floatfield);
      outputFile.setf(ios::showpoint);
      isAscii=true;
   
      cout << "Warning: output format not defined. Setting ascii format as default." << endl;
   }
   
   //test if file is opened            
   if (!outputFile.is_open())
   {
      cout << "Error opening file" << endl;
      exit(1);
   }
}

void Output::close_file()
{
   outputFile.close();
}

void Output::write_file(ARRAY2D& Vm, int tstep, int output_freq)
{
   if (isAscii)
   {
      if ((tstep % output_freq)==0)
      {
         for (size_t r=0; r<Vm.size(); ++r)
         {
             for (size_t c=0; c<Vm[r].size(); ++c)
             {
               outputFile << Vm[r][c] << " ";
             }
         }
         outputFile << endl;
      }
   }
   else
   {
      if ((tstep % output_freq)==0)
      {
         for (size_t r=0; r<Vm.size(); ++r)
         {
             for (size_t c=0; c<Vm[r].size(); ++c)
             {
               //outputFile << Vm[r][c] << " ";
               //outputFile.write(reinterpret_cast<const  char*>(&Vm[r][c]), Vm.size()*sizeof(double));
               outputFile.write((char*)&Vm[r][c], sizeof(double));
               //outputFile.write(" ");
             }
         }
         //outputFile << endl;
         outputFile << "\n";
      }
   }
}

void Output::write_file(Particle* Particles, int Number_particles, int tstep, int output_freq)
{
   if (isAscii)
   {
      if ((tstep % output_freq)==0)
      {
        for(int k = 0; k < Number_particles; k++)
        {
          outputFile << Particles[k].Vm << " ";
        }
         outputFile << endl;
      }
   }
   else
   {
      if ((tstep % output_freq)==0)
      {
        for(int k = 0; k < Number_particles; k++)
        {
          outputFile.write((char*)&((&Particles[k])->Vm), sizeof(double));
        }
         //outputFile << endl;
         outputFile << "\n";
      }
   }
}
