const int sonarTrigPin_OUT = 7;
const int sonarEchoPin_IN = 8;

const int doorClosePin_OUT = 13;
const int doorOpenPin_IN = 2;

const int carPresentPin_LED = 3;
const int doorClosedPin_LED = 4;
const int timerRunningPin_LED = 5;

const int carAwayThresCM = 150;
const int userInputThresCM = 5;

const unsigned long timerBlinkInterval = 500;
const unsigned long doorCloseDelay = 20000; // 20 sec

bool timerIsRunning = false;
unsigned long timerStartTime = 0;

bool doorIsOpen = false;
bool carIsPresent = false;

#define LED_ON_VAL 240
#define LED_OFF_VAL 0

void setup() {
  Serial.begin(9600);

  pinMode(sonarTrigPin_OUT, OUTPUT);
  pinMode(sonarEchoPin_IN, INPUT);

  pinMode(doorOpenPin_IN, INPUT);
  pinMode(doorClosePin_OUT, OUTPUT);

  pinMode(carPresentPin_LED, OUTPUT);
  pinMode(doorClosedPin_LED, OUTPUT);
  pinMode(timerRunningPin_LED, 5);
}

void loop() {
  long carPingDuration, carCM;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(sonarTrigPin_OUT, LOW);
  delayMicroseconds(2);
  digitalWrite(sonarTrigPin_OUT, HIGH);
  delayMicroseconds(10);
  digitalWrite(sonarTrigPin_OUT, LOW);

  carPingDuration = pulseIn(sonarEchoPin_IN, HIGH);

  // convert the time into a distance
  carCM = microsecondsToCentimeters(carPingDuration);

  Serial.print(carCM);
  Serial.print("cm");
  Serial.println();

  if (digitalRead(doorOpenPin_IN)) {
    digitalWrite(doorClosedPin_LED, LOW);
    doorIsOpen = true;
  } else {
    digitalWrite(doorClosedPin_LED, HIGH);
    doorIsOpen = false;
  }

  if (carCM < userInputThresCM) {
    delayedCloseDoor();
  }

  if (carCM < carAwayThresCM) {
    digitalWrite(carPresentPin_LED, HIGH);
    carIsPresent = true;
  } else {
    digitalWrite(carPresentPin_LED, LOW);
    
    carIsPresent = false;
  }

  if (doorIsOpen && !carIsPresent && !timerIsRunning) {
    timerIsRunning = true;
    timerStartTime = millis();
  } else if ((!doorIsOpen || carIsPresent) && timerIsRunning) {
    timerIsRunning = false;
    digitalWrite(timerRunningPin_LED, LOW);
  }

  if (timerIsRunning) {
    unsigned long timerRunningTime = millis() - timerStartTime;
    if ((timerRunningTime / timerBlinkInterval) % 2 == 0) {
      digitalWrite(timerRunningPin_LED, HIGH);
    } else {
      digitalWrite(timerRunningPin_LED, LOW) ; 
    }

    if (timerRunningTime > doorCloseDelay) {
      closeDoor();
    }
  }

  delay(100);
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}

void closeDoor() {
  digitalWrite(doorClosePin_OUT, HIGH);
  delay(500); // 0.5 sec
  digitalWrite(doorClosePin_OUT, LOW);
  delayMicroseconds(2000); // 2 sec (Gives the door carriage time to move away from the limit switch)
}

void delayedCloseDoor() {
  digitalWrite(carPresentPin_LED, HIGH);
  digitalWrite(doorClosedPin_LED, HIGH);
  digitalWrite(timerRunningPin_LED, HIGH);
  delay(2000);
  digitalWrite(carPresentPin_LED, LOW);
  digitalWrite(doorClosedPin_LED, LOW);
  digitalWrite(timerRunningPin_LED, LOW);
  
  for(int i = 0; i < 10; i++) {
    delay(500);
    digitalWrite(timerRunningPin_LED, HIGH);
    delay(500);
    digitalWrite(timerRunningPin_LED, LOW);
  }

  closeDoor();
}

