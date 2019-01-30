const int numReadings = 20;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int inputPin = A0;
int readingsv[numReadings];     // the readings from the analog input
int readIndexv= 0;              // the index of the current reading
int totalv = 0;                 // the running total
int averagev = 0;               // the average
int inputPinv = A0;
int duty = 127;
double v_old=0;
double i_old=0;
double p_old=0;
int dutylast= 3;
double highest = 0;
int count = 0;

void setup() 
{
  TCCR1B = TCCR1B & B11111000 | B00000001; // set timer 1 divisor to 1 for PWM frequency of 31372.55 Hz
  //Set up pin 9 or 10 here for this to work
  pinMode(10, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A5, INPUT);
  Serial.begin(9600);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
    readingsv[thisReading] = 0;
  }
}

void loop() 
{
  //initializing values
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
  
  if(d_p < 0){
    if(i + (d_i/d_v)*v<=0){
      duty +=1;
      dutylast = 1;
    }
    else{
      duty -=1;
      dutylast = -1;
    }    
  }
  
  if(d_p>0){
  duty += dutylast;
  }
  
  if(duty<38){
    duty = 38;
  }
  
  if(duty >217){
  duty = 217;
  }
  analogWrite(10, duty);

  //printing information on serial monitor
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
