#include <Adafruit_NeoPixel.h>

// Configuration parameters
#define NEOPIXEL_PIN 3
#define NEOPIXEL_LED_COUNT 13
#define POT_1_PIN A0
#define POT_2_PIN A1
#define POT_1 0
#define POT_2 1

// pick which light show to run (1, 2, or 3)
#define LIGHTSHOW 3

// Adafruit_NeoPixel parameters
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_LED_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);


// read the 10-bit analog value of the selected pot (pot_num)
// scale the 10-bit number to 8-bit value and return
int getPot(int pot_num) {
    int pot_val = 0;
    switch (pot_num) {
        case POT_1:
            pot_val = analogRead(POT_1_PIN);
            break;
        case POT_2:
            pot_val = analogRead(POT_2_PIN);
            break;
        default:
            break;
    }
    // scale the pot value to 0-255 (80bit)
    return (map(pot_val, 0, 1023, 0, 255));
}


// One LED on at a time
// =================================================
// POT1 defines LED position
// POT2 defines LED color
void lightShow1(int pot1_val, int pot2_val) {
    // first clear all LEDs
    for (int i = 0; i < NEOPIXEL_LED_COUNT; i++) {
        strip.setPixelColor(i, 0, 0, 0);
    }
    int pixel_pos = map(pot1_val, 0, 255, 0, NEOPIXEL_LED_COUNT);
    byte r = 255;
    byte g = pot2_val;
    byte b = 255 - pot2_val;
    strip.setPixelColor(pixel_pos, r, g, b);
    strip.show();
}


// Random Lighting and Random
// =================================================
// Pot1: intensity of the randomness
// Pot2: number of pixels on the strip to randomly light up
void lightShow2(int pot1_val, int pot2_val) {
    int pixel_pos = map(pot2_val, 0, 255, 0, NEOPIXEL_LED_COUNT);
    byte r = random(pot1_val);
    byte g = random(pot1_val);
    byte b = random(pot1_val);

    // randomly shuts off one pixel
    strip.setPixelColor(random(pixel_pos), 0, 0, 0);

    // set a random pixel to a random color
    strip.setPixelColor(random(pixel_pos), r, g, b);
    
    strip.show();
    delay(100);
}


// ping pong animation
// =================================================
// Pot1: color
// Pot2: speed
void lightShow3(int pot1_val, int pot2_val) {
    enum dir_t {UP, DN};
    static int pixel_pos = 0;
    static dir_t dir = UP;

    byte r = random(pot1_val);
    byte g = random(pot1_val);
    byte b = random(pot1_val);

    strip.setPixelColor(pixel_pos, r, g, b);
    strip.show();
    delay(pot2_val);

    strip.setPixelColor(pixel_pos, 0, 0, 0);

    // determine the next pixel position
    if (pixel_pos == 0) {
        pixel_pos++;
        dir = UP;
    } else if (pixel_pos == NEOPIXEL_LED_COUNT - 1) {
        pixel_pos--;
        dir = DN;
    } else if ((dir == UP) && (pixel_pos < NEOPIXEL_LED_COUNT - 1)) {
        pixel_pos++;
    } else if ((dir == DN) && (pixel_pos > 0)) {
        pixel_pos--;
    }
}


// The setup() loop is executed only once when the Arduino is 
// powered on or reset. In this program, we only need to initialize
// the LED strip. Optionaly, we can initialize the Serial library to
// allow debugging
void setup() {
    // for debugging pots, uncomment this
    // Serial.begin(9600);

    // Initialize the strip
    strip.begin();
}


// The main loop() is executed continously after setup()
// In this program, the two pot values are read, then passed to
// the selected light show routine.
void loop() {
    // put your main code here, to run repeatedly:
    int pot1_val = getPot(POT_1);
    int pot2_val = getPot(POT_2);

    // for debugging pots, uncomment this
    // Serial.println(pot1_val);
    // Serial.println(pot2_val);

    switch (LIGHTSHOW) {
        case 1:
            lightShow1(pot1_val, pot2_val);
            break;
        case 2:
            lightShow2(pot1_val, pot2_val);
            break;
        case 3:
            lightShow3(pot1_val, pot2_val);
            break;
        default:
            break;
    }
}
