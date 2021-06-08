#ifndef AEFIS_TURN_H
#define AEFIS_TURN_H

#include <UTFT.h>

class TurnCoordinator {
    public:
        double slip_deg = 0;
        int selected_color[3] = {242, 239, 27};
        void draw(UTFT& TFTDisp);
};

#endif