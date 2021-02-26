#include <Button.h>

//#include <IRremote.h>
//#include <IRremote.h>
//#include "IRLremote.h"

// Choose a valid PinInterrupt or PinChangeInterrupt* pin of your Arduino board
#define pinIR 2

// Choose the IR protocol of your remote. See the other example for this.
//CNec IRLremote;
//#define pinLed 4

// temporary variables to save latest IR input
//uint8_t IRProtocol = 0;
//uint16_t IRAddress = 0;
//char* IRcommand = "";
bool autorunl=true;
unsigned int dimmer = 1;

//#define IRL_BLOCKING true


#include "FastLED.h"
const int prevbuttonPin = 8;  // Set digital pin used with debounced pushbutton
Button prevmyButton(prevbuttonPin, true, true, 50);  // Declare the butto
const int nextbuttonPin = 9;  // Set digital pin used with debounced pushbutton
Button nextmyButton(nextbuttonPin, true, true, 50);  // Declare the butto
const int homebuttonPin = 10;  // Set digital pin used with debounced pushbutton
Button homemyButton(homebuttonPin, true, true, 50);  // Declare the butto

const int brithupbuttonPin = 11;  // Set digital pin used with debounced pushbutton
Button brithupmyButton(brithupbuttonPin, true, true, 50);  // Declare the butto
const int brithdownbuttonPin = 12;  // Set digital pin used with debounced pushbutton
Button brithdownmyButton(brithdownbuttonPin, true, true, 50);  // Declare the butto

//#include <IRremote.h>

//int RECV_PIN = 2;
//IRrecv irrecv(RECV_PIN);
//decode_results results;

// Good Reference!
// http://fastled.io/docs/3.1/group___colorutils.html#gafcc7dac88e25736ebc49a9faf2a1c2e2

/*
//ir stuff

int RECV_PIN = 13; //IR Receiving LED connected to this pin
bool onoff; //defines the current state of the LEDs
unsigned long command; //stores the latest input command
unsigned long lastcommand; //stores the most recent command if the remote button is held and command repeated

IRrecv irrecv(RECV_PIN); //initialise the IR module

decode_results results; //store results from decoding remote signal
*/
#define DATA_PIN    4//22
#define BRIGHTNESS_PIN    A1
//#define COLOR_ORDER BRG     // this might be OK for strip light, but not for each pixel
//#define COLOR_ORDER GRB     
#define COLOR_ORDER RGB
#define LED_TYPE    WS2812
#define NUM_LEDS    31
#define VOLTS          5

#define FRAMES_PER_SECOND  100



// brightness control added 3/6/2016
// original limits:
//#define MASTER_BRIGHTNESS 128 // Set the master brigtness value [should be greater then min_brightness value].
//#define MIN_BRIGHTNESS 30     // Set a minimum brightness level.
int MASTER_BRIGHTNESS=112; // Set the master brigtness value [should be greater then min_brightness value]. 192
#define MIN_BRIGHTNESS 10     // Set a minimum brightness level.
int potValA;                  // Variable to store potentiometer A value (for brightness A)
uint8_t brightnessa;          // Mapped master brightness for color A based on potentiometer reading.

#define NUM_VIRTUAL_LEDS NUM_LEDS+1
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
CRGB leds[NUM_VIRTUAL_LEDS];


int color2;
int center = 0;
int step = -1;
const int maxSteps = 16;
const float fadeRate2 = 0.8;
int diff;
 
//background color
uint32_t currentBg = random(256);
uint32_t nextBg = currentBg;



// variables for marque_v3
  uint16_t holdTime = 60;  // Milliseconds to hold position before advancing.
  uint8_t spacing = 20;      // Sets pixel spacing. [Use 2 or greater]
  int8_t delta = 1;         // Sets forward or backwards direction amount. (Can be negative.)
  uint8_t width = 5;        // Can increase the number of pixels (width) of the chase. [1 or greater]
  
  uint8_t hue = 60;         // Starting color (marque_v3)
  
  boolean fadingTail = 1;   // Add fading tail? [1=true, 0=falue]
  //uint8_t fadeRate = 150;   // How fast to fade out tail. [0-255]
  uint8_t fadeRate = 70;   // How fast to fade out tail. [0-255]
  
  uint8_t hue2_shift = 60;  // Hue shift for secondary color.  Use 0 for no shift. [0-255]
 boolean DEBUG = 0;        // Print some info to serial monitor. [1=true, 0=falue]
  
  int16_t pos;              // Pixel position.
  int8_t advance;           // Stores the advance amount.
  uint8_t color;            // Stores a hue color.
  
byte ledsX[NUM_LEDS][3];
boolean RAINBOWs = false;
boolean RANDOMpick = false;
//uint8_t hue;
uint16_t timeframe;
byte idex = 0;
byte colorTIP = 0;
byte meteorLENGTH;
byte loopCount = 1;    //low number loop counter

// variables for Twinkle
CRGBPalette16 gCurrentPalette;
CRGBPalette16 gTargetPalette;
CRGBPalette16 gPal;
// How often to change color palettes.
#define SECONDS_PER_PALETTE  15 // 30

//float redStates[NUM_LEDS];
//float blueStates[NUM_LEDS];
//float greenStates[NUM_LEDS];
// Also: toward the bottom of the file is an array 
// called "ActivePaletteList" which controls which color
// palettes are used; you can add or remove color palettes
// from there freely.
/*
void IREvent(uint8_t protocol, uint16_t address, uint32_t ircommand) {
  
  if (IRL_BLOCKING && !IRProtocol) {
    IRProtocol = protocol;                                        // update the values to the newest valid input
    IRAddress = address;
    IRCommand = ircommand;
    Serial.println(F("Received IR:"));
    Serial.print(F("Protocol:"));
    Serial.print(protocol);
    Serial.print(F("Adress:"));
    Serial.print(address);
    Serial.print(F("Command:"));
    Serial.println(IRCommand);
  }
  
} // IREvent()*/



