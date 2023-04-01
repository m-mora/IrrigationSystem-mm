/*----------------------------------------------------------------------*
 *  _   __ _     _____ _____                                            *
 * | | / /| |   |_   _/  __ \   This software is been developed by      *
 * | |/ / | |     | | | /  \/   a group of enthusiast hobbiest          *
 * |    \ | |     | | | |       with the purpose of learn and           *
 * | |\  \| |_____| |_| \__/\   have fun, so nobody is responsible or   *
 * \_| \_/\_____/\___/ \____/   will provide warranty.                  *
 *                                                                      *
 * This software will run in a ESP8266 microcontrolller, the objective  *
 * is to have a irrigation system controler that can read some sensors  *
 * and decide if a valve should be open.                                *
 * There is not restriction to use, modify and improve the code, so     *
 * please do it and share the improvements.                             *
 *                                                                      *
 * Let's have Fun!!                                                     *
 *                                                                      *
 * ---------------------------------------------------------------------*/

#include "display.h"
#include "system/time/itime_provider.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool Display::init(uint8_t address)
{

  if (!display.begin(SSD1306_SWITCHCAPVCC, address))
  {
    return false;
  }
  display.clearDisplay();
  initialDrawLine();
  display.display();
  delay(100);
  display.clearDisplay();
  return true;
}

void Display::clean()
{
  display.clearDisplay();
}

void Display::update(String t)
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(25,0);
  display.setTextSize(1);
  display.println("Current Time");
  display.println("");
  display.setTextSize(2);
  display.println(t);
  display.display();
}

void Display::initialDrawLine() {

  display.clearDisplay(); // Clear display buffer

  for(uint16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(0, 0, i, display.height()-1, SSD1306_WHITE);
    display.display(); // Update screen with each newly-drawn line
    delay(1);
  }
  for(uint16_t i = 0; i < display.height(); i += 4) {
    display.drawLine(0, 0, display.width()-1, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();
}