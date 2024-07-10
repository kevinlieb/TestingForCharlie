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
        void speedoSetup() {
            USBSerial.begin(9600);

#ifdef SMALL_DIAL
            M5Canvas img(&M5Dial.Display);
            M5.begin();
            auto cfg = M5.config();
            M5Dial.begin(cfg, true, false);
#endif

#ifdef BIG_DIAL
            gfx->begin();
            gfx->fillScreen(BLACK);
            pinMode(TFT_BL, OUTPUT);
            digitalWrite(TFT_BL, HIGH);
            Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
#endif

#ifdef BIG_DIAL
            sprite.createSprite(480, 480);
#endif

#ifdef SMALL_DIAL
            sprite.createSprite(240, 240);
#endif

            sprite.createSprite(240, 240);
            sprite.setSwapBytes(true);
            sprite.setTextDatum(4);
            sprite.setTextColor(TFT_WHITE, backColor);
            sprite.setTextDatum(4);

            int a = 120;
            for(int i=0; i<360; i++) {
                x[i]  =((r - (5 * scaleFactor)) *cos(rad * a)) + cx;
                y[i]  =((r - (5 * scaleFactor))*sin(rad * a)) + cy;
                px[i] =((r - (14 * scaleFactor))*cos(rad * a)) + cx;
                py[i] =((r - (14 * scaleFactor))*sin(rad * a)) + cy;
                lx[i] =((r - (26 * scaleFactor))*cos(rad * a)) + cx;
                ly[i] =((r - (26 * scaleFactor))*sin(rad * a)) + cy;
                nx[i] =((r - (86 * scaleFactor))*cos(rad * a)) + cx;
                ny[i] =((r - (86 * scaleFactor))*sin(rad * a)) + cy;

                a++;
                if(a == 360) {
                    a=0;
                }
            }
        } /* setup*/

        void drawGage() {
            /* outermost semicircle */
            sprite.drawSmoothArc(cx, cy, r, ir, 30, 330, gaugeColor, backColor);
            //sprite.drawSmoothArc(320 - cx, cy, r, ir, 30, 330, gaugeColor, backColor);

            sprite.drawSmoothArc(cx, cy, rir, rir - 1, 10, 350, gaugeColor, backColor);

            for (int i = 0; i < 26; i++) {
                if (i < 20) {
                    color1 = gaugeColor;
                    color2 = TFT_WHITE;
                }
                else {
                    color1 = purple;
                    color2 = purple;
                }

                if (i % 2 == 0) {
                    sprite.drawWedgeLine(x[i * 12], y[i * 12], px[i * 12], py[i * 12], 2, 1, color1);
                    sprite.setTextColor(color2, backColor);
                    sprite.drawString(String(i * 10), lx[i * 12], ly[i * 12]);
                }
                else
                    sprite.drawWedgeLine(x[i * 12], y[i * 12], px[i * 12], py[i * 12], 1, 1, color2);
            }
        } /* drawGage */


        void speedoLoop() {
            static u16_t milliseconds = 0;

            if (millis() - milliseconds > 100) {
                milliseconds = millis();
                speedAngle++;
                if(speedAngle > 240) {
                    speedAngle=0;
                }
            }
    
#ifdef SMALL_DIAL
            M5Dial.update();
#endif

            sA = speedAngle * 1.2;
            sprite.createSprite(240, 240);
            sprite.fillSprite(0x010B);
            sprite.setTextColor(TFT_WHITE, backColor);
    
#ifdef SMALL_DIAL
            sprite.loadFont(smallFont);
#endif

#ifdef BIG_DIAL
            sprite.loadFont(asap32Font);
#endif
            drawGage();

            sprite.drawWedgeLine(px[(int)sA], py[(int)sA], nx[(int)sA], ny[(int)sA], 2, 2, needleColor);

#ifdef SMALL_DIAL
            M5Dial.Display.pushImage(0, 0, 240, 240, (uint16_t *)sprite.getPointer());
#else    
            gfx->draw16bitBeRGBBitmap(0,0,(uint16_t*)sprite.getPointer(),480,480);    
#endif
    
            //USBSerial.println(speedAngle);

        }
}; /* end class */