// =============================================================================
//   setup routine
// =============================================================================
// put your setup code here, to run once:
void setup() {
  delay(3000); // 3 second delay for recovery
// attachInterrupt(digitalPinToInterrupt(pinIR), IRLinterrupt<IR_NEC>, CHANGE); // IR definition
// attachInterrupt(digitalPinToInterrupt(pinIR), IRLinterrupt<IR_NEC>, CHANGE); // IR definition
  
  Serial.begin(57600);

  // Serial.println(F("Startup"));
   pinMode(8, INPUT_PULLUP);
 pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
   pinMode(11, INPUT_PULLUP);
   pinMode(12, INPUT_PULLUP);
   pinMode(13, INPUT_PULLUP);
  //irrecv.enableIRIn(); // Start the receiver
 
  // Set LED to output
  //pinMode(pinLed, OUTPUT);

  // Start reading the remote. PinInterrupt or PinChangeInterrupt* will automatically be selected
  //if (!IRLremote.begin(pinIR))
   // Serial.println(F("You did not choose a valid pin."));
  
// irrecv.enableIRIn(); // Start the IR receiver
//  FastLED.setMaxPowerInVoltsAndMilliamps( VOLTS, NUM_LEDS*40);
//  FastLED.setMaxPowerInVoltsAndMilliamps( VOLTS, 4000);
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
FastLED.setMaxPowerInVoltsAndMilliamps( VOLTS, 2000);
  FastLED.setBrightness(MASTER_BRIGHTNESS);       

  // for Twinkle
  chooseNextColorPalette(gTargetPalette);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
//SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };
//SimplePatternList gPatterns = { fill_solid_color, trace_inner_star, rainbow, sinelon, trace_star, juggle, bpm };
//SimplePatternList gPatterns = { fill_solid_color, trace_inner_star, trace_star, gradient_fill };
//SimplePatternList gPatterns = { gradient_fill_2, sinelon, fill_solid_color, trace_inner_star, juggle, trace_star, rainbow, bpm };

// I love these 4 patterns -- this is a great base:
// SimplePatternList gPatterns = { gradient_fill_2, fill_solid_color, trace_inner_star, trace_star };
// Tjese are 4 patterns are very energetic, they should be used intermittenly within the whole solution, maybe 1 per 2 above?
// SimplePatternList gPatterns = { rainbow, sinelon, juggle, bpm };

// 11/23/2016 ~ these are patterns for star -- some don't make sense for arch
//SimplePatternList gPatterns = { gradient_fill_2, fill_solid_color, trace_inner_star, trace_star };

// 11/23/2016 good arch patterns -- maybe better sort order
//SimplePatternList gPatterns = { gradient_fill, rainbow, sinelon, juggle, bpm, fill_solid_color };
//SimplePatternList gPatterns = { gradient_fill, bpm, rainbow, sinelon, fill_solid_color, juggle };
//SimplePatternList gPatterns = { drawTwinkles, marque_v3, gradient_fill, bpm, sinelon, juggle };
// 12/11/16 removed Gradient Fill,, bmp, sinelon - it was too "rainbowy"
//SimplePatternList gPatterns = { marque_v3, juggle };  // <-- version recorded to YouTube 12/12/16
SimplePatternList gPatterns = { drawTwinkles,  juggle, sinelon, meteorShower,SnowSparkle,Sparkle,rainbowCycle,theaterChaseRainbow,randomPositionFill,alternateColor,randomColorFill,stars }; //fire,balls
//marque_v3,ripple2,flashes

 
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

//---------Function to read the button and do something----------
void nextreadbutton() {
  nextmyButton.read();
  if(nextmyButton.wasPressed()) {
    Serial.println("Button pressed!  Next pattern...   ");
    //nextPattern();  // Change to the next pattern
gCurrentPatternNumber++;
if (gCurrentPatternNumber>ARRAY_SIZE(gPatterns)) gCurrentPatternNumber=0;
autorunl=false;
    //Flash pixel zero white as a visual that button was pressed.
    leds[1] = CHSV(0,0,255);  //Set first pixel color white
    FastLED.show();  //Update display
    delay(100);  //Short pause so we can see leds[0] flash
    leds[1] = CRGB::Black;  //Set first pixel off
    FastLED.show();  //Update display
    delay(100);  //Short pause so we can see leds[0] flash
  }
}//end_readbutton

//---------Function to read the button and do something----------
void prevreadbutton() {
  prevmyButton.read();
  if(prevmyButton.wasPressed()) {
    Serial.println("Button pressed!  Next pattern...   ");
    ////nextPattern();  // Change to the next pattern
 gCurrentPatternNumber--;
if (gCurrentPatternNumber<0) gCurrentPatternNumber=ARRAY_SIZE(gPatterns)-1;
autorunl=false;
    //Flash pixel zero white as a visual that button was pressed.
    leds[0] = CHSV(0,0,255);  //Set first pixel color white
    FastLED.show();  //Update display
    delay(100);  //Short pause so we can see leds[0] flash
    leds[0] = CRGB::Black;  //Set first pixel off
    FastLED.show();  //Update display
    delay(100);  //Short pause so we can see leds[0] flash
  }
}//end_readbutton

//---------Function to read the button and do something----------
void homereadbutton() {
  homemyButton.read();
  if(homemyButton.wasPressed()) {
    Serial.println("Button pressed!  Next pattern...   ");
    ////nextPattern();  // Change to the next pattern
autorunl=true;
    //Flash pixel zero white as a visual that button was pressed.
    leds[2] = CHSV(0,0,255);  //Set first pixel color white
    FastLED.show();  //Update display
    delay(100);  //Short pause so we can see leds[0] flash
    leds[2] = CRGB::Black;  //Set first pixel off
    FastLED.show();  //Update display
    delay(100);  //Short pause so we can see leds[0] flash
  }
}//end_readbutton

//FastLED.setBrightness(MASTER_BRIGHTNESS); 
//---------Function to read the button and do something----------
void brithupreadbutton() {
  brithupmyButton.read();
  if(brithupmyButton.wasPressed()) {
    Serial.println("Button pressed!  Next pattern...   ");
    ////nextPattern();  // Change to the next pattern
    MASTER_BRIGHTNESS=MASTER_BRIGHTNESS+10;;
    if (MASTER_BRIGHTNESS>245) MASTER_BRIGHTNESS=255;
 FastLED.setBrightness(MASTER_BRIGHTNESS);
    //Flash pixel zero white as a visual that button was pressed.
    leds[3] = CHSV(0,0,255);  //Set first pixel color white
    FastLED.show();  //Update display
    delay(100);  //Short pause so we can see leds[0] flash
    leds[3] = CRGB::Black;  //Set first pixel off
    FastLED.show();  //Update display
    delay(100);  //Short pause so we can see leds[0] flash
  }
}//end_readbutton

//---------Function to read the button and do something----------
void brithdownreadbutton() {
  brithdownmyButton.read();
  if(brithdownmyButton.wasPressed()) {
    Serial.println("Button pressed!  Next pattern...   ");
    ////nextPattern();  // Change to the next pattern
    MASTER_BRIGHTNESS=MASTER_BRIGHTNESS-10;
    if (MASTER_BRIGHTNESS<10) MASTER_BRIGHTNESS=10;
 FastLED.setBrightness(MASTER_BRIGHTNESS);
    //Flash pixel zero white as a visual that button was pressed.
    leds[4] = CHSV(0,0,255);  //Set first pixel color white
    FastLED.show();  //Update display
    delay(100);  //Short pause so we can see leds[0] flash
    leds[4] = CRGB::Black;  //Set first pixel off
    FastLED.show();  //Update display
    delay(100);  //Short pause so we can see leds[0] flash
  }
}//end_readbutton

void flashes()
{
  int positionl=random(NUM_LEDS);
  int randomwidth=random(NUM_LEDS);
  if (randomwidth>(NUM_LEDS-positionl)) randomwidth=NUM_LEDS-positionl;
    uint8_t hue1 = random8(255);
  uint8_t hue2 = hue1 + random8(30,61);
  //fill_solid(positionl, randomwidth, CHSV(random8(255), random8(255), 255/dimmer) );
  for (int flashCounter = 0; flashCounter < random8(3,8); flashCounter++)
  {
    if(flashCounter == 0) dimmer = 5;     // the brightness of the leader is scaled down by a factor of 5
    else dimmer = random8(1,3);           // return strokes are brighter than the leader

   // fill_solid(leds+positionl, randomwidth, CHSV(random8(255), random8(255), 255/dimmer) );

        fill_gradient (leds, positionl, CHSV(hue1, 255, 255),randomwidth, CHSV(hue2, 255, 255), SHORTEST_HUES);
    delay(15);
    FastLED.show();  //Update display
    //FastLED.showColor(CHSV(random8(255), random8(255), 255/dimmer)); //255,0,255/dimmer
    delay(random8(4,10));                 // each flash only lasts 4-10 milliseconds
    //FastLED.showColor(CHSV(255, 0, 0));
//fill_solid(leds+positionl, randomwidth, CHSV(255, 0, 0) );

fill_gradient (leds, positionl, CHSV(255, 0, 0),randomwidth, CHSV(255, 0, 0), SHORTEST_HUES);
    delay(15);
    
    FastLED.show();  //Update display
    if (flashCounter == 0) delay (150);   // longer delay until next flash after the leader
    delay(50+random8(100));               // shorter delay between strokes  
    if ((digitalRead(8)==0) || (digitalRead(9)==0) || (digitalRead(10)==0)|| (digitalRead(11)==0)|| (digitalRead(12)==0)) return;
  }
delay(random8(50)*100); // delay between strikes FREQUENCY
}
// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100 
#define COOLING  75 //55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 60 //120
void meteorShower(){
  //hue master
  hue++;  
 
  //populate the leds[] with stored ledsX[] array data
  for(byte i = 0; i < NUM_LEDS; i++ ) {
    ledsX[i][0] = leds[i].r;
    ledsX[i][1] = leds[i].g;
    ledsX[i][2] = leds[i].b;
  }
  //clear the previous counter clockwise position
  byte iCCW;
 
  //we are keeping track of elapsed time
  timeframe++;    //fx timer
 
  //meteorLENGTH fx is only shown for this time frame
  if((timeframe >= 1) && (timeframe <= 280)) { meteorLENGTH = 29; }
  if((timeframe > 280) && (timeframe <= 500)) { meteorLENGTH = 45; }
 
  //RAINBOWs fx add rainbow tails during this time frame only
  if((timeframe > 0) && (timeframe <= 280)) { RAINBOWs = true; }
  else{ RAINBOWs = false; }  
  //keep our RAINBOWs within a specific range of hue
  if(RAINBOWs == true){ hue = hue - 20; if(hue <= 0){  hue = 1; } }
 
  //RANDOMpick fx is only enabled during this timeframe
  if((timeframe > 600) && (timeframe <= 790)) { RANDOMpick = true; }
  else{ RANDOMpick = false; }
 
  //pick a random spot in the meteor switch case statement below
  if (RANDOMpick == true){ idex = random8(46); }
  else{
    //increment the meteor display frame
    idex++;
    //make sure we don't drift into space
    if (idex > meteorLENGTH) { idex = 0; } }  
 
  //meteorLENGTH is randomized during this timeframe only
  if((timeframe > 790) && (timeframe <= 1090)) { meteorLENGTH = random8(7, 38); }
 
  //during this point in the animation timeframe
  if(timeframe == 1180) {
    //reset the timeframe
    timeframe = 0;
    //increment the loop counter
    loopCount++;
  }
 
  //during this part of the loopCount, all meteors have a white colored tip
  if(loopCount == 1) { colorTIP = 0; }
  if(loopCount == 2) { colorTIP = 1; }
  if(loopCount == 3) { colorTIP = random8(11); }
  //end of the desired fx, reset the variable for the next time around
  if(loopCount == 4) {
    colorTIP = 0;
    loopCount = 0;
  }
 
  //there are two switch case statements nestled into one another
  //we always want to control the color of the meteor tip
  //the other controls the actual meteor animation in 45 frames/case statements
  switch (idex) {
  case 0:
    switch (colorTIP){
    case 0:
      leds[0] = CHSV(hue, 255, 255);
      break;
    case 1:
      leds[0] = CRGB(100,100,100);
      break;
    case 2:
      leds[0] = CRGB::Yellow;
      break;
    case 3:
      leds[0] = CRGB::Violet;
      break;
    case 4:
      leds[0] = CRGB::Green;
      break;
    case 5:
      leds[0] = CRGB::Purple;
      break;
    case 6:
      leds[0] = CRGB::Orange;
      break;
    case 7:
      leds[0] = CRGB::Cyan;
      break;
    case 8:
      leds[0] = CRGB::GreenYellow;
      break;
    case 9:
      leds[0] = CRGB::Magenta;
      break;
    case 10:
      leds[0] = CRGB::SkyBlue;
    }
    break;
  case 1:
    leds[0] = CHSV((hue - 20), 255, 210);
    break;
  case 2:
    leds[0] = CHSV((hue - 22), 255, 180);
    break;
  case 3:
    leds[0] = CHSV((hue - 23), 255, 150);
    break;
  case 4:
    leds[0] = CHSV((hue - 24), 255, 110);
    break;
  case 5:
    leds[0] = CHSV((hue - 25), 255, 90);
    break;
  case 6:
    leds[0] = CHSV((hue - 26), 160, 60);
    break;
  case 7:
    leds[0] = CHSV((hue - 27), 140, 40);
    break;
  case 8:
    leds[0] = CHSV((hue - 28), 120, 20);
    break;
  case 9:
    leds[0] = CHSV((hue - 29), 100, 20);
    break;
  case 10:
    leds[0] = CRGB::Black;
    break;
  case 11:
    leds[0] = CRGB::Black;
    break;
  case 12:
    leds[0] = CRGB::Black;
    break;
  case 13:
    leds[0] = CRGB::Black;
    break;
  case 14:
    leds[0] = CRGB::Black;
    break;
  case 15:
    leds[0] = CRGB::Black;
    break;
  case 16:
    leds[0] = CRGB::Black;
    break;
  case 17:
    leds[0] = CRGB::Black;
    break;
  case 18:
    leds[0] = CRGB::Black;
    break;
  case 19:
    leds[0] = CRGB::Black;
    break;
  case 20:
    leds[0] = CRGB::Black;
    break;
  case 21:
    leds[0] = CRGB::Black;
    break;
  case 22:
    leds[0] = CRGB::Black;
    break;
  case 23:
    leds[0] = CRGB::Black;
    break;
  case 24:
    leds[0] = CRGB::Black;
    break;
  case 25:
    leds[0] = CRGB::Black;
    break;
  case 26:
    leds[0] = CRGB::Black;
    break;
  case 27:
    leds[0] = CRGB::Black;
    break;
  case 28:
    leds[0] = CRGB::Black;
    break;
  case 29:
    leds[0] = CRGB::Black;
    break;
  case 30:
    leds[0] = CRGB::Black;
    break;
  case 31:
    leds[0] = CRGB::Black;
    break;
  case 32:
    leds[0] = CRGB::Black;
    break;
  case 33:
    leds[0] = CRGB::Black;
    break;
  case 34:
    leds[0] = CRGB::Black;
    break;
  case 35:
    leds[0] = CRGB::Black;
    break;
  case 36:
    leds[0] = CRGB::Black;
    break;
  case 37:
    leds[0] = CRGB::Black;
    break;
  case 38:
    leds[0] = CRGB::Black;
    break;
  case 39:
    leds[0] = CRGB::Black;
    break;
  case 40:
    leds[0] = CRGB::Black;
    break;
  case 41:
    leds[0] = CRGB::Black;
    break;
  case 42:
    leds[0] = CRGB::Black;
    break;
  case 43:
    leds[0] = CRGB::Black;
    break;
  case 44:
    leds[0] = CRGB::Black;
    break;
  case 45:
    leds[0] = CRGB::Black;
    break;
  }
  //copy the LED Array
  for(byte i = 1; i < NUM_LEDS; i++ ) {
    iCCW = adjacent_ccw(i);
    leds[i].r = ledsX[iCCW][0];
    leds[i].g = ledsX[iCCW][1];
    leds[i].b = ledsX[iCCW][2];
  }
 delay(30);

}

//find the adjacent counter clockwise postion of the led
//funkboxing code snippet
byte adjacent_ccw(byte i) {
  byte r;
  if (i > 0) { r = i - 1; }
  else { r = NUM_LEDS - 1; }
  return r;  
}

void balls() {
     leds[0].r =0;

   leds[0].g = 0;

   leds[0].b = 0;
int BallCount=5;
  byte colors[5][3] = { {0xff, 0,0},       // red

                        {0, 0xff, 0},      // green

                        {0, 0, 0xff},      // blue

                        {0xff, 0xff, 0xff},// white

                        {0xff, 0xff, 0} }; // yellow

  float Gravity = -9.81;

  int StartHeight = 1;

  

  float Height[BallCount];

  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );

  float ImpactVelocity[BallCount];

  float TimeSinceLastBounce[BallCount];

  int   Position[BallCount];

  long  ClockTimeSinceLastBounce[BallCount];

  float Dampening[BallCount];

  

  for (int i = 0 ; i < BallCount ; i++) {   

    ClockTimeSinceLastBounce[i] = millis();

    Height[i] = StartHeight;

    Position[i] = 0; 

    ImpactVelocity[i] = ImpactVelocityStart;

    TimeSinceLastBounce[i] = 0;

    Dampening[i] = 0.90 - float(i)/pow(BallCount,2); 

  }

long ballscounter=0;

  while (ballscounter<7000) {
if ((digitalRead(8)==0) || (digitalRead(9)==0) || (digitalRead(10)==0)|| (digitalRead(11)==0)|| (digitalRead(12)==0)) break;
    for (int i = 0 ; i < BallCount ; i++) {

      TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];

      Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;

  

      if ( Height[i] < 0 ) {                      

        Height[i] = 0;

        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];

        ClockTimeSinceLastBounce[i] = millis();

  

        if ( ImpactVelocity[i] < 0.01 ) {

          ImpactVelocity[i] = ImpactVelocityStart;

        }

      }

      Position[i] = round( Height[i] * (NUM_LEDS - 1) / StartHeight);

    }

  

    for (int i = 0 ; i < BallCount ; i++) {

      setPixel(Position[i],colors[i][0],colors[i][1],colors[i][2]);

    }

    

    showStrip();

    setAll(0,0,0);
