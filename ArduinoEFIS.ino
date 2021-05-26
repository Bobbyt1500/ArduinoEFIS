#include <UTFT.h>
#include <UTFT_Geometry.h>
#include <Rotary.h>

UTFT GLCD(ILI9486,38,39,40,41);
UTFT_Geometry geo(&GLCD);

extern uint8_t BigFont[];
extern uint8_t SmallFont[];

// Globals:
String received;

double ind_roll = 0;
double ind_pitch = 0;
double slip_deg = 0;

float barometer_setting = 29.92;

int altitude = 0;
int airspeed = 0;
int heading = 0;
int vertical_speed = 0;

double slope = 0;
int y1;
int y2;
int last_lower = 160;
int last_upper = 160;

int last_mark1_pos = 0;
int last_mark2_pos = 0;
int last_mark3_pos = 0;

int airspeed_marker_pos[6] = {0, 0, 0, 0, 0, 0};
int airspeed_markers[6] = {0, 0, 0, 0, 0, 0};

int heading_markers[4] = {0, 0, 0, 0};
int heading_marker_pos[4] = {70, 70, 70, 70};

int last_vs_arrow = 160;

Rotary r = Rotary(5, 6);

void setup() {


  GLCD.InitLCD();
  GLCD.clrScr();
  GLCD.setFont(BigFont);

  pinMode(4, INPUT);
  digitalWrite(4, HIGH);

  init_efis();

  

  Serial.begin(9600);
  
}

void loop() {

  unsigned char result = r.process();

  if (result) {  
    if (result == DIR_CW) {
      Serial.write('z');
    } else {
      Serial.write('x');
    }
  }

  if (Serial.available() > 0){
    received = Serial.readStringUntil('\n');

    if (received[0] == 'a') {
        //Roll
        ind_roll = received.substring(1).toDouble();
        slope = -1 * tan(ind_roll / 57.29);
        update_attitude();
    }
    if (received[0] == 'b') {
        ind_pitch = received.substring(1).toDouble();
        update_attitude();
    }
    if (received[0] == 'c') {
        altitude = received.substring(1).toInt();
        update_altitude();
    }
    if (received[0] == 'd') {
        airspeed = received.substring(1).toInt();
        update_airspeed();
    }
    if (received[0] == 'e') {
        barometer_setting = received.substring(1).toFloat();
        update_barometer_setting();
    }
    if (received[0] == 'f') {
        heading = received.substring(1).toInt();
        update_heading();
    }
    if (received[0] == 'g') {
        vertical_speed = received.substring(1).toInt();
        update_vertical_speed();
    }
    if (received[0] == 'h') {
        slip_deg = received.substring(1).toDouble();
        update_turn_slip();
    }
  }
}

void init_efis() {
  GLCD.clrScr();
  attitude_setup();
  update_attitude();
  
  altitude_setup();
  update_altitude();

  update_heading();

  airspeed_setup();
  update_airspeed();

  vertical_speed_setup();
  update_vertical_speed();

  update_barometer_setting();

  update_turn_slip();

}

void airspeed_setup() {
  GLCD.setColor(255, 255, 255);
  geo.fillTriangle(48, 150, 52, 160, 48, 168); 
}

void update_airspeed() {

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
    GLCD.setColor(0, 0, 0);

    marker1 = marker1-40;
    for (int i = 0; i < 6; ++i) {
      airspeed_markers[i] = marker1;
      marker1 = marker1 + 20;
      GLCD.fillRect(0, airspeed_marker_pos[i], 47, airspeed_marker_pos[i] + 16);
    }

    // Clear old lines
    GLCD.fillRect(60, 0, 70, 320);
    GLCD.setColor(255, 255, 255);

    // Draw markers on the screen
    for (int i = 0; i < 10; ++i) {
        int y = (i*30) + 10 + (diff/0.33);

        for (int j = 0; j < 6; ++j) {
          if (i-5 == (th - airspeed_markers[j])/10) {
            GLCD.printNumI(airspeed_markers[j], 0, y-8, 3);
            airspeed_marker_pos[j] = y-8;
            break;
          }  
        }

        GLCD.fillRect(60, y, 70, y + 1);
    }

    // Draw actual airspeed
    GLCD.fillRect(0, 151, 48, 168);
    GLCD.printNumI(airspeed, LEFT, 152, 3);
}

void altitude_setup() {
  GLCD.setColor(255, 255, 255);
  geo.fillTriangle(368, 150, 364, 160, 368, 168); 
}

void update_altitude() {

  int ten_thousands = (altitude/10/10/10/10) % 10;
  int thousands = (altitude/10/10/10) % 10;
  int hundreds = (altitude/10/10) % 10;

  int tth = ten_thousands * 10000 + thousands * 1000 + hundreds * 100;
  int marker1 = ten_thousands * 10000 + thousands * 1000 + 500;
  int marker2 = ten_thousands * 10000 + thousands * 1000;
  int marker3 = ten_thousands * 10000 + thousands * 1000 + 1000;
  int diff = altitude - tth;
  
  GLCD.setColor(0, 0, 0);
  GLCD.fillRect(370, last_mark1_pos, 450, last_mark1_pos+16);
  GLCD.fillRect(370, last_mark2_pos, 450, last_mark2_pos+16);
  GLCD.fillRect(370, last_mark3_pos, 450, last_mark3_pos+16);
  GLCD.fillRect(350, 0, 360, 320);
  GLCD.setColor(255, 255, 255);
  
  for(int i = 0; i < 10; ++i) {
      int y = (i*30) + 10 + (diff/3.33);

      if (y >= 290) {
        GLCD.fillRect(350, y, 360, y + 1);
        continue;
      }
    
      if (i-5 == (tth - marker1)/100) {
        GLCD.printNumI(marker1, 370, y-8);
        last_mark1_pos = y-8;  
      }

      if (i-5 == (tth - marker2)/100) {
        GLCD.printNumI(marker2, 370, y-8);
        last_mark2_pos = y-8;  
      }

      if (i-5 == (tth - marker3)/100) {
        GLCD.printNumI(marker3, 370, y-8);
        last_mark3_pos = y-8;  
      }
      
      GLCD.fillRect(350, y, 360, y + 1);
      
  }
  
  GLCD.fillRect(369, 151, 450, 168);
  GLCD.printNumI(altitude, 370, 152, 5);
  
}

