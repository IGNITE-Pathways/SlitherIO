//Coordinates of the Dice points in the LED matrix
int DicePic[8][6][2] =
{
  { //empty matrix and start position:
    {9,9}, //1. Point
    {9,8}, //2. Point
    {9,7}, //3. Point
    {9,6}, //4. Point
    {9,5}, //5. Point
    {9,4} //6. Point
  },
  { //1:
    {4,4}, //1. Point
    {-1,-1}, //2. Point
    {-1,-1}, //3. Point
    {-1,-1}, //4. Point
    {-1,-1}, //5. Point
    {-1,-1}  //6. Point
  },
  { //2:
    {2,2}, //1. Point
    {6,6}, //2. Point
    {-1,-1}, //3. Point
    {-1,-1}, //4. Point
    {-1,-1}, //5. Point
    {-1,-1}  //6. Point
  },
  { //3:
    {2,6}, //1. Point
    {6,2}, //2. Point
    {4,4}, //3. Point
    {-1,-1}, //4. Point
    {-1,-1}, //5. Point
    {-1,-1}  //6. Point
  },
  { //4:
    {2,2}, //1. Point
    {2,6}, //2. Point
    {6,2}, //3. Point
    {6,6}, //4. Point
    {-1,-1}, //5. Point
    {-1,-1}  //6. Point
  },
  { //5:
    {2,2}, //1. Point
    {2,6}, //2. Point
    {6,2}, //3. Point
    {6,6}, //4. Point
    {4,4}, //5. Point
    {-1,-1}  //6. Point
  },
  { //6:
    {2,1}, //1. Point
    {2,4}, //2. Point
    {2,7}, //3. Point
    {6,1}, //4. Point
    {6,4}, //5. Point
    {6,7}  //6. Point
  },
  { //Start:
    {-1,-1}, //1. Point
    {-1,-1}, //2. Point
    {-1,-1}, //3. Point
    {-1,-1}, //4. Point
    {-1,-1}, //5. Point
    {-1,-1}  //6. Point
  }
  };

//Variables of the dice: position, direction, speed for X and Y
float DiceXpos[6];
float DiceXdir[6];
volatile byte DiceXspeed[6];
float DiceYpos[6];
float DiceYdir[6];
volatile byte DiceYspeed[6];

int DiceValue;
unsigned long timestamp;
byte Mode;
int volatile shakes;
int ShakesPerSecond;
int step;

void InterruptChecks() {
  //Count Shakes
  shakes=shakes+1;
  Serial.println(millis());
  timestamp=millis();
}

void SetSpeedX() {
  if (Mode==0) {
    //Speed-up dice in X
    for (int i = 0; i < 6; i++) {
      if (DiceXspeed[i]<255) {DiceXspeed[i]=DiceXspeed[i]+5;}
    }
  }  
  InterruptChecks();
}

void SetSpeedY() {
  if (Mode==0) {
    //Speed-up dice in Y
    for (int i = 0; i < 6; i++) {
      if (DiceYspeed[i]<255) {DiceYspeed[i]=DiceYspeed[i]+5;}
    }
  }
  InterruptChecks();
}

void ShowLed(int x, int y, bool onoff) {
  //show only, when x/y in matrix
  if ((x<8) and (y<8) and (x>=0) and (y>=0)) {
      if (onoff) {
        matrix.drawPixel(y+12, x+4, matrix.Color333(7, 7, 7));
      } else {
        matrix.drawPixel(y+12, x+4, matrix.Color333(0, 0, 0));        
      }
  }
  
}

void ShowDot(int x, int y, bool onoff) {
//Show or hide dice point
  ShowLed(x-1, y-1, onoff);
  ShowLed(x, y-1, onoff);
  ShowLed(x-1, y, onoff);
  ShowLed(x, y, onoff);
}

void ShowDicePic(int value) {
  //Show dice

  boolean done;

  //move all points from current position to destination of DiceValue
  for (int i = 0; i < 6; i++) {
    DiceXspeed[i]=100;
    DiceYspeed[i]=100;

    //Calc x values
    DiceXdir[i]=0;
    if (int(DiceXpos[i])>DicePic[value][i][0]) {DiceXdir[i]=-1;} 
    else if (int(DiceXpos[i])<DicePic[value][i][0]) {DiceXdir[i]=1;} 
    
    DiceYdir[i]=0;
    if (int(DiceYpos[i])>DicePic[value][i][1]) {DiceYdir[i]=-1;} 
    else if (int(DiceYpos[i])<DicePic[value][i][1]) {DiceYdir[i]=1;} 
  }

  // Serial.println(value);
  // Serial.println("Start moving");
  //Move Points
  do {
  // Serial.println("Moving");
    for (int i = 0; i < 6; i++) {
      if (int(DiceXpos[i])!=DicePic[value][i][0]) {
        DoStep(DiceXpos[i],DiceXdir[i],DiceXspeed[i],false);
      }
      if (int(DiceYpos[i])!=DicePic[value][i][1]) {
        DoStep(DiceYpos[i],DiceYdir[i],DiceYspeed[i],false);
      }
    }

//    lc.clearDisplay(1);
//    lc.clearDisplay(2);
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0));
  for (int i = 0; i < 6; i++) {
      ShowDot(int(DiceXpos[i]), int(DiceYpos[i]), true);
    }
    
    delay(30);

    //Dice points are on destition position
    done=true;
    for (int i = 0; i < 6; i++) {
      if (int(DiceXpos[i])!=DicePic[value][i][0]) {done=false;}
      if (int(DiceYpos[i])!=DicePic[value][i][1]) {done=false;}
    }

  } while (done==false);
  //Serial.println("End moving");

