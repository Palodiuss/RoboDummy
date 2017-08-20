double ch[4];

void setup() {
  // Engines pins
  pinMode(3, OUTPUT);

  // RC pins
  pinMode(4, INPUT); //R -> L/R
  pinMode(5, INPUT); //R -> U/D
  pinMode(6, INPUT); //L -> U/D
  pinMode(7, INPUT); //L -> L/R

  Serial.begin(9600);
}

// Engine
int cycleTime = 0;


// RC
int RC_state = 0;
int RC_prevState = 0;

int RC_stateTime = 0;
int RC_prevStateTime = 0;

int dT_RC1 = 1500;
int value_RC1 = 0;

void loop() {

  // RC
  RC_state = digitalRead(4);
  if (RC_state != RC_prevState)
  {
    RC_prevState = RC_state;
    RC_stateTime= micros();

    if(RC_state == LOW)
    {
      dT_RC1 = RC_stateTime - RC_prevStateTime;

      dT_RC1 = (dT_RC1 - 1000) / 10;    // scale to <0, 100>
      if(dT_RC1 > 105) dT_RC1 = 10;
      if(dT_RC1 < 0) dT_RC1 = 0;
    }
    RC_prevStateTime = RC_stateTime;
  }

  // Engine
  int actualTime = micros();
  int dT_E = actualTime - cycleTime;

  if(dT_E < dT_RC1)
    digitalWrite(3, HIGH);
  else if(dT_E < 100)
  {
    digitalWrite(3, LOW);
  }
  else
  {
      cycleTime = actualTime;
  }
}
