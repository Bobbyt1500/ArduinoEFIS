#include "AEFISAirspeed.h"

void Airspeed::init(UTFT& TFTDisp, UTFT_Geometry geo) {
    TFTDisp.setColor(255, 255, 255);
    geo.fillTriangle(48, 150, 52, 160, 48, 168); 
}

void Airspeed::draw(UTFT& TFTDisp) {

    
    // Get Tenths and Hundreds digit
    int tens = (airspeed/10) % 10;
    int hundreds = (airspeed/10/10) % 10;

    int th = (tens * 10) + (hundreds * 100);
    int diff = airspeed - th;

    // Generate a nearest value divisible by 20 for markers
    int marker1;

    if ((hundreds * 100 + tens * 10) % 20 == 0) {
        marker1 = hundreds * 100 + tens * 10;
    } else {
        marker1 = hundreds * 100 + tens * 10 - 10;
    }

    // Clear last markers and create new ones
    TFTDisp.setColor(0, 0, 0);

    int airspeed_markers[6] = {0, 0, 0, 0, 0, 0};
    marker1 = marker1-40;
    for (int i = 0; i < 6; ++i) {
      airspeed_markers[i] = marker1;
      marker1 = marker1 + 20;
      TFTDisp.fillRect(0, airspeed_marker_pos[i], 47, airspeed_marker_pos[i] + 16);
    }

    // Clear old lines
    TFTDisp.fillRect(60, 0, 70, 320);
    TFTDisp.setColor(255, 255, 255);

    // Draw markers on the screen
    for (int i = 0; i < 10; ++i) {
        int y = (i*30) + 10 + (diff/0.33);

        for (int j = 0; j < 6; ++j) {
          if (i-5 == (th - airspeed_markers[j])/10) {
            TFTDisp.printNumI(airspeed_markers[j], 0, y-8, 3);
            airspeed_marker_pos[j] = y-8;
            break;
          }  
        }

        TFTDisp.fillRect(60, y, 70, y + 1);
    }

    // Draw actual airspeed
    TFTDisp.fillRect(0, 151, 48, 168);
    TFTDisp.printNumI(airspeed, LEFT, 152, 3);
}