#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

// pins
const int resetPin = 3;
const int buttonPin = 2;
const int redPin = 7;
const int greenPin = 6;
const int bluePin = 5;

const int maxDifficulty = 5;
const int winningScore = 20;

int scores[2] = {0, 0};
int pointGiven = 0;
int hasStarted = 0;
int buttonState = 0;
int interval = 0;
int color = 1;

long startTime = 0;
long currentTime = 0;
long lastTime = 0;

float difficulty = 1.0;

// Color arrays
int black[3]  = { 0, 0, 0 };
int white[3]  = { 100, 100, 100 };
int red[3]    = { 100, 0, 0 };
int green[3]  = { 0, 100, 0 };
int blue[3]   = { 0, 0, 100 };
int yellow[3] = { 40, 95, 0 };
int dimWhite[3] = { 30, 30, 30 };
// etc.

// Set initial color
int redVal = black[0];
int grnVal = black[1];
int bluVal = black[2];

int fadeDelay = 10;      // 10ms internal crossFade delay; increase for slower fades
int hold = 0;       // Optional hold when a color is complete, before the next crossFade
int DEBUG = 0;      // DEBUG counter; if set to 1, will write values back via serial
int loopCount = 60; // How often should DEBUG report?
int j = 0;          // Loop counter for repeat

// Initialize color variables
int prevR = redVal;
int prevG = grnVal;
int prevB = bluVal;


void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  lcd.begin(16, 2);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  // start the game
  if (buttonState == 1 && !hasStarted) {
    lcd.clear();
    // init new game variables
    hasStarted = 1;
    interval = getInterval();
    color = random(0, 2);
    startTime = millis();
    lastTime = 0;

    // turn off led
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
  }

  // waiting for game to start, fade colours
  if (!hasStarted) {
    lcd.setCursor(0, 0);
    lcd.print("Push button");
    lcd.setCursor(0, 1);
    lcd.print("to begin");
    crossFade(red);
    crossFade(green);
    crossFade(blue);
    crossFade(yellow);
    return;
  }

  // if we get this far, it's game on!
  currentTime = millis();

  if (currentTime - startTime < 1000) {
    lcd.setCursor(0, 0);
    lcd.print('3');
    return;
  } else if (currentTime - startTime < 2000) {
    lcd.setCursor(0, 0);
    lcd.print('2');
    return;
  } else if (currentTime - startTime < 3000) {
    lcd.setCursor(0, 0);
    lcd.print('1');
    return;
  } else if (currentTime - startTime < 3500) {
    lcd.setCursor(0, 0);
    lcd.print("GO!");
    return;
  }

  if (currentTime > lastTime + interval) {
    getNewColor();
  }

  // button pressed, give point
  if (buttonState == 1 && !pointGiven) {

    // increase difficulty if needed
    if (difficulty < maxDifficulty) {
      difficulty *= 1.05;
    }

    // turn led off
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);

    scores[color]++;
    setScores();
    pointGiven = 1;
  }
}

void getNewColor() {
  pointGiven = 0;
  color = random(0, 2);
  interval = getInterval();
  lastTime = millis();

  if (color == 0) {
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
  } else if (color == 1) {
    analogWrite(redPin, 0);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0);
  } else {
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 255);
  }
}

void setScores() {
  if (scores[0] >= winningScore || scores[1] >= winningScore) {
    reset();
    return;
  }
  lcd.clear();
  lcd.print("Red        Green");
  lcd.setCursor(0, 1);
  lcd.print(scores[0]);
  lcd.print("          ");
  lcd.print(scores[1]);
  lcd.print("      ");
}

int getInterval() {
  int interval = random(1000, 3000) / difficulty;
  return interval;
}

void reset() {
  lcd.clear();
  if (scores[0] > scores[1]) {
    lcd.print("Red wins!");
  } else {
    lcd.print("Green wins!");
  }
  scores[0] = 0;
  scores[1] = 0;
  hasStarted = 0;
  delay(5000);
  lcd.clear();
}


// ---- FADING STUFF ---- //

int calculateStep(int prevValue, int endValue) {
  int step = endValue - prevValue; // What's the overall gap?
  if (step) {                      // If its non-zero,
    step = 1020 / step;            //   divide by 1020
  }
  return step;
}

int calculateVal(int step, int val, int i) {

  if ((step) && i % step == 0) { // If step is non-zero and its time to change a value,
    if (step > 0) {              //   increment the value if step is positive...
      val += 1;
    }
    else if (step < 0) {         //   ...or decrement it if step is negative
      val -= 1;
    }
  }
  // Defensive driving: make sure val stays in the range 0-255
  if (val > 255) {
    val = 255;
  }
  else if (val < 0) {
    val = 0;
  }
  return val;
}


void crossFade(int color[3]) {
  // Convert to 0-255
  int R = (color[0] * 255) / 100;
  int G = (color[1] * 255) / 100;
  int B = (color[2] * 255) / 100;

  int stepR = calculateStep(prevR, R);
  int stepG = calculateStep(prevG, G);
  int stepB = calculateStep(prevB, B);

  for (int i = 0; i <= 1020; i++) {
    if (digitalRead(buttonPin)) {
      return;
    }
    redVal = calculateVal(stepR, redVal, i);
    grnVal = calculateVal(stepG, grnVal, i);
    bluVal = calculateVal(stepB, bluVal, i);

    analogWrite(redPin, redVal);   // Write current values to LED pins
    analogWrite(greenPin, grnVal);
    analogWrite(bluePin, bluVal);

    delay(fadeDelay); // Pause for 'wait' milliseconds before resuming the loop
  }
  // Update current values for next loop
  prevR = redVal;
  prevG = grnVal;
  prevB = bluVal;
  //delay(fadeDelay); // Pause for optional 'wait' milliseconds before resuming the loop
}

