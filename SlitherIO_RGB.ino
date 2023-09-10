
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <RGBmatrixPanel.h>

#define CLK  8   // USE THIS ON ARDUINO UNO, ADAFRUIT METRO M0, etc.
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

int gameLevel = 0;
int MAX_LEVEL = 201;
int tail[201][2]; //Head = Position [0,0]. 0 is Position, 1 is the color
int food[201]; //food balls placed all over
int foodFound = 0;
int currentX = 8;
int currentY = 16;
int game = -1; //1 or 2. Zero means not selected

#define key2 1 //2 = Orange wire
#define key1 11 //1 = Yellow wire
#define key4 12 //4 = Green wire
#define key3 13 //3 = Blue wire

uint16_t color1; 
uint16_t color2;
uint16_t color3;
uint16_t color4;

boolean key1bouce = false;
boolean key2bouce = false;
boolean key3bouce = false;
boolean key4bouce = false;

uint16_t selectedColor;

MPU6050 mpu6050(Wire);

unsigned long timerStart;
int TIMEOUT = 60000; //60s
boolean timeRanout = false;

int addToTail(int pos, uint16_t color333) {
  int pop = tail[foodFound][0]; //last item in tail 
  uint16_t popColor = tail[foodFound][1];
  for (int i = foodFound; i > 0; i--) {
    //Shift the tail to make room on head;
    tail[i][0] = tail[i - 1][0];
    tail[i][1] = tail[i - 1][1];    
  }
  tail[0][0] = pos;
  tail[0][1] = color333;  
  return pop;
}

void deleteFromTail(int pos) { 
    for (int i = 0; i < MAX_LEVEL; i++) {
    if (tail[i][0] == pos) {
      tail[i][0] = -1;
      tail[i][1] = matrix.Color333(0, 7, 0);
      break;
    }
  }
  return;
}

boolean isTouchingTheTail(int pos) {
  boolean touching = false;
  for (int i = 1; i <= foodFound; i++) {
    if (tail[i][0] == pos) {
      touching = true;
      break;
    }
  }
  return touching;
}

void displayFood(boolean resetDisplay) {
  if (resetDisplay) {
    clearScreen();
  }
  for (int i = 0; i < MAX_LEVEL; i++) {
    if (food[i] >= 0) {
      int row = food[i] % 16; //y axis (0 to 7)
      int col = food[i] / 16; //x axis (0 to 31)
      matrix.drawPixel(col, row, matrix.Color333(7, 0, 0));
    }
  }
}

boolean isFood(int pos) {
  boolean found = false;
  for (int i = 0; i < MAX_LEVEL; i++) {
    if (food[i] == pos) {
      found = true;
      break;
    }
  }
  return found;
}

void eatFood(int pos) {      
  for (int i = 0; i < MAX_LEVEL; i++) {
    if (food[i] == pos) {
      food[i] = -1;
      break;
    }
  }
}

void addFood(int pos) {
   for (int i = 0; i < MAX_LEVEL; i++) {
      if (food[i] == -1) {
        //empty location
        food[i] = pos;
        break;
      }
   }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Slither IO");
  Wire.begin();
  pinMode(key1, INPUT_PULLUP);// set pin as input
  pinMode(key2, INPUT_PULLUP);// set pin as input
  pinMode(key3, INPUT_PULLUP);// set pin as input
  pinMode(key4, INPUT_PULLUP);// set pin as input  
  
  attachInterrupt(digitalPinToInterrupt(key1),key1Pressed,FALLING); //CHANGE, RISING, FALLING, LOW, HIGH
  attachInterrupt(digitalPinToInterrupt(key2),key2Pressed,FALLING); 
  attachInterrupt(digitalPinToInterrupt(key3),key3Pressed,FALLING); 
  attachInterrupt(digitalPinToInterrupt(key4),key4Pressed,FALLING); 

  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  randomSeed(analogRead(0));
  matrix.begin(); 
  displayStart();
}

