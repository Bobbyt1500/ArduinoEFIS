#include <UTFT.h>
#include <UTFT_Geometry.h>
#include <Rotary.h>

#include <AEFISMenu.h>
#include <AEFISBarometer.h>
#include <AEFISTurnCoord.h>
#include <AEFISAirspeed.h>
#include <AEFISAltitude.h>
#include <AEFISHeading.h>
#include <AEFISAttitude.h>
#include <AEFISVertSpeed.h>

UTFT TFTDisp(ILI9486,38,39,40,41);
UTFT_Geometry geo(&TFTDisp);
Menu menu;
Barometer barometer_component;
TurnCoordinator turn_coord_component;
Airspeed airspeed_component;
Altitude altitude_component;
Heading heading_component;
Attitude attitude_component;
VerticalSpeed vertspeed_component;

extern uint8_t BigFont[];
extern uint8_t SmallFont[];

String received;

Rotary r = Rotary(5, 6);

void setup() {

  TFTDisp.InitLCD();
  TFTDisp.clrScr();
  TFTDisp.setFont(BigFont);

  pinMode(4, INPUT);
  digitalWrite(4, HIGH);

  boot_sequence();

  init_efis();
  

  Serial.begin(9600);
  
}

void loop() {

  unsigned char result = r.process();

  if (menu.in_menu) {
    if (result) {
      menu.handle_scroll(result);
      menu.draw(TFTDisp, geo);
    }
    if (digitalRead(4) == LOW) {
      
      if (menu.handle_select(barometer_component, turn_coord_component)) {
        menu.draw(TFTDisp, geo);
      } else {
        init_efis();
      }
      
    }
  } else {
    if (result) {  
      if (result == DIR_CW) {
        Serial.write('z');
      } else {
        Serial.write('x');
      }
    }

    if (digitalRead(4) == LOW) {
      menu.in_menu = true;
      menu.menu_page = 0;
      menu.draw(TFTDisp, geo);
    }
  
    if (Serial.available() > 0){
      received = Serial.readStringUntil('\n');
  
      if (received[0] == 'a') {
          //Roll
          attitude_component.ind_roll = received.substring(1).toDouble();
          attitude_component.draw(TFTDisp, geo);
      }
      if (received[0] == 'b') {
          attitude_component.ind_pitch = received.substring(1).toDouble();
          attitude_component.draw(TFTDisp, geo);
      }
      if (received[0] == 'c') {
          altitude_component.altitude = received.substring(1).toInt();
          altitude_component.draw(TFTDisp);
      }
      if (received[0] == 'd') {
          airspeed_component.airspeed = received.substring(1).toInt();
          airspeed_component.draw(TFTDisp);
      }
      if (received[0] == 'e') {
          barometer_component.setting = received.substring(1).toFloat();
          barometer_component.draw(TFTDisp);
      }
      if (received[0] == 'f') {
          heading_component.heading = received.substring(1).toInt();
          heading_component.draw(TFTDisp);
      }
      if (received[0] == 'g') {
          vertspeed_component.vertical_speed = received.substring(1).toInt();
          vertspeed_component.draw(TFTDisp, geo);
      }
      if (received[0] == 'h') {
          turn_coord_component.slip_deg = received.substring(1).toDouble();
          turn_coord_component.draw(TFTDisp);
      }
    }
  }
  
}

void init_efis() {
  // Load saved settings
  menu.load_settings(barometer_component, turn_coord_component);
  
  // Draw first graphics
  TFTDisp.clrScr();
  
  attitude_component.init(TFTDisp, geo);
  attitude_component.draw(TFTDisp, geo);
  
  altitude_component.init(TFTDisp, geo);
  altitude_component.draw(TFTDisp);

  heading_component.draw(TFTDisp);

  airspeed_component.init(TFTDisp, geo);
  airspeed_component.draw(TFTDisp);

  vertspeed_component.init(TFTDisp);
  vertspeed_component.draw(TFTDisp, geo);

  barometer_component.draw(TFTDisp);

  turn_coord_component.draw(TFTDisp);

}

void boot_sequence() {
  TFTDisp.setColor(255, 255, 255);
  TFTDisp.print("Arduino", CENTER, 100);
  TFTDisp.print("EFIS", CENTER, 120);

  TFTDisp.fillRect(199, 251, 281, 149);
  TFTDisp.setColor(25, 89, 209);
  TFTDisp.fillRect(200, 150, 280, 200);
  TFTDisp.setColor(209, 126, 25);
  TFTDisp.fillRect(200, 200, 280, 250);

  delay(10000);
  TFTDisp.setColor(0, 0, 0);
  TFTDisp.fillRect(0, 0, 480, 320);
}
