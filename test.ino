/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// In questa sezione viene utilizzato il WiFi. Assicurati che l'ESP8261 stia ricevendo un'alimentazione sufficiente.   //
// Nei miei test, utilizzando un'alimentatore da 5V 2A, questo progetto (questa sezione) ha funzionato correttamente.  //
// Ma se l'alimentazione è inferiore a quella, allora ci sono "rumori" nelle letture del sensore di impulsi.           //                                                                                                                    //
// Se vuoi cambiare il PIN per il sensore di impulsi, assicurati di selezionare il PIN analogico su "A0".              //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------Inclusione delle librerie
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <ESP8266WiFi.h>
#include <AsyncHttpClient.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h"
#include "thingProperties.h"
//----------------------------------------

// Include il codice web (HTML, JavaScript e altro) all'interno della variabile di array di caratteri per la pagina del server web.
#include "PageIndex.h"

// Definizione dei PIN utilizzati.
#define PulseSensor_PIN A0 
#define LED_PIN         D0 
#define Button_PIN      D4

#define i2c_Address 0x3c // Inizializza con l'indirizzo I2C 0x3C, tipicamente per OLED generici
//#define i2c_Address 0x3d // Inizializza con l'indirizzo I2C 0x3D, tipicamente per OLED di Adafruit

#define SCREEN_WIDTH 128 // Larghezza del display OLED, in pixel
#define SCREEN_HEIGHT 64 // Altezza del display OLED, in pixel
#define OLED_RESET -1   
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


const char* serverAddress = "http://192.168.1.173";
const int serverPort = 3000; // Porta del server Node.js
//HttpClient client = HttpClient(wifi, serverAddress, serverPort);
AsyncHttpClient aClient;

unsigned long previousMillisGetHB = 0; //--> Memorizza l'ultimo tempo in Millis (per ottenere il battito).
unsigned long previousMillisResultHB = 0; //--> Memorizza l'ultimo tempo in Millis (per ottenere il BPM).

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

const long intervalGetHB = 35; //--> Intervallo per leggere la frequenza cardiaca (battito) = 35ms.
const long intervalResultHB = 1000; //--> Intervallo di lettura per il risultato del calcolo della frequenza cardiaca.

int timer_Get_BPM = 0;

int PulseSensorSignal; //--> Variabile per ospitare il valore del segnale dal sensore.
int UpperThreshold = 558; //--> Determina quale segnale "contare come battito", e quale ignorare.
int LowerThreshold = 555; 

int cntHB = 0; //--> Variabile per contare il numero di battiti cardiaci.
boolean ThresholdStat = true; //--> Variabile per attivare il calcolo dei battiti cardiaci.
int BPMval = 0; //--> Variabile per contenere il risultato del calcolo dei battiti cardiaci.

int x=0; //--> Variabile valori asse x per visualizzare sull'OLED
int y=0; //--> Variabile valori asse y per visualizzare sull'OLED
int lastx=0; //--> Ultimo valore variabile asse x del grafico da visualizzare sull'OLED
int lasty=0; //--> Ultimo valore variabile asse y del grafico da visualizzare sull'OLED

bool get_BPM = false; // Variabili booleane per avviare e interrompere l'ottenimento dei valori BPM.

byte tSecond = 0; // Variabili per il "timestamp" della lettura BPM.
byte tMinute = 0;
byte tHour   = 0;

char tTime[10]; // Array di caratteri per contenere i dati "timestamp" che verranno inviati al server web.

//----------------------------------------Icona del cuore, 16x16px
// L'icona del cuore è stata disegnata su: http://dotmatrixtool.com/
const unsigned char Heart_Icon [] PROGMEM = {
  0x00, 0x00, 0x18, 0x30, 0x3c, 0x78, 0x7e, 0xfc, 0xff, 0xfe, 0xff, 0xfe, 0xee, 0xee, 0xd5, 0x56, 
  0x7b, 0xbc, 0x3f, 0xf8, 0x1f, 0xf0, 0x0f, 0xe0, 0x07, 0xc0, 0x03, 0x80, 0x01, 0x00, 0x00, 0x00
};
//----------------------------------------
const char* PARAM_INPUT_1 = "BTN_Start_Get_BPM";
String BTN_Start_Get_BPM = "";
// Inizializzazione di JSONVar
JSONVar JSON_All_Data;

// Crea un oggetto AsyncWebServer sulla porta 80
AsyncWebServer server(80);

// Crea un Event Source su /events
AsyncEventSource events("/events");

