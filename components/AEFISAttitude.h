#ifndef AEFIS_ATTITUDE_H
#define AEFIS_ATTITUDE_H

#include <UTFT.h>
#include <UTFT_Geometry.h>

class Attitude {
    private:
        int last_lower = 160;
        int last_upper = 160;
    public:
        double ind_roll = 0;
        double ind_pitch = 0;
        void init(UTFT& TFTDisp, UTFT_Geometry geo);
        void draw(UTFT& TFTDisp, UTFT_Geometry geo);
};

#endif