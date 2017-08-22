double ch[4];

void setup() {
  // Engines pins
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  // RC pins
  pinMode(4, INPUT); //R -> L/R
  pinMode(5, INPUT); //R -> U/D
  pinMode(6, INPUT); //L -> U/D
  pinMode(7, INPUT); //L -> L/R

  //Serial.begin(9600);
}

// Engine
int cycleTime1 = 0;
int cycleTime2 = 0;
int e1 = 0;
int e2 = 0;


// RC
int RC1_state = 0;
int RC1_prevState = 0;
int RC1_stateTime = 0;
int RC1_prevStateTime = 0;
int dT_RC1 = 0;

int RC2_state = 0;
int RC2_prevState = 0;
int RC2_stateTime = 0;
int RC2_prevStateTime = 0;
int dT_RC2 = 0;

void loop() {

  // RC 1
  RC1_state = digitalRead(6);
  if (RC1_state != RC1_prevState)
  {
    RC1_prevState = RC1_state;
    RC1_stateTime= micros();

    if(RC1_state == LOW)
    {
      dT_RC1 = RC1_stateTime - RC1_prevStateTime;

      dT_RC1 = (dT_RC1 - 1500) / 5;    // scale to <-100, 100>
      if(dT_RC1 > 15) dT_RC1 = 15;
      if(dT_RC1 < 0) dT_RC1 = 0;
    }
    RC1_prevStateTime = RC1_stateTime;
  }

  // RC 2
  RC2_state = digitalRead(4);
  if (RC2_state != RC2_prevState)
  {
    RC2_prevState = RC2_state;
    RC2_stateTime= micros();

    if(RC2_state == LOW)
    {
      dT_RC2 = RC2_stateTime - RC2_prevStateTime;

      dT_RC2 = (dT_RC2 - 1500) / 10;    // scale to <-50, 50>
      if(dT_RC2 > 15) dT_RC2 = 15;
      if(dT_RC2 < -15) dT_RC1 = -15;
    }
    RC2_prevStateTime = RC2_stateTime;
  }
  
  // Directions
  e1 = dT_RC1;
  e2 = dT_RC1;

  int dir = dt_RC2;

  if (dir > 6) 
  {
    e1 = dt_RC1 + dir;
    e2 = dt_Rc1 - (dir/2);
  }
  else if (dir <-6) 
  {
    e1 = dt_Rc1 - (dir/2);
    e2 = dt_Rc1 + dir;
  }

  

  // Engine 1
  int actualTime = micros();
  int dT_E = actualTime - cycleTime1;

  if(dT_E < e1 * 3)
    //TODO: IF e1<0 zmienic kierunek
    digitalWrite(3, HIGH);
  else if(dT_E < 300)
  {
    digitalWrite(3, LOW);
  }
  else
  {
      cycleTime1 = actualTime;
  }

  // Engine 2
  int actualTime = micros();
  int dT_E = actualTime - cycleTime2;

  if(dT_E < e2 * 3)
    //TODO: IF e1<0 zmienic kierunek
    digitalWrite(2, HIGH);
  else if(dT_E < 300)
  {
    digitalWrite(2, LOW);
  }
  else
  {
      cycleTime2 = actualTime;
  }
}
