#include "SpeedoTypeGauge.hpp"

       void SpeedoTypeGauge::speedoSetup() {
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

        void SpeedoTypeGauge::drawGage() {
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


        void SpeedoTypeGauge::speedoLoop() {
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
