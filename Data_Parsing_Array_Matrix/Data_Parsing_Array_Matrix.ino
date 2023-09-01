#include <ArduinoJson.h>

void setup() {
  Serial.begin(115200);
}

void loop() {
  if (Serial.available()) {
    
    // Read incoming data until newline ('\n')
    String jsonString = Serial.readStringUntil('\n');
    Serial.println("Received data: " + jsonString);
    DynamicJsonDocument doc(512);
    
    // Parse JSON
    DeserializationError error = deserializeJson(doc, jsonString);
  
    if (error) {
      Serial.print("Error parsing JSON: ");
      Serial.println(error.c_str());
      return;
    }
    
    // Mendapatkan nilai dari kunci yang diinginkan
    const char* feedback = doc["feedback"];
    int cmd = doc["CMD"];
    const char* status = doc["status"];
    int ok = doc["OK"];

    // Melakukan sesuatu dengan nilai yang didapatkan
    Serial.println("Feedback: " + String(feedback));
    Serial.println("CMD: " + String(cmd));
    Serial.println("Status: " + String(status));
    Serial.println("OK: " + String(ok));

    // Remove commas from status
    String statusString = "";
    for (int i = 0; i < strlen(status); i++) {
      if (status[i] != ',') {
        statusString += status[i];
      }
    }
    Serial.println ("Status string: " + statusString);
    
    int zigbeeBin[6]; //array zigbeevalue dalam binary integer sepanjang 6 bit   
    int zigbeeDec[6]; //array zigbeevalue dalam desimal sepanjang 6 bit
    
    // Split the statusString into 6-bit zigbeeBinStr and convert them to decimal
    for (int i = 0; i < 6; i++) {
      String zigbeeBinStr = statusString.substring(i * 6, (i * 6) + 6); // Mengambil potongan 6 karakter
      zigbeeBin[i]=zigbeeBinStr.toInt(); // meletakan 6 karakter yang diambil dalam bentuk binary integer
      
      //debugging
      Serial.println ("zigbeeBinStr[" + String (i) + "] = " + zigbeeBinStr + ", binary integer = " + zigbeeBin[i]);
      
      zigbeeDec[i] = strtol(zigbeeBinStr.c_str(), NULL, 2); // Mengonversi potongan biner ke desimal
    }
    
    Serial.println("===================================DEBUGGING====================================");
    
    // Print tiap array konversi potongan biner ke desimal
    for (int i = 0; i < 6; i++) {
      Serial.print("zigbeeBin ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(zigbeeBin[i]);
    }
      
    // Print tiap array konversi potongan biner ke desimal
    for (int i = 0; i < 6; i++) {
      Serial.print("zigbeeDec ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(zigbeeDec[i]);
    }
  }
}
