#include "pitches.h"
#define NOTE_C4_1 260

// notes in the melody:
int melody[] = {
   NOTE_C4_1,NOTE_C4, NOTE_D4, NOTE_C4,NOTE_F4,NOTE_E4,
   NOTE_C4_1,NOTE_C4,NOTE_D4,NOTE_C4,NOTE_G4,NOTE_F4,
   NOTE_C4_1,NOTE_C4,NOTE_C5,NOTE_A4,NOTE_F4,NOTE_F4, NOTE_E4,NOTE_D4,
   NOTE_AS4,NOTE_AS4,NOTE_A4,NOTE_F4,NOTE_G4,NOTE_F4
   };
   
   
int noteDurations[] = {
  4, 4, 2, 2,2,1,
  4, 4, 2, 2,2,1,
  4, 4, 2, 2,4,4,2,1, 
  4, 4, 2, 2,2,1 
  };
  
int length = 56; // the number of notes
char notes[] = "ggaeggCDEEEDDCC gEEFEED EDDC EDCEGAGGEDCagE EEEFEED EDDC"; // a space represents a rest
int beats[] = { 2, 1, 2, 2, 3, 2, 1, 2, 1, 1, 4, 1, 2, 1, 2, 2, 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 2, 2, 8, 1, 2, 5, 1, 2, 5, 1, 1, 1, 1, 4, 1, 2, 5, 5, 1, 1, 1, 2, 1, 1, 4, 1, 1, 1, 2, 2};
int tempo = 200;


/*Functions for the songs
 * 
 * 
 * 
 * 
 */
void hbd(){
  for (int thisNote = 0; thisNote < 26; thisNote++) {
    
     if (digitalRead(PUSH1)==LOW){
    break;
   }
    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote],noteDuration);

    int pauseBetweenNotes = noteDuration + 50;      //delay between pulse
    delay(pauseBetweenNotes);
    
    noTone(buzzerPin);                // stop the tone playing
  }
}

void playTone(int tone, int duration) {
 for (long i = 0; i < duration * 1000L; i += tone * 2) {
   digitalWrite(buzzerPin, HIGH);
   delayMicroseconds(tone);
   digitalWrite(buzzerPin, LOW);
   delayMicroseconds(tone);
 }
}

void playNote(char note, int duration) {
 char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C', 'D', 'E', 'F', 'G', 'A', 'B' };
 int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956, 851, 760, 716, 628, 568, 507 };
 
 // play the tone corresponding to the note name
 for (int i = 0; i < 14; i++) {
   if (names[i] == note) {
     playTone(tones[i], duration);
   }
 }
}

void lib(){

 for (int i = 0; i < length; i++) {
   
   if (notes[i] == ' ') {
     delay(beats[i] * tempo); // rest
   } else {
     playNote(notes[i], beats[i] * tempo);
   }
   
   // pause between notes
   delay(tempo / 2); 
 }

}
