// Assorted useful functions and variables

// Global variables
boolean effectInit = false; // indicates if a pattern has been recently switched
uint16_t effectDelay = 0; // time between automatic effect changes
unsigned long effectMillis = 0; // store the time of last effect function run
unsigned long cycleMillis = 0; // store the time of last effect change
unsigned long currentMillis; // store current loop's millis value
unsigned long hueMillis; // store time of last hue change
unsigned long eepromMillis; // store time of last setting change
unsigned long audioMillis; // store time of last audio update
byte currentEffect = 0; // index to the currently running effect
boolean autoCycle = true; // flag for automatic effect changes
boolean eepromOutdated = false; // flag for when EEPROM may need to be updated
byte currentBrightness = STARTBRIGHTNESS; // 0-255 will be scaled to 0-MAXBRIGHTNESS
boolean audioEnabled = true; // flag for running audio patterns
boolean audioActive = false;
uint8_t fadeActive = 0;

CRGBPalette16 currentPalette(RainbowColors_p); // global palette storage

typedef void (*functionList)(); // definition for list of effect function pointers
extern byte numEffects;

#define HOLD_PALETTES_X_TIMES_AS_LONG 2
#define MAX_DIMENSION ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)
 
uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];
uint16_t scale = 72;
uint8_t colorLoop = 1;
// The 16 bit version of our coordinates
static uint16_t x;
static uint16_t y;
static uint16_t z;

static uint16_t nx;
static uint16_t ny;
static uint16_t nz;
uint16_t nspeed = 0;

// Increment the global hue value for functions that use it
byte cycleHue = 0;
byte cycleHueCount = 0;
void hueCycle(byte incr) {
    cycleHueCount = 0;
    cycleHue+=incr;
}

// Set every LED in the array to a specified color
void fillAll(CRGB fillColor) {
  for (byte i = 0; i < NUM_LEDS; i++) {
    leds[i] = fillColor;
  }
}

// Fade every LED in the array by a specified amount
void fadeAll(byte fadeIncr) {
  for (byte i = 0; i < NUM_LEDS; i++) {
    leds[i] = leds[i].fadeToBlackBy(fadeIncr);
  }
}

// Shift all pixels by one, right or left (0 or 1)
void scrollArray(byte scrollDir) {
  
    byte scrollX = 0;
    for (byte x = 1; x < kMatrixWidth; x++) {
      if (scrollDir == 0) {
        scrollX = kMatrixWidth - x;
      } else if (scrollDir == 1) {
        scrollX = x - 1;
      }
      
      for (byte y = 0; y < kMatrixHeight; y++) {
        leds[XY(scrollX,y)] = leds[XY(scrollX + scrollDir*2 - 1,y)];
      }
    }
  
}

