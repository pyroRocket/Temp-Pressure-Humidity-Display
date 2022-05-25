//sensor
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
//Display
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

//sensor pins
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

//display pins
#define TFT_DC 8
#define TFT_CS 10

#define SEALEVELPRESSURE_HPA (1024)

//for sensor:
Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

//for dsplay:
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

unsigned long delayTime;

void setup() {
    Serial.begin(9600);
    while(!Serial);    // time to get serial running
    Serial.println(F("BME280 test"));
    Serial.println("ILI9341 Test!");

    unsigned status;
    
    // default settings
    status = bme.begin();  
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }

    tft.begin();
    tft.setRotation(2);
    tft.fillScreen(ILI9341_BLACK);
    tft.drawFastHLine(0, 30 ,  tft.width(), ILI9341_CYAN);   // draw horizontal white line at position (0, 30)
    tft.drawFastVLine(120, 0, tft.width(), ILI9341_CYAN); //test line for easy centering
    tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);  // set text color to white and black background
    tft.setTextSize(2);                 // text size = 2
    tft.setCursor(31, 10);              // move cursor to position (31, 10) pixel
    tft.print("WEATHER STATION");
    tft.drawFastHLine(0, 80,  tft.width(), ILI9341_CYAN);  // draw horizontal white line at position (0, 76)
    tft.drawFastHLine(0, 150,  tft.width(), ILI9341_CYAN);  // draw horizontal white line at position (0, 122)
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);     // set text color to red and black background
    tft.setCursor(55, 39);              // move cursor to position (55, 39) pixel
    tft.print("TEMPERATURE ");
    tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);  // set text color to cyan and black background
    tft.setCursor(75, 95);              // move cursor to position (75, 95) pixel
    tft.print("HUMIDITY ");
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);  // set text color to green and black background
    tft.setCursor(34, 200);              // move cursor to position (34, 131) pixel
    tft.print("PRESSURE ");
    tft.setTextSize(2);     

    delay(3000);
    
    Serial.println("-- Default Test --");
    delayTime = 1000;

    Serial.println();
}

//char _buffer[11];

void loop() {  
    printValues();
    
    char _buffer[8];

    float temp = bme.readTemperature()* 9/5 + 32;    // get temperature in °C
    float humi = bme.readHumidity();       // get humidity in rH%
    float pres = bme.readPressure();       // get pressure in Pa

    // print temperature (in °C)
    if(temp < 0)    // if temperature < 0
      sprintf( _buffer, "-%02u.%02u", (int)abs(temp), (int)(abs(temp) * 100) % 100 );
    else            // temperature >= 0
      sprintf( _buffer, " %02u.%02u", (int)temp, (int)(temp * 100) % 100 );
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);  // set text color to yellow and black background
    tft.setCursor(40, 60);
    tft.print(_buffer);
    tft.drawCircle(119, 60, 2, ILI9341_YELLOW);  // print degree symbol ( ° )
    tft.setCursor(125, 60);
    tft.print("F");
 
    // 2: print humidity
    sprintf( _buffer, "%02u.%02u %%", (int)humi, (int)(humi * 100) % 100 );
    tft.setTextColor(ILI9341_MAGENTA, ILI9341_BLACK);  // set text color to magenta and black background
    tft.setCursor(23, 125);
    tft.print(_buffer);
 
    // 3: print pressure (in hPa)
    sprintf( _buffer, "%04u.%02u", (int)(pres/100), (int)((uint32_t)pres % 100) );
    tft.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);  // set text color to orange and black background
    tft.setCursor(3, 150);
    tft.print(_buffer);
    tft.setCursor(91, 150);
    tft.print("hPa");

    delay(delayTime);

}

void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature()* 9/5 + 32);
    Serial.println(" *F");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA)* 3.281);
    Serial.println(" ft");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}
