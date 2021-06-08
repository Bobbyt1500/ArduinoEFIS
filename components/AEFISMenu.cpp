#include "AEFISMenu.h"

#include <stdlib.h>
#include <Rotary.h>
#include <EEPROM.h>

void Menu::draw(UTFT& TFTDisp, UTFT_Geometry geo) {
    switch (menu_page) {
        case 0:
            draw_main_menu(TFTDisp, geo);
            break;
        default:
            draw_colors_menu(TFTDisp, geo);
            break;
    }
}

void Menu::draw_main_menu(UTFT& TFTDisp, UTFT_Geometry geo) {
    TFTDisp.clrScr();

    // Selection border
    TFTDisp.setColor(255, 255, 255);
    switch (menu_scroll) {
        case 0:
            TFTDisp.fillRoundRect(48, 28, 202, 152);
            break;
        case 1:
            TFTDisp.fillRoundRect(278, 28, 432, 152);
            break;
        case 2:
            TFTDisp.fillRoundRect(163, 168, 317, 292);
            break;
    }

    // Gray rects
    TFTDisp.setColor(30, 30, 30);

    TFTDisp.fillRoundRect(50, 30, 200, 150);
    TFTDisp.fillRoundRect(280, 30, 430, 150);
    TFTDisp.fillRoundRect(165, 170, 315, 290);

    // Icons
    TFTDisp.setColor(3, 236, 252);
    TFTDisp.fillCircle(125, 65, 20);
    TFTDisp.fillCircle(355, 65, 20);
    

    TFTDisp.setColor(255, 255, 255);

    // Back arrow
    geo.fillTriangle(240, 195, 240, 235, 220, 215);
    TFTDisp.fillRect(240, 220, 270, 210);

    // Text
    TFTDisp.print("Barometer", 53, 100);
    TFTDisp.print("Color", 85, 120);
    TFTDisp.print("Slip Indc", 283, 100);
    TFTDisp.print("Color", 315, 120);
    TFTDisp.print("Back", 208, 250);
}

void Menu::draw_colors_menu(UTFT& TFTDisp, UTFT_Geometry geo) {
  TFTDisp.clrScr();

  for (int i = 0; i < 4; ++i) {
    if (i == menu_scroll) {
      TFTDisp.setColor(255, 255, 255);
      TFTDisp.fillCircle(80 + (100 * i), 160, 42);
    }
    TFTDisp.setColor(color_options[i][0], color_options[i][1], color_options[i][2]);
    TFTDisp.fillCircle(80 + (100 * i), 160, 40);
  }
}

void Menu::handle_scroll(unsigned char result) {
    int upper_limit = menu_page > 0 ? 3 : 2;
    if (result == DIR_CW) {
        menu_scroll--;
        if (menu_scroll < 0) menu_scroll = 0;
    } else {
        menu_scroll++;
        if (menu_scroll > upper_limit) menu_scroll = upper_limit;
    }


      
}

bool Menu::handle_select(Barometer& barometer, TurnCoordinator& turn_coord) {
    switch (menu_page) {
        // Main Menu
        case 0:
            switch(menu_scroll) {
                case 0:
                    menu_page = 1;
                    break;
                case 1:
                    menu_page = 2;
                    break;
                case 2:
                    in_menu = false;
            }
            break;
        //Barometer Menu
        case 1:
            barometer.selected_color[0] = color_options[menu_scroll][0];
            barometer.selected_color[1] = color_options[menu_scroll][1];
            barometer.selected_color[2] = color_options[menu_scroll][2];
            EEPROM.write(0, menu_scroll);
            in_menu = false;
            break;
        // Slip Indc Menu
        case 2:
            turn_coord.selected_color[0] = color_options[menu_scroll][0];
            turn_coord.selected_color[1] = color_options[menu_scroll][1];
            turn_coord.selected_color[2] = color_options[menu_scroll][2];
            EEPROM.write(1, menu_scroll);
            in_menu = false;
            break;        
    }
    
    // Return false if the menu has been exited

    return in_menu;
}

void Menu::load_settings(Barometer& barometer, TurnCoordinator& turn_coord) {
    int barometer_selected = EEPROM.read(0);
    int turn_coord_selected = EEPROM.read(1);

    barometer.selected_color[0] = color_options[barometer_selected][0];
    barometer.selected_color[1] = color_options[barometer_selected][1];
    barometer.selected_color[2] = color_options[barometer_selected][2];

    turn_coord.selected_color[0] =color_options[turn_coord_selected][0];
    turn_coord.selected_color[1] =color_options[turn_coord_selected][1];
    turn_coord.selected_color[2] =color_options[turn_coord_selected][2];
}