void SetupBlackAndWhiteStripedPalette() {
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette() {
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;
  
  currentPalette = CRGBPalette16( 
    green,  green,  black,  black,
    purple, purple, black,  black,
    green,  green,  black,  black,
    purple, purple, black,  black );
}

// Pick a random palette from a list
void selectRandomPalette() {

  switch(random8(49)) {
    case 0:
    currentPalette = CloudColors_p;
    break;
    
    case 1:
    currentPalette = LavaColors_p;
    break;
    
    case 2:
    currentPalette = OceanColors_p;
    break;
    
    case 4:
    currentPalette = ForestColors_p;
    break;
    
    case 5:
    currentPalette = RainbowColors_p;
    break;
    
    case 6:
    currentPalette = PartyColors_p;
    break;
    
    case 7:
    currentPalette = HeatColors_p;
    break;
    
    case 8:
    currentPalette = Sunset_Real_gp;
    break;
    
    case 9:
    currentPalette = es_rivendell_15_gp;
    break;
    
    case 10:
    currentPalette = es_ocean_breeze_036_gp;
    break;
    
    case 11:
    currentPalette = rgi_15_gp;
    break;
    
    case 12:
    currentPalette = retro2_16_gp;
    break;
    
    case 13:
    currentPalette = Analogous_1_gp;
    break;
    
    case 14:
    currentPalette = es_pinksplash_08_gp;
    break;
    
    case 15:
    currentPalette = Coral_reef_gp;
    break;
    
    case 16:
    currentPalette = es_ocean_breeze_068_gp;
    break;
    
    case 17:
    currentPalette = es_pinksplash_07_gp;
    break;
    
    case 18:
    currentPalette = es_vintage_01_gp;
    break;
    
    case 19:
    currentPalette = departure_gp;
    break;
    
    case 20:
    currentPalette = es_landscape_64_gp;
    break;
    
    case 21:
    currentPalette = es_landscape_33_gp;
    break;
    
    case 22:
    currentPalette = rainbowsherbet_gp;
    break;
    
    case 23:
    currentPalette = gr65_hult_gp;
    break;
    
    case 24:
    currentPalette = gr64_hult_gp;
    break;
    
    case 25:
    currentPalette = GMT_drywet_gp;
    break;
    
    case 26:
    currentPalette = ib_jul01_gp;
    break;
    
    case 27:
    currentPalette = es_vintage_57_gp;
    break;
    
    case 28:
    currentPalette = ib15_gp;
    break;
    
    case 29:
    currentPalette = Fuschia_7_gp;
    break;
    
    case 30:
    currentPalette = es_emerald_dragon_08_gp;
    break;
    
    case 31:
    currentPalette = lava_gp;
    break;
    
    case 32:
    currentPalette = fire_gp;
    break;
    
    case 33:
    currentPalette = Colorfull_gp;
    break;
    
    case 34:
    currentPalette = Magenta_Evening_gp;
    break;
    
    case 35:
    currentPalette = Pink_Purple_gp;
    break;
    
    case 36:
    currentPalette = es_autumn_19_gp;
    break;
    
    case 37:
    currentPalette = BlacK_Blue_Magenta_White_gp;
    break;
    
    case 38:
    currentPalette = BlacK_Magenta_Red_gp;
    break;
    
    case 39:
    currentPalette = BlacK_Red_Magenta_Yellow_gp;
    break;
    
    case 40:
    currentPalette = Blue_Cyan_Yellow_gp;
    break;

    case 41:
    currentPalette = CRGBPalette16(CRGB::Red, CRGB::Orange, CRGB::Gray);
    break;
    
    case 42:
    currentPalette = CRGBPalette16(CRGB::Blue, CRGB::Red, CRGB::Red);
    break;
    
    case 43:
    currentPalette = CRGBPalette16(CRGB::LightGrey, CRGB::MidnightBlue, CRGB::Black);
    break;
    
    case 44:
    currentPalette = CRGBPalette16(CRGB::DarkGreen, CRGB::PaleGreen);
    break;

    case 45:
    currentPalette = CRGBPalette16(CRGB::Black, CRGB::Red, CRGB::Black, CRGB::Blue);
    break;
    
    case 46:
    currentPalette = CRGBPalette16(CRGB::DarkGreen, CRGB::Black, CRGB::Green);
    break;
    
    case 47:
    currentPalette = CRGBPalette16(CRGB(0,0,8), CRGB(0,0,16), CRGB(0,0,32), CRGB::White);
    break;
    
    case 48:
    currentPalette = CRGBPalette16(CRGB(255,0,127), CRGB::Black, CRGB::OrangeRed);
    break;
  }
}

// Pick a random palette from a list
void selectRandomAudioPalette() {

  switch(random8(49)) {
    case 0:
    currentPalette = CloudColors_p;
    break;
    
    case 1:
    currentPalette = LavaColors_p;
    break;
    
    case 2:
    currentPalette = OceanColors_p;
    break;
    
    case 4:
    currentPalette = ForestColors_p;
    break;
    
    case 5:
    currentPalette = RainbowColors_p;
    break;
    
    case 6:
    currentPalette = PartyColors_p;
    break;
    
    case 7:
    currentPalette = HeatColors_p;
    break;
    
    case 8:
    currentPalette = Sunset_Real_gp;
    break;
    
    case 9:
    currentPalette = es_rivendell_15_gp;
    break;
    
    case 10:
    currentPalette = es_ocean_breeze_036_gp;
    break;
    
    case 11:
    currentPalette = rgi_15_gp;
    break;
    
    case 12:
    currentPalette = retro2_16_gp;
    break;
    
    case 13:
    currentPalette = Analogous_1_gp;
    break;
    
    case 14:
    currentPalette = es_pinksplash_08_gp;
    break;
    
    case 15:
    currentPalette = Coral_reef_gp;
    break;
    
    case 16:
    currentPalette = es_ocean_breeze_068_gp;
    break;
    
    case 17:
    currentPalette = es_pinksplash_07_gp;
    break;
    
    case 18:
    currentPalette = es_vintage_01_gp;
    break;
    
    case 19:
    currentPalette = departure_gp;
    break;
    
    case 20:
    currentPalette = es_landscape_64_gp;
    break;
    
    case 21:
    currentPalette = es_landscape_33_gp;
    break;
    
    case 22:
    currentPalette = rainbowsherbet_gp;
    break;
    
    case 23:
    currentPalette = gr65_hult_gp;
    break;
    
    case 24:
    currentPalette = gr64_hult_gp;
    break;
    
    case 25:
    currentPalette = GMT_drywet_gp;
    break;
    
    case 26:
    currentPalette = ib_jul01_gp;
    break;
    
    case 27:
    currentPalette = es_vintage_57_gp;
    break;
    
    case 28:
    currentPalette = ib15_gp;
    break;
    
    case 29:
    currentPalette = Fuschia_7_gp;
    break;
    
    case 30:
    currentPalette = es_emerald_dragon_08_gp;
    break;
    
    case 31:
    currentPalette = lava_gp;
    break;
    
    case 32:
    currentPalette = fire_gp;
    break;
    
    case 33:
    currentPalette = Colorfull_gp;
    break;
    
    case 34:
    currentPalette = Magenta_Evening_gp;
    break;
    
    case 35:
    currentPalette = Pink_Purple_gp;
    break;
    
    case 36:
    currentPalette = es_autumn_19_gp;
    break;
    
    case 37:
    currentPalette = BlacK_Blue_Magenta_White_gp;
    break;
    
    case 38:
    currentPalette = BlacK_Magenta_Red_gp;
    break;
    
    case 39:
    currentPalette = BlacK_Red_Magenta_Yellow_gp;
    break;
    
    case 40:
    currentPalette = Blue_Cyan_Yellow_gp;
    break;

    case 41:
    currentPalette = CRGBPalette16(CRGB::Red, CRGB::Orange, CRGB::Gray);
    break;
    
    case 42:
    currentPalette = CRGBPalette16(CRGB::Blue, CRGB::Red, CRGB::Red);
    break;
    
    case 43:
    currentPalette = CRGBPalette16(CRGB::LightGrey, CRGB::MidnightBlue, CRGB::Black);
    break;
    
    case 44:
    currentPalette = CRGBPalette16(CRGB::DarkGreen, CRGB::PaleGreen);
    break;

    case 45:
    currentPalette = CRGBPalette16(CRGB::Black, CRGB::Red, CRGB::Black, CRGB::Blue);
    break;
    
    case 46:
    currentPalette = CRGBPalette16(CRGB::DarkGreen, CRGB::Black, CRGB::Green);
    break;
    
    case 47:
    currentPalette = CRGBPalette16(CRGB(0,0,8), CRGB(0,0,16), CRGB(0,0,32), CRGB::White);
    break;
    
    case 48:
    currentPalette = CRGBPalette16(CRGB(255,0,127), CRGB::Black, CRGB::OrangeRed);
    break;
  }

}

void selectRandomNoisePalette() {

  switch(random8(49)) {
    case 0:
    currentPalette = CloudColors_p;
    break;
    
    case 1:
    currentPalette = LavaColors_p;
    break;
    
    case 2:
    currentPalette = OceanColors_p;
    break;
    
    case 4:
    currentPalette = ForestColors_p;
    break;
    
    case 5:
    currentPalette = RainbowColors_p;
    break;
    
    case 6:
    currentPalette = PartyColors_p;
    break;
    
    case 7:
    currentPalette = HeatColors_p;
    break;
    
    case 8:
    currentPalette = Sunset_Real_gp;
    break;
    
    case 9:
    currentPalette = es_rivendell_15_gp;
    break;
    
    case 10:
    currentPalette = es_ocean_breeze_036_gp;
    break;
    
    case 11:
    currentPalette = rgi_15_gp;
    break;
    
    case 12:
    currentPalette = retro2_16_gp;
    break;
    
    case 13:
    currentPalette = Analogous_1_gp;
    break;
    
    case 14:
    currentPalette = es_pinksplash_08_gp;
    break;
    
    case 15:
    currentPalette = Coral_reef_gp;
    break;
    
    case 16:
    currentPalette = es_ocean_breeze_068_gp;
    break;
    
    case 17:
    currentPalette = es_pinksplash_07_gp;
    break;
    
    case 18:
    currentPalette = es_vintage_01_gp;
    break;
    
    case 19:
    currentPalette = departure_gp;
    break;
    
    case 20:
    currentPalette = es_landscape_64_gp;
    break;
    
    case 21:
    currentPalette = es_landscape_33_gp;
    break;
    
    case 22:
    currentPalette = rainbowsherbet_gp;
    break;
    
    case 23:
    currentPalette = gr65_hult_gp;
    break;
    
    case 24:
    currentPalette = gr64_hult_gp;
    break;
    
    case 25:
    currentPalette = GMT_drywet_gp;
    break;
    
    case 26:
    currentPalette = ib_jul01_gp;
    break;
    
    case 27:
    currentPalette = es_vintage_57_gp;
    break;
    
    case 28:
    currentPalette = ib15_gp;
    break;
    
    case 29:
    currentPalette = Fuschia_7_gp;
    break;
    
    case 30:
    currentPalette = es_emerald_dragon_08_gp;
    break;
    
    case 31:
    currentPalette = lava_gp;
    break;
    
    case 32:
    currentPalette = fire_gp;
    break;
    
    case 33:
    currentPalette = Colorfull_gp;
    break;
    
    case 34:
    currentPalette = Magenta_Evening_gp;
    break;
    
    case 35:
    currentPalette = Pink_Purple_gp;
    break;
    
    case 36:
    currentPalette = es_autumn_19_gp;
    break;
    
    case 37:
    currentPalette = BlacK_Blue_Magenta_White_gp;
    break;
    
    case 38:
    currentPalette = BlacK_Magenta_Red_gp;
    break;
    
    case 39:
    currentPalette = BlacK_Red_Magenta_Yellow_gp;
    break;
    
    case 40:
    currentPalette = Blue_Cyan_Yellow_gp;
    break;

    case 41:
    currentPalette = CRGBPalette16(CRGB::Red, CRGB::Orange, CRGB::Gray);
    break;
    
    case 42:
    currentPalette = CRGBPalette16(CRGB::Blue, CRGB::Red, CRGB::Red);
    break;
    
    case 43:
    currentPalette = CRGBPalette16(CRGB::LightGrey, CRGB::MidnightBlue, CRGB::Black);
    break;
    
    case 44:
    currentPalette = CRGBPalette16(CRGB::DarkGreen, CRGB::PaleGreen);
    break;

    case 45:
    currentPalette = CRGBPalette16(CRGB::Black, CRGB::Red, CRGB::Black, CRGB::Blue);
    break;
    
    case 46:
    currentPalette = CRGBPalette16(CRGB::DarkGreen, CRGB::Black, CRGB::Green);
    break;
    
    case 47:
    currentPalette = CRGBPalette16(CRGB(0,0,8), CRGB(0,0,16), CRGB(0,0,32), CRGB::White);
    break;
    
    case 48:
    currentPalette = CRGBPalette16(CRGB(255,0,127), CRGB::Black, CRGB::OrangeRed);
    break;
  }

}

void ChangePaletteAndSettingsPeriodically() {
  uint16_t speed = 10; // speed is set dynamically once we've started up
  uint16_t scale = 40; // scale is set dynamically once we've started up

  uint8_t secondHand = ((millis() / 1000) / HOLD_PALETTES_X_TIMES_AS_LONG) % 60;
  static uint8_t lastSecond = 99;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    if( secondHand ==  0)  { currentPalette = RainbowColors_p;        speed = 5; scale = 30; colorLoop = 0; }
    if( secondHand == 20)  { SetupPurpleAndGreenPalette();            speed = 5; scale = 50; colorLoop = 1; }
    if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();      speed = 5; scale = 30; colorLoop = 1; }
    if( secondHand == 40)  { selectRandomNoisePalette();                   speed = 5; scale =120; colorLoop = 1; }
    if( secondHand == 50)  { selectRandomPalette();                   speed = 5; scale = 30; colorLoop = 1; }
    if( secondHand == 60)  { selectRandomNoisePalette();                   speed = 5; scale = 50; colorLoop = 1; }
    if( secondHand == 70)  { selectRandomPalette();                   speed = 5; scale = 90; colorLoop = 0; }
    if( secondHand == 80)  { selectRandomPalette();                   speed = 5; scale = 30; colorLoop = 0; }
    if( secondHand == 90)  { selectRandomNoisePalette();                   speed = 5; scale = 20; colorLoop = 0; }
    if( secondHand == 10)  { selectRandomPalette();                   speed = 5; scale = 50; colorLoop = 0; }
    if( secondHand == 110) { selectRandomPalette();                   speed = 5; scale = 90; colorLoop = 0; }
    if( secondHand == 120) { selectRandomPalette();                   speed = 5; scale = 20; colorLoop = 0; }
    if( secondHand == 140) { selectRandomPalette();                   speed = 5; scale = 20; colorLoop = 0; }
    if( secondHand == 160) { selectRandomNoisePalette();                   speed = 5; scale = 20; colorLoop = 0; }
    if( secondHand == 180) { selectRandomPalette();                   speed = 5; scale = 20; colorLoop = 0; }
    if( secondHand == 200) { selectRandomPalette();                   speed = 5; scale = 20; colorLoop = 0; }
    if( secondHand == 220) { selectRandomNoisePalette();                   speed = 5; scale = 20; colorLoop = 0; }
    if( secondHand == 240) { selectRandomPalette();                   speed = 5; scale = 20; colorLoop = 0; }
    if( secondHand == 260) { selectRandomNoisePalette();                   speed = 5; scale = 20; colorLoop = 1; }
  }
}
// Interrupt normal operation to indicate that auto cycle mode has changed
void confirmBlink(CRGB blinkColor, byte count) {

  for (byte i = 0; i < count; i++) {
    fillAll(blinkColor);
    FastLED.show();
    delay(200);
    fillAll(CRGB::Black);
    FastLED.show();
    delay(200);
  }

}

