#ifndef AEFIS_AIRSPEED_H
#define AEFIS_AIRSPEED_H

#include <UTFT.h>
#include <UTFT_Geometry.h>

class Airspeed {
    private:
        int airspeed_marker_pos[6] = {0, 0, 0, 0, 0, 0};
    public:
        int airspeed = 0;
        void init(UTFT& TFTDisp, UTFT_Geometry geo);
        void draw(UTFT& TFTDisp);

};

#endif