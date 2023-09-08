#include <ArduinoJson.h>

int zigbeeDim[6][6];
int zigbeeDec[6];

int zigbeeData[31];  // Declare an array to store data from zigbeeDim

// Function to parse dimmer data
void parseDimmer(const char* status) {
  int i = 0;
  char* token = strtok(const_cast<char*>(status), ",");

  // Use strtok() to separate values in the status with a comma (',')
  while (token != NULL) {
    for (uint8_t i = 0; i < 6; i++) {
      for (uint8_t j = 0; j < 6; j++) {
        zigbeeDim[i][j] = atoi(token); // Convert token to integer and store it in the array
        token = strtok(NULL, ",");
        //    i++;
      }
    }
  }

// Function to parse kinetic data
void parseKinetic(const char* status) {
  String statusString = "";

  // Remove commas from the status
  for (int i = 0; i < strlen(status); i++) {
    if (status[i] != ',') {
      statusString += status[i];
    }
  }

  // Convert each 6-character substring to an integer
  for (int i = 0; i < 6; i++) {
    String zigbeeBinStr = statusString.substring(i * 6, (i * 6) + 6);
    zigbeeDec[i] = strtol(zigbeeBinStr.c_str(), NULL, 2);
  }

  // Process the kinetic data as needed
  // Add your logic here based on the parsed data
}

// Function to handle JSON data
void handleJsonData(const String& jsonString) {
  DynamicJsonDocument doc(512);

  // Parse the received JSON data
  DeserializationError error = deserializeJson(doc, jsonString);

  if (error) {
    Serial.print("Error parsing JSON: ");
    Serial.println(error.c_str()); // Display an error message if there's a parsing error
    return;
  }

  // Extract values from the JSON
  const char* feedback = doc["feedback"];
  int cmd = doc["CMD"];
  const char* status = doc["status"];
  int ok = doc["OK"];

  // Display values from the JSON
  Serial.println("Feedback: " + String(feedback));
  Serial.println("CMD: " + String(cmd));
  Serial.println("Status: " + String(status));
  Serial.println("OK: " + String(ok));

  // Handle different CMD values using a switch statement
  switch (cmd) {
    case 3002:
      //{"feedback":"Dimmer","CMD":3002,"status":"0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,","OK":1}
      //status: from bit36 --> bit1 ( show the value every bit) Value from 0 to 255)    
      parseDimmer(status);
        for (uint8_t i = 0; i <6; i++) {
        zigbeeData[5]= i; //address
        zigbeeData[6] = zigbeeDim[i][0];
        zigbeeData[7] = zigbeeDim[i][1];
        zigbeeData[8] = zigbeeDim[i][2];
        zigbeeData[9] = zigbeeDim[i][3];
        zigbeeData[10] = zigbeeDim[i][4];
        zigbeeData[11] = zigbeeDim[i][5];
        zigbeeData[12] = doc["OK"]; 
        Serial.printf("Zigbee Data ke [%u]:[ ", i); //for debugging
        for (uint8_t j = 0; j < 17; j++) {
          Serial1.write(zigbeeData[j]);
          Serial.print(zigbeeData[j],HEX);
          Serial.print(" ");
        }
        Serial.println("]"); //for debugging
        delay(5);
      }    
      break;

    case 3000:
      //{"feedback":"Kinetic","CMD":3000,"status":"0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,","OK":1}
      //status: from bit36 --> bit1 ( show the value every bit) Kinetic Value only 0(Off) and 1(On)
      parseKinetic(status);
      for (int i = 0; i < 6; i++) {
        zigbeeData[i + 5] = zigbeeDec[i];
      }        
      zigbeeData[11] = doc["OK"];      
      for (int i = 0; i < 17; i++) {
        Serial1.write(zigbeeData[i]);
        Serial.print(zigbeeData[i], HEX);
        Serial.print(" ");
      }
      break;

    // Add more cases for different CMD values as needed
    default:
      Serial.println("Unknown CMD value");
      break;
  }
}

void setup() {
  Serial.begin(115200); // Initialize serial communication with a baud rate of 115200
}

void loop() {
  if (Serial.available()) {
    // Read data until a newline character ('\n') is received
    String jsonString = Serial.readStringUntil('\n');
    Serial.println("Received data: " + jsonString); // Print the received data

    // Call the function to handle JSON data
    handleJsonData(jsonString);
  }
}