// Determine flash address of text string
unsigned int currentStringAddress = 0;
void selectFlashString(byte string) {
  currentStringAddress = pgm_read_word(&stringArray[string]);
}

// Fetch font character bitmap from flash
byte charBuffer[5] = {0};
void loadCharBuffer(byte character) {
  byte mappedCharacter = character;
  if (mappedCharacter >= 32 && mappedCharacter <= 95) {
    mappedCharacter -= 32; // subtract font array offset
  } else if (mappedCharacter >= 97 && mappedCharacter <= 122) {
    mappedCharacter -= 64; // subtract font array offset and convert lowercase to uppercase
  } else {
    mappedCharacter = 96; // unknown character block
  }
  
  for (byte i = 0; i < 5; i++) {
    charBuffer[i] = pgm_read_byte(Font[mappedCharacter]+i);
  }
  
}

// Fetch a character value from a text string in flash
char loadStringChar(byte string, byte character) {
  return (char) pgm_read_byte(currentStringAddress + character);
}

// write EEPROM value if it's different from stored value
void updateEEPROM(byte location, byte value) {
  if (EEPROM.read(location) != value) EEPROM.write(location, value);
}

// Write settings to EEPROM if necessary
void checkEEPROM() {
  if (eepromOutdated) {
    if (currentMillis - eepromMillis > EEPROMDELAY) {
      updateEEPROM(0, 99);
      updateEEPROM(1, currentEffect);
      updateEEPROM(2, autoCycle);
      updateEEPROM(3, currentBrightness);
      updateEEPROM(4, audioEnabled);
      eepromOutdated = false;
    }
  }
}

