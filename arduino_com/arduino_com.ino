#include <Servo.h>
#include <math.h>
#include <Adafruit_PWMServoDriver.h>

#define MAX_GAMMA 50
///VARIABLES SERVOMOTORES
#define MAX_SERVOS 12
#define MAX_PULSE 432
#define MIN_PULSE 188
Servo Servos[MAX_SERVOS];

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//VARIABLES PARA CONTROLAR EL TIEMPO
unsigned long previousMillis = 0;
const long interval = 20;
unsigned long loopTime;
unsigned long previousLooptime;
double t;

struct angles {
  double tetta;
  double alpha;
  double gamma;
};////vector
struct angles anglesFR;
struct angles anglesFL;
struct angles anglesBR;
struct angles anglesBL;
//VARIABLES PARA RECIVIR EL COMANDO
const byte numChars = 32;
char receivedChars[numChars];
int spaceCounter = 0;

boolean newData = false;

//int pulse0, pulse1, pulse2, pulse3, pulse4, pulse5, pulse6, pulse7, pulse8, pulse9, pulse10, pulse11;

char a;
int pulse[12];
int batterieVoltage;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pwm.begin();
  pwm.setPWMFreq(50);
  setPulse();
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  //IMUSetup();
  //connectServos(); //서보모터 I2C통신으로 연결 방법 다름


}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    t = float(currentMillis) / 1000;
    /////////cuenta el tiempo que tarda el bucle en ejecutarse
    loopTime = currentMillis - previousLooptime;
    //Serial.print("<"); Serial.print(loopTime); Serial.print("#0#0#0"); Serial.print(">"); //IMU의 값 0으로 집어넣음
    previousLooptime = currentMillis;

    //readAngles(); IMU 연결 안됨

    recvWithStartEndMarkers();

    //batterieStatus();  // 배터리 표시 코드

    newData = false;
    moveServos(pulse);
  }
}

void setPulse() {
  for (int i = 0 ; i < 12; i++) {
    pulse[i] = 0;
  }
}

void moveServos(int *pulse) {

  for (int i = 0 ; i < 12; i++) {
    int ra = constrain(map(pulse[i], 0, 180, 180, 440), 180, 440);
    pwm.setPWM(i, 0, ra);
  }
}


void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char spaceMarker = '#';

  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();
    //Serial.println("serial In");
    if (recvInProgress == true) {
      if (rc != endMarker && rc != spaceMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else if (rc == spaceMarker ) {
        receivedChars[ndx] = '\0';
        for (int i = 0; i < 12; i++) {
          if (spaceCounter == i) {
            //Serial.println(receivedChars);
            pulse[i] = atoi(receivedChars);
            spaceCounter++;
            ndx = 0;
          }
        }
        /*if (spaceCounter == 0) {
          //Serial.println(receivedChars);
          pulse0 = atoi(receivedChars);
          spaceCounter++;
          ndx = 0;
          }
          else if (spaceCounter == 1) {
          //Serial.println(receivedChars);
          pulse1 = atoi(receivedChars);
          spaceCounter++;
          ndx = 0;
          }
          else if (spaceCounter == 2) {
          //Serial.println(receivedChars);
          pulse2 = atoi(receivedChars);
          spaceCounter++;
          ndx = 0;
          }
          else if (spaceCounter == 3) {
          //Serial.println(receivedChars);
          pulse3 = atoi(receivedChars);
          spaceCounter++;
          ndx = 0;
          }
          else if (spaceCounter == 4) {
          //Serial.println(receivedChars);
          pulse4 = atoi(receivedChars);
          spaceCounter++;
          ndx = 0;
          }
          else if (spaceCounter == 5) {
          //Serial.println(receivedChars);
          pulse5 = atoi(receivedChars);
          spaceCounter++;
          ndx = 0;
          }
          else if (spaceCounter == 6) {
          //Serial.println(receivedChars);
          pulse6 = atoi(receivedChars);
          spaceCounter++;
          ndx = 0;
          }
          else if (spaceCounter == 7) {
          //Serial.println(receivedChars);
          pulse7 = atoi(receivedChars);
          spaceCounter++;
          ndx = 0;
          }
          else if (spaceCounter == 8) {
          //Serial.println(receivedChars);
          pulse8 = atoi(receivedChars);
          spaceCounter++;
          ndx = 0;
          }
          else if (spaceCounter == 9) {
          //Serial.println(receivedChars);
          pulse9 = atoi(receivedChars);
          spaceCounter++;
          ndx = 0;
          }
          else if (spaceCounter == 10) {
          //Serial.println(receivedChars);
          pulse10 = atoi(receivedChars);
          spaceCounter++;
          ndx = 0;
          }
          else if (spaceCounter == 11) {
          //Serial.println(receivedChars);
          pulse11 = atoi(receivedChars);
          spaceCounter++;
          ndx = 0;
          }
          }
          else {
          receivedChars[ndx] = '\0'; // terminate the string
          //Serial.println(receivedChars);
          pulse11 = atoi(receivedChars);
          recvInProgress = false;
          ndx = 0;
          spaceCounter = 0;
          newData = true;
          }*/
      }

      else if (rc == startMarker) {
        recvInProgress = true;
      }
    }
  }
}

void batterieStatus() {
  batterieVoltage = analogRead(A1);

  if (batterieVoltage >= 164) {
    digitalWrite(13, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(11, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(9, HIGH);
  }
  else if (batterieVoltage >= 154) {
    digitalWrite(13, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(11, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);
  }
  else if (batterieVoltage >= 144) {
    digitalWrite(13, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(11, HIGH);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
  }
  else if (batterieVoltage >= 134) {
    digitalWrite(13, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
  }
  else if (batterieVoltage >= 124) {
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
  }
  else {
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
  }
}
