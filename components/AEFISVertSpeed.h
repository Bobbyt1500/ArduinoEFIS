#ifndef AEFIS_VERTSPEED_H
#define AEFIS_VERTSPEED_H

#include <UTFT.h>
#include <UTFT_Geometry.h>

class VerticalSpeed {
    private:
        int last_vs_arrow = 160;
    public:
        int vertical_speed = 0;
        void init(UTFT& TFTDisp);
        void draw(UTFT& TFTDisp, UTFT_Geometry geo);
};


#endif