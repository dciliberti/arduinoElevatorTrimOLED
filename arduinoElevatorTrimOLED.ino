/************************************************************************    
    Copyright (C) 2018 Danilo Ciliberti dancili@gmail.com
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program. If not, see <https://www.gnu.org/licenses/>
*************************************************************************/    

#include <U8glib.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI
 
int xmax = 128;                                  
int ymax = 64;                                   
int xcenter = xmax/2;                            
int ycenter = ymax/2+10;                        
int arc = ymax/2;                             
int angle = 0;                                   
char* label = "TRIM";  
int labelXpos = 53;                               
int p, w, m;
u8g_uint_t xx = 0;
int const potPin = A6;
float potVal;

// Customized function
void gauge(uint8_t angle) {
  
  // draw border of the gauge
  u8g.drawCircle(xcenter,ycenter,arc+6, U8G_DRAW_UPPER_RIGHT);
  u8g.drawCircle(xcenter,ycenter,arc+4, U8G_DRAW_UPPER_RIGHT);
  u8g.drawCircle(xcenter,ycenter,arc+6, U8G_DRAW_UPPER_LEFT);
  u8g.drawCircle(xcenter,ycenter,arc+4, U8G_DRAW_UPPER_LEFT);
  
  // draw the needle
  float x1 = sin(2*angle*2*3.14/360);           
  float y1 = cos(2*angle*2*3.14/360); 
  u8g.drawLine(xcenter, ycenter, xcenter+arc*x1, ycenter-arc*y1);
  u8g.drawDisc(xcenter, ycenter, 5, U8G_DRAW_UPPER_LEFT);
  u8g.drawDisc(xcenter, ycenter, 5, U8G_DRAW_UPPER_RIGHT);
  u8g.setFont(u8g_font_chikita);
   
  // show scale labels
//  u8g.drawStr( 10, 42, "-1.0");                   
//  u8g.drawStr( 19, 14, "-0.5");
//  u8g.drawStr( 63, 14, "0");
//  u8g.drawStr( 92, 14, "0.5");
//  u8g.drawStr( 105, 42, "1.0");
  u8g.drawStr( 1, 42, "DOWN");
  u8g.drawStr( 52, 17 , "NEUT");
  u8g.drawStr( 105, 42, "UP");
  
  // show gauge label
  u8g.setPrintPos(labelXpos,32);            
  u8g.print(label); 
  // show digital value and align its position
  u8g.setFont(u8g_font_profont22);             

  if (w >= 99) {                                  
    u8g.setPrintPos(47,60);
  }
  if (w >= 10 && w < 99) {
    u8g.setPrintPos(54,60);
  }
  if (w >= 0 && w < 9) {                                  
    u8g.setPrintPos(60,60);
  }
  if (w >= -9 && w < 0) {                                  
    u8g.setPrintPos(48,60);
  }
  if (w >= -99 && w < -9) {                                  
    u8g.setPrintPos(42,60);
  }
  if (w < -99) {                                  
    u8g.setPrintPos(35,60);
  }
  u8g.print(w);
}

void setup() {
  Serial.begin(9600);

  u8g.setFont(u8g_font_chikita);
  u8g.setColorIndex(1);                        
  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);                    
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);                      
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);                      
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}

void loop() {
  potVal = analogRead(potPin);
  potVal = (potVal - 512.0)/512.0;
  Serial.print(potVal);
  Serial.print("\n");

  // p is the same as potVal, but it is an integer
  p = analogRead(potPin);                  
  w = map(p,0,1023,100,-100);                    
  m = map(p,0,1023,90,0);                   
  // show needle and dial
  xx = m;                                    
  if (xx < 45){                                
    xx = xx + 135;
  }
  else {
    xx = xx - 45;
  } 
  // picture loop
  {
    u8g.firstPage(); 
    do {             
      gauge(xx);
    }
    while( u8g.nextPage() );
  }
  
  delay(10);
}
