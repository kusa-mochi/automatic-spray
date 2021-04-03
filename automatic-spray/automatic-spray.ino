#include <Servo.h>
#define loopInterval 100  // loop関数のループ間隔[msec]
#define echoPin 2
#define trigPin 4
#define servoPin1 3
#define servoPin2 5
#define sonicSpeed 340 // 音速[m/sec]
#define switchDistance 10.0 // サーボモータの角度のON/OFFを切り替える基準とする距離[cm]
#define servo1OnDegree 0 // サーボモータONのときの角度[deg] 0
#define servo1OffDegree 180  // サーボモータOFFのときの角度[deg] 180
#define servo2OnDegree 180 // サーボモータONのときの角度[deg] 180
#define servo2OffDegree 0  // サーボモータOFFのときの角度[deg] 0

double duration = 0; //受信した間隔
double distance = 0; //距離
Servo gripServo1;
Servo gripServo2;

void setup() {
  Serial.begin( 9600 );
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
    Serial.print(distance);
    Serial.println(" cm");

    // 距離が<switchDistance>cm未満である場合
    if (distance < switchDistance) {
      gripServo1.write(servo1OnDegree);
      gripServo2.write(servo2OnDegree);
    } else {
      gripServo1.write(servo1OffDegree);
      gripServo2.write(servo2OffDegree);
    }
  }
  delay(loopInterval);
}