//----------------------------------------Funzione per ottenere la frequenza cardiaca
void GetHeartRate() {
  //----------------------------------------Processo di lettura del battito cardiaco.
  unsigned long currentMillisGetHB = millis();

  if (currentMillisGetHB - previousMillisGetHB >= intervalGetHB) {
    previousMillisGetHB = currentMillisGetHB;

    PulseSensorSignal = analogRead(PulseSensor_PIN); //--> Legge il valore del PulseSensor e lo assegna alla variabile "Signal".
    Serial.println(PulseSensorSignal);
    if (PulseSensorSignal > UpperThreshold && ThresholdStat == true) {
      if (get_BPM == true) cntHB++;
      ThresholdStat = false;
      digitalWrite(LED_PIN, HIGH);
    }

    if (PulseSensorSignal < LowerThreshold) {
      ThresholdStat = true;
      digitalWrite(LED_PIN, LOW);
    }
    
    DrawGraph(); //--> Chiama la subroutine DrawGraph().
    // Inserisce i dati in JSONVar (JSON_All_Data).
    JSON_All_Data["heartbeat_Signal"] = PulseSensorSignal;
    JSON_All_Data["temperature_Signal"] = PulseSensorSignal;
    JSON_All_Data["BPM_TimeStamp"] = tTime;
    JSON_All_Data["BPM_Val"] = BPMval;
    JSON_All_Data["BPM_State"] = get_BPM;
    
    // Crea una stringa JSON per contenere tutti i dati.
    String JSON_All_Data_Send = JSON.stringify(JSON_All_Data);
// Invia l'evento al browser con JSON_All_Data ogni 35 millisecondi.
    events.send(JSON_All_Data_Send.c_str(), "allDataJSON" , millis());

    //Verifica lo stato della connsessione WiFi
    if(WiFi.status()== WL_CONNECTED){
      Serial.println("POST request");
      String contentType = "application/json";
      String postData = JSON_All_Data_Send ;
      aClient.init("POST","http://192.168.1.236:3000/iot?id=24", "application/json", postData);
      aClient.send();

    }
    else {
      Serial.println("WiFi Disconnected");
    }

    
  }
  //----------------------------------------

  //----------------------------------------Processo per ottenere il valore BPM.
  unsigned long currentMillisResultHB = millis();

  if (currentMillisResultHB - previousMillisResultHB >= intervalResultHB) {
    previousMillisResultHB = currentMillisResultHB;

    if (get_BPM == true) {
      timer_Get_BPM++;
      
      if (timer_Get_BPM > 10) {
        timer_Get_BPM = 1;

        tSecond += 10;
        if (tSecond >= 60) {
          tSecond = 0;
          tMinute += 1;
        }
        if (tMinute >= 60) {
          tMinute = 0;
          tHour += 1;
        }

        sprintf(tTime, "%02d:%02d:%02d",  tHour, tMinute, tSecond);

        BPMval = cntHB * 6;
        Serial.print("BPM : ");
        Serial.println(BPMval);
        
        display.fillRect(20, 48, 108, 18, SH110X_BLACK);
        
        display.drawBitmap(0, 47, Heart_Icon, 16, 16, SH110X_WHITE);
        display.drawLine(0, 43, 127, 43, SH110X_WHITE);
      
        display.setTextSize(2);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(20, 48);
        display.print(": ");
        display.print(BPMval);
        display.setCursor(92, 48);
        display.print("BPM");
        display.display(); 
        
        cntHB = 0;
      }
    }
  }
}


//----------------------------------------Subroutine per disegnare o visualizzare i segnali grafici del battito cardiaco.
void DrawGraph() {
  //----------------------------------------Condizione per ripristinare la visualizzazione grafica se riempie la larghezza dello schermo OLED
  if (x > 127) {
    display.fillRect(0, 0, 128, 42, SH110X_BLACK);
    x = 0;
    lastx = 0;
  }
  //----------------------------------------

  //----------------------------------------Elabora i dati del segnale da visualizzare sull'OLED in forma grafica
  int ySignal = PulseSensorSignal; 
  
  if (ySignal > 565) ySignal = 565;
  if (ySignal < 545) ySignal = 545;

  int ySignalMap = map(ySignal, 545, 565, 0, 40); // L'asse y utilizzato sugli OLED va da 0 a 40
  
  y = 40 - ySignalMap;
  //----------------------------------------

  //----------------------------------------Visualizza il grafico del battito cardiaco
  display.writeLine(lastx, lasty, x, y, SH110X_WHITE);
  display.display(); 
  //----------------------------------------
  
  lastx = x;
  lasty = y;
  
  x++;
}


