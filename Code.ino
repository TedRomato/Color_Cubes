#include<Adafruit_NeoPixel.h>


bool updateInput = false;
uint8_t mode = 0;

const uint16_t PIN = 6;
const uint16_t LED_COUNT = 4;

int update_but_pin = 13;
int mode_but_pin = 12;

const uint8_t diodesInNode = 1;

const uint8_t nodeAmount =  LED_COUNT/diodesInNode;

double nodeColors[nodeAmount*3];

uint8_t selectedArr = 0;

uint8_t rainbowShift[] = {0,255,0,255,0,0,0,0,255};
uint8_t purpleBlueShift[] = {0,0,255,120,0,191};
uint8_t redGoldShift[] = {255,0,0,225,60,0};
uint8_t greenYellowShift[] = {0,255,0,225,100,0};


uint8_t diodePatternLocations[LED_COUNT];

double brightness = 0.1;

uint8_t difference = 30;
uint8_t changeVal = 1;

uint8_t noiseLvl = 25;

Adafruit_NeoPixel pixels = new Adafruit_NeoPixel(LED_COUNT, PIN, NEO_RGB + NEO_KHZ800);

void setup() {
//  put your setup code here, to run once:
  pixels.begin();
  Serial.begin(9600);
  pinMode(update_but_pin, INPUT);
  pinMode(mode_but_pin, INPUT);
}

//ON CHANGE DO SETUP WITH NEW VALS


void loop() {  
  delay(30);
  mode = digitalRead(mode_but_pin);
  updateInput = digitalRead(update_but_pin);
  Serial.print(analogRead(A0));Serial.print(", ");Serial.print(analogRead(A1));Serial.print(", ");Serial.print(analogRead(A2));Serial.print(", ");Serial.println(analogRead(A3));
  if(mode == 0){
    if(updateInput){
      int nodeToUpdate = map(analogRead(A3),0,1023,0,nodeAmount-1);
      if(nodeToUpdate == nodeAmount){
        for(int i = 0; i < nodeAmount; i++){
          nodeColors[i*3] = filterNoise(map(analogRead(A0),0,1023,0,255),noiseLvl);
          nodeColors[i*3 + 1] = filterNoise(map(analogRead(A1),0,1023,0,255),noiseLvl);
          nodeColors[i*3 + 2] = filterNoise(map(analogRead(A2),0,1023,0,255),noiseLvl);       
        }
      }else{
        nodeColors[nodeToUpdate*3] = filterNoise(map(analogRead(A0),0,1023,0,255),noiseLvl);
        nodeColors[nodeToUpdate*3 + 1] = filterNoise(map(analogRead(A1),0,1023,0,255),noiseLvl);
        nodeColors[nodeToUpdate*3 + 2] = filterNoise(map(analogRead(A2),0,1023,0,255),noiseLvl);   
      }
      
      for(uint8_t i = 0; i < nodeAmount; i++){
        for(uint8_t x = 0; x < diodesInNode; x++){
          pixels.setPixelColor(i*diodesInNode + x,nodeColors[i*3],nodeColors[i*3 + 1],nodeColors[i*3 + 2]);
        }
      }
    }
  }else{
    if(updateInput){
      uint8_t formerSelectedArr = selectedArr;
      uint8_t formerDifference = difference;
      selectedArr = map(analogRead(A0),0,1023,0,3);
      brightness = map(analogRead(A1),0,1023,0,100)/100.00;
      changeVal = map(analogRead(A2),0,1023,0,50);
      difference = map(analogRead(A3),0,1023,0,127);
      if(formerSelectedArr == selectedArr && numberIsBetween(difference,formerDifference - 10, formerDifference + 10)){
        updateInput = false;
        difference = formerDifference;
      }
    }
    //color change
    switch (selectedArr){
      case 0:
        if(updateInput){
          doSetUp(rainbowShift,3);
        }
        useColorPattern(rainbowShift,3);
        break;
  
      case 1:
        if(updateInput){
          doSetUp(purpleBlueShift,2);
        }
        useColorPattern(purpleBlueShift,2);
        break;
  
      case 2:
        if(updateInput){
          doSetUp(redGoldShift,2);
        }
        useColorPattern(redGoldShift,2);
        break;
  
      case 3:
        if(updateInput){
          doSetUp(greenYellowShift,2);
        }
        useColorPattern(greenYellowShift,2);
        break;
  
      default:
        break;
    }
  }
  pixels.show();
}

void doSetUp(uint8_t pattern[], uint8_t patternSteps){
  for(uint16_t i = 0; i < nodeAmount; i++){
    diodePatternLocations[i] = 0;
    nodeColors[i*3] = 0;
    nodeColors[i*3 + 1] = 0;
    nodeColors[i*3 + 2] = 0;
    for(uint16_t x = 0; x < diodesInNode;x++){
      pixels.setPixelColor(i*diodesInNode + x,nodeColors[i*3]*brightness,nodeColors[i*3 + 1]*brightness,nodeColors[i*3 + 2]*brightness);  
    } 
     
    for(int x = 0; x < i; x++){
      makeNodePatternStep(i,pattern,patternSteps,difference);
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

uint8_t filterNoise(uint8_t val, uint8_t noiseMaxLVL){
  if(val > noiseMaxLVL){
    return val;
  }else{
    return 0;
  }
}
