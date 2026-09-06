#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <time.h>

#define BAT A0
#define BUZZ 20

#define CP1 GPIO_NUM_3
#define CP2 GPIO_NUM_4
#define CP3 GPIO_NUM_5

#define TFT_SCLK 9    // SCL
#define TFT_MOSI 10   // SDA
#define TFT_RST 8
#define TFT_DC 6
#define TFT_CS 7
#define TFT_BL 21

const String days[7] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
const String months[12] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "SEP", "OCT", "NOV", "DEC"};

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

float get_battery_level() {
  uint32_t Vbatt = 0;
  for (int i = 0; i < 16; i++) {
    Vbatt = Vbatt + analogReadMilliVolts(BAT);
  }
  float Vbattf = 2 * Vbatt / 16 / 1000.0;
  return Vbattf;
}

void draw_clock(Adafruit_ST7789 tft, tm time, float vbatt) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(5, 5);
  tft.setTextSize(6);
  tft.setTextColor(ST77XX_WHITE);
  tft.printf("%s %i %s %i", days[time.tm_wday], time.tm_mday, months[time.tm_mon], 1990 + time.tm_year);
  tft.setCursor(5, 250);
  tft.printf("BAT: %.2f%", vbatt);

  tft.setCursor(20, 50);
  tft.setTextSize(50);
  tft.printf("%i:%i", time.tm_hour, time.tm_min);
}

void setup() {
  gpio_hold_dis(CP2);
  gpio_deep_sleep_hold_dis();

  Serial.begin(115200);

  pinMode(BAT, INPUT);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, LOW); 

  tft.init(76, 284);
  tft.invertDisplay(false);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  Serial.println("TFT Initialized!");
  tft.setCursor(0,0);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(20);
  tft.setCursor(20,0);
  
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    tft.print("WAKEY-WAKEY");
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    timeinfo.tm_isdst = false;
    timeinfo.tm_mday = 1;
    timeinfo.tm_mon = 0;
    timeinfo.tm_wday= 1;
    timeinfo.tm_yday= 0;
    delay(2000);
  }

  draw_clock(tft, timeinfo, get_battery_level());
  delay(5000);

  pinMode(CP1, INPUT);
  pinMode(CP3, INPUT);
  pinMode(CP2, OUTPUT);
  digitalWrite(CP2, HIGH);
  gpio_hold_en(CP2);

  uint64_t pinBitmask = (1ULL << CP3);
  esp_deep_sleep_enable_gpio_wakeup(pinBitmask, ESP_GPIO_WAKEUP_GPIO_HIGH);

  gpio_deep_sleep_hold_en();

  digitalWrite(TFT_BL, LOW);

  esp_deep_sleep_start();

}

void loop() {
  // work in progress
  // will only be used for when active config is being done
}