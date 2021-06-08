#ifndef AEFIS_ALTITUDE_H
#define AEFIS_ALTITUDE_H

#include <UTFT.h>
#include <UTFT_Geometry.h>

class Altitude {
    private:
        int last_mark1_pos = 0;
        int last_mark2_pos = 0;
        int last_mark3_pos = 0;
    public:
        int altitude = 0;
        void init(UTFT& TFTDisp, UTFT_Geometry geo);
        void draw(UTFT& TFTDisp);

};

#endif