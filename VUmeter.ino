#define SOUND A0

#define LEDS 10
const int LEDS_PINS[LEDS] = {9, 10, 11, 8, 7, 6, 5, 4, 3, 2};

#define SAMPLE_PERIOD 10 //ms
#define SAMPLES 64
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
  int discarded = sample();
  int current = samples[head];
  
  int value = map(avgSample(discarded, current), 0, 1023, 0, 10);
  int peak = map(maxSample(), 0, 1023, 0, 10);
  
  bargraph(value, peak);

  delay(SAMPLE_PERIOD);
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
  --peak;
  for(int i = 0; i < LEDS; i++){
    digitalWrite(LEDS_PINS[i], (i < value) || (i == peak));
  }
}
