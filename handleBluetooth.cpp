#include "BlueFunctions.h"

void handleBluetooth() {
  char charBuffer[20];

  if (bluetooth.available() > 0) { 
    int numberOfBytesReceived = 0;
    // if (!game) {
    numberOfBytesReceived = bluetooth.readBytesUntil('\r', charBuffer, 19);
    // } else {
    //   numberOfBytesReceived = bluetooth.readBytesUntil('\r', charBuffer, 2);
    // }
    charBuffer[numberOfBytesReceived] = NULL;
    Serial.println(charBuffer);
   if(strstr(charBuffer, "OFF") == &charBuffer[0]) { //Handle off signal
      FastLED.clear();
      FastLED.show();
      animSpeed = 5;
      playingAnim = "~";
      bluetooth.print("OFF");
    } else if (strstr(charBuffer, "B") == &charBuffer[0]) { // Handle brightness
      char brightness[3] = "";
      strcpy(brightness, &charBuffer[1]);
      FastLED.setBrightness(atoi(brightness));
      FastLED.show();
      bluetooth.println("success");
    } else if (strstr(charBuffer, "R") == &charBuffer[0]) { //Handle rain signal
      raining = true;
      char numString[2] = "";
      strcpy(numString, &charBuffer[1]);
      raindropAmount = atoi(numString);
      if (charBuffer[3] != '\0') {
        char frameName[7] = "";
        strcpy(frameName, &charBuffer[3]);
        background = frameName;
      }
      bluetooth.print("RAIN");
      raindrop();
    } else if (strstr(charBuffer, "SM") == &charBuffer[0]) { //Handle stop rain/audio visualizer
      Serial.println("Stopped mode");
      bluetooth.print("SM");
      curColorLength = 0;
      raining = false;
      avActive = false;
    } else if (strstr(charBuffer, "CMP")) { //Handle color palette
      getPaletteAndSet(charBuffer);
      bluetooth.print("success");
    } else if(strstr(charBuffer, "CM") == &charBuffer[0]) { //Handle rain/audio visualizer color added
      handleColorChange(charBuffer, true);
    } else if (strstr(charBuffer, "C") == &charBuffer[0]) { //Handle color change
      handleColorChange(charBuffer, false);
    } else if (strstr(charBuffer, "P") == &charBuffer[0]) { //Handle pixel drawing
      handleDraw(charBuffer);
    } else if (strstr(charBuffer, "HAV") == &charBuffer[0]) { //Handle audio visualizer
      avActive = true;
      bluetooth.print("HAV");
      if (charBuffer[3] == '1') {
        audioVisualizer(true);
      } else {
        audioVisualizer(false);
      }
    } else if (strstr(charBuffer, "S") == &charBuffer[0]) { //Handle frame save
      handleFrameSave(charBuffer, false);
    } else if (strstr(charBuffer, "F") == &charBuffer[0]) { //Handle display frame
      char nameString[7] = "";
      strcpy(nameString, &charBuffer[1]);
      if (raining) {
        background = nameString;
        bluetooth.print("FRAME");
      } else {
        displayFrame("/drawings/" + String(nameString) + ".TXT");
      }
    } else if (strstr(charBuffer, "names") == &charBuffer[0]) { //Handle retrieve frame ammount
      Colors = COLORS(200, 200, 200);
      sendFileNames("/drawings", false);
      sendFileNames("/ANIMS", true);
      bluetooth.print("~");
    } else if (strstr(charBuffer, "D") == &charBuffer[0]) { //Handle frame delete
      handleFrameDelete(charBuffer);
    } else if (strstr(charBuffer, "A") == &charBuffer[0]) { //Handle frame save
      handleFrameSave(charBuffer, true);
    } else if (strstr(charBuffer, "I") == &charBuffer[0]) { //Handle animation play
     char nameString[7] = "";
     char animSpeedString[1] = "";
     strcpy(animSpeedString, &charBuffer[1]);
     strcpy(nameString, &charBuffer[2]);
     animSpeed = pow(5-atoi(animSpeedString), 5);
     playingAnim = String(nameString);
     bluetooth.print("success");
    } else if (strstr(charBuffer, "Z") == &charBuffer[0]) { //Handle animation delete
     handleAnimDelete(charBuffer);
    }
  }
}