void displayGameStart() { 
  matrix.setCursor(4, 4);  // start at top left, with one pixel of spacing
  matrix.setTextSize(1);   // size 1 == 8 pixels high
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.print('1');
  matrix.print('2');
  matrix.print('3');
  matrix.print('4');
  //matrix.print('5');
}

void clearScreen() {
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    //matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0));
}

void displayStart() {
  matrix.setCursor(5, 0);  // start at top left, with one pixel of spacing
  matrix.setTextSize(1);   // size 1 == 8 pixels high
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.print("GAME");
  matrix.setCursor(1, 9);  // next line
  matrix.print("START");
  delay(1000);
}

void displayRetry(uint8_t retrycount) {
  clearScreen();
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.setCursor(2, 1);  // start at top left, with one pixel of spacing
  matrix.print("RETRY");
  matrix.setCursor(13, 9);  // start at top left, with one pixel of spacing
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print(retrycount);
}

void displayLost() {
  clearScreen();
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.setCursor(1, 5);  // start at top left, with one pixel of spacing
  matrix.print("LOST");
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print(gameLevel);
}

void displayTimeOut() {
  clearScreen();
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.setCursor(5, 0);  // start at top left, with one pixel of spacing
  matrix.print("TIME");
  matrix.setCursor(8, 8);  // start at top left, with one pixel of spacing
  matrix.print("OUT");
}

void displayWin() {
  matrix.setCursor(1, 0);  // start at top left, with one pixel of spacing
  matrix.setTextColor(matrix.Color333(0,7,0));
  matrix.print("WIN");
  matrix.setTextColor(matrix.Color333(7,4,4));
  matrix.print("*"); 
}

void displayBanner() {
  matrix.setCursor(2, 5);  // start at top left, with one pixel of spacing
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.print("LEV-");
  matrix.print(gameLevel);    
}

void displayScore(int score) {
  clearScreen();
  matrix.setCursor(2, 0);  // start at top left, with one pixel of spacing
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.print("SCORE");
  matrix.setTextColor(matrix.Color333(4,4,4));
  if (score / 10 == 0) { // Score is 1-digit
    matrix.setCursor(14, 8);    
  } else if (score / 100 == 0) { //Score is 2-digit
    matrix.setCursor(11, 8);        
  } else { //Score is 3-digit
    matrix.setCursor(8, 8);        
  }
  matrix.print(score);
}

void displayNumber(uint8_t ledAddress, uint8_t count) {
    matrix.print(count);    
}
