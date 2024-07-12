#include <Arduino.h>

#define needleColor 0xF811
#define backColor 0x0026
#define gaugeColor 0x055D
#define purple 0xEA16


#define _BIG_DIAL
#define SMALL_DIAL

#ifdef SMALL_DIAL
#include "M5Dial.h"
#endif

#ifdef BIG_DIAL
#include <Arduino_GFX_Library.h>
#endif

#include <FS.h>
#include <TFT_eSPI.h>

#include "SmallFont.h"
#include "AsapFont.h"


class SpeedoTypeGauge {
     private:
        TFT_eSPI tft = TFT_eSPI();
        TFT_eSprite sprite = TFT_eSprite(&tft);

#ifdef BIG_DIAL
        Arduino_DataBus *bus = new Arduino_SWSPI(
            GFX_NOT_DEFINED /* DC */, 1 /* CS */,
            46 /* SCK */, 0 /* MOSI */, GFX_NOT_DEFINED /* MISO */);

        Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
            2 /* DE */, 42/* VSYNC */, 3 /* HSYNC */, 45 /* PCLK */,
            4 /* R0 */, 41 /* R1 */, 5 /* R2 */, 40 /* R3 */, 6 /* R4 */,
            39 /* G0/P22 */, 7 /* G1/P23 */, 47 /* G2/P24 */, 8 /* G3/P25 */, 48 /* G4/P26 */, 9 /* G5 */,
            11 /* B0 */, 15 /* B1 */, 12 /* B2 */, 16 /* B3 */, 21 /* B4 */,
            1 /* hsync_polarity */, 10 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 50 /* hsync_back_porch */,
            1 /* vsync_polarity */, 10 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 20 /* vsync_back_porch */);


        Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
            480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
            bus, GFX_NOT_DEFINED /* RST */, st7701_type5_init_operations, sizeof(st7701_type5_init_operations));

        #define I2C_SDA_PIN 17
        #define I2C_SCL_PIN 18
        #define TFT_BL 38
#endif

#ifdef SMALL_DIAL
        M5GFX display;        
        //M5Dial M5Dial;
        // M5Canvas canvas(&display);    

#endif


        unsigned short color1;
        unsigned short color2;

#ifdef SMALL_DIAL
        int cx = 120; //center of x
        int cy = 120; //center of y
        int r  = 120;
        int ir = r - 2;
        int rir = ir - 40;
        int n = 0;
        int angle = 0;
        int scaleFactor = 1;
#endif

#ifdef BIG_DIAL
        int cx = 240; //center of x
        int cy = 240; //center of y
        int r = 240; //outermost radius
        int ir = r - 2;
        int rir = ir - 78;
        int n=0;
        int angle=0;
        int scaleFactor = 2;
#endif

        float x[360]; //outer points of Speed gaouges
        float y[360];
        float px[360]; //inner point of Speed gaouges
        float py[360];
        float lx[360]; //text of Speed gaouges
        float ly[360];
        float nx[360]; //needle low of Speed gauges
        float ny[360];

        float sA;

        double rad=0.01745;
        float speedAngle;

  
    public:
        SpeedoTypeGauge(){};
        ~SpeedoTypeGauge(){};
        void speedoSetup();
        void speedoLoop();
        void drawGage() ;


};