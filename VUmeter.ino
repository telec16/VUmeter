#define SOUND A0

#define PWM_PERIOD 50UL //us
#define LEDS 10
const int LEDS_PINS[LEDS] = {9, 10, 11, 8, 7, 6, 5, 4, 3, 2};
//0: OFF, 255: Fully ON
const unsigned char SOFT_PWM[LEDS] = {50, 50, 50, 50, 50, 255, 255, 255, 200, 200};

#define SAMPLE_PERIOD 10000UL //us
#define SAMPLES 64 // => 640ms
int samples[SAMPLES] = {0};
int head = 0;
int tail = 0;
long sum = 0;


void setup() {
  analogReference(EXTERNAL);
  
  for(int i=0; i<LEDS; i++){
    pinMode(LEDS_PINS[i], OUTPUT);
  }
}


void loop() {
  static unsigned long sampleTimer = micros();
  static unsigned long pwmTimer = micros();

  static int value = 0;
  static int peak = 0;
  
  if(micros() > (sampleTimer+SAMPLE_PERIOD))
  {
    sampleTimer = micros();
    
    int discarded = sample();
    int current = samples[head];
    
    value = map(current/*avgSample(discarded, current)*/, 0, 1023, 0, 10);
    peak = map(maxSample(), 0, 1023, 0, 10);
  }
  
  if(micros() > (pwmTimer+PWM_PERIOD))
  {
    pwmTimer = micros();
    
    bargraph(value, peak);
  }

}


//Circular buffer
int sample(){
  int discarded;

  head = (head+1)%SAMPLES;
  if(head == tail) //Buffer is full;
    tail = (tail+1)%SAMPLES;

  discarded = samples[head];
  samples[head] = analogRead(SOUND);

  return discarded;
}

//Sliding mean
int avgSample(int discarded, int current){
  sum -= discarded;
  sum += current;

  return sum/SAMPLES;
}


int maxSample(){
  int peak = 0;
  
  for(int i=0; i<SAMPLES; i++){
    if(samples[i]>peak){
      peak = samples[i];
    }
  }
  
  return peak;
}


void bargraph(int value, int peak){
  static unsigned char counter = 0;
  
  --peak;
  --counter;
  for(int i = 0; i < LEDS; i++){
    digitalWrite(LEDS_PINS[i], ((i < value) || (i == peak)) && (SOFT_PWM[i]>counter));
  }
}
