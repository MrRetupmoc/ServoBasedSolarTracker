#include <Servo.h> 

int movesleep_find = 30; //25
int movesleep_track = 15;
int movesleep_home = 500; //25
int tracking_offset = 5;

Servo myservoX, myservoY;
  int posX = 0;
  int posX_home = 0;
  int posX_min = 0;
  int posX_max = 180;
  
  int posY = 0;
  int posY_home = 93;
  int posY_min = 35;
  int posY_max = 150;
  int posY_min_mid = (posY_home - ((posY_home - posY_min) / 2));
  int posY_max_mid = (posY_home + ((posY_max - posY_home) / 2));
  

int LightValue[4]; //lightX, positionX, lightY, positionY
  int sensorPin = A0;
  int sensorValue = 0;

int x,y = 0;
int xless, xmore = 0;



void setup() {
  myservoX.attach(11);
  myservoY.attach(10);
  //myservoX.write(posX_home);
  //myservoY.write(posY_home);
    Serial.begin(9600);
    Serial.println(".");
    Serial.println("Hello Sunny World.");
    Serial.println("Now Finding Light...");
  delay(movesleep_home);
  
  //Tilt Min Mid
    testXaxis(posX_min, posX_max, posY_min_mid);
  
  //Tilt Home - Flat
    //testXaxis(posX_min, posX_max, posY_home);
  
  //Tilt Max Mid
    testXaxis(posX_max, posX_min, posY_max_mid);
  
  //Y locate
    // Test for Max Sun Angle while at this Location
    //testYaxis(posY_min, posY_max);
    testYaxis(posY_max, posY_min);
}

void loop() {
  printcapturedYvalue();
  delay(10000);
  
  // Retest X axis
  myservoX.write(LightValue[1]);
    delay(movesleep_track);
    LightValue[0] = checkLightLevels();
  
  myservoX.write(LightValue[1] + tracking_offset);
    delay(movesleep_track);
    xmore = checkLightLevels();
  
  myservoX.write(LightValue[1] - tracking_offset);
    delay(movesleep_track);
    xless = checkLightLevels();
  
  if (xmore > LightValue[0] && xless <= LightValue[0]) {
    for(int i = 0; i <= 9; i++) {
      myservoX.write(LightValue[1] + i);
      delay(movesleep_track);
      x = checkLightLevels();
        if (x > LightValue[0]) {
          LightValue[1] = LightValue[1] + i;
          LightValue[0] = x;
        }
    }
  }
  else if (xless > LightValue[0] && xmore <= LightValue[0]) {
    for(int i = 0; i <= 9; i++) {
      myservoX.write(LightValue[1] - i);
      delay(movesleep_track);
      x = checkLightLevels();
        if (x > LightValue[0]) {
          LightValue[1] = LightValue[1] - i;
          LightValue[0] = x;
        }
    }
  }
  /*yvalues
  myservoX.write(LightValue[1]);
    delay(movesleep_track);
    LightValue[0] = checkLightLevels();
  
  myservoX.write(LightValue[1] + tracking_offset);
    delay(movesleep_track);
    xmore = checkLightLevels();
  
  myservoX.write(LightValue[1] - tracking_offset);
    delay(movesleep_track);
    xless = checkLightLevels();
  */
  /*
  // Negitive
  for(int i = 0; i <= 15; i=i+3) {
    myservoX.write(LightValue[1] - i);
    delay(movesleep_track);
    x = checkLightLevels();
      if (x > LightValue[2]) {
        LightValue[1] = LightValue[1] - i;
        LightValue[0] = x;
      }
    delay(movesleep_find);
  }
  
  // Set X axis
  myservoX.write(LightValue[1]);
  delay(movesleep_track);
  
  
  
  // Retest Y axis
  myservoY.write(LightValue[3]);
  delay(movesleep_track);
  LightValue[2] = checkLightLevels();
  delay(movesleep_find);
  
  // Positive
  for(int i = 0; i <= 15; i=i+3) {
    myservoY.write(LightValue[3] + i);
    delay(movesleep_track);
    y = checkLightLevels();
      if (y > LightValue[2]) {
        LightValue[3] = LightValue[3] + i;
        LightValue[2] = y;
      }
    delay(movesleep_find);
  }
  
  
  // Negitive
  for(int i = 0; i <= 15; i=i+3) {
    myservoY.write(LightValue[3] - i);
    delay(movesleep_track);
    y = checkLightLevels();
      if (y > LightValue[2]) {
        LightValue[3] = LightValue[3] - i;
        LightValue[2] = y;
      }
    delay(movesleep_find);
  }
  
  // Set Y axis
  myservoY.write(LightValue[3]);
  delay(movesleep_track);
  
  //delay(5000);
  */
} 

void testXaxis(int Xmin, int Xmax, int Ymin) {
  //
    myservoX.write(Xmin);
    myservoY.write(Ymin);
  delay(movesleep_home);
  
  //Retest Locate Sun X axis
  if(Xmin < Xmax) {
    for(posX = Xmin; posX <= Xmax; posX++) {
      testXloop(posX);
    }
  }
  else if(Xmin > Xmax)  {
    for(posX = Xmin; posX >= Xmax; posX--) {
      testXloop(posX);
    }
  }
  
  printcapturedXvalue();
}

void testXloop(int pos) {
  myservoX.write(pos);
  delay(movesleep_find);
  x = checkLightLevels();
    if (x > LightValue[0]) {
      LightValue[0] = x;
      LightValue[1] = pos;
    }
}

void testYaxis(int Ymin, int Ymax) {
  //
    //myservoX.write(posX_home);
    //myservoY.write(posY_home);
    delay(movesleep_home);
  
  //Retest Locate Sun Y axis
  if(Ymin < Ymax) {
    for(posY = Ymin; posY <= Ymax; posY++) {
      testYloop(posY);
    }
  }
  else if(Ymin > Ymax)  {
    for(posY = Ymin; posY >= Ymax; posY--) {
      testYloop(posY);
    }
  }
}

void testYloop(int pos) {
  myservoY.write(pos);
  delay(movesleep_find*2);
  y = checkLightLevels();
    if (y > LightValue[2]) {
      LightValue[2] = y;
      LightValue[3] = pos;
    }
}

void printcapturedXvalue() {
  myservoX.write(LightValue[1]);
    Serial.print(" When Init Scanning X : ");
    Serial.print(LightValue[0]);
    Serial.print(" @ X = ");
    Serial.println(LightValue[1]);
  delay(movesleep_home);
}

void printcapturedYvalue() {
  myservoX.write(LightValue[1]);
  myservoY.write(LightValue[3]);
    Serial.print(" When Scanning X : ");
    Serial.print(LightValue[0]);
    Serial.print(" When Scanning Y : ");
    Serial.print(LightValue[2]);
    Serial.print(" @ X = ");
    Serial.print(LightValue[1]);
    Serial.print(" @ Y = ");
    Serial.println(LightValue[3]);
  delay(movesleep_home);
}

int checkLightLevels() {
  sensorValue = analogRead(sensorPin);
    //Serial.print("Light Value : ");
    //Serial.println(sensorValue);
  return sensorValue;
} 
