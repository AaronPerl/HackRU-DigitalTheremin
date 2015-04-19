// Copyright 2015
// Author: Aaron Perl
// USB Theremin Teensy Code

const int led = 13;
const int ECHO0 = 2;
const int ECHO1 = 3;
const int TRIG0 = 4;
const int TRIG1 = 5;
int start0 = 0;
int start1 = 0;
float distance0;
float distance1;
boolean new0 = false;
boolean new1 = false;

typedef struct descriptor {
  float dist0;
  float dist1;
  unsigned char in0;
  unsigned char in1;
  char padding[48];
} descriptor;

typedef union descriptorData {
  descriptor values;
  char bytes[sizeof(descriptor)];
} descriptorData;


void echo(int pin)
{
  switch(pin)
  {
    case ECHO0:
      new0 = false;
      start0 = micros();
      return;
    case ECHO1:
      new1 = false;
      start1 = micros();
      return;
  }
}

float calcDistance(unsigned long dt)
{
  return dt / 58.0;
}

void endEcho(int pin)
{
  switch(pin)
  {
    case ECHO0:
      distance0 = calcDistance(micros() - start0);
      start0 = 0;
      new0 = true;
      break;
    case ECHO1:
      distance1 = calcDistance(micros() - start1);
      start1 = 0;
      new1 = true;
      break;
  }
}

void echoISR0()
{
  if (digitalRead(ECHO0))
    echo(ECHO0);
  else
    endEcho(ECHO0);
}

void echoISR1()
{
  if (digitalRead(ECHO1))
    echo(ECHO1);
  else
    endEcho(ECHO1);
}

void pulseTrig(int trig)
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(trig, LOW);
}

byte recvBuffer[64];
descriptorData sendBuffer;

// the setup routine runs once when you press reset:
void setup() {                
  Serial.begin(115200);
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
  pinMode(TRIG0, OUTPUT);
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO0, INPUT);
  pinMode(ECHO1, INPUT);
  attachInterrupt(ECHO0, echoISR0, CHANGE);
  attachInterrupt(ECHO1, echoISR1, CHANGE);
  memset(sendBuffer.bytes, 0, sizeof(sendBuffer));
  pulseTrig(TRIG0);
  pulseTrig(TRIG1);
}

// the loop routine runs over and over again forever:
void loop() {
  int n;
  n = RawHID.recv(recvBuffer, 0);
  if (n > 0)
  {
    Serial.println("Received data!");
    sendBuffer.values.padding[0] = 0xFF;
    n = RawHID.send(sendBuffer.bytes, 1000);
    if (n < 0)
    {
      Serial.println("Failed to send data!");
    }
  }
  if (new0)
  {
    if (distance0 < 150.0 && distance0 > 6.0)
    {
      Serial.print("Echo 0 length: ");
      Serial.println(distance0);
      sendBuffer.values.dist0 = distance0;
      sendBuffer.values.in0 = 1;
    }
    else
    {
      sendBuffer.values.in0 = 0;
    }
    new0 = false;
    pulseTrig(TRIG0);
    
  }
  if (new1)
  {
    if (distance1 < 150.0 && distance1 > 6.0)
    {
      Serial.print("Echo 1 length: ");
      Serial.println(distance1);
      sendBuffer.values.dist1 = distance1;
      sendBuffer.values.in1 = 1;
    }
    else
    {
      sendBuffer.values.in1 = 0;
    }
    new1 = false;
    pulseTrig(TRIG1);
  }
  delay(10);
}
