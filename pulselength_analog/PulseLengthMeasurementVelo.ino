/*
	simple interface to get the speed in which the stationary bike is turning.

	Heavily inspired by Ralf Stoffels http://surasto.de/index.php?cat=Hacks&page=Kettler-Hack
*/

// Settings
int RPM_INPUT = A0;
int led = 13;
int state = 0;

int rpmState, lastRpmState;   // Holds the signal state of the RPM input 
unsigned long lastMillis;     // Holds the last time
unsigned long lastWriteTime;
unsigned long deltaT=255;

void setup() {
  // initialize serial communication at 9600 bits per second
  pinMode(RPM_INPUT, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int value = getSpeedByte();
  if (value == 1000){
    //nothing todo
  } else if (value < 30){
    Serial.println("nach vorne immer");
    digitalWrite(led, HIGH);
    Serial.println(value);
  } else {
    Serial.println("nach hinten nimmer");
    digitalWrite(led, LOW);
    Serial.println(value);
  }
}


int getSpeedByte(void) {
  int value = 0;
  unsigned long speedByte;
  unsigned long newMillis;
  rpmState = analogRead(RPM_INPUT);

  if ( (rpmState > lastRpmState) && (rpmState >= 100 )){     // rising edge detected
     newMillis = millis();
     deltaT = newMillis - lastMillis;
     lastMillis = newMillis;
     
     if (deltaT > 510) deltaT = 510; 
  }   
  
  lastRpmState = rpmState;
  if (millis()-lastWriteTime > 100) {   // Write every 1000ms
     speedByte = deltaT /2;
     lastWriteTime = millis();
     deltaT += 5;    // Avoids that deltaT becomes 255 if there are no pulses over a longer period
     if (deltaT > 510) deltaT = 510;
     return speedByte;
  }
  return 1000;
}
