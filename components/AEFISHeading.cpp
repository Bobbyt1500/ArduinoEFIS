#include "AEFISHeading.h"

void Heading::init(UTFT& TFTDisp, UTFT_Geometry geo) {

}

void Heading::draw(UTFT& TFTDisp) {
      
    int hundreds = (heading/10/10) % 10;
    int tens = (heading/10) % 10;

    int ht = hundreds * 100 + tens * 10;
    int diff = heading - ht;

    // Generate a nearest value divisible by 20 for markers
    int marker1;

    if ((ht) % 20 == 0) {
        marker1 = hundreds * 100 + tens * 10;
    } else {
        marker1 = hundreds * 100 + tens * 10 - 10;
    }

    // Generate markers and clear past markers
    TFTDisp.setColor(0,0,0);
    
    int heading_markers[4] = {0, 0, 0, 0};
    marker1-=20;
    for (int i = 0; i < 4; ++i) {
        heading_markers[i] = marker1+(i*20);
        TFTDisp.fillRect(heading_marker_pos[i], 3, heading_marker_pos[i] + 48, 19);
    }

    TFTDisp.fillRect(70, 30, 350, 35);
    TFTDisp.setColor(255, 255, 255);

    
    // Draw markers on display
    for (int i = 0; i < 6; ++i) {
        int x =  (310 - (i*50)) - (diff*5);

        if (x >= 320) continue;
        if (x <= 100) continue;

        for (int j = 0; j < 4; ++j) {
            if (i-2 == (ht - heading_markers[j])/10) {
                int temp = heading_markers[j];
                if (temp < 0) {
                temp += 360;
                } else if (temp > 360) {
                temp -= 360;
                } else if (temp == 360) {
                temp = 0;
                }
                TFTDisp.printNumI(temp, x-24, 3, 3, '0');
                heading_marker_pos[j] = x-24;
                break;
            }  
        }

        TFTDisp.fillRect(x, 30, x+1, 35);
        
    }
    
    TFTDisp.fillRect(185, 17, 234, 0);
    TFTDisp.printNumI(heading, 186, 1, 3, '0');
}