ballscounter++;
  }

}

void showStrip() {

 #ifdef ADAFRUIT_NEOPIXEL_H

   // NeoPixel

   strip.show();

 #endif

 #ifndef ADAFRUIT_NEOPIXEL_H

   // FastLED

   FastLED.show();

 #endif

}



void setPixel(int Pixel, byte red, byte green, byte blue) {

 #ifdef ADAFRUIT_NEOPIXEL_H

   // NeoPixel

   strip.setPixelColor(NUM_LEDS-Pixel, strip.Color(red, green, blue));

 #endif

 #ifndef ADAFRUIT_NEOPIXEL_H

   // FastLED

   leds[NUM_LEDS-Pixel].r = red;

   leds[NUM_LEDS-Pixel].g = green;

   leds[NUM_LEDS-Pixel].b = blue;

 #endif

}



void setAll(byte red, byte green, byte blue) {

  for(int i = 0; i < NUM_LEDS+1; i++ ) {

    setPixel(i, red, green, blue); 

  }

  showStrip();

}

void SnowSparkle() {

byte red=0x00;
byte green=0x00;
byte blue=0x10;
int SparkleDelay=20;
int SpeedDelay=random(100,1000);

  setAll(red,green,blue);

  

  int Pixel = random(NUM_LEDS+1);

  setPixel(Pixel,0xaa,0xaa,0xff);

  showStrip();

  delay(SparkleDelay);

  setPixel(Pixel,red,green,blue);

  showStrip();

  delay(SpeedDelay);

}

