/* Point Stimulus Electrode */

#ifndef CARDIAC_POINTSTIM_H
#define CARDIAC_POINTSTIM_H

#include "Types.h"

class PointStim
{
  public:
     PointStim();
     void setStim(ARRAY2D& stim, double radius, double stim_strength);
     void turnOffStim(ARRAY2D& stim, double t, double stim_duration);
  
  private:
     bool isStimOn;
};

#endif
