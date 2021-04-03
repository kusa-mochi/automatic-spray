#include <Servo.h>
#define loopInterval 33  // loop関数のループ間隔[msec]
#define echoPin 2
#define trigPin 4
#define servoPin1 3
#define servoPin2 5
#define sonicSpeed 340 // 音速[m/sec]
#define sensorFilterLength 30 // センサ入力の平滑化フィルタの長さ
#define switchOnDistance 1.0 // サーボモータの角度をONに切り替える基準とする距離[cm]
#define switchOffDistance 6.0  // サーボモータの角度をOFFに切り替える基準とする距離[cm]
#define servo1OnDegree 0 // サーボモータONのときの角度[deg] 0
#define servo1OffDegree 180  // サーボモータOFFのときの角度[deg] 180
#define servo2OnDegree 180 // サーボモータONのときの角度[deg] 180
#define servo2OffDegree 0  // サーボモータOFFのときの角度[deg] 0

double duration = 0; //受信した間隔
double distance = 0; //距離
double distanceBuffer[sensorFilterLength];
Servo gripServo1;
Servo gripServo2;

void initBuffer() {
  int i = 0;
  for (i = 0; i < sensorFilterLength; i++) {
    distanceBuffer[i] = 0.0;
  }
}

void pushBuffer(double d) {
  int i = 0;
  for(i = sensorFilterLength - 2; i <= 0; i--) {
    distanceBuffer[i + 1] = distanceBuffer[i];
  }
  distanceBuffer[0] = d;
}

double getFilteredValue() {
  int i = 0;
  double output = 0.0;
  for(i = 0; i < sensorFilterLength; i++) {
    output += distanceBuffer[i];
  }
  output /= sensorFilterLength;
  return output;
}

void setup() {
  Serial.begin( 9600 );
  initBuffer();
  pinMode( echoPin, INPUT );
  pinMode( trigPin, OUTPUT );
  gripServo1.attach(servoPin1);
  gripServo2.attach(servoPin2);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); //超音波を出力
  delayMicroseconds(10); //
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); //センサからの入力
  if (duration > 0) {
    duration = duration / 2; //センサから対象物までの片道経過時間
    distance = duration * sonicSpeed * 100 / 1000000; // 経過時間を距離に換算する。
    pushBuffer(distance);
    //Serial.print(distance);
    //Serial.println(" cm");
    double filteredDistance = getFilteredValue();
    Serial.print(filteredDistance);
    Serial.print(" cm : ");

    // 距離が<switchDistance>cm未満である場合
    if (filteredDistance < switchOnDistance) {
      gripServo1.write(servo1OnDegree);
      gripServo2.write(servo2OnDegree);
      Serial.println("on");
    }
    if(filteredDistance > switchOffDistance) {
      gripServo1.write(servo1OffDegree);
      gripServo2.write(servo2OffDegree);
      Serial.println("off");
    }
  }
  delay(loopInterval);
}
