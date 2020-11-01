/*
 * 
 *  Arduino Morse Decoder
 *  (c) Craig McIntyre 2019
 *  Licensed under GPL 3.0
 *  
 *  NOTE for sound, drive with a separate oscillator (e.g. 555 and a speaker). 
 *  Alternatively, use a buzzer, though this may offend cohabitants
 *  
*/

#include <Keyboard.h>

unsigned long signal_valid,timer1,timer2,timer3,timer4;   //timing variables
int inputPin = 3;                      //input pin for push button
int morsePin = 16;                      //output for morse LED or buzzer5AI
String code = "";                       //string to hold letters
int space = true;                       //boolean to prevent multiple spaces
int first_space = true;                 //boolean to prevent leading space (useful if you want to use an LCD)

void setup() {
  Keyboard.begin();
  Serial.begin(9600);
  pinMode(inputPin, INPUT_PULLUP);      //to help prevent bouncing
  pinMode(morsePin, OUTPUT);
}

void loop() {

NextDotDash:

  timer3 = millis();                    //space timer checks time from last press
  while (digitalRead(inputPin) == HIGH) {}
    space = false;                      //reset to allow spaces to occur
    timer1 = millis();                  //letter timer for letter spacing
    digitalWrite(morsePin, HIGH);       //turn on the led :)
//    digitalWrite(buzzPin, HIGH);
  
  while (digitalRead(inputPin) == LOW) {}
    timer2 = millis();                            
    digitalWrite(morsePin, LOW);        //LED off on button release
//    digitalWrite(buzzPin, LOW);
    signal_valid = timer2 - timer1;       //time for which button is pressed
  
  if (signal_valid > 50) {                //to account for switch debouncing
    code += morsify();                   //function to read dot or dash
  }
  
  while ((millis() - timer2) < 500) {   //move to next letter after 0.5sec 
    timer4 = millis();
    if ((timer4 - timer3) > 2000) {     // add a space at 2secs
      if (space == false && first_space == false) {
      Keyboard.print(" ");
      space = true;
      }
    }
    if (digitalRead(inputPin) == LOW) {
      goto NextDotDash;
    }
  }
  decoder();                          //function to decipher code into alphabet
}

char morsify() {
  if (signal_valid < 250 && signal_valid > 50)
  {
    return '0';                        //dot if signal < than 0.25sec
  }
  else if (signal_valid > 250)
  {
    return '1';                        //dash if signal > 0.25 sec
  }
}

void decoder() {
  
  static String letters[] = {"01", "1000", "1010", "100", "0", "0010", "110",
                             "0000", "00", "0111", "101", "0100", "11", "10",
                             "111", "0110", "1101","010", "000", "1", "001", 
                             "0001", "011", "1001", "1011", "1100", "E"};

  static String numbers[] = {"11111", "01111", "00111", "00011", "00001",
                             "00000", "10000", "11000", "11100", "11110", "E"};
                             
  int i = 0;                          //counter for letters loop
  int j = 0;                          //counter for numbers loop

  if (code == "010101") {             //manual checks for punctuation           
    Keyboard.print(".");
  }
  else if (code == "110011") {
    Keyboard.print(",");
  }
  else if (code == "001100") {
    Keyboard.print("?");
  }
  else if (code == "10010") {
    Keyboard.print("/");
  }
  else if (code == "011010") {
    Keyboard.print("@");
  }
  else {
    while (letters[i] != "E") {       //loops letter array for match
      if (letters[i] == code) {     
        Keyboard.print(char('A' + i));     //adds index of the array to 'A' to give letter
        break;
      }
      i++;
    }
    while (numbers[j] != "E") {       //loops number array for match
      if (numbers[j] == code) {
        Keyboard.print(0 + j);             //adds index of the array to 0 to give number
        break;
      }
      j++;
    }
    if (letters[i] == "E" && numbers[j] == "E") { //hash if character not found
      Keyboard.print("#");
    }
  }
  code = "";                          //empty code variable for next loop
  first_space = false;                //spaces can happen after first loop
}