// Setup iniziale del programma
void setup() {
  // Impostazioni iniziali
  
  Serial.begin(115200); // Inizializza la comunicazione seriale a una determinata velocità.
  Serial.println();
  delay(2000);

  pinMode(LED_PIN, OUTPUT); 
  pinMode(Button_PIN, INPUT_PULLUP);

  sprintf(tTime, "%02d:%02d:%02d",  tHour, tMinute, tSecond);

  // Inizializzazione del display OLED
  if (!display.begin()) { 
    Serial.println(F("Assegnazione SSD1106 fallita"));
    for (;;) { } // Termina il processo, loop infinito
  }

  // Inizializzazione del display OLED
  if (!display.begin()) { 
    Serial.println(F("Assegnazione SSD1106 fallita"));
    for (;;) { } // Termina il processo, loop infinito
  }

  


  
  display.clearDisplay(); 
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(2);
  display.setCursor(5, 20); 
  display.print("MedConnect"); 
  display.setCursor(90, 40); 
  display.print("IOT"); 
  display.display();
  delay(2000);

  // Impostazione del dispositivo come client WiFi in modalità STA (Station)
  Serial.println();
  Serial.println("-------------");
  Serial.println("Modo WIFI: STA");
  WiFi.mode(WIFI_STA);
  Serial.println("-------------");
  delay(100);

  // Connessione alla rete WiFi specificata
  display.clearDisplay(); 
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0); 
  display.print("Connessione...");
  display.display();
  delay(1000);
  
  Serial.println("------------");
  Serial.println("WIFI STA");
  Serial.print("Connessione a ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASS);

  // Processo di connessione alla rete WiFi con timeout di 20 secondi
  int timeout_connessione_wifi = 20; // Timeout di 20 secondi
  timeout_connessione_wifi = timeout_connessione_wifi * 2;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    if (timeout_connessione_wifi > 0) timeout_connessione_wifi--;
    if (timeout_connessione_wifi == 0) {
      delay(1000);
      ESP.restart();
    }
  }



  // Connessione riuscita
  Serial.println("");
  Serial.println("WiFi connesso");
  Serial.println("------------");

  display.clearDisplay(); 
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0); 
  display.print("Connesso");
  display.setCursor(0, 10); 
  display.print("correttamente.");
  display.display();
  delay(1000);

   // Configurazione del server web e degli eventi
  display.clearDisplay(); 
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0); 
  display.print("Configurazione");
  display.setCursor(0, 10); 
  display.print("dei server...");
  display.display();
  delay(1000);

  // Gestione della pagina principale sul server web
  Serial.println();
  Serial.println("Configurazione della pagina principale sul server.");
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", MAIN_page);
  });



  // Gestione degli eventi sul server web
  Serial.println();
  Serial.println("Preparazione del server completata.");
  events.onConnect([](AsyncEventSourceClient * client) {
    if (client->lastId()) {
      Serial.printf("Client riconnesso! Ultimo ID messaggio ricevuto: %u\n", client->lastId());
    }
    // Invia un evento con il messaggio "hello!", id attuale e ritardo di riconnessione di 10 secondi
    client->send("hello!", NULL, millis(), 10000);
  });



  // Gestione delle richieste GET per il controllo del BPM
  Serial.println();
  Serial.println("Aggiunta gestione richieste al server.");
  server.on("/BTN_Comd", HTTP_GET, [] (AsyncWebServerRequest * request) {
    // Ottieni il valore di input su <ESP_IP>/BTN_Comd?BTN_Start_Get_BPM=<inputMessage1>
    // PARAM_INPUT_1 = inputMessage1
    // BTN_Tare = PARAM_INPUT_1

    if (request->hasParam(PARAM_INPUT_1)) {
      BTN_Start_Get_BPM = request->getParam(PARAM_INPUT_1)->value();

      Serial.println();
      Serial.print("BTN_Start_Get_BPM : ");
      Serial.println(BTN_Start_Get_BPM);
    }
    else {
      BTN_Start_Get_BPM = "Nessun battito";
      Serial.println();
      Serial.print("BTN_Start_Get_BPM : ");
      Serial.println(BTN_Start_Get_BPM);
    }
    request->send(200, "text/plain", "OK");
  });

  // Aggiunta di eventi al server
  Serial.println();
  Serial.println("Aggiunta degli eventi al server.");
  server.addHandler(&events);

  // Avvio del server
  Serial.println();
  Serial.println("Avvio del server.");
  server.begin();



  // Messaggio di avvenuta connessione e indicazione dell'indirizzo IP
  Serial.println();
  Serial.println("------------");
  Serial.print("MedConnect IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("------------");
  Serial.println();

  // Mostra l'indirizzo IP sulla schermata OLED
  display.clearDisplay(); 
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0); 
  display.print("MedConnect IP:");
  display.setCursor(0, 10); 
  display.print(WiFi.localIP());
  display.display();
  delay(3000);

 
  String response;
  if(WiFi.status()== WL_CONNECTED){
      // Serial.println("GET request");
      // String contentType = "application/json";
      // aClient.init("GET","http://192.168.1.173:3000/pairiot?id=24&type=1", "application/json");
      // aClient.send();


      response = httpGETRequest("http://192.168.1.236:3000/pairiot?id=24&type=1");
      Serial.println(response);
      JSONVar persona = JSON.parse(response);
      
      StaticJsonDocument<200> doc;

      // Il tuo JSON

      // Parsa il JSON
      DeserializationError error = deserializeJson(doc, response);

      // Gestisci eventuali errori
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

      // Estrai i valori dal JSON
      JsonArray abbinamento = doc["abbinamento"];
      for (JsonObject obj : abbinamento) {
        int id_persona = obj["id_persona"];
        const char* nome = obj["nome"];
        const char* cognome = obj["cognome"];
        int id_tipologia = obj["id_tipologia"];
        const char* data_inizio = obj["data_inizio"];
        const char* data_fine = obj["data_fine"];
        int id_device = obj["id_device"];

        display.clearDisplay(); 
        display.setTextColor(SH110X_WHITE);
        display.setTextSize(1);
        display.setCursor(0, 0); 
        display.print("Ciao ");
        display.setCursor(0, 10); 
        display.print(nome);
        display.display();
        delay(3000);

        display.clearDisplay(); 
        display.setTextColor(SH110X_WHITE);
        display.setTextSize(1);
        display.setCursor(0, 0); 
        display.print("Scadenza:");
        display.setCursor(0, 10); 
        display.print(data_fine);
        display.display();
        delay(3000);

        // Stampa i valori estratti
        // Serial.print("id_persona: ");
        // Serial.println(id_persona);
        // Serial.print("nome: ");
        // Serial.println(nome);
        // Serial.print("cognome: ");
        // Serial.println(cognome);
        // Serial.print("id_tipologia: ");
        // Serial.println(id_tipologia);
        // Serial.print("data_inizio: ");
        // Serial.println(data_inizio);
        // Serial.print("data_fine: ");
        // Serial.println(data_fine);
        // Serial.print("id_device: ");
        // Serial.println(id_device);
      }
    }
     // Visualizzazione iniziale sull'OLED
    display.clearDisplay(); 
    display.drawLine(0, 43, 127, 43, SH110X_WHITE); // Disegna una linea sull'OLED
    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(10, 48); // Posizione del cursore sull'OLED
    display.print("Battito");
    display.display(); 

}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // If you need Node-RED/server authentication, insert user and password below
  //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void loop() {


  if (digitalRead(Button_PIN) == LOW) {
    delay(100);

    // Imposta variabili a valori predefiniti e controlla il pulsante per avviare/fermare il monitoraggio
    //BTN_Start_Get_BPM = "";
    cntHB = 0;
    BPMval = 0;
    x = 0;
    y = 0;
    lastx = 0;
    lasty = 0;

    tSecond = 0;
    tMinute = 0;
    tHour = 0;

    sprintf(tTime, "%02d:%02d:%02d", tHour, tMinute, tSecond);

    get_BPM = !get_BPM;

    // Azioni in base al pulsante di avvio o arresto
    if (get_BPM == true) {
      // Visualizzazione dell'avvio del calcolo del BPM sull'OLED
      display.clearDisplay(); 
      display.setTextColor(SH110X_WHITE);
      display.setTextSize(1);
      display.setCursor(14, 0); 
      display.print("Calcolo BPM"); 

      display.setTextSize(3);

      for (byte i = 3; i > 0; i--) {
        display.setTextColor(SH110X_WHITE);
        display.setCursor(50, 20); 
        display.print(i); 
        display.display();
        delay(1000);
        display.setTextColor(SH110X_BLACK);
        display.setCursor(50, 20); 
        display.print(i); 
        display.display();
      }

      // Inizializzazione della visualizzazione del valore BPM sull'OLED
      display.clearDisplay(); 
      display.drawBitmap(0, 47, Heart_Icon, 16, 16, SH110X_WHITE); 
      display.drawLine(0, 43, 127, 43, SH110X_WHITE); 
      display.setTextSize(2);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(20, 48); 
      display.print(": 0 ");
      display.setCursor(92, 48); 
      display.print("BPM");
      display.display(); 
    } else {
      // Visualizzazione dell'arresto del monitoraggio sul display OLED
      display.clearDisplay();
      display.setTextColor(SH110X_WHITE);
      display.setTextSize(2);
      display.setCursor(42, 25); 
      display.print("STOP"); 
      display.display();
      delay(1000);
      display.clearDisplay(); 
      display.drawLine(0, 43, 127, 43, SH110X_WHITE); 
      display.setTextSize(2);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(10, 48); 
      display.print("Battito");
      display.display(); 
    }
  }

  GetHeartRate(); // Chiamata alla funzione per ottenere il battito cardiaco
}


