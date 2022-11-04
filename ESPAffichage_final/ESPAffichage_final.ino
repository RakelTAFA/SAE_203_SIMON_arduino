#include <Tone32.h>
#include"FeatherShieldPinouts.h"
#include<Adafruit_NeoPixel.h>
#include "pitches.h"
#include<WiFi.h>

// 0 = rate | 1 = reussi | 2 = commencer

#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 33

Adafruit_NeoPixel pixel(32, PIN, NEO_GRB + NEO_KHZ800);

char numero_recu;
String valeur = "", val_serial = "";
bool continuer = false;
float delay_affiche = 1500.0F;

const char *ssid = "SimonAp";
const char *password = "yourPassword";
WiFiServer server(23);

void affiche_vert();
void affiche_rouge();
void affiche_jaune();
void affiche_bleu();
void affiche_et_son(char);
void bonne_reponse();


void setup() {
  Serial.begin(115200);
  pixel.begin();
  pixel.show();
  pixel.setBrightness(15);

  Serial.println();
  Serial.println("Configuring access point...");

  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}


void loop() {

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {
        char numero_recu = client.read();

        if (numero_recu == '1') {
          bonne_reponse();
          valeur += (String)random(0, 4);
          Serial.print("Valeur : "); Serial.println(valeur);

          // Boucle
          for (int k = 0; k < valeur.length(); k++) {
            affiche_et_son(valeur[k]);
            pixel.clear();
            pixel.show();
          }

          char message[valeur.length()+1];
          valeur.toCharArray(message,valeur.length()+1);
          client.write(message);
          attente_reponse();

          if (delay_affiche >= 500) {
            delay_affiche *= 0.93F;
          }
          
        }
        else if (numero_recu == '0') {
          Serial.println("PERDU !");
          mauvaise_reponse();
          valeur = "";
          delay_affiche = 1500.0F;
        }
      }
    }
  }
}

// FONCTION :

void affiche_vert() {
  for (int i = 0; i < 4; i++) {
    pixel.setPixelColor(i, pixel.Color(0, 255, 0));
    pixel.setPixelColor(i + 8, pixel.Color(0, 255, 0));
    pixel.show();
  }
}

void affiche_rouge() {
  for (int i = 4; i < 8; i++) {
    pixel.setPixelColor(i, pixel.Color(255, 0, 0));
    pixel.setPixelColor(i + 8, pixel.Color(255, 0, 0));
    pixel.show();
  }
}

void affiche_jaune() {
  for (int i = 16; i < 20; i++) {
    pixel.setPixelColor(i, pixel.Color(255, 255, 0));
    pixel.setPixelColor(i + 8, pixel.Color(255, 255, 0));
    pixel.show();
  }
}

void affiche_bleu() {
  for (int i = 20; i < 24; i++) {
    pixel.setPixelColor(i, pixel.Color(0, 0, 255));
    pixel.setPixelColor(i + 8, pixel.Color(0, 0, 255));
    pixel.show();
  }
}

void affiche_et_son(char ch) {
  // Partie son à rajouter pour chaque couleur
  pixel.clear();
  pixel.show();
  delay(150);
  switch (ch) {
    case '0':
      affiche_vert();
      son_vert();
      break;

    case '1':
      affiche_rouge();
      son_rouge();
      break;

    case '2':
      affiche_jaune();
      son_jaune();
      break;

    case '3':
      affiche_bleu();
      son_bleu();
      break;
  }
}

void bonne_reponse() {
  pixel.clear();
  pixel.show();
  delay(400);
  affiche_vert();
  affiche_rouge();
  affiche_jaune();
  affiche_bleu();
  son_bonne_rep();
  pixel.clear();
  pixel.show();
  delay(400);
}

void mauvaise_reponse() {
  pixel.clear();
  pixel.show();
  for (int i = 0; i < 8; i++) {
    pixel.setPixelColor(i, pixel.Color(255, 10, 20));
    pixel.setPixelColor(i + 24, pixel.Color(255, 10, 20));
    if (i == 0 || i == 7) {
      pixel.setPixelColor(i + 8, pixel.Color(255, 10, 20));
      pixel.setPixelColor(i + 16, pixel.Color(255, 10, 20));
    }
  }
  pixel.show();
  son_mauvaise_rep();
  pixel.clear();
  pixel.show();
}

void attente_reponse() {
  noTone(A0, 0);
  pixel.setPixelColor(11, pixel.Color(230, 246, 209));
  pixel.setPixelColor(12, pixel.Color(230, 246, 209));
  pixel.setPixelColor(19, pixel.Color(230, 246, 209));
  pixel.setPixelColor(20, pixel.Color(230, 246, 209));
  pixel.show();
}


void son_bonne_rep() {
  tone(A0, NOTE_C6, 400, 0);
  noTone(A0, 0);
}


void son_mauvaise_rep() {
  tone(A0, NOTE_CS3, 400, 0);
  noTone(A0, 0);
}


void son_vert() {
  tone(A0, NOTE_DS3, delay_affiche, 0);
  noTone(A0, 0);
}


void son_rouge() {
  tone(A0, NOTE_E4, delay_affiche, 0);
  noTone(A0, 0);
}


void son_jaune() {
  tone(A0, NOTE_AS4, delay_affiche, 0);
  noTone(A0, 0);
}


void son_bleu() {
  tone(A0, NOTE_FS4, delay_affiche, 0);
  noTone(A0, 0);
}
