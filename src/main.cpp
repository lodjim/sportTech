#include <Arduino.h>
#include "EmotiBit.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SD.h>

#define THRESHOLD  0.0 // Threshold for peak detection
#define NUM_SAMPLES  0.2 // Number of samples to average for peak detection

// Define your variables
float previousMagnitude = 0.0;
int step_count = 0;
bool peaked = false;
float sum = 0.0;

const char* ssid = "DAUST-LAB";
const char* password = "Innov2021";
const String base = "192.168.1.90";
const String url = "http://" + base + ":8000/upload-data";
const String identity = "8a058b9c-be92-4638-9482-99e97417a46d";

float PPG_GREEN, PPG_INFRARED, PPG_RED, HR, EDA, EDL, EDR, TEMPERATURE_1, HUMIDITY_0, ACCELEROMETER_X, ACCELEROMETER_Y, ACCELEROMETER_Z, GYROSCOPE_X, GYROSCOPE_Y, GYROSCOPE_Z, BATTERY_PERCENT, ACCELERATION, DISTANCE_COVERED;
int STEP_COUNT;

#define SerialUSB SERIAL_PORT_USBVIRTUAL  // Required to work in Visual Micro / Visual Studio IDE
const uint32_t SERIAL_BAUD = 115200;    //

EmotiBit emotibit;
const size_t dataSize = EmotiBit::MAX_DATA_BUFFER_SIZE;
float data[dataSize];


void onShortButtonPress() {
  // toggle wifi on/off
  if (emotibit.getPowerMode() == EmotiBit::PowerMode::NORMAL_POWER) {
    emotibit.setPowerMode(EmotiBit::PowerMode::WIRELESS_OFF);
    Serial.println("PowerMode::WIRELESS_OFF");
  } else {
    emotibit.setPowerMode(EmotiBit::PowerMode::NORMAL_POWER);
    Serial.println("PowerMode::NORMAL_POWER");
  }
}

void onLongButtonPress() {
  emotibit.sleep();
}

void setup() {

        Serial.begin(SERIAL_BAUD);
        Serial.println("Serial started");
        delay(2000);  // short delay to allow user to connect to serial, if desired

        // Capture the calling ino into firmware_variant information
        String inoFilename = __FILE__;   // returns absolute path of ino file
        inoFilename.replace("/", "\\");  // conform to standard directory separator
        // extract filename only if directory separator found in absolute path
        if (inoFilename.lastIndexOf("\\") != -1) {
          inoFilename = inoFilename.substring((inoFilename.lastIndexOf("\\")) + 1, (inoFilename.indexOf(".")));
        }
        emotibit.setup(inoFilename);

        // Attach callback functions
        emotibit.attachShortButtonPress(&onShortButtonPress);
        emotibit.attachLongButtonPress(&onLongButtonPress);
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
          delay(1000);
          Serial.println("Connecting to WiFi...");
        }
        Serial.println("Connected to WiFi");

}


