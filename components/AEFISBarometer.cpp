#include "AEFISBarometer.h"

void Barometer::draw(UTFT& TFTDisp) {
    TFTDisp.setColor(selected_color[0], selected_color[1], selected_color[2]);
    TFTDisp.fillRect(480, 320, 396, 300);
    TFTDisp.printNumF(setting, 2, 398, 302);
}