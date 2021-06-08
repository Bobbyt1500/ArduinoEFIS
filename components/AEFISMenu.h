#ifndef AEFIS_MENU_H
#define AEFIS_MENU_H

#include "AEFISBarometer.h"
#include "AEFISTurnCoord.h"

#include <UTFT.h>
#include <UTFT_Geometry.h>

class Menu {
    private:
        int menu_scroll = 0;
        int color_options[4][3] = {{3, 236, 252},{245, 66, 233},{242, 239, 27},{31, 235, 0}};
    public:
        int menu_page = 0;
        bool in_menu = false;
        void draw(UTFT& TFTDisp, UTFT_Geometry geo);
        void draw_main_menu(UTFT& TFTDisp, UTFT_Geometry geo);
        void draw_colors_menu(UTFT& TFTDisp, UTFT_Geometry geo);
        void handle_scroll(unsigned char result);
        bool handle_select(Barometer& barometer, TurnCoordinator& turn_coord);
        void load_settings(Barometer& barometer, TurnCoordinator& turn_coord);
};

#endif