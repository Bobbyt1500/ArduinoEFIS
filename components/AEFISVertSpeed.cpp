#include "AEFISVertSpeed.h"

extern uint8_t BigFont[];
extern uint8_t SmallFont[];

void VerticalSpeed::init(UTFT& TFTDisp) {
    TFTDisp.setColor(255, 255, 255);
    TFTDisp.setFont(SmallFont);

    int marker1 = 15;

    for (int i = 0; i < 31; ++i) {
        int y = (i * 8) + 40;
        
        if (i % 5 == 0) {
        
        TFTDisp.fillRect(480, y-1, 475, y+1);
        if (abs(marker1) >= 10) {
            TFTDisp.printNumI(abs(marker1), 459, y-6);
        } else {
            TFTDisp.printNumI(abs(marker1), 467, y-6);
        }
        marker1-=5;
        
        } else {
        
        TFTDisp.drawLine(480, y, 475, y);
        
        }
    }
    
    TFTDisp.setFont(BigFont);
}

void VerticalSpeed::draw(UTFT& TFTDisp, UTFT_Geometry geo) {
     // Clear last vertical speed arrow
    TFTDisp.setColor(0, 0, 0);
    TFTDisp.fillRect(452, last_vs_arrow-5, 457, last_vs_arrow + 5);

    // Calculate vertical speed arrow position
    if (vertical_speed > 1500) {
        vertical_speed = 1500;
    } else if (vertical_speed < -1500) {
        vertical_speed = -1500;
    }
    
    int y = 8.0 * (abs(vertical_speed)/100.0);
    if (vertical_speed >= 0) {
        y = 160 - y;
    } else {
        y = y + 160;
    }

    // Draw new vertical speed arrow
    last_vs_arrow = y;
    TFTDisp.setColor(251, 0, 255);
    geo.fillTriangle(452, y-5, 452, y+5, 457, y);
}