#include <ArduinoJson.h>

int zigbeeDim[6][6];
int zigbeeDec[6];

int zigbeeData[31];  // Declare an array to store data from zigbeeDim
//int zigbeeDataIndex = 5;  // Indeks awal untuk menyimpan data ke zigbeeData

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

//  // Use strtok() to separate values in the status with a comma (',')
//  while (token != NULL && i < 36) {
//    zigbeeDim[i] = atoi(token); // Convert token to integer and store it in the array
//    token = strtok(NULL, ",");
//    i++;
//  }
//}
  // Process the dimmer data as needed
  // Add your logic here based on the parsed data
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
      parseDimmer(status);
      /*******************************************************************************
      // Display values from the parsed dimmer data
      Serial.println("Dimmer Data:");
      //      for (int i = 0; i < 36; i++) {
      //        Serial.print(zigbeeDim[i]);
      //        Serial.print(" ");
      //
      //        // Check if i is a multiple of 6 and print the address value
      //        if ((i + 1) % 6 == 0) {
      //          int addressIndex = (i + 1) / 6 - 1; // Calculate address index
      //          Serial.print("Address[");
      //          Serial.print(addressIndex);
      //          Serial.print("]: ");
      //          // Add your logic here to access the address array based on addressIndex
      //          // For example, if you have an 'address' array, you can access it like this:
      //          // Serial.print(address[addressIndex]);
      //          Serial.println();
      //        }

      for (uint8_t i = 0; i < 6; i++) {
        Serial.printf("Address [%u]:[ ", i);
        for (uint8_t j = 0; j < 6; j++) {
          Serial.print(zigbeeDim[i][j]);
          Serial.print(",");
        }
        Serial.println("]");
      }
      
      for (uint8_t i = 0; i <6; i++) {
        zigbeeData[5]= i;
        zigbeeData[6] = zigbeeDim[i][0];
        zigbeeData[7] = zigbeeDim[i][1];
        zigbeeData[8] = zigbeeDim[i][2];
        zigbeeData[9] = zigbeeDim[i][3];
        zigbeeData[10] = zigbeeDim[i][4];
        zigbeeData[11] = zigbeeDim[i][5];
        Serial.printf("Zigbee Data ke [%u]:[ ", i);
        for (uint8_t j = 0; j < 30; j++) {
          //Serial2.write(zigbeeData[i]);
          Serial.print(zigbeeData[j]);
          Serial.print(",");
        }
        Serial.println("]");
        delay(5);
      }
      
      /*parseDimmer(status);
        Serial.println("Dimmer Data:");
        //
        for (int i = 0; i < 36; i++) {
        Serial.print(zigbeeDim[i]);
        Serial.print(" ");

        // Check if i is a multiple of 6 and store the value in zigbeeData
        if ((i + 1) % 6 == 0) {
          int addressIndex = (i + 1) / 6 - 1; // Calculate address index
          Serial.print("Address[");
          Serial.print(addressIndex);
          Serial.print("]: ");

          // Check if zigbeeDataIndex is within the range [5, 10]
          if (zigbeeDataIndex >= 5 && zigbeeDataIndex <= 10) {
            // Store zigbeeDim[i] in zigbeeData[zigbeeDataIndex]
            zigbeeData[zigbeeDataIndex] = zigbeeDim[i];
            Serial.print(zigbeeData[zigbeeDataIndex]);
          } else {
            Serial.print("Out of range");
          }

          // Increment zigbeeDataIndex
          zigbeeDataIndex++;

          Serial.println();
        }*/

      //      }
      /*******************************************************************************/
      // Add any additional logic here for handling dimmer data

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
      parseKinetic(status);
      /*******************************************************************************
      // Display values from the parsed kinetic data
      Serial.println("Kinetic Data:");
      // Print each array conversion from binary to decimal
      for (int i = 0; i < 6; i++) {
        Serial.print("zigbeeDec ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(zigbeeDec[i]);
      }
      *******************************************************************************/
      for (int i = 0; i < 6; i++) {
        zigbeeData[i + 5] = zigbeeDec[i];
      }        
      zigbeeData[11] = doc["OK"];      
      for (int i = 0; i < 17; i++) {
        Serial1.write(zigbeeData[i]);
        Serial.print(zigbeeData[i], HEX);
        Serial.print(" ");
      }
      // Add any additional logic here for handling kinetic data
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
