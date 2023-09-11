int address[6] = {0, 1, 2, 3, 4, 5}; // Inisialisasi array 'address' dengan 6 elemen
int zigbeedim[36]; // Deklarasi array 'zigbeedim' tanpa inisialisasi
int zigbesend[36] = {0}; // Deklarasi dan inisialisasi array 'zigbesend'

void setup() {
  Serial.begin(115200); // Inisialisasi komunikasi serial dengan baud rate 115200

  // Mengisi 'zigbeedim' dengan angka 0 hingga 35
  for (int i = 0; i < 36; i++) {
    zigbeedim[i] = i;
  }
}

void loop() {
  int index = 5; // Mulai dari indeks 5 di 'zigbesend'

  for (int i = 0; i < 6; i++) { // Loop untuk nilai 'i' dari 0 hingga 5 (indeks 'address')
    for (int j = 0; j <= 6; j++) { // Loop untuk nilai 'j' dari 0 hingga 6 (termasuk 6)
      if (j < 6) {
        zigbesend[index] = zigbeedim[i * 6 + j]; // Mengisi elemen 'zigbesend' dengan nilai dari 'zigbeedim' sesuai pola
        Serial.print("i: ");
        Serial.print(i);
        Serial.print(", j: ");
        Serial.print(j);
        Serial.print(", zigbesend[");
        Serial.print(index);
        Serial.print("]: ");
        Serial.println(zigbesend[index]); // Mencetak nilai i, j, dan zigbesend[index] ke Serial Monitor
        index++; // Menaikkan 'index' untuk mengakses elemen berikutnya di 'zigbesend'
      } else {
        zigbesend[11] = address[i]; // Mengisi elemen 'zigbesend' sesuai dengan 'address'
        Serial.print("i: ");
        Serial.print(i);
        Serial.print(", j: 6, zigbesend[11]: ");
        Serial.println(zigbesend[11]); // Mencetak nilai i, j, dan zigbesend[11] ke Serial Monitor
      }
    }
  }

  delay(1000); // Tunggu 1 detik sebelum mengulangi
}
