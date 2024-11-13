#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2);
#define trigPin 8
#define echoPin 9
int redled = 13;

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(9600);
  mySerial.begin(9600); // original 19200. While enter 9600 for sim900A
  Serial.println("Logging time completed!");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redled, OUTPUT);
  digitalWrite(redled, LOW);
  delay(100);
}

void loop() {
  long time_duration, distance_in_cm;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  time_duration = pulseIn(echoPin, HIGH);
  distance_in_cm = time_duration / 29 / 2;

  Serial.print(distance_in_cm);
  Serial.println(" cm");

  // Ranging Distance − 2cm – 400 cm
  // if the distance is equal or more than 10 cm
  if (distance_in_cm >= 10) {
    digitalWrite(redled, HIGH);
    Serial.println("Motion detected!");
    Serial.println("Calling....");

    mySerial.println("AT"); // Handshake test with SIM900
    updateSerial();

    // Make a call (dial the phone number)
    mySerial.println("ATD+ZZXXXXXXXXXX;");  // Replace ZZ with country code and XXXXXXXXXX with phone number
    updateSerial();
    delay(20000); // wait for 20 seconds

    // Send SMS to the mobile number
    sendSMS("+ZZXXXXXXXXXX", "Motion detected!");  // Replace ZZ with country code and XXXXXXXXXX with the phone number

    // Hang up the call
    mySerial.println("ATH"); 
    updateSerial();

    delay(2000);
  } else {
    digitalWrite(redled, LOW);
  }
}

void sendSMS(String phoneNumber, String message) {
  mySerial.println("AT+CMGF=1"); // Set SMS to text mode
  updateSerial();
  
  mySerial.print("AT+CMGS=\"");
  mySerial.print(phoneNumber);
  mySerial.println("\""); // Send the phone number

  updateSerial();
  delay(500);

  mySerial.println(message); // Send the message
  updateSerial();

  mySerial.write(26);  // ASCII code for CTRL+Z to send the message
  updateSerial();
  delay(2000);
}

void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read()); // Forward what Serial received to Software Serial Port
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read()); // Forward what Software Serial received to Serial Port
  }
}
