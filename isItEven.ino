/**
 * Is It Odd
 * 
 * Simple game that asks the user to decide if a displayed 4-bit binary number
 * indicated by the lights is even. Yeah...not too difficult if you know how to read binary numbers. 
 * 
 * Simple experiment in wiring up LEDs, Piezo element, and buttons after working through the 
 * ARDX experimentation kit. 
  */



/**
 * LED Pins showing the current number
 */
const int firstLedPin =  9;      // the number of the LED pin
const int secondLedPin =  10;      // the number of the LED pin
const int thirdLedPin =  11;      // the number of the LED pin
const int fourthLedPin =  12;      // the number of the LED pin

/**
 * Button Pins
 */
const int button1 = 2; // The NO pin
const int button2 = 4; // The YES pin

/**
 * Sound 
 */
const int soundPin = 13;
const int tones[] = { 1915, 956 };

/**
 * Game Variables
 */
// Current Answer
int randomNumber = 99;
boolean isEven = -1;
boolean buttonPressed = false;
boolean buttonHasBeenReleased = false;
boolean answer = -1;

void setup() {

  // Serial for debugging
  Serial.begin(9600);

  // Initialize LEDs
  pinMode(firstLedPin, OUTPUT);
  pinMode(secondLedPin, OUTPUT);
  pinMode(thirdLedPin, OUTPUT);
  pinMode(fourthLedPin, OUTPUT);    

  // Initialize Buttons
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);  

  // Sound
  pinMode(soundPin, OUTPUT);

  // Setup first answer
  displayNewNumber();
}

void loop() {
  // read the state of the pushbutton value:
  int button1State = digitalRead(button1);  // NO
  int button2State = digitalRead(button2);  // YES

  if (buttonPressed && (button1State == HIGH && button2State == HIGH)) {
     buttonPressed = false;
     buttonHasBeenReleased = true;
  }

  // Answer has been submitted
  if (button1State == LOW || button2State == LOW) {
    buttonPressed = true;
    
    // Now check game logic
    if (button1State == LOW) {  // Button1 == NO
      answer = false;
    } else if (button2State == LOW) { // Button2 == YES
      answer = true;
    } 
  }

  // Check answer once it's been released
  if (buttonHasBeenReleased) {
    buttonHasBeenReleased = false;

    if (isEven) {
      if (answer) {
        playCorrectSound();
      } else {
        playErrorSound();
      }
    } else {
      if (answer) {
        playErrorSound();
      } else {
        playCorrectSound();
      }
    }

    delay(1000); 
    displayNewNumber();
  }
  
}

void displayNewNumber() {

  digitalWrite(firstLedPin, LOW); 
  digitalWrite(secondLedPin, LOW);   
  digitalWrite(thirdLedPin, LOW);  
  digitalWrite(fourthLedPin, LOW);   

  int newRandom = random(1, 15);
  if (newRandom == randomNumber) {
    displayNewNumber();
    return;
  } else {
    randomNumber = newRandom;
  }
  
  if (randomNumber % 2 == 0) {
    isEven = true;
  } else {
    isEven = false;
  }

  Serial.println(randomNumber);  

  int bit1 = randomNumber & B00000001;
  int bit2 = randomNumber & B00000010;
  int bit3 = randomNumber & B00000100;  
  int bit4 = randomNumber & B00001000;

  if (bit1 == 1) {
    digitalWrite(firstLedPin, HIGH); 
  }
  if (bit2 == 2) {
    digitalWrite(secondLedPin, HIGH); 
  }
  if (bit3 == 4) {
    digitalWrite(thirdLedPin, HIGH); 
  }
  if (bit4 == 8) {
    digitalWrite(fourthLedPin, HIGH); 
  }
}

/**
 * Sounds
 */
void playCorrectSound() {
  playSound(tones[1]); 
}

void playErrorSound() {
  playSound(tones[0]);
}

void playSound(int tone) {
  for (long i = 0; i < 100000L; i += tone * 2) {
    digitalWrite(soundPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(soundPin, LOW);
    delayMicroseconds(tone);
  }
}

