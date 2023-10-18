#ifndef _SD_SETUP_H_
#define _SD_SETUP_H_

#include <SPI.h>
#include <SD.h>

struct Player{
  String name;
  String identifier;
  int points;
};

//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************
void SD_Init(void);
char* formatFileName(char* fileName);
char* trim(char* str);
void writeFile(File myFileW, String text);

void SD_Init(void){
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work

  if (!SD.begin(12)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
}
void readFile(char* textName, Player* recordPlayers, int& numPlayers) {
  // Re-open the file for reading:
  myFile = SD.open(formatFileName(textName));
  if (myFile) {
    Serial.println(textName);

    // Initialize index and keep track of the top 3 players
    int index = 0;
    Player topPlayers[3]; // Array to store the top 3 players
    int topPoints[3] = {0}; // Array to store the points of the top 3 players

    // Read from the file until there's nothing else in it:
    while (myFile.available()) {
      String line = myFile.readStringUntil('\n');
      line.trim();
      
      // Assuming the format is "P2: Charlie=12"
      int equalsIndex = line.indexOf('=');
      int colonIndex = line.indexOf(':');

      if (equalsIndex != -1 && colonIndex != -1) {
        Player player;
        player.identifier = line.substring(0, colonIndex) + " " + line.substring(colonIndex + 2, equalsIndex);
        player.name = line.substring(colonIndex + 2, equalsIndex);
        player.points = line.substring(equalsIndex + 1).toInt();

        // Check if the player has more points than the current top players
        for (int i = 0; i < 3; i++) {
          if (player.points > topPoints[i]) {
            // Shift the lower-ranked players down
            for (int j = 2; j > i; j--) {
              topPoints[j] = topPoints[j - 1];
              topPlayers[j] = topPlayers[j - 1];
            }
            topPoints[i] = player.points;
            topPlayers[i] = player;
            break;
          }
        }

        index++; // Increment index when a valid player is found
      }
    }

    // Copy the top players into the recordPlayers array
    numPlayers = min(3, index); // Ensure numPlayers doesn't exceed 3
    for (int i = 0; i < numPlayers; i++) {
      recordPlayers[i] = topPlayers[i];
    }

    // Close the file:
    myFile.close();
  } else {
    // If the file didn't open, print an error:
    Serial.println("Error opening file");
  }
}






void writeFile(File myFileW, String text){
  //myFileW = SD.open("/");
  //myFileW.close();
  myFileW = SD.open("scoreb~1.txt", FILE_WRITE);
  if (myFileW) {
    myFileW.println(text);
    myFileW.close();
    Serial.println("Printed.");
  } else {
    Serial.println("Error opening file for writing.");
  }
}


char* trim(char* str) {
    while (isspace(*str)) str++; // Remove leading spaces
    char* end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--; // Remove trailing spaces
    *(end + 1) = '\0'; // Null-terminate the trimmed string
    return str;
}

char* formatFileName(char* fileName) {
    // Convert to lowercase
    trim(fileName);
    for(int i = 0; fileName[i] != '\0'; i++) {
        fileName[i] = tolower(fileName[i]);
    }

    // Capitalize first letter
    fileName[0] = toupper(fileName[0]);

    return fileName;
}


void printDirectory(File dir, int numTabs) {
   dir.rewindDirectory();
   while(true) {
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