void loop() {
  HTTPClient http;
  // Dont forget to edit the 0.0.0.0 by your own ip address
  http.begin(url);

  emotibit.update();
  DynamicJsonDocument doc(1024);
  size_t dataAvailable = emotibit.readData(EmotiBit::DataType::PPG_GREEN, &data[0], dataSize);
  if (dataAvailable > 0) {
    for (size_t i = 0; i < dataAvailable && i < dataSize; i++) {

      PPG_GREEN += data[i];
    }
  }
  PPG_GREEN /= dataSize;
  dataAvailable = emotibit.readData(EmotiBit::DataType::PPG_RED, &data[0], dataSize);
  if (dataAvailable > 0) {
    for (size_t i = 0; i < dataAvailable && i < dataSize; i++) {
      PPG_RED += data[i];
    }
  }
  PPG_RED /= dataSize;

  dataAvailable = emotibit.readData(EmotiBit::DataType::PPG_INFRARED, &data[0], dataSize);
  if (dataAvailable > 0) {
    for (size_t i = 0; i < dataAvailable && i < dataSize; i++) {
      PPG_INFRARED += data[i];
    }
  }
  PPG_INFRARED /= dataSize;

  dataAvailable = emotibit.readData(EmotiBit::DataType::EDA, &data[0], dataSize);
  if (dataAvailable > 0) {
    for (size_t i = 0; i < dataAvailable && i < dataSize; i++) {
      EDA += data[i];
    }
  }

  EDA /= dataSize;

  // Read and print Temperature data
  dataAvailable = emotibit.readData(EmotiBit::DataType::TEMPERATURE_1, &data[0], dataSize);
  if (dataAvailable > 0) {
    for (size_t i = 0; i < dataAvailable && i < dataSize; i++) {
      TEMPERATURE_1 += data[i];
    }
  }
  TEMPERATURE_1 /= dataSize;

  // Read and print Humidity data
  dataAvailable = emotibit.readData(EmotiBit::DataType::HUMIDITY_0, &data[0], dataSize);
  if (dataAvailable > 0) {
    for (size_t i = 0; i < dataAvailable and i < dataSize; i++) {
      HUMIDITY_0 += data[i];
    }
  }
  HUMIDITY_0 /= dataSize;

  dataAvailable = emotibit.readData(EmotiBit::DataType::EDL, &data[0], dataSize);
  if (dataAvailable > 0) {
    for (size_t i = 0; i < dataAvailable and i < dataSize; i++) {
      EDL += data[i];
    }
  }
  EDL /= dataSize;

  dataAvailable = emotibit.readData(EmotiBit::DataType::EDR, &data[0], dataSize);
  if (dataAvailable > 0) {
    for (size_t i = 0; i < dataAvailable and i < dataSize; i++) {
      EDR += data[i];
    }
  }
  EDR /= dataSize;

  dataAvailable = emotibit.readData(EmotiBit::DataType::ACCELEROMETER_X, &data[0], dataSize);
  if (dataAvailable > 0) {
    for (size_t i = 0; i < dataAvailable and i < dataSize; i++) {
      ACCELEROMETER_X += data[i];
    }
  }
  ACCELEROMETER_X /= dataSize;

  dataAvailable = emotibit.readData(EmotiBit::DataType::ACCELEROMETER_Y, &data[0], dataSize);
  if (dataAvailable > 0) {
    for (size_t i = 0; i < dataAvailable and i < dataSize; i++) {
      ACCELEROMETER_Y += data[i];
    }
  }
  ACCELEROMETER_Y /= dataSize;

  dataAvailable = emotibit.readData(EmotiBit::DataType::ACCELEROMETER_Z, &data[0], dataSize);
  if (dataAvailable > 0) {
    for (size_t i = 0; i < dataAvailable and i < dataSize; i++) {
      ACCELEROMETER_Z += data[i];
    }
  }
  ACCELEROMETER_Z /= dataSize;


  ACCELERATION = sqrt(ACCELEROMETER_X * ACCELEROMETER_X + ACCELEROMETER_Y * ACCELEROMETER_Y + ACCELEROMETER_Z * ACCELEROMETER_Z);
  if(ACCELERATION < 1 && ACCELERATION >= 0){
    Serial.print("Average Acceleration: ");
  Serial.print(0 );
  Serial.println("m/s^2");
  } else{
    Serial.print("Average Acceleration: ");
    Serial.print(ACCELERATION );
    Serial.println("m/s^2");
  }
  

  float magnitude = ACCELERATION;

  if (magnitude > previousMagnitude && !peaked) {
    step_count++;
    if (step_count >= NUM_SAMPLES) {
      if (magnitude - previousMagnitude > THRESHOLD) {
          Serial.print("Step detected! ");
          Serial.print("Total Steps: ");
          Serial.println(step_count);
          }
          peaked = true;
      }

    } else {
        peaked = false;
        step_count = 0;
    }

    previousMagnitude = magnitude;

  delay(150);   

  DISTANCE_COVERED = step_count * 0.65 / 1000.0;
  Serial.print("Distance covered: ");
  Serial.print(DISTANCE_COVERED, 3 );
  Serial.println("km");



      dataAvailable = emotibit.readData(EmotiBit::DataType::GYROSCOPE_X, &data[0], dataSize);
      if (dataAvailable > 0) {
        for (size_t i = 0; i < dataAvailable and i < dataSize; i++) {
          GYROSCOPE_X += data[i];
        }
      }

    GYROSCOPE_X /= dataSize;

    dataAvailable = emotibit.readData(EmotiBit::DataType::GYROSCOPE_Y, &data[0], dataSize);
    if (dataAvailable > 0) {
      for (size_t i = 0; i < dataAvailable and i < dataSize; i++) {
        GYROSCOPE_Y += data[i];
      }
    }
    GYROSCOPE_Y /= dataSize;

    dataAvailable = emotibit.readData(EmotiBit::DataType::GYROSCOPE_Z, &data[0], dataSize);
    if (dataAvailable > 0) {
      for (size_t i = 0; i < dataAvailable and i < dataSize; i++) {
        GYROSCOPE_Z += data[i];
      }
    }
    GYROSCOPE_Z /= dataSize;

    dataAvailable = emotibit.readData(EmotiBit::DataType::BATTERY_PERCENT, &data[0], dataSize);
    if (dataAvailable > 0) {
      for (size_t i = 0; i < dataAvailable and i < dataSize; i++) {
        BATTERY_PERCENT += data[i];
      }
    }
    BATTERY_PERCENT /= dataSize;
    // doc["identity"] = identity;
    // doc["EDA"] = EDA;
    // doc["HUMIDITY_0"] = HUMIDITY_0;
    // doc["EDL"] = EDL;
    // doc["EDR"] = EDR;
    // doc["PPG_GREEN"] = PPG_GREEN;
    // doc["PPG_RED"] = PPG_RED;
    // doc["PPG_INFRARED"] = PPG_INFRARED;
    // doc["TEMPERATURE_1"] = TEMPERATURE_1;
    // doc["ACCELEROMETER_X"] = ACCELEROMETER_X;
    // doc["ACCELEROMETER_Y"] = ACCELEROMETER_Y;
    // doc["ACCELEROMETER_Z"] = ACCELEROMETER_Z;
    // doc["GYROSCOPE_X"] = GYROSCOPE_X;
    // doc["GYROSCOPE_Y"] = GYROSCOPE_Y;
    // doc["GYROSCOPE_Z"] = GYROSCOPE_Z;
    // doc["BATTERY_PERCENT"] = BATTERY_PERCENT;
    // doc["AVR_ACCELERATION"] = AVR_ACCELERATION;
    // doc["STEP_COUNT"] = STEP_COUNT;
    // doc["DISTANCE_COVERED"] = DISTANCE_COVERED;

    // String requestBody;
    // serializeJson(doc, requestBody);
    // // Specify content-type header
    // http.addHeader("Content-Type", "application/json");
    // // Send POST request
    // int httpCode = http.POST(requestBody);
    // // Process response
    // if (httpCode <= 0) {
    //   Serial.println("Error on sending POST");
    // }
    //   // Close connection
    //   http.end();
    //   delay(500);
}