void loop() {
  delay(50);

  mpu6050.update();
  int yAngle = mpu6050.getAccAngleY();   
  int xAngle = mpu6050.getAccAngleX(); 
  int zAcc = mpu6050.getAccZ()*100;
  // Serial.print("zAcc : "); Serial.println(zAcc);

  if ((abs(zAcc) > 160) ) {
    game = -1;
    resetDrawScreen();
    currentX = 8;
    currentY = 16;
    return;
  }
  
 Serial.print("accAngleX : "); Serial.print(xAngle);
 Serial.print("\taccAngleY : "); Serial.print(yAngle);
  
  currentX = (xAngle < -8) ? currentX + 1 : ((xAngle > 8) ? currentX - 1 : currentX);
  currentY = (yAngle < -6) ? currentY + 1 : ((yAngle > 6) ? currentY - 1 : currentY);
  currentX = (currentX > 15) ? 15 : ((currentX < 0) ? 0 : currentX);
  currentY = (currentY > 31) ? 31 : ((currentY < 0) ? 0 : currentY); 

  unsigned int currentPos = currentX + 16*currentY; 
              
 Serial.print("\tX : "); Serial.print(currentX);   
 Serial.print("\tY : "); Serial.print(currentY);
 Serial.print("\tCurrentPos : "); Serial.println(currentPos);
  
//  Serial.print("Tail: ");
//  for (int i = 0; i <= foodFound; i++) {
//    Serial.print(tail[i]);
//    Serial.print(" ");
//  }  
//  Serial.println();

  switch (game) {
    case -1:
      for (int i = 0; i<MAX_LEVEL; i++) {
        tail[i][0] = -1;
        tail[i][1] = matrix.Color333(0, 7, 0);
        food[i] = -1;
      }
      currentX = 0;
      currentPos = currentX + 16*currentY; 
      clearScreen();
      displayGameStart();
      game = 0;
      break;
    case 0:
      displayGameStart();
      selectGame(currentPos);
      break;
    case 1: 
      playGame1(currentPos);
      break;
    case 2:
      playGame2(currentPos);
      break;
    case 3:
      playDice();
      break;
    case 4:
      draw(currentPos);
      break;
//    case 5:
//      loopAudio();
//      break;
    }
}

void blinkLED(int row, int col, uint16_t color333, boolean on=true) {
    if (on) {
      for (int i = 0; i < 2; i++) {
        matrix.drawPixel(col, row, matrix.Color333(0, 0, 0));
        delay(40);
        matrix.drawPixel(col, row, color333);
        delay(40);
      }  
    } else {
      for (int i = 0; i < 2; i++) {
        matrix.drawPixel(col, row, color333);
        delay(40);
        matrix.drawPixel(col, row, matrix.Color333(0, 0, 0));
        delay(40);
      }  
    }
}

void selectGame(int currentPos) {    
   Serial.print("Select Game"); 
   Serial.print("\tCurrentPos : "); Serial.println(currentPos);

    int clearPos = addToTail(currentPos, matrix.Color333(0, 7, 0));
    int row = clearPos % 16; //y axis (0 to 7)
    int col = clearPos / 16; //x axis (0 to 31)
    Serial.print("\tclearPos : "); Serial.println(clearPos);
    matrix.drawPixel(col, row, matrix.Color333(0, 0, 0));

    //set the currentPos
    row = currentPos % 16; //y axis (0 to 7)
    col = currentPos / 16; //x axis (0 to 31)
    blinkLED(row,col, matrix.Color333(0, 7, 0));

    if ((row >5 ) && (row <= 11)) {
      if ((col >=5 ) && (col <= 7)) {
        game = 1; 
        resetGame1();
      } else if ((col >= 10) & (col <= 14)) {
        game = 2; 
        resetGame2();
      } else if ((col >= 16) & (col <= 20)) {
        game = 3; 
        setupDice();
      } else if ((col >= 22) & (col <= 26)) {
        game = 4; 
        resetDrawScreen();
    //  } else if ((col >= 25) & (col <= 29)) {
    //    game = 5; 
    //    resetDrawScreen();
    //    setupAudio();
      }
    }
}

void key1Pressed() {
  if (!key1bouce) {
    key1bouce = true;
    Serial.print(millis());
    Serial.println("\tkey 1 is pressed");
    selectedColor = color1;
  }
}

void key2Pressed() {
  if (!key2bouce) {
    key2bouce = true;
    Serial.print(millis());
    Serial.println("\tkey 2 is pressed");
    selectedColor = color2;
  }
}

void key3Pressed() {
  if (!key3bouce) {
   key3bouce = true;
   Serial.print(millis());
   Serial.println("\tkey 3 is pressed");
   selectedColor = color3;
  }
}

void key4Pressed() {
  if (!key4bouce) {
    key4bouce = true;
    Serial.print(millis());
    Serial.println("\tkey 4 is pressed");
    selectedColor = color4;
  }
}

void timeoutBar() {
   unsigned long timePassed = millis() - timerStart;
   int ledToLightUp = map(timePassed, 0, TIMEOUT, 0,32);
   switch (game) {
     case 1: 
       //matrix.drawLine(0, 15, ledToLightUp, 15, matrix.Color333(1, 1, 1));
       if (ledToLightUp >=32) {
           timeRanout = true;
       }
       break;
     case 2:
     case 4:
       //matrix.drawPixel(ledToLightUp, 15, matrix.Color333(1, 1, 1));
       if (ledToLightUp >=32) {
           timeRanout = true;
       }
       break;
   }
}