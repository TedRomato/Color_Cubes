#include<Adafruit_NeoPixel.h>


uint8_t mode = 0;
uint8_t prevMode = mode;

const uint16_t PIN = 6;
const uint16_t LED_COUNT = 16;



int mode_but_pin = 12;

const uint8_t nodeAmount =  4;

const uint8_t diodesInNode = LED_COUNT/nodeAmount;

double nodeColors[nodeAmount*3];

uint8_t selectedArr = 0;


uint8_t custom[] = {0,0,0,0,0,0,0,0,0,0,0,0};

uint8_t diodePatternLocations[LED_COUNT];

double brightness = 0.1;

uint8_t difference = 30;
uint8_t changeVal = 1;

uint8_t noiseLvl = 3;


const int analogLen = 10;
int analog0Arr[analogLen]; 
int analog1Arr[analogLen]; 

int analog0Val = 0;
int analog1Val = 0;

Adafruit_NeoPixel pixels = new Adafruit_NeoPixel(LED_COUNT, PIN, NEO_RGB + NEO_KHZ800);

void setup() {
//  put your setup code here, to run once:
  pixels.begin();
  pinMode(mode_but_pin, INPUT);
}

//ON CHANGE DO SETUP WITH NEW VALS

void loop() { 
  for(int i = 0; i < analogLen; i++){
    analog0Arr[i] = analogRead(0);
    analog1Arr[i] = analogRead(1);
    delay(30/analogLen);
  }
  mode = digitalRead(mode_but_pin);
  updateAnalogValues();
  if(mode == 0){
    if(prevMode != mode){
      for(uint16_t i = 0; i < nodeAmount; i++){
        for(uint16_t x = 0; x < diodesInNode;x++){
          pixels.setPixelColor(i*diodesInNode + x,custom[i*3],custom[i*3 + 1],custom[i*3 + 2]);  
        } 
      }
      prevMode = mode;
    }
    uint16_t nodeToUpdate = (uint16_t) map(analog0Val,0,1023,0,nodeAmount);
    updateRGB(analog1Val,nodeToUpdate);
  }else{
    if(prevMode != mode){
      doSetUp(custom,4);
      prevMode = mode;
    }
    changeVal = map(analog0Val,0,1023,0,25);
    brightness = map(analog1Val,0,1023,0,100)/100.00;
    //color change   
    useColorPattern(custom,4);
  }
  pixels.show();
}

void doSetUp(uint8_t pattern[], uint8_t patternSteps){
  int patternLocation = 0;
  for(uint16_t i = 0; i < nodeAmount; i++){
    diodePatternLocations[i] = patternLocation;
    nodeColors[i*3] = pattern[patternLocation*3];
    nodeColors[i*3 + 1] = pattern[patternLocation*3+1];
    nodeColors[i*3 + 2] = pattern[patternLocation*3+2];
    patternLocation++;
    for(uint16_t x = 0; x < diodesInNode;x++){
      pixels.setPixelColor(i*diodesInNode + x,nodeColors[i*3]*brightness,nodeColors[i*3 + 1]*brightness,nodeColors[i*3 + 2]*brightness);  
    } 
    if(patternLocation == patternSteps){
      patternLocation = 0;
    }
  }
}

void useColorPattern(uint8_t pattern[], uint8_t patternSteps){
  for(uint16_t i = 0; i < nodeAmount; i++){
    makeNodePatternStep(i,pattern,patternSteps,changeVal);
  }
}

void makeNodePatternStep(uint8_t i, uint8_t pattern[], uint8_t patternSteps, uint8_t changeVal){
  if(changeNodeColor(i,pattern[diodePatternLocations[i]*3],pattern[diodePatternLocations[i]*3 + 1], pattern[diodePatternLocations[i]*3 + 2], changeVal)){
    diodePatternLocations[i]++;
    if(diodePatternLocations[i] == patternSteps){
      diodePatternLocations[i] = 0; 
    }
  }
    
  for(uint16_t x = 0; x < diodesInNode;x++){
    pixels.setPixelColor(i*diodesInNode + x,nodeColors[i*3]*brightness,nodeColors[i*3 + 1]*brightness,nodeColors[i*3 + 2]*brightness);  
  } 
}

