#define SOUND A0

#define LEDS 10
const int LEDS_PINS[LEDS] = {9, 10, 11, 8, 7, 6, 5, 4, 3, 2};

void setup() {
  //analogReference(EXTERNAL);
  
  for(int i=0; i<LEDS; i++){
    pinMode(LEDS_PINS[i], OUTPUT);
  }
}

void loop() {
  int reading = analogRead(SOUND);
  int value = map(reading, 0, 1023, 0, 10);
  
  bargraph(value, 10);

  delay(100);
}

void bargraph(int value, int peak){
  --peak;
  for(int i = 0; i < LEDS; i++){
    digitalWrite(LEDS_PINS[i], (i < value) || (i == peak));
  }
}
