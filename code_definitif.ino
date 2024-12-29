
//---------------------------------ENTETE DU CODE 
//---------------------INCLUSION DES BIBLIOTHEQUES
#include<ESP32_Servo.h>
#include<ESPAsyncWebServer.h>
#include<SPIFFS.h>

//---------------------------------PARTIE DECLARATIVE DU CODE
//-----------------------------DECLARATION DE LA VARIABLE DE CHANGEMENT
int changement = 0;
//-----------------DECLARATION DE LA LED
const int led = 2;
//-----------------DECLARATION DES SERVOMOTEURS
Servo epaule;
Servo coude;
Servo poignet;
Servo pince;
//-----------------DECLARATION DES PINS DES SERVOMOTEURS
int servoPin1 =  18;
int servoPin2 =  19;
int servoPin3 =  25;
int servoPin4 =  26;
//-----------------DECLARATION DES POTENTIOMETRE
int potPin1 = 34;
int potPin2 = 35;
int potPin3 = 36;
int potPin4 = 39;
//-----------------DECLARATION DE L'ADC PAR DEFAUT DES POTENTIOMETRE
int ADC_Max = 4096;
//-----------------DECLARATION DES VARIABLES POUR LES ANALOG READ DES PINS ANALOGIQUES 
int val1;
int val2;
int val3;
int val4;
//--------------POSITION DES SERVOMOTEURS
int pos1;
int pos2;
int pos3;
int pos4;
//--------------INITIALISATION DE LA CONNEXION WIFI
const char* ssid="Vodafone CMR-D20B";
const char* password="30786932";
//--------------MISE EN ROUTE DU SERVEUR ASYNCHRONE
AsyncWebServer server(80);

void setup() 
{
  //----------------------------------SERIAL
  Serial.begin(115200);
  Serial.println("\n");
  Serial.println("bonjour l'esp est disponible mon cho");

  //----------------------------------GPIO
  //---------LA LED
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  //--------LES SERVOMOTEURS
  epaule.attach(servoPin1, 500, 2400);
  coude.attach(servoPin2, 500, 2400);
  poignet.attach(servoPin3, 500, 2400);
  pince.attach(servoPin4, 500, 2400);

  //-----------------------------------SPIFFS
  if(!SPIFFS.begin())
  {
    Serial.println("Erreur SPIFFS....");
    return;
  }

  File root = SPIFFS.open("/");
  File file = root.openNextFile();

  while(file)
  {
    Serial.print("File: ");
    Serial.println(file.name());
    file.close();
    file = root.openNextFile();
  }

  //--------------------------------WIFI
  WiFi.begin(ssid, password);
  Serial.print("Tentative de connexion...");

  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  Serial.println("\n");
  Serial.println("Connexion etablie!");
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());

  //----------------------------------- SERVER ASYNCHRONE
  //--ENVOI PAGE HTML A L'UTILISATEUR
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request ->send(SPIFFS, "/index.html", "text/html");
  });

  //--ENVOI IMAGE POUR LE LOGO DU SITE 
  server.on("/armrobotic.jpg", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request ->send(SPIFFS, "/armrobotic.jpg", "img/jpg");
  });

  //--ENVOI DE LA BIBLIOTHEQUE DE STYLES W3 POUR LES STYLES CSS
  server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request ->send(SPIFFS, "/w3.css", "text/css");
  });

  //--ENVOI DE LA BIBLIOTHEQUE JQUERY DE VERSION 3.7.1 POUR LA GESTION DES REQUETES AJAX HTTP ASYNCHRONES
  server.on("/jquery-3.7.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request ->send(SPIFFS, "/jquery-3.7.1.min.js", "text/javascript");
  });

  //--ENVOI DU FICHIER JAVASCRIPT POUR LES INTERACTIONS AVEC L'UTILISATEUR ET LE SERVEUR(ESP32)
  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request ->send(SPIFFS, "/index.js", "text/javascript");
  });

  //--RECUPERATION DES VALEURS DE POSITIONNEMENT DES SERVOMOTEURS DONNEES PAR L'UTILISATEURS DEPUIS LA PAGE WEB
  server.on("/position", HTTP_POST, [](AsyncWebServerRequest *request)
  {
   String message1; 
   String message2;
   String message3;
   String message4;
   if(request ->hasParam("position1", true) && request ->hasParam("position2", true) && request ->hasParam("position3", true) && request ->hasParam("position4", true))
   {
    message1 = request->getParam("position1",true)->value();
    message2 = request->getParam("position2",true)->value();
    message3 = request->getParam("position3",true)->value();
    message4 = request->getParam("position4",true)->value();
    pos1 = message1.toInt();
    pos2 = message2.toInt();
    pos3 = message3.toInt();
    pos4 = message4.toInt();
   }
 
   changement = 1;
   request->send(204);
  });
  //--DESACTIVATION DES POSITIONNEMENT WEB
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    changement = 0;
    request ->send(204);
  });

  //--DEMARRAGE DU SERVEUR WEB DEPUIS(ESP32)
  server.begin();
  Serial.println("Serveur actif!");
}

