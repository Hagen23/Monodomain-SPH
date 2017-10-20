/* Point Stimulus Electrode */

#include <assert.h>
#include "PointStim.h"
#include "Types.h"
#include "Constants.h"

PointStim::PointStim()
{
}

void PointStim::setStim(ARRAY2D& stim, double radius, double stim_strength)
{   
     isStimOn=true;

     for (size_t r=0; r<stim.size(); ++r)
     {
        for (size_t c=0; c<stim[r].size(); ++c)
        {
            if (((c*dx-0.5)*(c*dx-0.5)+(r*dy-0.5)*(r*dy-0.5)) <= radius)
            {
                stim[r][c] = stim_strength;
            }
        }
     }
}

void PointStim::turnOffStim(ARRAY2D& stim, double t, double stim_duration)
{
     //ensure stimulation duration is valid
     assert(stim_duration>=0 && "Stimulus duration not positive");

     if ((t>=stim_duration)&&(isStimOn==true))
     {
        for (size_t r=0; r<stim.size(); ++r)
        {
            for (size_t c=0; c<stim[r].size(); ++c)
            {
                stim[r][c]=0;
            }
        }
        isStimOn=false;
     }
}
