#include "AEFISAltitude.h"

void Altitude::init(UTFT& TFTDisp, UTFT_Geometry geo) {
    TFTDisp.setColor(255, 255, 255);
    geo.fillTriangle(368, 150, 364, 160, 368, 168); 
}

void Altitude::draw(UTFT& TFTDisp) {
    
    int ten_thousands = (altitude/10/10/10/10) % 10;
    int thousands = (altitude/10/10/10) % 10;
    int hundreds = (altitude/10/10) % 10;

    int tth = ten_thousands * 10000 + thousands * 1000 + hundreds * 100;
    int marker1 = ten_thousands * 10000 + thousands * 1000 + 500;
    int marker2 = ten_thousands * 10000 + thousands * 1000;
    int marker3 = ten_thousands * 10000 + thousands * 1000 + 1000;
    int diff = altitude - tth;
    
    TFTDisp.setColor(0, 0, 0);
    TFTDisp.fillRect(370, last_mark1_pos, 450, last_mark1_pos+16);
    TFTDisp.fillRect(370, last_mark2_pos, 450, last_mark2_pos+16);
    TFTDisp.fillRect(370, last_mark3_pos, 450, last_mark3_pos+16);
    TFTDisp.fillRect(350, 0, 360, 320);
    TFTDisp.setColor(255, 255, 255);
    
    for(int i = 0; i < 10; ++i) {
        int y = (i*30) + 10 + (diff/3.33);

        if (y >= 290) {
        TFTDisp.fillRect(350, y, 360, y + 1);
        continue;
        }

        if (i-5 == (tth - marker1)/100) {
        TFTDisp.printNumI(marker1, 370, y-8);
        last_mark1_pos = y-8;  
        }

        if (i-5 == (tth - marker2)/100) {
        TFTDisp.printNumI(marker2, 370, y-8);
        last_mark2_pos = y-8;  
        }

        if (i-5 == (tth - marker3)/100) {
        TFTDisp.printNumI(marker3, 370, y-8);
        last_mark3_pos = y-8;  
        }

        TFTDisp.fillRect(350, y, 360, y + 1);
        
    }
    
    TFTDisp.fillRect(369, 151, 450, 168);
    TFTDisp.printNumI(altitude, 370, 152, 5);
}