const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int inputPin = A0;
int readingsv[numReadings];      // the readings from the analog input
int readIndexv= 0;              // the index of the current reading
int totalv = 0;                  // the running total
int averagev = 0;                // the average
int inputPinv = A0;
int duty = 127;
double v_old=0;
double i_old=0;
double p_old=0;
int dutylast= 3;
double highest = 0;
int count = 0;

int i=0;
int x=0;
int OK=0;
int sinPWM[]={1,2,5,7,10,12,15,17,19,22,24,27,30,32,34,37,39,42,
44,47,49,52,54,57,59,61,64,66,69,71,73,76,78,80,83,85,88,90,92,94,97,99,
101,103,106,108,110,113,115,117,119,121,124,126,128,130,132,134,136,138,140,142,144,146,
148,150,152,154,156,158,160,162,164,166,168,169,171,173,175,177,178,180,182,184,185,187,188,190,192,193,
195,196,198,199,201,202,204,205,207,208,209,211,212,213,215,216,217,219,220,221,222,223,224,225,226,227,
228,229,230,231,232,233,234,235,236,237,237,238,239,240,240,241,242,242,243,243,244,244,245,245,246,246,
247,247,247,248,248,248,248,249,249,249,249,249,255,255,255,255,249,249,249,249,249,248,
248,248,248,247,247,247,246,246,245,245,244,244,243,243,242,242,241,240,240,239,238,237,237,236,235,234,
233,232,231,230,229,228,227,226,225,224,223,222,221,220,219,217,216,215,213,212,211,209,208,207,205,204,
202,201,199,198,196,195,193,192,190,188,187,185,184,182,180,178,177,175,173,171,169,168,166,164,162,160,
158,156,154,152,150,148,146,144,142,140,138,136,134,132,130,128,126,124,121,119,117,115,113,110,108,106,
103,101,99,97,94,92,90,88,85,83,80,78,76,73,71,69,66,64,61,59,57,54,52,49,47,44,42,39,37,34,32,30,
27,24,22,19,17,15,12,10,7,5,2,1};


void setup() 
{
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(3, OUTPUT);
TCCR2B = TCCR2B & B11111000 | B00000001; // set timer 1 divisor to 1 for PWM frequency of 31372.55 Hz
  //Set up pin 9 or 10 here for this to work
  pinMode(A0, INPUT);
  pinMode(A5, INPUT);
cli();// stop interrupts
TCCR0A=0;//reset the value
TCCR0B=0;//reset the value
TCNT0=0;//reset the value
//0b allow me to write bits in binary
TCCR0A=0b10100001;//phase correct pwm mode
TCCR0B=0b00000001; //no prescaler
TCCR1A=0;//reset the value
TCCR1B=0;//reset the value
TCNT1=0;//reset the value
OCR1A=509;// compare match value
TCCR1B=0b00001001; //WGM12 bit is 1 and no prescaler

TIMSK1 |=(1 << OCIE1A);

sei();// enable interrupts

for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
    readingsv[thisReading] = 0;
  }
  
}

ISR(TIMER1_COMPA_vect){// interrupt when timer 1 match with OCR1A value
if(i>313 && OK==0){// final value from vector for pin 6
i=0;// go to first value of vector
OK=1;//enable pin 5
}
if(i>313 && OK==1){// final value from vector for pin 5
i=0;//go to firs value of vector
OK=0;//enable pin 6
}
x=sinPWM[i];// x take the value from vector corresponding to position i(i is zero indexed)
i=i+1;// go to the next position
if(OK==0){
OCR0B=0;//make pin 5 0
OCR0A=x;//enable pin 6 to corresponding duty cycle
}
if(OK==1){
OCR0A=0;//make pin 6 0
OCR0B=x;//enable pin 5 to corresponding duty cycle
}
}
void loop() 
{
  int voltn1 = 0;
  int ampn1 = 0;
  double i = 0;
  double v = 0;
  double p = 0;
  double current = 0;
  double factor = 5;
  double voltage = 0;
  double currentfactor = 0.00590;
  double power = 0;
  double d_v = 0;
  double d_i = 0;
  double d_p = 0;
  voltn1 = analogRead(A0);
  voltage = voltn1*(5.0/1045.0);
  voltage = voltage*factor;
  
  

  ampn1 = analogRead(A5);
  
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(A5);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  i = (average-18)*currentfactor;
  // send it to the computer as ASCII digits
  ----------------------------------------------------------
  voltn1 = analogRead(A0);
    // subtract the last reading:
  totalv = totalv - readingsv[readIndexv];
  // read from the sensor:
  readingsv[readIndexv] = analogRead(A0);
  // add the reading to the total:
  totalv = totalv + readingsv[readIndexv];
  // advance to the next position in the array:
  readIndexv = readIndexv + 1;

  // if we're at the end of the array...
  if (readIndexv >= numReadings) {
    // ...wrap around to the beginning:
    readIndexv = 0;
  }

  // calculate the average:
  averagev = totalv / numReadings;
 v = averagev*(5.0/1045.0)*6.27;
  // send it to the computer as ASCII digits
p = v* i;
d_v = v - v_old;
d_i = i - i_old;
d_p = p - p_old;

//if(d_p < 0.01 && d_p > -0.01){}
  

//else{
 if(d_p < 0)
  {
    if((i + (d_i/d_v)*v<0) && v > 13.5 )
    {
      duty +=1;
     dutylast = 1;
    }
    else
    {
      duty -=1;
      dutylast = -1;
    }    
  
  }
 if(d_p>0)
 {
  duty += dutylast;
  
  }

    
    

    

  if(duty<15){
    duty = 15;
  }
 if(duty >217){
  duty = 217;
 }
analogWrite(3  , duty);

  Serial.print(d_v);
  Serial.print(" V ");
  Serial.print(i);
  Serial.print(" A            ");
  Serial.print(duty);
  Serial.print(" duty cycle          ");
  Serial.print(p);
  Serial.print( "    ");
  Serial.println(d_p);

  i_old = i;
  v_old = v;
  p_old = p;
}
