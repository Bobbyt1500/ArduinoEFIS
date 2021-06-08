#ifndef AEFIS_HEADING_H
#define AEFIS_HEADING_H

#include <UTFT.h>
#include <UTFT_Geometry.h>

class Heading {
    private:
        int heading_marker_pos[4] = {70, 70, 70, 70};
    public:
        int heading = 0;
        void init(UTFT& TFTDisp, UTFT_Geometry geo);
        void draw(UTFT& TFTDisp);
};

#endif