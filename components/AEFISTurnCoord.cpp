#include "AEFISTurnCoord.h"

void TurnCoordinator::draw(UTFT& TFTDisp) {
    TFTDisp.setColor(0, 0, 0);
    TFTDisp.fillRect(70, 320, 350, 300);
  
    TFTDisp.setColor(255, 255, 255);
    TFTDisp.fillRect(195, 320, 194, 300);
    TFTDisp.fillRect(225, 320, 226, 300);

    int x = 10.0 * abs(slip_deg);

    if (slip_deg < 0) {
        x = x + 210;
    } else {
        x = 210 - x;
    }
    
    TFTDisp.setColor(selected_color[0], selected_color[1], selected_color[2]);
    TFTDisp.fillCircle(x, 310, 10);
}