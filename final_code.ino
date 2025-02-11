#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <RTClib.h>
#include <Wire.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
RTC_DS3231 rtc;
char t[32];
int lightSensor = 0;
int distanceSensor = 1;
int valueFromLightSensor = 0; // Added variable for light sensor
int valueFromDistanceSensor = 0; // Added variable for distance sensor

Servo myservo; // Create a Servo object

bool accessGranted = false;
bool mrng = true;
bool mid = true;
bool evng = true;
int c=0;
void setup() {
  Serial.begin(9600); // Initiate a serial communication
  SPI.begin(); // Initiate SPI bus
  mfrc522.PCD_Init(); // Initiate MFRC522
  myservo.attach(9); // Attach the servo to pin 9
  myservo.write(0); // Initialize servo position to 0 degrees
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}
int z=0;
void loop() {
  int i = 0;
  z++;
  if(z==2)
  {
    Serial.println("Authorized access");
  }
  if (i == 0 ){
    myservo.write(90);
  }
  i += 1;
  DateTime now = rtc.now();
  sprintf(t, "%02d:%02d:%02d %02d/%02d/%02d", now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());
  Serial.print(F("Date/Time: "));
  Serial.println(t);
  delay(1000);

  valueFromLightSensor = analogRead(lightSensor);
  valueFromDistanceSensor = analogRead(distanceSensor);

  
  Serial.print("Distance Value: ");
  int distance = 4800 / (valueFromDistanceSensor - 20);
  Serial.println(distance);

  if (distance >= 16) {
    Serial.println();
  }

  if (accessGranted) {
    if (8 <= now.hour() && now.hour() <= 12 && mrng == true) {
      myservo.write(90); // Rotate the servo to 90 degrees
      delay(5000); // Wait for 5 seconds
      myservo.write(0); // Return the servo to the original position (0 degrees)
      mrng = false;
    } else if (12 < now.hour() && now.hour() <= 16 && mid == true) {
      myservo.write(90); // Rotate the servo to 90 degrees
      delay(5000); // Wait for 5 seconds
      myservo.write(0); // Return the servo to the original position (0 degrees)
      mid = false;
    } else if (16 < now.hour() && now.hour() <= 21 && evng == true) {
      myservo.write(90); // Rotate the servo to 90 degrees
      delay(5000); // Wait for 5 seconds
      myservo.write(0); // Return the servo to the original position (0 degrees)
      evng = false;
    }
    
  }
  if (now.hour() == 7) {
    mid = true;
    mrng = true;
    evng = true;
  }
if (Serial.available() > 0) {
    String content = Serial.readString();
    content.trim();
    if (content == "4F00A51CB147" or content == "5F01A56CB147") {
      printf("Authorized access");
      accessGranted = true;
    } else {
      Serial.println("Access denied");
    }
  }
  else{
    accessGranted=true;
    delay(3000);
  }
  delay(3000);
}