void loop() {
  while(changement == 1)
  {
    //--AFFICHAGE DES INFORMATIONS RECUES PAR LE SERVEUR(ESP32)
    Serial.print("position1=");
    Serial.println(pos1);
    Serial.print("position2=");
    Serial.println(pos2);
    Serial.print("position3=");
    Serial.println(pos3);
    Serial.print("position4=");
    Serial.println(pos4);

    //--------------ALLUMAGE LED
    digitalWrite(led, HIGH);
    //DEPLACEMENT DES SERVOMOTEURS
    epaule.write(pos1);
    coude.write(pos2);
    poignet.write(pos3);
    pince.write(pos4);
  
  }
  while(changement == 0)
  {

    //-----------------------LECTURE DE LA VALEUR DES POTENTIOMETRES
    //--POTENTIOMETRE 1
    //--LECTURE DE LA VALEUR DU POTENTIOMETRE 
    val1 = analogRead(potPin1);
    //--CONVERSION DE LA VARIABLE EN ANGLE PAR MAPPING POUR SERVOMOTEUR
    val1 = map(val1, 0, ADC_Max, 0, 180);
    //--DEPLACEMENT DU SERVOMOTEUR 
    epaule.write(val1);     

    //--POTENTIOMETRE 2
    //--LECTURE DE LA VALEUR DU POTENTIOMETRE 
    val2 = analogRead(potPin2);
    //CONVERSION DE LA VARIABLE EN ANGLE PAR MAPPING POUR SERVOMOTEUR 
    val2 = map(val2, 0, ADC_Max, 0, 180);
    //--DEPLACEMENT DU SERVOMOTEUR 
    coude.write(val2);

    //--------------------------POTENTIOMETRE 3
    //--LECTURE DE LA VALEUR DU POTENTIOMETRE 
    val3 = analogRead(potPin3);
    //--CONVERSION DE LA VARIABLE EN ANGLE PAR MAPPING POUR SERVOMOTEUR
    val3 = map(val3, 0, ADC_Max, 0, 180);
    //--DEPLACEMENT DU SERVOMOTEUR 
    poignet.write(val3);

    //------------------------------------------POTENTIOMETRE 4
    //--LECTURE DE LA VALEUR DU POTENTIOMETRE 
    val4 = analogRead(potPin4);
    //--CONVERSION DE LA VARIABLE EN ANGLE PAR MAPPING POUR SERVOMOTEUR
    val4 = map(val4, 0, ADC_Max, 0, 180);
    //--DEPLACEMENT DU SERVOMOTEUR 
    pince.write(val4); 

    //-----------------------EXTINTION DE LA LED
    Serial.println(val4);
    
    digitalWrite(led,LOW); 
  }
  //DEFINITION DU DELAI
  delay(5);
}