// Fill the x/y array of 8-bit noise values using the inoise8 function.
void fillnoise8() {
  for(int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for(int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j;
      noise[i][j] = inoise8(nx + ioffset,ny + joffset,nz);
    }
  }
  nz += nspeed;
}

byte nextBrightness(boolean resetVal) {
    const byte brightVals[6] = {32,64,96,160,224,255};

    if (resetVal) {
      currentBrightness = STARTBRIGHTNESS;
    } else {
      currentBrightness++;
      if (currentBrightness > sizeof(brightVals)/sizeof(brightVals[0])) currentBrightness = 0;
    }

  return brightVals[currentBrightness];
}

void mapNoiseToLEDsUsingPalette() {
  static uint8_t ihue=0;
  
  for(int i = 0; i < kMatrixWidth; i++) {
    for(int j = 0; j < kMatrixHeight; j++) {

      uint8_t index = noise[j][i];
      uint8_t bri =   noise[i][j];

      if( colorLoop) { 
        index += ihue;
      }
      
      if( bri > 127 ) {
        bri = 255;
      } else {
        bri = dim8_raw( bri * 2);
      }

      CRGB color = ColorFromPalette( currentPalette, index, bri);
      leds[XY(i,j)] = color;
    }
  }
  
  ihue+=1;
}
