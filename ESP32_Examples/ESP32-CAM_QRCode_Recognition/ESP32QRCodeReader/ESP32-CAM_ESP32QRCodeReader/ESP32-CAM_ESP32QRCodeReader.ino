//https://github.com/alvarowolfx/ESP32QRCodeReader

#include <Arduino.h>
#include "soc/soc.h"             //For power instability non-reset 
#include "soc/rtc_cntl_reg.h"    //For power instability non-reset 
#include "ESP32CameraPins.h"
#include "ESP32QRCodeReader.h"

ESP32QRCodeReader reader(CAMERA_MODEL_AI_THINKER);
struct QRCodeData qrCodeData;

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  //Disable brownout reset
    
  Serial.begin(115200);
  Serial.setDebugOutput(true);  //Enable debug output
  Serial.println();

  reader.setup();
  //reader.setDebug(true);
  Serial.println("Setup QRCode Reader");

  reader.begin();
  Serial.println("Begin QR Code reader");
}

void loop() {
  if (reader.receiveQrCode(&qrCodeData, 100)) {
    if (qrCodeData.valid) {
      Serial.print("Payload: ");
      String qrcode = (const char *)qrCodeData.payload;
      Serial.print(qrcode);
    } else {
      Serial.print("Invalid: ");
      String qrcode = (const char *)qrCodeData.payload;
      Serial.print(qrcode);
    }
  }
  delay(300);
}