//    lc.clearDisplay(1);
//    lc.clearDisplay(2);
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0));
  for (int i = 0; i < 6; i++) {
    ShowDot(DicePic[value][i][0],DicePic[value][i][1], true);
  }
  //Should we reset?
  ShakesPerSecond = 0;
}

void DoStep(float &pos, float &dir, volatile byte &sp, bool check) {
  pos=pos+float(sp)/255*dir;

  if (check==true) {
    if (pos>7) {
        pos=7;
        dir=dir*(-1);
        }
    if (pos<1) {
        pos=1;
        dir=dir*(-1);
      }

  }
  // Velocity decreases every step
    if (sp>0) {sp=sp-1;}
}

void MoveDots() {
  //move dice points one step further
  for (int i = 0; i < 6; i++) {
    //calc new coordinates
    DoStep(DiceXpos[i],DiceXdir[i],DiceXspeed[i],true);
    DoStep(DiceYpos[i],DiceYdir[i],DiceYspeed[i],true);
  }

    //show dice points
//    lc.clearDisplay(1);
//    lc.clearDisplay(2);
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0));
  for (int i = 0; i < 6; i++) {
      ShowDot(int(DiceXpos[i]), int(DiceYpos[i]), true);
    }
  
}

void setupDice() {
  DiceValue=0;
  
  for (int i = 0; i < 6; i++) {
    DiceXpos[i]=DicePic[7][i][0];
    DiceYpos[i]=DicePic[7][i][1];
    
    DiceXdir[i]=random(3)-1;
    DiceYdir[i]=random(3)-1;
    DiceXspeed[i]=random(126)+120;
    DiceYspeed[i]=random(126)+120;
  }
  
  clearScreen();  
  timestamp=millis();
  Mode=1;

  ShowDicePic(6);
  delay(1000);
  
  clearScreen();  
  
  Mode=0;
  Serial.begin(9600);
  step=0;
  shakes=0;
  timerStart = millis();
}

int oldXAngle = 0;
int oldYAngle = 0;


//Loop
void playDice() {
  mpu6050.update();

//  Serial.print("accX : ");Serial.print(mpu6050.getAccX());
//  Serial.print("\taccY : ");Serial.print(mpu6050.getAccY());
//  Serial.print("\taccZ : ");Serial.println(mpu6050.getAccZ());
  
//  int yAngle = mpu6050.getAngleY();   
//  int xAngle = mpu6050.getAngleX(); 
//  
//  if (abs(xAngle - oldXAngle) > 4) {
//  Serial.print("angleX : ");
//  Serial.print(xAngle);
//  Serial.print("\tangleY : ");
//  Serial.println(yAngle);
//    SetSpeedX();
//  }
//  if (abs(yAngle - oldYAngle) > 4) {
//  Serial.print("angleX : ");
//  Serial.print(xAngle);
//  Serial.print("\tangleY : ");
//  Serial.println(yAngle);
//    SetSpeedY();
//  }

  int xAcc = mpu6050.getAccX()*100;
  int yAcc = mpu6050.getAccY()*100;
  int zAcc = mpu6050.getAccZ()*100;
  Serial.print("accX : ");Serial.print(xAcc);
  Serial.print("\taccY : ");Serial.print(yAcc);
  Serial.print("\taccZ : ");Serial.println(zAcc);

  if (abs(xAcc) > 5) {
    SetSpeedX();
  }
  if (abs(yAcc) > 5) {
    SetSpeedY();
  }

//  if ((abs(zAcc) > 140) || (abs(zAcc) < 50)) {
//    game = -1;
//    return;
//  }
  
  step=step+1;
  if (step>20) {
    //1 sec is over
    step=0;
    ShakesPerSecond=shakes;
    shakes=0;
  }

  if (Mode==0) {
    MoveDots();
    if (millis()-timestamp>2000) {
      //there is no shaking since 2 sec
      Mode=1;
      DiceValue=random(6)+1;
      ShowDicePic(DiceValue);
    }
  }

  if (ShakesPerSecond>5) {
    Mode=0; //shaking again
  }

}
