const int buttonPin = 2;
const int buzzerPin = 5;
const int red = 11;
const int green = 10;
const int blue = 9;
const int maxDifficulty = 5;
int scores[3][1];
boolean pointGiven = false;
int buttonState = 0;
int lastTime = 0;
int interval = 0;
int color = 1;
float difficulty = 1.0;

void setup() {
  interval = getInterval();
  color = random(0, 3);
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
}

void loop() {
  // TODO long-press to restart
  // TODO add display
  // TODO countdown to begin
  // TODO stop at 10?
  // TODO fade colors when not being played
  int currentTime = millis();
  buttonState = digitalRead(buttonPin);

  if (currentTime > lastTime + interval) {
    getNewColor();
  }

  if (buttonState == 1 && !pointGiven) {
    if (difficulty < maxDifficulty) {
      difficulty *= 1.05;
    }

    pointGiven = true;
    analogWrite(red, 0);
    analogWrite(green, 0);
    analogWrite(blue, 0);
    scores[color][0]++;
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.print("red ");
    Serial.println(scores[0][0]);
    Serial.print("green ");
    Serial.println(scores[1][0]);
    Serial.print("blue ");
    Serial.println(scores[2][0]);
  }
}

void getNewColor() {
  color = random(0, 3);
  pointGiven = false;
  interval = getInterval();
  lastTime = millis();

  if (color == 0) {
    analogWrite(red, 255);
    analogWrite(green, 0);
    analogWrite(blue, 0);
  } else if (color == 1) {
    analogWrite(red, 0);
    analogWrite(green, 255);
    analogWrite(blue, 0);
  } else {
    analogWrite(red, 0);
    analogWrite(green, 0);
    analogWrite(blue, 255);
  }
}

int getInterval() {
  int interval = random(1000, 3000) / difficulty;
  return interval;
}