void Fire2012WithPalette()
{

// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      int pixelnumber;
      if( true) { //reverse
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}
// =============================================================================
//   loop routine 
// =============================================================================
// put your main code here, to run repeatedly:
void loop() {
   // Check if new IR protocol data is available
  //  if (irrecv.decode(&results)) {
  //  Serial.println(results.value, HEX);
  //  irrecv.resume(); // Receive the next value
  //}
  /*
  if (IRLremote.available())
  {
    // Light Led
    digitalWrite(pinLed, HIGH);

    // Get the new data from the remote
    auto data = IRLremote.read();
// Serial.print(F("Protocol:"));
    //Serial.println(data.protocol);
    // Print the protocol data
    Serial.print(F("Address: 0x"));
    Serial.println(data.address, HEX);
    Serial.print(F("Command: 0x"));
    Serial.println(data.command, HEX);
    Serial.println();
  if ((data.address==0xF708)&&(data.command==0x0a))
  IRcommand="switch";
  
    if ((data.address==0xF708)&&(data.command==0x14))
  IRcommand="next";
  if ((data.address==0xF708)&&(data.command==0x18))
  IRcommand="home";
    // Turn Led off after printing the data
    digitalWrite(pinLed, LOW);
  }
  if ( digitalRead(9)==0)
  IRcommand="prev";
   if ( digitalRead(10)==0)
  IRcommand="next";
   if ( digitalRead(11)==0)
  IRcommand="home";*/
  /*
if (irrecv.decode(&results)) {//if a new command is received from the IR sensor
    
    command = results.value; //store in command variable
    //Serial.println(results.value, HEX); //print result in hex to serial port
    Serial.println(command); //print result to serial port - use to figure out what commands are being sent by your remote
    
 
    //ChangePalettePeriodically();
    //store last command for repetition in case button is held
    if (command != 4294967295){//when button is held on remote, this value is received. If so, repeat the previous command until release.
      lastcommand = command;
    }else{
      command = lastcommand;
}
}*/
    // brightness control 
/*
  potValA = analogRead(BRIGHTNESS_PIN);  // Read potentiometer B (for brightness).
  //Serial.println (potValA);
  brightnessa = map(potValA, 0, 1023, MIN_BRIGHTNESS, MASTER_BRIGHTNESS);
      // Map value between min_brightness and MASTER brightness values.
      // Note: We are limiting the lowest possible brightness value to the
      // min_brightness value assigned up top.
  FastLED.setBrightness(brightnessa);       
  //Serial.println (brightnessa);
*/
/*
if(RE(digitalRead(nextbtn), nextbtnState)){
{ gCurrentPatternNumber++;
if (gCurrentPatternNumber>ARRAY_SIZE(gPatterns)) gCurrentPatternNumber=0;
autorunl=false;
  }
if(RE(digitalRead(prevbtn), prevbtnState)){)
{ gCurrentPatternNumber--;
if (gCurrentPatternNumber<0) gCurrentPatternNumber=ARRAY_SIZE(gPatterns)-1;
autorunl=false;
  }
  if (RE(digitalRead(homebtn), homebtnState){)
{ 
autorunl=true;
  }*/
  // for Twinkle
  // Call the current pattern function once, updating the 'leds' array
    potValA = analogRead(BRIGHTNESS_PIN);  // Read potentiometer B (for brightness).
    if (potValA>990)
    {
  gPatterns[gCurrentPatternNumber]();

  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 


  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  if (autorunl)
  {
  EVERY_N_SECONDS( 25 ) { nextPattern(); } // change patterns periodically
  }
  // for marque_v3
  EVERY_N_SECONDS(7){  // Demo: Change the hue every N seconds.
    hue = hue + random8(30,61);  // Shift the hue to something new.
  }

  EVERY_N_SECONDS( SECONDS_PER_PALETTE ) { 
    chooseNextColorPalette( gTargetPalette ); 
  }
  
  EVERY_N_MILLISECONDS( 10 ) {
    nblendPaletteTowardPalette( gCurrentPalette, gTargetPalette, 12);
  }

//if (!IRLremote.isreceiving()) {
  // send the 'leds' array out to the actual LED strip
  EVERY_N_MILLISECONDS( 40 ) {
  FastLED.show();  
  
}
    }
    else
    {
      
      FastLED.clear();
      FastLED.show();
    }
 prevreadbutton();  // check for button press
  nextreadbutton();  // check for button press
   homereadbutton();  // check for button press
     brithupreadbutton();  // check for button press
   brithdownreadbutton();  // check for button press
}



// =============================================================================
//  Sub-Routines
// =============================================================================


void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
   FastLED.clear();
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}


