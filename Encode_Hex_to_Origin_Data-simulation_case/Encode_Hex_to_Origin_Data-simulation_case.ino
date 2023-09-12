/************************CMD 3002 Dimmer ******************************************************
//09:17:58.400 -> Received data: {"feedback":"Dimmer","CMD":3002,"status":"0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,","OK":1}
//09:17:58.400 -> Feedback: Dimmer
//09:17:58.400 -> CMD: 3002
//09:17:58.400 -> Status: 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,
//09:17:58.447 -> OK: 1
                                                 18 19  20  21  22  23  24  --> Receiver
//09:17:58.447 -> Zigbee Data ke [0]:[ 0 0 0 0 0 0  FF  EF  12  BC  FE  AE  1 0 0 0 0 ]
                                                    36  35  34  33  32  31 --> bit
                                      --> parsingDecimal-->return 255,150,120,20,10,50
                                      String newStatus+= return+",";
                                       255,150,120,20,10,50,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,100,100,100,100
                                                    
//09:17:58.447 -> Zigbee Data ke [1]:[ 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 ]
                                                   30 29 28 27 26 25 --> bit
//09:17:58.447 -> Zigbee Data ke [2]:[ 0 0 0 0 0 2 0 0 0 0 0 0 1 0 0 0 0 ]
                                                   24 23 22 21 20 19 --> bit
//09:17:58.447 -> Zigbee Data ke [3]:[ 0 0 0 0 0 3 0 0 0 0 0 0 1 0 0 0 0 ]
                                                   18 17 16 15 14 13 --> bit
//09:17:58.447 -> Zigbee Data ke [4]:[ 0 0 0 0 0 4 0 0 0 0 0 0 1 0 0 0 0 ]
                                                   12 11 10 9  8  7  --> bit
//09:17:58.447 -> Zigbee Data ke [5]:[ 0 0 0 0 0 5 FF FF FF FF FF FF 1 0 0 0 0 ]
                                                   6  5  4  3  2  1 --> bit

/************************CMD 3000 Kinetic ******************************************************
//09:53:14.084 -> Received data: {"feedback":"Kinetic","CMD":3000,"status":"0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,","OK":1}
//09:53:14.084 -> Feedback: Kinetic
//09:53:14.084 -> CMD: 3000
//09:53:14.084 -> Status: 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,
//09:53:14.131 -> OK: 1
//09:53:14.131 -> 0 0 0 0 0 0 0 0 0 0 3F 1 0 0 0 0 0
**********************************************************************************************/
#define u8 unsigned char
#include <ArduinoJson.h>
String dimstatusData[7]; 
int kinstatusData[6];
String binaryValue[6];
String data;
u8 RX2_Buff[200];

int cmd1; // Declare cmd1 as an integer
String strSend; // Declare strSend as a String
int timer1; // Declare timer1 as an integer

String encodeKinetic(String data) {
  String encodedData = "";
  // Memisahkan data berdasarkan spasi
  String extractData[data.length()]; // Sesuaikan ukuran array sesuai dengan kebutuhan
  int indexNum = 0;
  while (data.length() > 0) {
    int spaceIndex = data.indexOf(' ');
    if (spaceIndex >= 0) {
      extractData[indexNum] = data.substring(0, spaceIndex);
      data = data.substring(spaceIndex + 1);
    } else {
      extractData[indexNum] = data;
      data = "";
    }
    indexNum++;
  }

  // Ambil indeks 5 hingga 10 dari extractData, masukkan ke variabel statusData
  Serial.print("kinstatusData: ");
  for (int i = 5; i < 11; i++) {
      kinstatusData[i - 5] = strtol(extractData[i].c_str(), NULL, HEX); //Konversi Hex ke Dec
      binaryValue[i - 5] = String(kinstatusData[i - 5], BIN);   //Konversi Dec ke Bin      
  }
  Serial.println();

  // Construct the encoded data as a string
  for (int i = 0; i < 6; i++) {
    encodedData += binaryValue[i];
  }

  return encodedData;

//      Serial.print(kinstatusData[i - 5]);
//      Serial.print(" ");
  
//  // Mencetak extractData
//  Serial.print("Extracted Data: ");
//  for (int i = 0; i < indexNum; i++) {
//    Serial.print(extractData[i]);
//    Serial.print(" ");
//  }
//  Serial.println();

//  Serial.print("statusData in BIN: ");
//  for (int i = 0; i < 6; i++) {
//    Serial.print(binaryValue[i]);
//    Serial.print(",");
//  }
//  Serial.println();

}

void encodeDimmer(String data){
  // Memisahkan data berdasarkan spasi
  String extractData[data.length()]; // Sesuaikan ukuran array sesuai dengan kebutuhan
  int indexNum = 0;
  while (data.length() > 0) {
    int spaceIndex = data.indexOf(' ');
    if (spaceIndex >= 0) {
      extractData[indexNum] = data.substring(0, spaceIndex);
      data = data.substring(spaceIndex + 1);
    } else {
      extractData[indexNum] = data;
      data = "";
    }
    indexNum++;
  }

//  // Mencetak extractData
//  Serial.print("Extracted Data: ");
//  for (int i = 0; i < indexNum; i++) {
//    Serial.print(extractData[i]);
//    Serial.print(" ");
//  }
//  Serial.println();

  // Ambil indeks 6 hingga 11 dari extractData, masukkan ke variabel statusData
//  Serial.print("dimstatusData: ");
  for (int i = 5; i < 12; i++) {
      dimstatusData[i - 6] = strtol(extractData[i].c_str(), NULL, HEX); //Konversi Hex ke Dec
//      RX2_Buff[i - 6] = strtol(extractData[i].c_str(), NULL, HEX); //Konversi Hex ke Dec
//      Serial.print(dimstatusData[i - 6]);
//      Serial.print(" ");
  }
  Serial.println();

  //store to matrix here
    
}

void processData(String receivedData) {
  DynamicJsonDocument backDoc(1024);
  int code = receivedData.toInt(); // Mengubah data menjadi integer
  String encodedData ;

  switch (code) {
    case 3000:
      encodedData = encodeKinetic(receivedData);
      //RX2_Buff[19] = encodedData.toInt(); // Set RX2_Buff[19] to the encodedData as an integer
      //encodeKinetic(receivedData);
      backDoc["data"] = "feedback";
      backDoc["CMD"] = cmd1;
      //backDoc["Time"] = timer1;
      backDoc["address"] = RX2_Buff[18];
      backDoc["status"] = encodedData;
      
//      backDoc["status"] = RX2_Buff[20];
//      backDoc["status"] = RX2_Buff[21];
//      backDoc["status"] = RX2_Buff[22];
//      backDoc["status"] = RX2_Buff[23];
//      backDoc["OK"] = RX2_Buff[24];
      //deserializeJson(backDoc, strSend);
      serializeJson(backDoc, strSend);
      // serializeJson(backDoc, Serial);
      // Print the JSON string to Serial Monitor
      Serial.println("Serialized JSON:");
      Serial.println(strSend);
       //serializeJson(backDoc, SoftSerial);
      break;
      
    case 3002:
      encodeDimmer(receivedData);
      break;
    default:
      // Kode yang tidak dikenali
      Serial.println("Kode tidak dikenali");
      break;
  }
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  if (Serial.available()) {
    // Baca data dari port serial
    data = Serial.readStringUntil('\n');
    // Panggil fungsi encode untuk memproses data
    processData(data);
  }
}
