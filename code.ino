// Sensor libraries
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
// Display Libraries
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// Sensor pins
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

// Display pins
#define TFT_DC 8
#define TFT_CS 10

// Current pressure at sea level for your area (in hPA)
#define SEALEVELPRESSURE_HPA (1024)

// Initialize sensor:
Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

// Initialize dsplay:
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Instantiate variable delayTime
unsigned long delayTime;

void setup() {

    delayTime = 1000;
    bme.begin(); // Start sensor (comment out if you want serial output)
    tft.begin(); // Start display

    // Uncomment to have serial output  
    /*
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
    Serial.println("-- Default Test --");

    Serial.println();
    */
    tft.setRotation(2); // Set text orientation (2 = upside down)
    tft.fillScreen(ILI9341_BLACK); // Make screen black
    
    tft.drawFastHLine(0, 30 ,  tft.width(), ILI9341_CYAN);   // draw horizontal white line at position (0, 30)
    tft.drawFastHLine(0, 102,  tft.width(), ILI9341_CYAN);  // draw horizontal white line at position (0, 102)
    tft.drawFastHLine(0, 174,  tft.width(), ILI9341_CYAN);  // draw horizontal white line at position (0, 174)
    tft.drawFastHLine(0, 246,  tft.width(), ILI9341_CYAN);  // draw horizontal white line at position (0, 246)
    //tft.drawFastVLine(120, 72, tft.width(), ILI9341_CYAN); //test line for easy centering
    
    tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);  // set text color to white and black background
    tft.setTextSize(2);                 // text size = 2
    tft.setCursor(31, 6);              // move cursor to position (31, 6) pixel
    tft.print("WEATHER STATION");
    
    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);     // set text color to red and black background
    tft.setCursor(55, 39);              // move cursor to position (55, 39) pixel
    tft.print("TEMPERATURE");
    
    tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);  // set text color to cyan and black background
    tft.setCursor(75, 111);              // move cursor to position (75, 111) pixel
    tft.print("HUMIDITY");
    
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);  // set text color to green and black background
    tft.setCursor(74, 183);              // move cursor to position (74, 183) pixel
    tft.print("PRESSURE"); 
    
    tft.setTextColor(ILI9341_BLUE, ILI9341_BLACK);  // set text color to green and black background
    tft.setCursor(74, 255);              // move cursor to position (74, 255) pixel
    tft.print("ALTITUDE");   

    delay(3000); //wait 3 seconds
    
}

void loop() {  
    //printValues(); // Uncomment for serial output
    
    char _buffer[8]; //Create buffer

    float temp = bme.readTemperature()* 9/5 + 32;    // get temperature in °F
    float humi = bme.readHumidity();       // get humidity in rH%
    float pres = bme.readPressure();       // get pressure in hPa
    float alt = bme.readAltitude(SEALEVELPRESSURE_HPA)* 3.281; //Get rough altitude in ft

    // 1: print temperature (in °F)
    if(temp < 0)    // if temperature < 0
      sprintf( _buffer, "-%02u.%02u", (int)abs(temp), (int)(abs(temp) * 100) % 100 );
    else            // temperature >= 0
      sprintf( _buffer, " %02u.%02u", (int)temp, (int)(temp * 100) % 100 );
    tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);  // set text color to yellow and black background
    tft.setCursor(69, 69); // move cursor to position (69, 69) pixel
    tft.print(_buffer); // Print value
    tft.drawCircle(150, 70, 2, ILI9341_YELLOW);  // print degree symbol ( ° )
    tft.setCursor(156, 69); // move cursor to position (156, 69) pixel
    tft.print("F");
 
    // 2: print humidity
    sprintf( _buffer, "%02u.%02u %%", (int)humi, (int)(humi * 100) % 100 );
    tft.setTextColor(ILI9341_MAGENTA, ILI9341_BLACK);  // set text color to magenta and black background
    tft.setCursor(81, 141); // move cursor to position (81, 141) pixel
    tft.print(_buffer); // Print value
 
    // 3: print pressure (in hPa)
    sprintf( _buffer, "%04u.%02u", (int)(pres/100), (int)((uint32_t)pres % 100) );
    tft.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);  // set text color to orange and black background
    tft.setCursor(58, 213); // move cursor to position (58, 213) pixel
    tft.print(_buffer); //Print value
    tft.setCursor(149, 213); // move cursor to position (149, 213) pixel
    tft.print("hPa");

    // 4: print altitude (in ft)
    sprintf( _buffer, "%04u.%02u", (int)alt, (int)(alt * 100) % 100);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);  // set text color to orange and black background
    tft.setCursor(62, 285); // move cursor to position (62,285) pixel
    tft.print(_buffer); // Print value
    tft.setCursor(150, 285); // move cursor to position (150, 285) pixel
    tft.print("ft");
    

    delay(delayTime); // Wait x ammount of time

}

// Uncomment for serial output
/*
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
*/