void fill_solid_color() 
{ 
  
  uint8_t hue1 = random8(255);
  uint8_t hue2 = random8(255);
   
  for (int i=0; i<NUM_LEDS; i++) {
/*
    Serial.println (i);
    leds[i] = CRGB::Red;
    leds[i-1] = CRGB::Blue;
    FastLED.show();
    leds[i] = CRGB::Black;
*/
    leds[i] = CHSV( hue1, 255, 255);
    leds[i-1] = CHSV( hue2, 255, 255);
    //FastLED.show();
    leds[i] = CRGB::Black;

    delay(30);    
  }
}

void fill_black()
{
  FastLED.clear();
  //fill_solid(leds, NUM_LEDS, CRGB::Black);
}


void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}


void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}


void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}


// draws a line that fades between 2 random colors
// TODO:  Add logic to rotate the starting point
void gradient_fill() {

//  uint8_t hue1 = 60;
//  uint8_t hue2 = random8(255);
  uint8_t hue1 = random8(255);
  uint8_t hue2 = hue1 + random8(30,61);
  
  for( int i = 0; i < NUM_LEDS; i++){
    //fill_gradient (leds, 0, CHSV(0, 255, 255), i, CHSV(96, 255, 255), SHORTEST_HUES);
    fill_gradient (leds, 0, CHSV(hue1, 255, 255), i, CHSV(hue2, 255, 255), SHORTEST_HUES);
    delay(15);
    FastLED.show();
  //  FastLED.clear();
  }
}

  DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
  0,     0,  0,  0,   //black
128,   0,  255,  0,   //red
102,   255,255,  0,   //bright yellow
255,   255,255,255 }; //full white

void fire()
{
  //gPal = HeatColors_p;
CRGBPalette16 myPal = heatmap_gp;
gPal = myPal;
hue=0;
   //gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
  Fire2012WithPalette(); // run simulation frame, using palette colors
}

// Adapted from code by Marc Miller.  Original Header:
//
//***************************************************************
// Marquee fun (v3)
//  Pixel position down the strip comes from this formula:
//      pos = spacing * (i-1) + spacing
//  i starts at 0 and is incremented by +1 up to NUM_LEDS/spacing.
//
// Marc Miller, May 2016
//***************************************************************
void marque_v3() {

    
  for (uint8_t i=0; i<(NUM_LEDS/spacing); i++){
    for (uint8_t w = 0; w<width; w++){
      pos = (spacing * (i-1) + spacing + advance + w) % NUM_LEDS;
      if ( w % 2== 0 ){  // Is w even or odd?
        color = hue;
      } else {
        color = hue + hue2_shift;
      }
      
      leds[pos] = CHSV(color,255,255);
      if ((digitalRead(8)==0) || (digitalRead(9)==0) || (digitalRead(10)==0)|| (digitalRead(11)==0)|| (digitalRead(12)==0)) return;
    }
    
    if (DEBUG==1) {  // Print out lit pixels if DEBUG is true.
      Serial.print(" "); Serial.print(pos);
    }
    delay(10);
  }
  if (DEBUG==1) { Serial.println(" "); }
  FastLED.show();
  
  // Fade out tail or set back to black for next loop around.
  if (fadingTail == 1) {
    fadeToBlackBy(leds, NUM_LEDS,fadeRate);
  } else {
    for (uint8_t i=0; i<(NUM_LEDS/spacing); i++){
      for (uint8_t w = 0; w<width; w++){
        pos = (spacing * (i-1) + spacing + advance + w) % NUM_LEDS;
        leds[pos] = CRGB::Black;
        if ((digitalRead(8)==0) || (digitalRead(9)==0) || (digitalRead(10)==0)|| (digitalRead(11)==0)|| (digitalRead(12)==0)) return;
      }
    }
  }

  // Advance pixel postion down strip, and rollover if needed.
  advance = (advance + delta + NUM_LEDS) % NUM_LEDS;
}





// ===================
// Adopted from Mark Kriegsman's Twinkle-Lights 2015
// G+:    https://plus.google.com/u/0/112916219338292742137/posts/XWiEEeDxFWZ?sfc=true
// CODE:  https://gist.github.com/kriegsman/756ea6dcae8e30845b5a

//  TwinkleFOX: Twinkling 'holiday' lights that fade in and out.
//  Colors are chosen from a palette; a few palettes are provided.
//
//  This December 2015 implementation improves on the December 2014 version
//  in several ways:
//  - smoother fading, compatible with any colors and any palettes
//  - easier control of twinkle speed and twinkle density
//  - supports an optional 'background color'
//  - takes even less RAM: zero RAM overhead per pixel
//  - illustrates a couple of interesting techniques (uh oh...)
//
//  The idea behind this (new) implementation is that there's one
//  basic, repeating pattern that each pixel follows like a waveform:
//  The brightness rises from 0..255 and then falls back down to 0.
//  The brightness at any given point in time can be determined as
//  as a function of time, for example:
//    brightness = sine( time ); // a sine wave of brightness over time
//
//  So the way this implementation works is that every pixel follows
//  the exact same wave function over time.  In this particular case,
//  I chose a sawtooth triangle wave (triwave8) rather than a sine wave,
//  but the idea is the same: brightness = triwave8( time ).  
//  
//  Of course, if all the pixels used the exact same wave form, and 
//  if they all used the exact same 'clock' for their 'time base', all
//  the pixels would brighten and dim at once -- which does not look
//  like twinkling at all.
//
//  So to achieve random-looking twinkling, each pixel is given a 
//  slightly different 'clock' signal.  Some of the clocks run faster, 
//  some run slower, and each 'clock' also has a random offset from zero.
//  The net result is that the 'clocks' for all the pixels are always out 
//  of sync from each other, producing a nice random distribution
//  of twinkles.
//
//  The 'clock speed adjustment' and 'time offset' for each pixel
//  are generated randomly.  One (normal) approach to implementing that
//  would be to randomly generate the clock parameters for each pixel 
//  at startup, and store them in some arrays.  However, that consumes
//  a great deal of precious RAM, and it turns out to be totally
//  unnessary!  If the random number generate is 'seeded' with the
//  same starting value every time, it will generate the same sequence
//  of values every time.  So the clock adjustment parameters for each
//  pixel are 'stored' in a pseudo-random number generator!  The PRNG 
//  is reset, and then the first numbers out of it are the clock 
//  adjustment parameters for the first pixel, the second numbers out
//  of it are the parameters for the second pixel, and so on.
//  In this way, we can 'store' a stable sequence of thousands of
//  random clock adjustment parameters in literally two bytes of RAM.
//
//  There's a little bit of fixed-point math involved in applying the
//  clock speed adjustments, which are expressed in eighths.  Each pixel's
//  clock speed ranges from 8/8ths of the system clock (i.e. 1x) to
//  23/8ths of the system clock (i.e. nearly 3x).
//
//  On a basic Arduino Uno or Leonardo, this code can twinkle 300+ pixels
//  smoothly at over 50 updates per seond.
//
//  -Mark Kriegsman, December 2015

