#ifndef AEFIS_BARO_H
#define AEFIS_BARO_H

#include <UTFT.h>

class Barometer {
    public:
        int selected_color[3] = {3, 236, 252};
        float setting = 29.92;
        void draw(UTFT& TFTDisp);
};

#endif