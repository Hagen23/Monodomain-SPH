/* Model output written to file */

#ifndef CARDIAC_WRITE_H
#define CARDIAC_WRITE_H

#include <fstream>
#include <string>
#include "Types.h"
#include "MonodomainSPH.h"

class Output
{
  public:
   void open_file(std::string format, std::string fileName = "");
   void write_file(ARRAY2D& Vm, int tstep, int output_freq);
   void write_file(Particle* Particles, int Number_particles, int tstep, int output_freq);
   void close_file();
  private:
   bool isAscii;
   std::ofstream outputFile;
};

#endif
