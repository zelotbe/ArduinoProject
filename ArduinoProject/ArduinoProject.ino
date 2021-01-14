#include <Keypad.h> //Keypad Library
#include <LiquidCrystal.h> //LCD library
#include <EEPROM.h> //Library om data te onthouden wanneer de Arduino uitstaat, momenteel niet gebruikt

//ALLES VOOR HET TOETSENPANEEL
const byte RIJ = 4;
const byte KOLOM = 4;

char toetsen[RIJ][KOLOM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
//Pinnen definiëren voor de keypad
byte rowPins[RIJ] = {A0, A1, A2, A3}; 
byte colPins[KOLOM] = {A4, A5, 3, 2};

//initialize an instance of class NewKeypad
Keypad paneel = Keypad( makeKeymap(toetsen), rowPins, colPins, RIJ, KOLOM);

LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // Pinnen definiëren voor het LCD scherm
char wachtwoord[] = {'1', '5', '9', '8', '0'};
char input[5]; //ingegeven wachtwoord door gebruiker
int i = 0;
int buzzer = 6;
void setup() {
  Serial.begin(9600); // Seriele monitor starten
  lcd.begin(16, 2); // LCD opdelen in 16 kolommen & 2 rijen
  lcd.write("Pincode:");
  lcd.setCursor(0, 2);
  pinMode(buzzer, OUTPUT); //Mijn buzzer oproepen
}

void loop() {
  char ingedrukteToets = paneel.getKey();
  inputSamenvoegen(ingedrukteToets);
  inloggen();
  wissen(ingedrukteToets);
}

void inputSamenvoegen(char ingedrukteToets) { //Input in een array steken & op het LCD scherm laten afprinten
  if (ingedrukteToets && ingedrukteToets != '*') {
    input[i++] = ingedrukteToets;
    delay(200);
    lcd.print(ingedrukteToets);
  }
}
void inloggen() {//Ingegeven wachtwoord van gebruiker vergelijken met het juist wachtwoord
  int check;
  for (int z = 0; z < sizeof(wachtwoord); z++) {
    check = memcmp(wachtwoord, input, sizeof(wachtwoord)); // Manier om eenvoudig arrays te vergelijken
    if (check == 0 && i == 5) {//juist wachtwoord
      lcd.clear();
      tone(buzzer, 1600, 250);
      lcd.print("Welkom!");
      i = 0;
      z = 0;
      break;

    } else if (check != 0 && i == 5) {//fout wachtwoord
      tone(buzzer, 400, 500);
      lcd.clear();
      lcd.write("Fout!");
      delay(1500);
      lcd.clear();
      delay(200);
      lcd.write("Pincode:");
      lcd.setCursor(0, 2);
      i = 0;
      z = 0;
      break;
    }
  }
}
void wissen(char ingedrukteToets) {//Input op het scherm wissen bij bv: een typfout, uitloggen en opnieuw code ingeven
  if (ingedrukteToets == '*') {
    lcd.clear();
    lcd.write("Pincode:");
    lcd.setCursor(0, 2);
    i = 0;
  }
}
