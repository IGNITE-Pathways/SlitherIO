const int MAX_RETRIES = 3;
int retryCount = 0;

void gameLevelUp() {
  gameLevel += 1;
  clearScreen();
  if (gameLevel >= 500) {
    displayWin();
    delay(10000);
    resetGame1();
    game = -1; //go back to select game
    return;
  } else {
    displayBanner(); //LEV-
    delay(2000);
  }
  mpu6050.update();
  for (int i = 0; i < gameLevel; i++) {
    int tmp = random(511);
    //Make sure there are no two food items on same cell
    while (isFood(tmp) || ((tmp % 16) == 15)) {
      tmp = random(511);
    }
    food[i] = tmp;
  }
  displayFood(true);
  foodFound = 0;
}

//Reset Game to start from LEV-1
void resetGame1() {
  for (int i = 0; i<MAX_LEVEL; i++) {
    tail[i][0] = -1;
    tail[i][1] = matrix.Color333(0, 7, 0);
    food[i] = -1;
  }
  tail[0][0] = 131; //Tail is always one bigger
  gameLevel = 0;
  currentX = 8;
  currentY = 16;
  retryCount = 0;
  gameLevelUp();
  timerStart = millis();
}

void playGame1(int currentPos) {
   if (currentPos != tail[0][0]) { 
      if(isTouchingTheTail(currentPos) || timeRanout) {
        //int row = 15 - (currentPos % 16); //y axis (0 to 7)
        int row = currentPos % 16; //y axis (0 to 7)
        int col = currentPos / 16; //x axis (0 to 31)
        //blink
        matrix.drawPixel(col, row, matrix.Color333(0, 0, 0));
        delay(500);
        matrix.drawPixel(col, row, matrix.Color333(7, 7, 7));
        delay(500);
        matrix.drawPixel(col, row, matrix.Color333(0, 0, 0));
        delay(500);
        matrix.drawPixel(col, row, matrix.Color333(7, 7, 7));
        if (timeRanout) {
          displayTimeOut();
          delay(3000);
          displayScore(gameLevel);
          delay(10000);
          game = -1; //go back to select game
        } else {
          retryCount++;
          if (retryCount > MAX_RETRIES) {       
            //restart game
            displayLost();
            delay(3000);
            //resetGame1();
            game = -1; //go back to select game
            return;
          } else {
            displayRetry(retryCount);
            delay(3000);
            gameLevel--;
            gameLevelUp();        
            return;
          }
       }
      }
    
      if(isFood(currentPos) == true) {
        foodFound = foodFound + 1;
        eatFood(currentPos);
      }

      if(foodFound >= gameLevel) {
        retryCount = 0; 
        gameLevelUp();
        return;
      }
    
      int clearPos = addToTail(currentPos, matrix.Color333(0, 7, 0));
      int row = clearPos % 16; //y axis (0 to 15)
      int col = clearPos / 16; //x axis (0 to 31)
      matrix.drawPixel(col, row, matrix.Color333(0, 0, 0));
  }
  //set the currentPos
  int row = currentPos % 16; //y axis (0 to 7)
  int col = currentPos / 16; //x axis (0 to 31)
  Serial.print("Row: "); Serial.print(row);
  Serial.print(", Col: "); Serial.println(col);
  blinkLED(col, row, matrix.Color333(0, 7, 0));
  displayFood(false); //fix for bug when food disappear sometimes!
}