//  This function loops over each pixel, calculates the 
//  adjusted 'clock' that this pixel should use, and calls 
//  "CalculateOneTwinkle" on each pixel.  It then displays
//  either the twinkle color of the background color, 
//  whichever is brighter.

// Overall twinkle speed.
// 0 (VERY slow) to 8 (VERY fast).  
// 4, 5, and 6 are recommended, default is 4.
#define TWINKLE_SPEED 6 // 4

// Overall twinkle density.
// 0 (NONE lit) to 8 (ALL lit at once).  
// Default is 5.
#define TWINKLE_DENSITY 5


// Background color for 'unlit' pixels
// Can be set to CRGB::Black if desired.
CRGB gBackgroundColor = CRGB::Black; 
// Example of dim incandescent fairy light background color
//CRGB gBackgroundColor = CRGB(CRGB::FairyLight).nscale8_video(16);

// If AUTO_SELECT_BACKGROUND_COLOR is set to 1,
// then for any palette where the first two entries 
// are the same, a dimmed version of that color will
// automatically be used as the background color.
#define AUTO_SELECT_BACKGROUND_COLOR 0

// If COOL_LIKE_INCANDESCENT is set to 1, colors will 
// fade out slighted 'reddened', similar to how
// incandescent bulbs change color as they get dim down.
#define COOL_LIKE_INCANDESCENT 1



void drawTwinkles()
{
  // "PRNG16" is the pseudorandom number generator
  // It MUST be reset to the same starting value each time
  // this function is called, so that the sequence of 'random'
  // numbers that it generates is (paradoxically) stable.
  uint16_t PRNG16 = 11337;
  
  uint32_t clock32 = millis();

  // Set up the background color, "bg".
  // if AUTO_SELECT_BACKGROUND_COLOR == 1, and the first two colors of
  // the current palette are identical, then a deeply faded version of
  // that color is used for the background color
  CRGB bg;
  if( (AUTO_SELECT_BACKGROUND_COLOR == 1) &&
      (gCurrentPalette[0] == gCurrentPalette[1] )) {
    bg = gCurrentPalette[0];
    uint8_t bglight = bg.getAverageLight();
    if( bglight > 64) {
      bg.nscale8_video( 16); // very bright, so scale to 1/16th
    } else if( bglight > 16) {
      bg.nscale8_video( 64); // not that bright, so scale to 1/4th
    } else {
      bg.nscale8_video( 86); // dim, scale to 1/3rd.
    }
  } else {
    bg = gBackgroundColor; // just use the explicitly defined background color
  }

  uint8_t backgroundBrightness = bg.getAverageLight();
  
  for( CRGB& pixel: leds) {
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    uint16_t myclockoffset16= PRNG16; // use that number as clock offset
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    // use that number as clock speed adjustment factor (in 8ths, from 8/8ths to 23/8ths)
    uint8_t myspeedmultiplierQ5_3 =  ((((PRNG16 & 0xFF)>>4) + (PRNG16 & 0x0F)) & 0x0F) + 0x08;
    uint32_t myclock30 = (uint32_t)((clock32 * myspeedmultiplierQ5_3) >> 3) + myclockoffset16;
    uint8_t  myunique8 = PRNG16 >> 8; // get 'salt' value for this pixel

    // We now have the adjusted 'clock' for this pixel, now we call
    // the function that computes what color the pixel should be based
    // on the "brightness = f( time )" idea.
    CRGB c = computeOneTwinkle( myclock30, myunique8);

    uint8_t cbright = c.getAverageLight();
    int16_t deltabright = cbright - backgroundBrightness;
    if( deltabright >= 32 || (!bg)) {
      // If the new pixel is significantly brighter than the background color, 
      // use the new color.
      pixel = c;
    } else if( deltabright > 0 ) {
      // If the new pixel is just slightly brighter than the background color,
      // mix a blend of the new color and the background color
      pixel = blend( bg, c, deltabright * 8);
    } else { 
      // if the new pixel is not at all brighter than the background color,
      // just use the background color.
      pixel = bg;
    }
  }
}


void ripple2() {
 
    if (currentBg == nextBg) {
      nextBg = random(256);
    }
    else if (nextBg > currentBg) {
      currentBg++;
    } else {
      currentBg--;
    }
    for(uint16_t l = 0; l < NUM_LEDS; l++) {
      leds[l] = CHSV(currentBg, 255, 50);         // strip.setPixelColor(l, Wheel(currentBg, 0.1));
    }
 
  if (step == -1) {
    center = random(NUM_LEDS);
    color2 = random(256);
    step = 0;
  }
 
  if (step == 0) {
    leds[center] = CHSV(color2, 255, 255);         // strip.setPixelColor(center, Wheel(color, 1));
    step ++;
  }
  else {
    if (step < maxSteps) {
      //Serial.println(pow(fadeRate2,step));
 
      leds[wrap(center + step)] = CHSV(color2, 255, pow(fadeRate2, step)*255);       //   strip.setPixelColor(wrap(center + step), Wheel(color, pow(fadeRate, step)));
      leds[wrap(center - step)] = CHSV(color2, 255, pow(fadeRate2, step)*255);       //   strip.setPixelColor(wrap(center - step), Wheel(color, pow(fadeRate, step)));
      if (step > 3) {
        leds[wrap(center + step - 3)] = CHSV(color2, 255, pow(fadeRate2, step - 2)*255);     //   strip.setPixelColor(wrap(center + step - 3), Wheel(color, pow(fadeRate, step - 2)));
        leds[wrap(center - step + 3)] = CHSV(color2, 255, pow(fadeRate2, step - 2)*255);     //   strip.setPixelColor(wrap(center - step + 3), Wheel(color, pow(fadeRate, step - 2)));
      }
      step ++;
    }
    else {
      step = -1;
    }
  }
 
  LEDS.show();
  delay(50);
}
 
 
int wrap(int step) {
  if(step < 0) return NUM_LEDS + step;
  if(step > NUM_LEDS - 1) return step - NUM_LEDS;
  return step;
}
 
 
void one_color_allHSV(int ahue, int abright) {                // SET ALL LEDS TO ONE COLOR (HSV)
  for (int i = 0 ; i < NUM_LEDS; i++ ) {
    leds[i] = CHSV(ahue, 255, abright);
  }
}

