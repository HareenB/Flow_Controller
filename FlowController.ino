#define PULSE_PER_LITTER 40
#define LITTER_LIMIT 2
#define PULSE_LIMIT PULSE_PER_LITTER*LITTER_LIMIT
#define TIME_LIMIT 120000  // 60*1000*(time in minutes)
#define PULSE_INPUT 2 //Interrupt pin
#define WATER_VALVE 13 // output for flow switch
#define PULSE_INTRRUPT_PIN 0 // intrrupt pin 0 is digital pin 2. input from flow meter 
#define MANUAL_RESET 1 //Manual reset button  connect to intrrupt pin 1(digital pin 3) 
#define FLOW_ERROR 10 //false increment of pulse count when there is no flow

volatile int pulseCount;
int pulseCount_1,pulseCount_5,bgPulseCount;
bool START_TIMER=false;
unsigned long timeLimit;
bool FLOW;
volatile unsigned long time,bgTime,offTime;
bool PULSE_CHECK=true;
bool SWON;
void setup() {
  
  Serial.begin(9600);

  //pinMode(PULSE_INPUT,INPUT_PULLUP);
  pinMode(WATER_VALVE,OUTPUT);
// pinMode(MANUAL_RESET,INPUT_PULLUP);
  attachInterrupt(PULSE_INTRRUPT_PIN, countIncrese, RISING);
  attachInterrupt(MANUAL_RESET, btnResetPluseCount, FALLING);
  digitalWrite(WATER_VALVE,HIGH);
  time=millis();
  bgTime=millis();
  bgPulseCount=0;
}

void loop() {
  
  offTime=time+TIME_LIMIT;
  // Serial.print("offtiem= "); Serial.println(offTime);
   //Serial.print("mills()= "); Serial.println(millis());
  if(pulseCount>(bgPulseCount+PULSE_LIMIT) || millis()>offTime){

 // if(millis()>offTime){
 // if(pulseCount>bgPulseCount+PULSE_LIMIT){
    digitalWrite(WATER_VALVE,LOW); // Stop flowF
    SWON=false;
  }
  else{
    digitalWrite(WATER_VALVE,HIGH); // continue flow
    SWON=true;
  }
  if(millis()>bgTime+5000){
    bg5000ms();
    bgTime=millis();
  }

//delay(1000);
}
  
void countIncrese()
{
  pulseCount=pulseCount+1;
 // Serial.print("pulse count= ");
  Serial.println(pulseCount);
}

void resetPluseCount(void){
  Serial.print("LIMITS RESET");
  bgPulseCount=pulseCount;
  pulseCount_1=0;
  pulseCount_5=0;
  //time = millis();
}
void btnResetPluseCount(void){
  Serial.print("RESET BTN PRESSED");
  pulseCount=0;
  bgPulseCount=0;
  pulseCount_1=0;
  pulseCount_5=0;
  time = millis();
}
void bg1000ms(void){
  pulseCount_1=pulseCount;
}

void bg5000ms(void){

  if(PULSE_CHECK){
    pulseCount_5=pulseCount;
    PULSE_CHECK=false;
  }
  else{
  Serial.print("pulse__5= ");
  Serial.println(pulseCount_5);
    if(pulseCount-pulseCount_5 < FLOW_ERROR && SWON){resetPluseCount();}
    PULSE_CHECK=true;
  }
  Serial.print("time -> mils()= ");
  Serial.println(millis());

}
