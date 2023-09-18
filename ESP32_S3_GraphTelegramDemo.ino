/*
   This sketch targets the ESP32-S3-BOX board ONLY
   It demonstrates a compatible Adafruit GFX Graphics Library and AsyncTelegram2 sketch
*/

#include "ESP32_S3_Box_TFT.h"
#include <AsyncTelegram2.h>
#include <time.h>
#include <WiFi.h>    
#include <HTTPClient.h>
#include <UrlEncode.h>
#include <WiFiClient.h>
#include <SSLClient.h>
#include "tg_certificate.h"

#define MYTZ "CET-1CEST,M3.5.0,M10.5.0/3"

//Comandos Display
#define INTRO             0
#define LED_ON            1
#define LED_OFF           2
#define DISPLAY_RECTANG   3
#define DISPLAY_TRIANG    4
#define FIN               5

#define LED_PIN        41

WiFiClient base_client;
//Used Analog pin 12 
SSLClient client(base_client, TAs, (size_t)TAs_NUM, 12, 1, SSLClient::SSL_ERROR);

//Graficos ESP32 S3 BOX
ESP32S3BOX_TFT tft = ESP32S3BOX_TFT();
 


//WIFI PARAMETERS
const char* ssid =  "Add your Wifi SSID";
const char* password = "Add your password";


//Telegram Parameters
const char* token = "Add your bot Token";
AsyncTelegram2 myBot(client);


void setDisplayAnimation(int opt)
{
  switch (opt)
  {
      //Animacion Inicial
      case INTRO:
        tft.setTextSize(2);   
        tft.fillScreen(ST7789_BLACK);
        tft.setTextWrap(true);
        tft.setTextColor(ST7789_WHITE);
        tft.setCursor(0, 0);
        tft.print("TECHWELL LIVING");
        tft.setCursor(0, 40);
        tft.print("Bienvenido a mi Demo!!!");
        tft.setCursor(0, 80);
        tft.print(" Que tal estas?");
        break;
      //Animacion LED ON
      case LED_ON:
        tft.setTextSize(2);    
        tft.fillScreen(ST7789_BLACK);
        tft.setTextWrap(true);
        tft.setTextColor(ST7789_WHITE);
        tft.setCursor(0, 0);
        tft.print("TECHWELL LIVING");
        tft.setCursor(0, 60);
        tft.print("Estado del LED: ENCENDIDO");
        break;

      //Animacion LED OFF
        case LED_OFF:
        tft.setTextSize(2);    
        // large block of text
        tft.fillScreen(ST7789_BLACK);
        tft.setTextWrap(true);
        tft.setTextColor(ST7789_WHITE);
        tft.setCursor(0, 0);
        tft.print("TECHWELL LIVING");
        tft.setCursor(0, 60);
        tft.print("Estado del LED: APAGADO");
        break;
        //Animacion rectángulos
        case DISPLAY_RECTANG:
          Serial.print("Rectangles (outline)     ");
          Serial.println(testRects(ST7789_GREEN));
          delay(200);
          Serial.print("Rectangles (filled)      ");
          Serial.println(testFilledRects(ST7789_YELLOW, ST7789_MAGENTA));
          delay(200);
        break;
        //Animacion triángulos
        case DISPLAY_TRIANG:
          Serial.print("Triangles (outline)      ");
          Serial.println(testTriangles());
          delay(200);
          Serial.print("Triangles (filled)       ");
          Serial.println(testFilledTriangles());
          delay(200);
        break;

        case FIN:
        Serial.print("Lines                    ");
        Serial.println(testLines(ST7789_CYAN));
        delay(300);
        Serial.print("Horiz/Vert Lines         ");
        Serial.println(testFastLines(ST7789_RED, ST7789_BLUE));
        delay(500);
        tft.setTextSize(2);   
        tft.fillScreen(ST7789_BLACK);
        tft.setTextWrap(true);
        tft.setTextColor(ST7789_WHITE);
        tft.setCursor(0, 0);
        tft.print("TechWell Living");
        tft.setCursor(0, 30);
        tft.print("Add some Text here:) ");
        tft.setCursor(0, 60); 
        tft.print("Muchas Gracias!");
        break;

        default:
        break;

  }

}
void setup()
{
  Serial.begin(115200);
  tft.init();
  tft.setRotation(0);
  tft.setTextSize(2);
  setDisplayAnimation(INTRO);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(LED_PIN,OUTPUT);
  // Sync time with NTP
  configTzTime(MYTZ, "time.google.com", "time.windows.com", "pool.ntp.org");

  // Set the Telegram bot properies
  myBot.setUpdateTime(1000);//Time in ms to refresh Telegram queries in order to avoid to much often
  myBot.setTelegramToken(token);

  // Check if all things are ok
  Serial.print("\nTest Telegram connection... ");
  myBot.begin() ? Serial.println("OK") : Serial.println("NOK");

  Serial.print("Bot name: @");
  Serial.println(myBot.getBotName());
}

