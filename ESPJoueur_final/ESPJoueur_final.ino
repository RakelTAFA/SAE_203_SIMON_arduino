#include <Tone32.h>
#include"FeatherShieldPinouts.h"
#include "pitches.h"
#include<WiFi.h>
//vert = 0
//rouge = 1
//jaune = 2
//bleu = 3


bool enPartie = false;
bool win = false;
String sequence;
String appuyer = "";
int reponse = 0;
hw_timer_t * timer = NULL;
int timeR = millis();
int timeB = millis();
int timeV = millis();
int timeJ = millis();

const char* ssid     = "SimonAp";
const char* password = "yourPassword";
const char* host = "192.168.4.1";

void IRAM_ATTR timerIsr() {
  enPartie = false ;
  Serial.println("Perdu");
}

void bRouge();
void bVert();
void bBleu();
void bJaune();
bool manche();
void boutonInt(int i);
String sendMessage(char m);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A1, INPUT);
  pinMode(A0, OUTPUT);
  pinMode(D5, INPUT);
  pinMode(D4, OUTPUT);
  pinMode(D3, INPUT);
  pinMode(D2, OUTPUT);
  pinMode(17, INPUT);
  pinMode(16, OUTPUT);


  attachInterrupt(A1, bRouge, FALLING);
  attachInterrupt(D3, bJaune, FALLING);
  attachInterrupt(17, bVert, FALLING);
  attachInterrupt(D5, bBleu, FALLING);
  timer = timerBegin(1, 80, true);
  timerAttachInterrupt(timer, &timerIsr, true);
  timerAlarmWrite(timer, 2000000, true);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  digitalWrite(D2, HIGH);
}

void loop() {


  // put your main code here, to run repeatedly:
  if (enPartie) {
    sequence = sendMessage('1');
    Serial.println("MessageTrouvé");
    manche();
  }
  delay(30);
}

bool manche() {
  while (sequence != "") {
    timerWrite(timer, 0);
    tone(22, NOTE_C6, 215, 0);
    timerAlarmEnable(timer);
    Serial.println("lenght sequence");
    Serial.println(sequence.length());
    while (enPartie) {
      if ((reponse) == sequence.length()) {
        win = true;
        enPartie = false;
      }
      else if (appuyer.length() > 0) {
        Serial.println("bouton");
        boutonInt(appuyer[0]);
        appuyer.remove(0);
      }
    }
    Serial.println("sendMessage");
    timerAlarmDisable(timer);
    if (win) {
      sequence = sendMessage((char)'1');
      enPartie = true;
    }
    else {
      sequence = sendMessage((char)'0');
      delay(1000);
    }
    Serial.println(win);

    appuyer="";
    reponse = 0;
    win = false;
  }
}

void bVert() {
  if (millis() > timeV + 200) {
    timeV = millis();
    if (enPartie) {
      appuyer += '0';
    }
    else {
      enPartie = true;
      Serial.println("start");
    }
  }
}

void bRouge() {
  if (millis() > timeR + 200) {
    timeR = millis();
    if (enPartie) {

      appuyer += '1';
    }
    else {
      enPartie = true;
      Serial.println("start");
    }
  }
}
void bJaune() {
  if (millis() > timeJ + 200) {
    timeJ = millis();
    if (enPartie) {
      appuyer += '2';
    }
    else {
      enPartie = true;
      Serial.println("start");
    }
  }
}
void bBleu() {
  if (millis() > timeB + 200) {
    timeB = millis();
    if (enPartie) {
      appuyer += '3';
    }
    else {
      enPartie = true;
      Serial.println("start");
    }
  }
}

void boutonInt(int i) {
  switch (i) {
    case '0' :
      digitalWrite(16, HIGH);
      tone(22, NOTE_DS3, 215, 0);
      break;
    case '1' :
      digitalWrite(A0, HIGH);
      tone(22, NOTE_E4, 215, 0);
      break;
    case '2' :
      digitalWrite(D2, LOW);
      tone(22, NOTE_AS4, 215, 0);
      break;
    case '3' :
      digitalWrite(D4, HIGH);
      tone(22, NOTE_FS4, 215, 0);
      break;
    default :
      Serial.println("defaut");
      break;
  }
  if (enPartie) {
    if (sequence[reponse] == i) {
      reponse++;
      timerWrite(timer, 0);
    }
    else {
      enPartie = false;
    }
  }
  delay(200);
  switch (i) {
    case '0' :
      digitalWrite(16, LOW);
      break;
    case '1' :

      digitalWrite(A0, LOW);
      break;
    case '2' :
      digitalWrite(D2, HIGH);
      break;
    case '3' :
      digitalWrite(D4, LOW);
      break;
    default :
      Serial.println("defaut");
      break;
  }
}

String sendMessage(char m) {
  String line = "";
  WiFiClient client;
  const int Port = 23;
  if (!client.connect(host, Port)) {
    Serial.println("connection failed");
    return line;
  }
  client.write(m);
  if (m == '1') {
    Serial.println("enAttente");
    while (!client.available()) {

      delay(200);
    }
    line = client.readStringUntil('\r');
  }
  return line;
}
