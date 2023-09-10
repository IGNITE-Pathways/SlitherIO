
void startGame2() {
  for (int i = 0; i < 4; i++) {
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

//Reset Game 2 to start from beginning 
void resetGame2() {
  for (int i = 0; i<MAX_LEVEL; i++) {
    tail[i][0] = -1;
    tail[i][1] = matrix.Color333(0, 7, 0);
    food[i] = -1;
  }
  tail[0][0] = 131; //Tail is always one bigger
  currentX = 8;
  currentY = 16;
  startGame2();
  timerStart = millis();
}

void playGame2(int currentPos) {
  if (currentPos != tail[0][0]) { 
      if(isTouchingTheTail(currentPos) || timeRanout) {
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
        }
        displayScore(foodFound);
        delay(10000);
        game = -1; //go back to select game
      }
    
      if(isFood(currentPos) == true) {
        foodFound = foodFound + 1;
        
        int foodToAdd = random(1,3);
        for (int i = 0; i < foodToAdd; i++) {
          // Add new Random Food
          int tmp = random(511);
          //Make sure there are no two food items on same cell
          while (isFood(tmp)) {
             tmp = random(511);
          }
          addFood(tmp);
        }
        eatFood(currentPos);
      }

      if (foodFound >= MAX_LEVEL) {
          displayWin();
          delay(3000);
          game = -1;
          return;
      }
 
      int clearPos = addToTail(currentPos, matrix.Color333(7, 0, 0)); 
      int row = clearPos % 16; //y axis (0 to 7)
      int col = clearPos / 16; //x axis (0 to 31)
      matrix.drawPixel(col, row, matrix.Color333(0, 0, 0));
  }
  int row = currentPos % 16; //y axis (0 to 7)
  int col = currentPos / 16; //x axis (0 to 31)
  blinkLED(col, row, matrix.Color333(0, 0, 7));  
  displayFood(false); //fix for bug when food disappear sometimes!
}
