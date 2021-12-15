#include <Servo.h>

Servo servo ;
unsigned long startTime[10];
unsigned long endTime[10];
unsigned long duration[10];
int i = 0;
int j = 0;
int k = 0;
bool shouldJump[10];                     // activates delay before jump
bool currentlyJumping = false;
unsigned long latestStart = 0;
int firstDetectorState = LOW;      // Prevents from detecting same obstacle multiple times
int secondDetectorState = LOW;     // Prevents from detecting same obstacle multiple times
unsigned long jumpTime = 0;


void setup() {
  Serial.begin(9600);
  servo.attach(7);
  pinMode(5, INPUT);
  servo.write(15);
  while (digitalRead(5) == LOW) {
  }
}


void loop() {
  detectStart();
  runFirstDetector();
  runSecondDetector();
  runJump();
}


void detectStart() {
  if (digitalRead(5) == HIGH) {     // resets game
    servo.write(0);                 // presses jump-button
    delay(1000);
    servo.write(15);                // releases jump-button
    delay(100);
    shouldJump[0] = false;
    i = 0;
    j = 0;
    k = 0;
  }
}


void runFirstDetector() {
  //Saves time A1 detects obstacle
  if (analogRead(A1) > 250 && firstDetectorState == LOW) {     //Only runs if not currently detecting obstacle
    startTime[i] = millis();
    i++;
    if (i == 10) {
      i = 0;
    }
    firstDetectorState = HIGH;       //HIGH = currently detecting obstacle
  }
  //
  if (analogRead(A1) < 250 && firstDetectorState == HIGH) {     //Detects end of obstacle
    firstDetectorState = LOW;      //LOW = currently not detecting obstacle
  }
}


void runSecondDetector() {
  //Saves time A0 detects obstacle and calculates a duration between detections
  if (analogRead(A0) > 200 && secondDetectorState == LOW) {    //Only runs if not currently detecting obstacle
    endTime[j] = millis();
    duration[j] = (endTime[j] - startTime[j]) * 1.2;
    shouldJump[j] = true;
    j++;
    if ( j == 10) {
      j = 0;
    }
    secondDetectorState = HIGH;
  }
  if (analogRead(A0) < 200 && secondDetectorState == HIGH) {
    secondDetectorState = LOW;
  }
}


void runJump() {
  //Runs when duration milliseconds has passed since detection at second detector and haven't jumped yet
  if (millis() - endTime[k] >= duration[k] && shouldJump[k]) {
    servo.write(0);                    // presses jump-button
    jumpTime = millis();               //Start timer to know when to stop jumping
    shouldJump[k] = false;
    currentlyJumping = true;
    k++;
    if ( k == 10) {
      k = 0;
    }
  }

  if (millis() - jumpTime >= 200 && currentlyJumping == true) {
    servo.write(15);                  // releases jump-button
    currentlyJumping = false;
  }
}
