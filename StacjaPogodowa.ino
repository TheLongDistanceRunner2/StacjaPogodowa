#include <Adafruit_BMP085.h>

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

/*************************************************** 
  This is an example for the BMP085 Barometric Pressure & Temp Sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> https://www.adafruit.com/products/391

  These pressure and temperature sensors use I2C to communicate, 2 pins
  are required to interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
// EOC is not used, it signifies an end of conversion
// XCLR is a reset pin, also not used here

Adafruit_BMP085 bmp;

//==============================================================
// MAIN PROGRAM:
  
void setup() {
  //==================================
  // USTAWIENIE CZUJNIKA TEMPERATURY:
  Serial.begin(9600);
  
  if (!bmp.begin()) {
  	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  	while (1) {}
  }
  
  //==================================
  // USTAWIENIE DIOD, PRZYCISKÓW I BUZZERA:

    // przypisanie pinów:
  pinMode(13, OUTPUT); // 30 *C - Dioda czerwona
  pinMode(12, OUTPUT); // 28 *C - Dioda czerwona
  pinMode(11, OUTPUT); // 26 *C - Dioda czerwona
  pinMode(10, OUTPUT); // 24 *C - Dioda czerwona
  pinMode(9, OUTPUT); // 22 *C - Dioda żółta
  pinMode(8, OUTPUT); // 20 *C - Dioda żołta
  pinMode(7, OUTPUT); // 18 *C - Dioda żółta
  pinMode(6, INPUT_PULLUP); //Przycisk termometru
  pinMode(2, INPUT_PULLUP); //Przycisk buzzera
    
    // poczatkowe wyłączenie diod:
  digitalWrite(13, LOW); //Wyłączenie diod
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  digitalWrite(7, LOW);
  
}
  
void loop() {
  double temperatura = getTemperature();
  double cisnienie = getPressure();
  float wysokoscNPM = getAltitude();

  // Jeśli wciśnięto przycisk termometru i wyświetlacza:  
  if (digitalRead(6) == LOW) {
    // to zapalamy diody:
    lightDiodesON(temperatura);
    // i wyświetlamy na konsolę:
    printToSerial(temperatura, cisnienie, wysokoscNPM);

    delay(5000); 
  }
  else { //Jeśli warunek nie został spełniony (przycisk nie jest wciśnięty)
    lightDiodesOFF();
  }

  // jeśli wciśnięto przycisk buzzera:
  if (digitalRead(2) == LOW) {
    // jeśli temperatura jest większa od 22 *C: 
    if(temperatura >= 22.0) {
      // to gramy wesołą melodię:
      playHappyMelody();
    }
    else {
      // w przeciwnym razie gramy smutną meoldię:
      playSadMelody();
    }
  }
}

//==========================================================
// FUNCTIONS:

double getTemperature() {
  return bmp.readTemperature();
}

double getPressure() {
  return bmp.readPressure() / 100;
}

float getAltitude() {
  return bmp.readAltitude();
}

void getWeatherData() {
    Serial.print("Temperatura = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    
    Serial.print("Ciśnienie = ");
    Serial.print(bmp.readPressure() / 100);
    Serial.println(" hPa");
    
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
//    Serial.print("Wysokość = ");
//    Serial.print(bmp.readAltitude());
//    Serial.println(" m");

//    Serial.print("Ciśnienie nad poziomem morza (obliczone) = ");
//    Serial.print(bmp.readSealevelPressure() / 100);
//    Serial.println(" hPa");

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
    Serial.print("Wysokość nad poziomem morza = ");
    Serial.print(bmp.readAltitude(100800));
    Serial.println(" m");
    
    Serial.println();
    delay(5000);  
}

void lightDiodesON(double temp) {
  double temperatura = temp;

  if (temperatura < 16.0) {
    //Wyłączamy wszystie diody:
    digitalWrite(13, LOW); 
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
  }
  else if (temperatura >= 16.0 && temperatura < 18.0) {
    // zapalamy 1 diodę:
    digitalWrite(13, LOW); 
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    digitalWrite(7, HIGH);
  }
  else if (temperatura >= 18.0 && temperatura < 20.0) {
    digitalWrite(13, LOW); 
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
  }
  else if (temperatura >= 20.0 && temperatura < 22.0) {
    digitalWrite(13, LOW); 
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
  }
  else if (temperatura >= 22.0 && temperatura < 24.0) {
    digitalWrite(13, LOW); 
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
  }
  else if (temperatura >= 24.0 && temperatura < 26.0) {
    digitalWrite(13, LOW); 
    digitalWrite(12, LOW);
    digitalWrite(11, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
  }
  else if (temperatura >= 26.0 && temperatura < 28.0) {
    digitalWrite(13, LOW); 
    digitalWrite(12, HIGH);
    digitalWrite(11, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
  }
  else if (temperatura >= 28.0 && temperatura < 30.0) {
    digitalWrite(13, HIGH); 
    digitalWrite(12, HIGH);
    digitalWrite(11, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
  }
}

void lightDiodesOFF() {
    digitalWrite(13, LOW); 
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
}

void printToSerial(double temperature, double pressure, float altitude) {
  Serial.println("================================");
  Serial.println("          NOWY POMIAR:          ");
  Serial.print("Temperatura = ");
  Serial.print(temperature);
  Serial.print(" *C");
  Serial.println();
  Serial.print("Ciśnienie = ");
  Serial.print(pressure);
  Serial.print(" hPa");
  Serial.println();
  Serial.print("Wysokość n.p.m. = ");
  Serial.print(altitude);
  Serial.print(" m. (na podstawie ciśnienia)");
  Serial.println();
  Serial.println("================================");
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
}

void playHappyMelody() {
    // change this to make the song slower or faster
  int tempo = 160;
  
  // change this to whichever pin you want to use
  int buzzer = 3;
  
  // notes of the moledy followed by the duration.
  // a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
  // !!negative numbers are used to represent dotted notes,
  // so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
  int melody[] = {
  
    // Take on me, by A-ha
    // Score available at https://musescore.com/user/27103612/scores/4834399
    // Arranged by Edward Truong
  
    NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
    REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
    NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
    REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  };
  
  // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
  // there are two values per note (pitch and duration), so for each note there are four bytes
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;
  
  // this calculates the duration of a whole note in ms
  int wholenote = (60000 * 4) / tempo;
  
  int divider = 0, noteDuration = 0;

    // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}

void playSadMelody() {
  // change this to make the song slower or faster
  int tempo = 80;
  
  // change this to whichever pin you want to use
  int buzzer = 3;
  
  // notes of the moledy followed by the duration.
  // a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
  // !!negative numbers are used to represent dotted notes,
  // so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
  int melody[] = {
  
    // The Godfather theme
    // Score available at https://musescore.com/user/35463/scores/55160
  
    REST, 4, REST, 8, REST, 8, REST, 8, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8, //1
    NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_A4, 8, NOTE_F4, 8, NOTE_G4, 8,
    NOTE_E4, 2, 
//NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8,
//    NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_E4, 8, NOTE_DS4, 8,
//    
//    NOTE_D4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8, //5
//    NOTE_B4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8,
//    NOTE_A4, 2, NOTE_C4, 8, NOTE_C4, 8, NOTE_G4, 8, 
//    NOTE_F4, 8, NOTE_E4, 8, NOTE_G4, 8, NOTE_F4, 8, NOTE_F4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_GS4, 8,
//  
//    NOTE_A4, 2, REST,8, NOTE_A4, 8, NOTE_A4, 8, NOTE_GS4, 8, //9
//    NOTE_G4, 2, NOTE_B4, 8, NOTE_A4, 8, NOTE_F4, 8, 
//    NOTE_E4, 2, NOTE_E4, 8, NOTE_G4, 8, NOTE_E4, 8,
//    NOTE_D4, 2, NOTE_D4, 8, NOTE_D4, 8, NOTE_F4, 8, NOTE_DS4, 8, 
//     
//    NOTE_E4, 2, REST, 8, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8, //13
 
  };
  
  // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
  // there are two values per note (pitch and duration), so for each note there are four bytes
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;
  
  // this calculates the duration of a whole note in ms
  int wholenote = (60000 * 4) / tempo;
  
  int divider = 0, noteDuration = 0;


  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}