bool changeNodeColor(uint16_t n, uint8_t goalR, uint8_t goalG, uint8_t goalB, int value){
  double differenceR = goalR - nodeColors[n*3];
  double differenceG = goalG - nodeColors[n*3 + 1];
  double differenceB = goalB - nodeColors[n*3 + 2];
 
  double redAddition;
  double greenAddition;
  double blueAddition;

  double maxDifference;
  
  if(abs(differenceR) > abs(differenceG) && abs(differenceR) > abs(differenceB)){
    maxDifference = abs(differenceR);
  }else if(abs(differenceG) > abs(differenceB)){
    maxDifference = abs(differenceG);
  }else{
    maxDifference = abs(differenceB);
  }
  
  if(maxDifference == 0){
    nodeColors[n*3] = goalR;
    nodeColors[n*3 + 1] = goalG;
    nodeColors[n*3 + 2] = goalB;
    return true;
  }
  
  redAddition = differenceR/maxDifference*value;
  greenAddition = differenceG/maxDifference*value;
  blueAddition = differenceB/maxDifference*value;

  if(numberIsBetween(goalR,nodeColors[n*3],nodeColors[n*3] + redAddition)){
    nodeColors[n*3] = goalR;
  }else{
    nodeColors[n*3] = nodeColors[n*3] + redAddition;  
  }
  
  if(numberIsBetween(goalG,nodeColors[n*3 + 1],nodeColors[n*3 + 1] + greenAddition)){
    nodeColors[n*3 + 1] = goalG;
  }else{
    nodeColors[n*3 + 1] = nodeColors[n*3 + 1] + greenAddition;  
  }
 
  if(numberIsBetween(goalB,nodeColors[n*3 + 2],nodeColors[n*3 + 2] + blueAddition)){
    nodeColors[n*3 + 2] = goalB;
  }else{
    nodeColors[n*3 + 2] = nodeColors[n*3 + 2] + blueAddition;  
  }  
  
  return false;
}

bool numberIsBetween(double n, double side1, double side2){
  return ((n < side1 && n > side2) || (n > side1 && n < side2));
}

void updateRGB(int val, uint16_t node){

  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;

  int fullSpectrumCircle = 333;
  int maxHue = 1023/fullSpectrumCircle;
  
  int hue = val / fullSpectrumCircle; 
  int colorVal = map(val - hue*fullSpectrumCircle,0,fullSpectrumCircle,0,765);
  
  if(colorVal > 765){
    colorVal = 765;
  }
  if(colorVal <= 255 && colorVal >= 0){
    r = colorVal;
    b = 255-colorVal;
    g = 0;
  }
  else if(colorVal <= 510 && colorVal >= 255){
    g = colorVal-255;
    r = 255-(colorVal-255);
    b = 0;
  }
  else if(colorVal <= 765 && colorVal >= 510){
    b = colorVal-510;
    g = 255-(colorVal-510);
    r = 0;
  }
  
  
  double value = (double) hue/maxHue;
  if(value != 1){
    value /= 1.5;
  }
  double redDiff = 85 - r;
  double greenDiff  = 85 - g;
  double blueDiff = 85 - b;
  r += redDiff*value;
  g += greenDiff*value;
  b += blueDiff*value; 
    
  custom[node*3] = r;
  custom[node*3 + 1] = g;
  custom[node*3 + 2] = b;
  
  for(uint16_t x = 0; x < diodesInNode;x++){
    pixels.setPixelColor(node*diodesInNode + x,r,g,b);  
  }      
}

void updateAnalogValues(){
  int val = getAverageFromArr(analog0Arr, analogLen);
  if(filterNoise(val, analog0Val, noiseLvl)){
    analog0Val = moveDownByOnVal(val,1023,1);
  }
  val = getAverageFromArr(analog1Arr, analogLen);
  if(filterNoise(val, analog1Val, noiseLvl)){
    analog1Val = moveDownByOnVal(val,1023,1);
  }
}

int getAverageFromArr(int arr[], int len){
  int sum = 0;
  for(int i = 0; i < len; i++){
    sum += arr[i];
  }
  return sum/len;
}

int moveDownByOnVal(int val, int moveDownVal, int moveDownBy){
  if(val == moveDownVal){
    return val - moveDownBy;
  }
  return val;
}

bool filterNoise(int val, int newVal, int noiseLVL){
  return ((newVal > val + noiseLVL) || (newVal < val - noiseLVL));
}

uint8_t filterNoise(uint8_t val, uint8_t noiseMaxLVL){
  if(val > noiseMaxLVL){
    return val;
  }else{
    return 0;
  }
}
