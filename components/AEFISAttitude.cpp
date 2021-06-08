#include "AEFISAttitude.h"

void correct_OOB(int& y1, int& y2) {
    if (y1 < 40) {
        y1 = 40;
    } else if (y1 > 300) {
        y1 = 300;
    }

    if (y2 < 40) {
        y2 = 40;
    } else if (y2 > 300) {
        y2 = 300;
    }
}

void Attitude::init(UTFT& TFTDisp, UTFT_Geometry geo) {
    TFTDisp.setColor(209, 126, 25);
    TFTDisp.fillRect(70, 300, 350, 170);
    TFTDisp.setColor(25, 89, 209);
    TFTDisp.fillRect(70, 40, 350, 170);
}

void Attitude::draw(UTFT& TFTDisp, UTFT_Geometry geo) {
    double slope = tan(ind_roll / 57.29);
    
    int y1 = (slope * (70 - 210)) + 170 + (ind_pitch * 5);
    int y2 = (slope * (350 - 210)) + 170 + (ind_pitch * 5);

    // Negative roll
    if (y1 >= y2) {
        for (int i = 0; i < (y1-y2); ++i) {
            int y = i + y2;
            if (y < 40) {
                i = 40-y2;
                continue;
            }
            if (y > 300) break;

            int x = i/abs(slope) + 70;
            
            TFTDisp.setColor(25, 89, 209);
            TFTDisp.drawLine(x, y, 350, y);

            TFTDisp.setColor(209, 126, 25);
            TFTDisp.drawLine(x, y, 70, y);
        } 

        correct_OOB(y1, y2);

        // Orange rect
        if (y1 < last_lower) {
            TFTDisp.setColor(209, 126, 25);
            TFTDisp.fillRect(70, last_lower, 350, y1);
        }

        // Blue rect
        if (y2 > last_upper) {
            TFTDisp.setColor(25, 89, 209);
            TFTDisp.fillRect(70, last_upper, 350, y2);
        }
        
        last_lower = y1;
        last_upper = y2;
    } else {
        // Positive Roll
        for (int i = 0; i < (y2-y1); ++i) {
            int y = i + y1;
            if (y < 40) {
                i = 40-y1;
                continue;
            }
            if (y > 300) break;
    
            int x = (280 - i/slope) + 70;

            TFTDisp.setColor(209, 126, 25);
            TFTDisp.drawLine(x, y, 350, y);

            TFTDisp.setColor(25, 89, 209);
            TFTDisp.drawLine(x, y, 70, y);
        }

        correct_OOB(y1, y2);

        // Orange rect
        if (y2 < last_lower) {
            TFTDisp.setColor(209, 126, 25);
            TFTDisp.fillRect(70, last_lower, 350, y2);
        }
        
        // Blue rect
        if (y1 > last_upper) {
            TFTDisp.setColor(25, 89, 209);
            TFTDisp.fillRect(70, last_upper, 350, y1);
        }
        
        last_lower = y2;
        last_upper = y1;
    }
    
    
    
}

