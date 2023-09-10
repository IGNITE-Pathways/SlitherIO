
void resetDrawScreen() {
  clearScreen();
  for (int i = 0; i<MAX_LEVEL; i++) {
    tail[i][0] = -1;
    tail[i][1] = matrix.Color333(0, 7, 0);
    food[i] = -1;
  }
  currentX = 8;
  currentY = 16;
  currentSnakeSize = 0;
  color1 = matrix.Color333(0, 7, 0);
  color2 = matrix.Color333(7, 0, 0);
  color3 = matrix.Color333(0, 0, 7); 
  color4 = matrix.Color333(0, 0, 0); //Eraser
  selectedColor = color1;
  timerStart = millis();
}

void draw(int currentPos) {
    if (timeRanout) {
      delay(200);
      matrix.drawLine(0, 15, 32, 15, matrix.Color333(0, 0, 0));
      delay(200);
      matrix.drawLine(0, 15, 32, 15, matrix.Color333(7, 0, 0));
    }
  
    if (key1bouce) {
      delay(500); key1bouce = false;        
    }
    if (key2bouce) {
      delay(500); key2bouce = false;        
    }
      if (key3bouce) {
      delay(500); key3bouce = false;        
    }
      if (key4bouce) {
      delay(500); key4bouce = false;        
    }
  
   if (currentPos != tail[0][0]) {  
      if (!isTouchingTheTail(currentPos)) {
        if (selectedColor != matrix.Color333(0, 0, 0)) {
          currentSnakeSize = currentSnakeSize + 1;
          addToTail(currentPos, selectedColor);
        }
      } else {
        //touching 
        if (selectedColor == matrix.Color333(0, 0, 0)) {
          //delete 
          deleteFromTail(currentPos);
        }
      }
      //set the currentPos     
      if (currentSnakeSize > MAX_LEVEL) {   
        delay(3000);
        resetDrawScreen();
        game = -1;
      }
      delay(40);
    }
    int row = currentPos % 16; //y axis (0 to 7)
    int col = currentPos / 16; //x axis (0 to 31)    
    blinkLED(col, row, selectedColor);
}