void loop() 
{
    TBMessage msg;
    // if there is an incoming message...
    if (myBot.getNewMessage(msg)) {
      String msgText = msg.text;

      if (msgText.equals("/ledon1")) {               // if the received message is "LIGHT ON"...
        digitalWrite(LED_PIN, HIGH);                 // turn on the LED 
        myBot.sendMessage(msg, "LED ENCENDIDO");     // notify the sender in Telegram
        setDisplayAnimation(LED_ON);                 //Change Display Animation
      }
      else if (msgText.equals("/ledoff2")) {         // if the received message is "LIGHT OFF"...
        digitalWrite(LED_PIN, LOW);                  // turn off the led 
        myBot.sendMessage(msg, "LED APAGADO");       // notify the sender in Telegram
        setDisplayAnimation(LED_OFF);                //Change Display Animation
      }
      else if (msgText.equals("/triang3")) {        
        myBot.sendMessage(msg, "Mostrando Triángulos!");
        setDisplayAnimation(DISPLAY_TRIANG);
      }
      else if (msgText.equals("/rectang4")) {          
        myBot.sendMessage(msg, "Mostrando Rectángulos!");
        setDisplayAnimation(DISPLAY_RECTANG);
      }
      else if (msgText.equals("/fin5")) {       
        myBot.sendMessage(msg, "Gracias por ver el Video!");
        setDisplayAnimation(FIN);
      }
  }
}


  /*
    Funciones para graficar en la pantalla
  */

unsigned long testFillScreen()
{
  unsigned long start = micros();
  tft.fillScreen(ST7789_BLACK);
  yield();
  tft.fillScreen(ST7789_RED);
  yield();
  tft.fillScreen(ST7789_GREEN);
  yield();
  tft.fillScreen(ST7789_BLUE);
  yield();
  tft.fillScreen(ST7789_BLACK);
  yield();
  return micros() - start;
}

unsigned long testText()
{
  tft.fillScreen(ST7789_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(ST7789_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ST7789_YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(ST7789_RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(ST7789_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("Add text");
  tft.setTextSize(1);
  tft.println("Add text");
  tft.println("");
  tft.println("Add text");
  tft.println("Add text");
  tft.println("Add text");
  tft.println("Add text");
  tft.println("Add text");
  return micros() - start;
}

unsigned long testLines(uint16_t color)
{
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(ST7789_BLACK);
  yield();

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  yield();
  tft.fillScreen(ST7789_BLACK);
  yield();

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  yield();
  tft.fillScreen(ST7789_BLACK);
  yield();

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  yield();
  tft.fillScreen(ST7789_BLACK);
  yield();

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);

  yield();
  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2)
{
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(ST7789_BLACK);
  start = micros();
  for (y = 0; y < h; y += 5) tft.drawFastHLine(0, y, w, color1);
  for (x = 0; x < w; x += 5) tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

unsigned long testRects(uint16_t color)
{
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen(ST7789_BLACK);
  n     = min(tft.width(), tft.height());
  start = micros();
  for (i = 2; i < n; i += 6) {
    i2 = i / 2;
    tft.drawRect(cx - i2, cy - i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2)
{
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ST7789_BLACK);
  n = min(tft.width(), tft.height());
  for (i = n; i > 0; i -= 6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx - i2, cy - i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx - i2, cy - i2, i, i, color2);
    yield();
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color)
{
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen(ST7789_BLACK);
  start = micros();
  for (x = radius; x < w; x += r2) {
    for (y = radius; y < h; y += r2) {
      tft.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color)
{
  unsigned long start;
  int           x, y, r2 = radius * 2,
                      w = tft.width()  + radius,
                      h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for (x = 0; x < w; x += r2) {
    for (y = 0; y < h; y += r2) {
      tft.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testTriangles()
{
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;

  tft.fillScreen(ST7789_BLACK);
  n     = min(cx, cy);
  start = micros();
  for (i = 0; i < n; i += 5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.color565(i, i, i));
  }

  return micros() - start;
}

unsigned long testFilledTriangles()
{
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen(ST7789_BLACK);
  start = micros();
  for (i = min(cx, cy); i > 10; i -= 5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                     tft.color565(0, i * 10, i * 10));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                     tft.color565(i * 10, i * 10, 0));
    yield();
  }

  return t;
}

unsigned long testRoundRects()
{
  unsigned long start;
  int           w, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ST7789_BLACK);
  w     = min(tft.width(), tft.height());
  start = micros();
  for (i = 0; i < w; i += 6) {
    i2 = i / 2;
    tft.drawRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects()
{
  unsigned long start;
  int           i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ST7789_BLACK);
  start = micros();
  for (i = min(tft.width(), tft.height()); i > 20; i -= 6) {
    i2 = i / 2;
    tft.fillRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.color565(0, i, 0));
    yield();
  }

  return micros() - start;
}
