#include <FastGPIO.h>

double ch[4];

void setup() 
{
  // Engines pins
  FastGPIO::Pin<2>::setOutput(LOW);
  FastGPIO::Pin<3>::setOutput(LOW);
  
  // Direction pins
  FastGPIO::Pin<12>::setOutput(LOW);
  FastGPIO::Pin<13>::setOutput(LOW);

  // RC pins
  FastGPIO::Pin<4>::setInput(); //R -> L/R
  FastGPIO::Pin<5>::setInput(); //R -> U/D
  FastGPIO::Pin<6>::setInput(); //L -> U/D
  FastGPIO::Pin<7>::setInput(); //L -> L/R
  Serial.begin(9600);
}

// Engine
int cycleTime1 = 0;
int cycleTime2 = 0;
int e1 = 0;
int e2 = 0;
int e1P = 0;
int e2P = 0;
int engDir = 0;
int engDirP = 0;
bool left = false;
bool right = false;
bool control_on = false;
int counter = 0;

int dT_E = 0;
int actualTime = 0;

// RC1
int RC1_state = 0;
int RC1_prevState = 0;
int RC1_stateTime = 0;
int RC1_prevStateTime = 0;
int dT_RC1 = 0;

// RC2
int RC2_state = 0;
int RC2_prevState = 0;
int RC2_stateTime = 0;
int RC2_prevStateTime = 0;
int dT_RC2 = 0;



void loop()
{
  // RC1
  RC1_state = FastGPIO::Pin<6>::isInputHigh();
  if (RC1_state != RC1_prevState)
  {
    counter = 0;
    RC1_prevState = RC1_state;
    RC1_stateTime = micros();
    if(RC1_state == LOW)
    {
      dT_RC1 = RC1_stateTime - RC1_prevStateTime;

      dT_RC1 = (dT_RC1 - 1500) / 5;    // scale to <-100, 100>
      
      if (dT_RC1 < 2 && dT_RC1 >-2) control_on = true;
      if (control_on==false) dT_RC1 = 0;
    }
    RC1_prevStateTime = RC1_stateTime;
  }
  else counter++;

  if (counter > 1000) {
    control_on=false;
    dT_RC1 = 0;
  }

  // RC2
  RC2_state = FastGPIO::Pin<4>::isInputHigh();
  if (RC2_state != RC2_prevState)
  {
    RC2_prevState = RC2_state;
    RC2_stateTime = micros();

    if(RC2_state == LOW)
    {
      dT_RC2 = RC2_stateTime - RC2_prevStateTime;
      dT_RC2 = (dT_RC2 - 1500) / 5;    // scale to <-100, 100>
    }
    RC2_prevStateTime = RC2_stateTime;
  }

  setDirections();

  setEngines();

  // Engine 1
  actualTime = micros();
  dT_E = actualTime - cycleTime1;
  
  if(dT_E < e1*2)
    FastGPIO::Pin<3>::setOutput(HIGH);
  else if(dT_E < 200)
  {
    FastGPIO::Pin<3>::setOutput(LOW);
  }
  else
  {
      cycleTime1 = actualTime;
  }


  // Engine 2
  actualTime = micros();
  dT_E = actualTime - cycleTime2;
  
  if(dT_E < e2*2)
    FastGPIO::Pin<2>::setOutput(HIGH);
  else if(dT_E < 200)
  {
    FastGPIO::Pin<2>::setOutput(LOW);
  }
  else
  {
      cycleTime2 = actualTime;
  }
  
}

void setEngines()
{   
  if (e1 >= 10)
    FastGPIO::Pin<12>::setOutput(HIGH);
  else if (e1<= -10) 
    FastGPIO::Pin<12>::setOutput(LOW);
 
  if (e2 >= 10)
    FastGPIO::Pin<13>::setOutput(HIGH);
  else if (e2<= -10) 
    FastGPIO::Pin<13>::setOutput(LOW);
    
  e1=abs(e1);
  e2=abs(e2);
  
  if (e1<5) e1P=0;
  else if (e1 > 12) e1P = e1;
   
  if(e2<5) e2P=0;
  else if (e2 > 12) e2P = e2;
  
  if (e1>25) e1P = 25;
  if (e2>25) e2P = 25;
  
  e1 = e1P;
  e2 = e2P;
  }

void setDirections()
{
  engDir = (dT_RC2);  

  if (engDir < 6 && engDir > -6) {
    engDirP = 0;
    left = false;
    right = false;
  }
  
  if (engDir > 13){
    engDirP = engDir;
    left = false;
    right = true;
  }
  if (engDir <-13) {
    engDirP = engDir;
    left = true;
    right = false;
  }
  
  if (engDir > 20) engDirP = 20;
  if (engDir < -20) engDirP = -20;

  engDir = engDirP;
  
  if (dT_RC1>=10 && left)           //Tyl - Lewo
  {
    e1 = dT_RC1;
    e2 = dT_RC1 - engDir;
  }
  else if (dT_RC1>=10 && right)     //Tyl - Prawo
  {
    e1 = dT_RC1 + engDir;
    e2 = dT_RC1;  
  }
    else if (dT_RC1<=-10 && left)   //Przod - lewo
  {
    e1 = dT_RC1;
    e2 = dT_RC1 + engDir;  
  }
    else if (dT_RC1<=-10 && right)   //Przod - prawo
  {
    e1 = dT_RC1 - engDir;
    e2 = dT_RC1;  
  }      
  else if (dT_RC1<=-5 || dT_RC1>=5)
  {
    e1 = dT_RC1;
    e2 = dT_RC1;
  }
  else
  {
      e1 = engDir;
      e2 = -engDir;
  } 


}

