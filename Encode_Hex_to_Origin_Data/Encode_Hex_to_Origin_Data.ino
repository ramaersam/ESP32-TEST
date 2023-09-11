/************************CMD 3002 Dimmer ******************************************************
//09:17:58.400 -> Received data: {"feedback":"Dimmer","CMD":3002,"status":"0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,","OK":1}
//09:17:58.400 -> Feedback: Dimmer
//09:17:58.400 -> CMD: 3002
//09:17:58.400 -> Status: 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,
//09:17:58.447 -> OK: 1
//09:17:58.447 -> Zigbee Data ke [0]:[ 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 ]
//09:17:58.447 -> Zigbee Data ke [1]:[ 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 ]
//09:17:58.447 -> Zigbee Data ke [2]:[ 0 0 0 0 0 2 0 0 0 0 0 0 1 0 0 0 0 ]
//09:17:58.447 -> Zigbee Data ke [3]:[ 0 0 0 0 0 3 0 0 0 0 0 0 1 0 0 0 0 ]
//09:17:58.447 -> Zigbee Data ke [4]:[ 0 0 0 0 0 4 0 0 0 0 0 0 1 0 0 0 0 ]
//09:17:58.447 -> Zigbee Data ke [5]:[ 0 0 0 0 0 5 FF FF FF FF FF FF 1 0 0 0 0 ]

/************************CMD 3000 Kinetic ******************************************************
//09:53:14.084 -> Received data: {"feedback":"Kinetic","CMD":3000,"status":"0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,","OK":1}
//09:53:14.084 -> Feedback: Kinetic
//09:53:14.084 -> CMD: 3000
//09:53:14.084 -> Status: 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,
//09:53:14.131 -> OK: 1
//09:53:14.131 -> 0 0 0 0 0 0 0 0 0 0 3F 1 0 0 0 0 0
**********************************************************************************************/

int statusData[6]; 
String binaryValue[6];
String data;

void setup() {
  Serial.begin(115200);
}

void encodeKinetic(String data) {
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

  // Mencetak extractData
  Serial.print("Extracted Data: ");
  for (int i = 0; i < indexNum; i++) {
    Serial.print(extractData[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Ambil indeks 5 hingga 10 dari extractData, masukkan ke variabel statusData
  Serial.print("Status Data: ");
  for (int i = 5; i < 11; i++) {
      statusData[i - 5] = strtol(extractData[i].c_str(), NULL, HEX); //Konversi Hex ke Dec
      binaryValue[i - 5] = String(statusData[i - 5], BIN);   //Konversi Dec ke Bin
      Serial.print(statusData[i - 5]);
      Serial.print(" ");
  }
  Serial.println();

  Serial.print("statusData in BIN: ");
  for (int i = 0; i < 6; i++) {
    Serial.print(binaryValue[i]);
    Serial.print(",");
  }
  Serial.println();
}

void loop() {
  if (Serial.available()) {
    // Baca data dari port serial
    data = Serial.readStringUntil('\n');
    // Panggil fungsi encode untuk memproses data
    encodeKinetic(data);
  }
}
