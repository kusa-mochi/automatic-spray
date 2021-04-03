#include <Servo.h>
#define echoPin 2
#define trigPin 4
#define servoPin 3
#define sonicSpeed 340 // 音速[m/sec]
#define servoOnDegree 1
#define servoOffDegree 0

double duration = 0; //受信した間隔
double distance = 0; //距離
Servo gripServo;

void setup() {
  Serial.begin( 9600 );
  pinMode( echoPin, INPUT );
  pinMode( trigPin, OUTPUT );
  gripServo.attach(servoPin);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite( trigPin, HIGH ); //超音波を出力
  delayMicroseconds( 10 ); //
  digitalWrite( trigPin, LOW );
  duration = pulseIn( echoPin, HIGH ); //センサからの入力
  if (duration > 0) {
    duration = duration / 2; //センサから対象物までの片道経過時間
    distance = duration * sonicSpeed * 100 / 1000000; // 経過時間を距離に換算する。
    Serial.print(distance);
    Serial.println(" cm");

    // 距離が10cm未満である場合
    if(distance < 10.0) {
      gripServo.write(servoOnDegree);
    } else {
      gripServo.write(servoOffDegree);
    }
  }
  delay(100);
}