//  This function takes a time in pseudo-milliseconds,
//  figures out brightness = f( time ), and also hue = f( time )
//  The 'low digits' of the millisecond time are used as 
//  input to the brightness wave function.  
//  The 'high digits' are used to select a color, so that the color
//  does not change over the course of the fade-in, fade-out
//  of one cycle of the brightness wave function.
//  The 'high digits' are also used to determine whether this pixel
//  should light at all during this cycle, based on the TWINKLE_DENSITY.
CRGB computeOneTwinkle( uint32_t ms, uint8_t salt)
{
  uint16_t ticks = ms >> (8-TWINKLE_SPEED);
  uint8_t fastcycle8 = ticks;
  uint16_t slowcycle16 = (ticks >> 8) + salt;
  slowcycle16 += sin8( slowcycle16);
  slowcycle16 =  (slowcycle16 * 2053) + 1384;
  uint8_t slowcycle8 = (slowcycle16 & 0xFF) + (slowcycle16 >> 8);
  
  uint8_t bright = 0;
  if( ((slowcycle8 & 0x0E)/2) < TWINKLE_DENSITY) {
    bright = attackDecayWave8( fastcycle8);
  }

  uint8_t hue = slowcycle8 - salt;
  CRGB c;
  if( bright > 0) {
    c = ColorFromPalette( gCurrentPalette, hue, bright, NOBLEND);
    if( COOL_LIKE_INCANDESCENT == 1 ) {
      coolLikeIncandescent( c, fastcycle8);
    }
  } else {
    c = CRGB::Black;
  }
  return c;
}


// This function is like 'triwave8', which produces a 
// symmetrical up-and-down triangle sawtooth waveform, except that this
// function produces a triangle wave with a faster attack and a slower decay:
//
//     / \ 
//    /     \ 
//   /         \ 
//  /             \ 
//

uint8_t attackDecayWave8( uint8_t i)
{
  if( i < 86) {
    return i * 3;
  } else {
    i -= 86;
    return 255 - (i + (i/2));
  }
}

// This function takes a pixel, and if its in the 'fading down'
// part of the cycle, it adjusts the color a little bit like the 
// way that incandescent bulbs fade toward 'red' as they dim.
void coolLikeIncandescent( CRGB& c, uint8_t phase)
{
  if( phase < 128) return;

  uint8_t cooling = (phase - 128) >> 4;
  c.g = qsub8( c.g, cooling);
  c.b = qsub8( c.b, cooling * 2);
}

// A mostly red palette with green accents and white trim.
// "CRGB::Gray" is used as white to keep the brightness more uniform.
const TProgmemRGBPalette16 RedGreenWhite_p FL_PROGMEM =
{  CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Red, 
   CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Red, 
   CRGB::Red, CRGB::Red, CRGB::Gray, CRGB::Gray, 
   CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Green };

// A mostly (dark) green palette with red berries.
#define Holly_Green 0x00580c
#define Holly_Red   0xB00402
const TProgmemRGBPalette16 Holly_p FL_PROGMEM =
{  Holly_Green, Holly_Green, Holly_Green, Holly_Green, 
   Holly_Green, Holly_Green, Holly_Green, Holly_Green, 
   Holly_Green, Holly_Green, Holly_Green, Holly_Green, 
   Holly_Green, Holly_Green, Holly_Green, Holly_Red 
};

// A red and white striped palette
// "CRGB::Gray" is used as white to keep the brightness more uniform.
const TProgmemRGBPalette16 RedWhite_p FL_PROGMEM =
{  CRGB::Red,  CRGB::Red,  CRGB::Red,  CRGB::Red, 
   CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::Gray,
   CRGB::Red,  CRGB::Red,  CRGB::Red,  CRGB::Red, 
   CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::Gray };

// A mostly blue palette with white accents.
// "CRGB::Gray" is used as white to keep the brightness more uniform.
const TProgmemRGBPalette16 BlueWhite_p FL_PROGMEM =
{  CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Blue, 
   CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Blue, 
   CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Blue, 
   CRGB::Blue, CRGB::Gray, CRGB::Gray, CRGB::Gray };

// A pure "fairy light" palette with some brightness variations
#define HALFFAIRY ((CRGB::FairyLight & 0xFEFEFE) / 2)
#define QUARTERFAIRY ((CRGB::FairyLight & 0xFCFCFC) / 4)
const TProgmemRGBPalette16 FairyLight_p FL_PROGMEM =
{  CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, 
   HALFFAIRY,        HALFFAIRY,        CRGB::FairyLight, CRGB::FairyLight, 
   QUARTERFAIRY,     QUARTERFAIRY,     CRGB::FairyLight, CRGB::FairyLight, 
   CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight };

// A palette of soft snowflakes with the occasional bright one
const TProgmemRGBPalette16 Snow_p FL_PROGMEM =
{  0x304048, 0x304048, 0x304048, 0x304048,
   0x304048, 0x304048, 0x304048, 0x304048,
   0x304048, 0x304048, 0x304048, 0x304048,
   0x304048, 0x304048, 0x304048, 0xE0F0FF };

// A palette reminiscent of large 'old-school' C9-size tree lights
// in the five classic colors: red, orange, green, blue, and white.
#define C9_Red    0xB80400
#define C9_Orange 0x902C02
#define C9_Green  0x046002
#define C9_Blue   0x070758
#define C9_White  0x606820
const TProgmemRGBPalette16 RetroC9_p FL_PROGMEM =
{  C9_Red,    C9_Orange, C9_Red,    C9_Orange,
   C9_Orange, C9_Red,    C9_Orange, C9_Red,
   C9_Green,  C9_Green,  C9_Green,  C9_Green,
   C9_Blue,   C9_Blue,   C9_Blue,
   C9_White
};

// A cold, icy pale blue palette
#define Ice_Blue1 0x0C1040
#define Ice_Blue2 0x182080
#define Ice_Blue3 0x5080C0
const TProgmemRGBPalette16 Ice_p FL_PROGMEM =
{
  Ice_Blue1, Ice_Blue1, Ice_Blue1, Ice_Blue1,
  Ice_Blue1, Ice_Blue1, Ice_Blue1, Ice_Blue1,
  Ice_Blue1, Ice_Blue1, Ice_Blue1, Ice_Blue1,
  Ice_Blue2, Ice_Blue2, Ice_Blue2, Ice_Blue3
};


// Add or remove palette names from this list to control which color
// palettes are used, and in what order.
const TProgmemRGBPalette16* ActivePaletteList[] = {
  &RetroC9_p,
  &BlueWhite_p,
  &RainbowColors_p,
  &FairyLight_p,
  &RedGreenWhite_p,
  &PartyColors_p,
  &RedWhite_p,
  &Snow_p,
  &Holly_p,
  &Ice_p  
};


// Advance to the next color palette in the list (above).
void chooseNextColorPalette( CRGBPalette16& pal)
{
  const uint8_t numberOfPalettes = sizeof(ActivePaletteList) / sizeof(ActivePaletteList[0]);
  static uint8_t whichPalette = -1; 
  whichPalette = addmod8( whichPalette, 1, numberOfPalettes);

  pal = *(ActivePaletteList[whichPalette]);
}

