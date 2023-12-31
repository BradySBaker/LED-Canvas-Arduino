#include "BlueFunctions.h"

bool displayFrame(String dir) {
  // Open the colors file on the SD card
  File file = SD.open(dir);
  if (!file) {
    Serial.println("File open failed!");
      bluetooth.print("failed");
    return false;
  }
  
  // Loop through all the pixels on the LED panel
  for (int i = 0; i < 256; i++) {
    // Read the color from the file as a 32-bit integer
    uint32_t color;
    file.read((uint8_t*)&color, sizeof(color));
    // Extract the red, green, and blue components from the color
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;
    // Set the pixel color on the LED panel
    leds[i].r = r;
    leds[i].g = g;
    leds[i].b = b;
  }
  
  // Close the colors file
  file.close();
  // Update the LED panel with the new colors
  FastLED.show();
  return true;
}

bool isValidInput(const String& input) {
    for (size_t i = 0; i < input.length(); i++) {
        if (!isdigit(input.charAt(i))) {
            return false; // Found a non-digit character, return false
        }
    }
    return true; // All characters are digits
}

void handleDraw(char charBuffer[20]) {
  // Serial.println(charBuffer);
  String positions[3] = {};
  int idx = 0;
  char* charPos = strtok(charBuffer, "P");
  while (charPos != NULL && idx != 3) {
    positions[idx] = charPos;
    charPos = strtok(NULL, "P");
    idx++;
  }
  for (int i = 0; i < idx; i++) {
    int commaPos = positions[i].indexOf(",");
    String x = positions[i].substring(0, commaPos);
    String y = positions[i].substring(commaPos + 1);

    if (!isValidInput(x) || !isValidInput(y)) {
        Serial.println("Invalid input value");
        bluetooth.print("INVALID");
        return;
    }
    int curPos = pgm_read_word(&LED_MAP[x.toInt()][y.toInt()]);
    leds[curPos].r = Colors.r;
    leds[curPos].g = Colors.g;
    leds[curPos].b = Colors.b;
  }
  FastLED.show();
  bluetooth.print("successful");
}


void handleAnimPlay() {
  String dirName = "/anims/" + playingAnim + "/";

  int result = 0;
  File dir = SD.open(dirName);
  while (true) {
    File file = dir.openNextFile();
    if (!file) {
      break; // No more files
    }
    result++;
    if (!displayFrame(dirName + String(result) + ".TXT")) {
      return;
    };
    file.close();
    delay(animSpeed);
  }
  dir.close();
}


void handleColorChange(char charBuffer[20], bool rainAudio) {
  char hexColorString[10] = "";
  if (rainAudio) {
      strcpy(hexColorString, &charBuffer[2]); //2 = cm
      paletteColors[curColorLength] = hexToRGB(hexColorString);
      curColorLength++;
      bluetooth.print("CM");
  } else {
      strcpy(hexColorString, &charBuffer[1]); //1 = c
      Colors = hexToRGB(hexColorString);
      Serial.println(hexColorString);
      bluetooth.print("success");
  }
}