void update_heading() {
  
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
  GLCD.setColor(0,0,0);
  marker1-=20;
  for (int i = 0; i < 4; ++i) {
    heading_markers[i] = marker1+(i*20);
    GLCD.fillRect(heading_marker_pos[i], 3, heading_marker_pos[i] + 48, 19);
  }

  GLCD.fillRect(70, 30, 350, 35);
  GLCD.setColor(255, 255, 255);

  
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
            GLCD.printNumI(temp, x-24, 3, 3, '0');
            heading_marker_pos[j] = x-24;
            break;
          }  
      }

      GLCD.fillRect(x, 30, x+1, 35);
      
  }
   
  GLCD.fillRect(185, 17, 234, 0);
  GLCD.printNumI(heading, 186, 1, 3, '0');
}

void attitude_setup() {
  
  GLCD.setColor(209, 126, 25);
  GLCD.fillRect(70, 300, 350, 180);
  GLCD.setColor(25, 89, 209);
  GLCD.fillRect(70, 40, 350, 180);
  
}

void update_attitude() {
  y1 = (slope * (70 - 240)) + 170 + (ind_pitch * 5);
  y2 = (slope * (350 - 240)) + 170 + (ind_pitch * 5);

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
  
  if (y1 >= y2) {
    // Positive Degrees/No Roll

    // Orange Triangle
    GLCD.setColor(209, 126, 25);
    geo.fillTriangle(70, y1, 350, y1, 350, y2);

    // Orange rect
    if (y1 < last_lower) {
      GLCD.fillRect(70, last_lower, 350, y1);
    }

    // Blue Triangle
    GLCD.setColor(25, 89, 209);
    geo.fillTriangle(70, y2, 70, y1, 350, y2);
    
    // Blue rect
    if (y2 > last_upper) {
      GLCD.fillRect(70, last_upper, 350, y2);
    }
    
    last_lower = y1;
    last_upper = y2;
    
  } else {
    // Negative Degrees 
    
    GLCD.setColor(209, 126, 25);
    geo.fillTriangle(70, y1, 70, y2, 350, y2);

    // Orange rect
    if (y2 < last_lower) {
      GLCD.fillRect(70, last_lower, 350, y2);
    }
    
    GLCD.setColor(25, 89, 209);
    geo.fillTriangle(70, y1, 350, y1, 350, y2);

    // Blue rect
    if (y1 > last_upper) {
      GLCD.fillRect(70, last_upper, 350, y1);
    }
    
    last_lower = y2;
    last_upper = y1;
  }
  
}

void update_barometer_setting() {
  GLCD.setColor(3, 236, 252);
  GLCD.fillRect(480, 320, 396, 300);
  GLCD.printNumF(barometer_setting, 2, 398, 302);
}

void vertical_speed_setup() {
  GLCD.setColor(255, 255, 255);
  GLCD.setFont(SmallFont);

  int marker1 = 15;

  for (int i = 0; i < 31; ++i) {
    int y = (i * 8) + 40;
    
    if (i % 5 == 0) {
      
      GLCD.fillRect(480, y-1, 475, y+1);
      if (abs(marker1) >= 10) {
        GLCD.printNumI(abs(marker1), 459, y-6);
      } else {
        GLCD.printNumI(abs(marker1), 467, y-6);
      }
      marker1-=5;
      
    } else {
     
      GLCD.drawLine(480, y, 475, y);
      
    }
  }
  
  GLCD.setFont(BigFont);
}

void update_vertical_speed() {
  // Clear last vertical speed arrow
  GLCD.setColor(0, 0, 0);
  GLCD.fillRect(452, last_vs_arrow-5, 457, last_vs_arrow + 5);

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
  GLCD.setColor(251, 0, 255);
  geo.fillTriangle(452, y-5, 452, y+5, 457, y);
}

void update_turn_slip() {
  GLCD.setColor(0, 0, 0);
  GLCD.fillRect(70, 320, 350, 300);
  
  GLCD.setColor(255, 255, 255);
  GLCD.fillRect(195, 320, 194, 300);
  GLCD.fillRect(225, 320, 226, 300);

  int x = 10.0 * abs(slip_deg);

  if (slip_deg < 0) {
    x = x + 210;
  } else {
    x = 210 - x;
  }
  
  GLCD.setColor(242, 239, 27);
  GLCD.fillCircle(x, 310, 10);
}

void boot_sequence() {
  GLCD.setColor(255, 255, 255);
  GLCD.print("Arduino", CENTER, 100);
  GLCD.print("EFIS", CENTER, 120);

  GLCD.fillRect(199, 251, 281, 149);
  GLCD.setColor(25, 89, 209);
  GLCD.fillRect(200, 150, 280, 200);
  GLCD.setColor(209, 126, 25);
  GLCD.fillRect(200, 200, 280, 250);

  delay(10000);
  GLCD.setColor(0, 0, 0);
  GLCD.fillRect(0, 0, 480, 320);
}