void Sparkle() {
  const int SpeedDelay=0;
byte red=random(255);
byte green= random(255);
byte blue=random(255);
  int Pixel = random(NUM_LEDS+1);
 FastLED.clear();
 
  setPixel(Pixel,red,green,blue);

  showStrip();

  delay(SpeedDelay);

  setPixel(Pixel,0,0,0);

}

byte * Wheel(byte WheelPos) {

  static byte c[3];

  

  if(WheelPos < 85) {

   c[0]=WheelPos * 3;

   c[1]=255 - WheelPos * 3;

   c[2]=0;

  } else if(WheelPos < 170) {

   WheelPos -= 85;

   c[0]=255 - WheelPos * 3;

   c[1]=0;

   c[2]=WheelPos * 3;

  } else {

   WheelPos -= 170;

   c[0]=0;

   c[1]=WheelPos * 3;

   c[2]=255 - WheelPos * 3;

  }



  return c;

}

void rainbowCycle() {
const int SpeedDelay=20;
  byte *c;

  uint16_t i, j;
 FastLED.clear();


  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel

    for(i=0; i< NUM_LEDS+1; i++) {

      c=Wheel(((i * 256 / NUM_LEDS) + j) & 255);

      setPixel(i, *c, *(c+1), *(c+2));
      if ((digitalRead(8)==0) || (digitalRead(9)==0) || (digitalRead(10)==0)|| (digitalRead(11)==0)|| (digitalRead(12)==0)) return;

    }

    showStrip();

    delay(SpeedDelay);

  }

}

void theaterChaseRainbow() {
const int SpeedDelay=60;
  byte *c;

   FastLED.clear();

  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel

    for (int q=0; q < 3; q++) {

        for (int i=0; i < NUM_LEDS; i=i+3) {

          c = Wheel( (i+j) % 255);

          setPixel(i+q, *c, *(c+1), *(c+2));    //turn every third pixel on
          //if ((i+q==NUM_LEDS-1)&(q==2)) 
          //{
           setPixel(NUM_LEDS, leds[3].r,leds[3].g,leds[3].b);    //turn last pixel on i+1+q
            
          //}
          
        if ((digitalRead(8)==0) || (digitalRead(9)==0) || (digitalRead(10)==0)|| (digitalRead(11)==0)|| (digitalRead(12)==0)) return;

       //

        }

        showStrip();

       

        delay(SpeedDelay);

       

        for (int i=0; i < NUM_LEDS; i=i+3) {

          setPixel(i+q, 0,0,0);        //turn every third pixel off

        }

    }

  }

}

void alternateColor() {
   FastLED.clear();
 byte red=random(255);
byte green= random(255);
byte blue=random(255);
 byte red2=random(255);
byte green2= random(255);
byte blue2=random(255);
 uint8_t wait=60;
  for(uint16_t i=0; i<NUM_LEDS+1; i++) {
    if ((digitalRead(8)==0) || (digitalRead(9)==0) || (digitalRead(10)==0)|| (digitalRead(11)==0)|| (digitalRead(12)==0)) return;
    if(i%2 == 0) { // set even LED to color 1
      setPixel(i,red,green,blue);
    } else { // set odd LED to color 2
      setPixel(i,red2,green2,blue2);
    }
     if ((digitalRead(8)==0) || (digitalRead(9)==0) || (digitalRead(10)==0)|| (digitalRead(11)==0)|| (digitalRead(12)==0)) return;
  }

  FastLED.show(); // apply the colors
  delay(wait);

  for(uint16_t i=0; i<NUM_LEDS+1; i++) {
    if(i%2 == 0) { // set even LED to color 2
       setPixel(i,red,green,blue);
    } else { // set odd LED to color 1
       setPixel(i,red2,green2,blue2);
    }
     if ((digitalRead(8)==0) || (digitalRead(9)==0) || (digitalRead(10)==0)|| (digitalRead(11)==0)|| (digitalRead(12)==0)) return;
  }

  FastLED.show(); // apply the colors
  delay(wait);
}


void randomPositionFill() {
   byte red=random(255);
byte green= random(255);
byte blue=random(255);
  uint8_t wait=60;//20
  FastLED.clear();
 
  int used[NUM_LEDS]; // array to keep track of lit LEDs
  int lights = 0; // counter
 
  for(int i = 0; i<NUM_LEDS; i++){ // fill array with 0
    used[i] = 0;
  }
 
  while(lights<NUM_LEDS) { //-1
    int j = random(0,NUM_LEDS+1); // pick a random LED
    if(used[j] != 1){ // if LED not already lit, proceed
       setPixel(j,red,green,blue);
      used[j] = 1; // update array to remember it is lit
      lights++;
        FastLED.show(); // apply the colors
        if ((digitalRead(8)==0) || (digitalRead(9)==0) || (digitalRead(10)==0)|| (digitalRead(11)==0)|| (digitalRead(12)==0)) return;
      delay(wait);
    }
  }
}

void stars()
{
   FastLED.clear();
   if (random(20) == 1) {
      uint16_t i = random(NUM_LEDS);
      if (ledsX[i][0] < 1 && ledsX[i][1] < 1 && ledsX[i][2] < 1) {
        ledsX[i][0] = random(256);
        ledsX[i][1] = random(256);
        ledsX[i][2] = random(256);
      }
    }
    
    for(uint16_t l = 0; l < NUM_LEDS; l++) {
      if ((digitalRead(8)==0) || (digitalRead(9)==0) || (digitalRead(10)==0)|| (digitalRead(11)==0)|| (digitalRead(12)==0)) return;
      if (ledsX[l][0] > 1 || ledsX[l][1] > 1 || ledsX[l][2] > 1) {
        setPixel(l, ledsX[l][0], ledsX[l][1], ledsX[l][2]);
        
        if (ledsX[l][0] > 1) {
          ledsX[l][0] = ledsX[l][0] * fadeRate;
        } else {
          ledsX[l][0] = 0;
        }
        
        if (ledsX[l][1] > 1) {
          ledsX[l][1] = ledsX[l][1] * fadeRate;
        } else {
          ledsX[l][1] = 0;
        }
        
        if (ledsX[l][2] > 1) {
          ledsX[l][2] = ledsX[l][2] * fadeRate;
        } else {
          ledsX[l][2] = 0;
        }
        
      } else {
        setPixel(l, 0, 0, 0);
      }
    }
    FastLED.show(); // apply the colors
delay(30);
}

void randomColorFill() {
  uint8_t wait=60;//10
 FastLED.clear();
 
  for(uint16_t i=0; i<NUM_LEDS+1; i++) { // iterate over every LED of the strip
    byte r = random(0,255); // generate a random color
    byte g = random(0,255);
    byte b = random(0,255);
 
    for(uint16_t j=1; j<NUM_LEDS-i+1; j++) { // iterate over every LED of the strip, that hasn't lit up yet j=0
      setPixel(j-1,0,0,0);
      setPixel(j,r,g,b);
     // dstrip.setPixelColor(j-1, dstrip.Color(0, 0, 0)); // turn previous LED off
     // dstrip.setPixelColor(j, dstrip.Color(r, g, b)); // turn current LED on
      FastLED.show(); // apply the colors
      if ((digitalRead(8)==0) || (digitalRead(9)==0) || (digitalRead(10)==0)|| (digitalRead(11)==0)|| (digitalRead(12)==0)) return;
      delay(wait);
    }
  }
}
// END
