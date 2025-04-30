#include <Preferences.h> // Biblioteca para almacenar datos en memoria NVS

// Configuración WiFi (sin cambios)
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "PoopConveyor";
const char* password = "12345678";
WebServer server(80);

Preferences preferences; // Objeto para manejar la memoria NVS

// Pines y parámetros configurables
const int pinPIR = 13;
const int in1 = 25;
const int in2 = 26;
const int enablePin = 27;
const int ledGreen = 14;
const int ledYellow = 12;
const int ledRed = 33;

int motorSpeed;
int motorRunTime = 10000;
int detectionTime = 5000;
int blinkInterval = 500;

bool motorRunning = false;
bool detecting = false;
unsigned long detectionEndTime = 0;
unsigned long motorStopTime = 0;
unsigned long lastBlinkTime = 0;
bool ledState = false;

// Página web (igual, pero se refiere a los parámetros guardados)
String webPage() {
  String page = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1'>";
  page += "<style>body{font-family:sans-serif;text-align:center;background:#f9e0b7;color:#5a3d2b;padding:20px;}";
  page += "input,button{margin:10px;padding:10px;width:90%;max-width:300px;border:2px solid #7b5e45;border-radius:5px;}";
  page += "button{background:#7b5e45;color:white;font-weight:bold;}";
  page += ".status{margin:20px 0;display:flex;justify-content:center;gap:20px;}";
  page += ".status span{padding:10px;border-radius:5px;font-weight:bold;font-size:18px;color:white;}";
  page += ".standby{background-color:#4caf50;}";
  page += ".detecting{background-color:#ffc107;animation:blink 1s infinite;}";
  page += ".unloading{background-color:#f44336;animation:blink 1s infinite;}";
  page += "@keyframes blink{50%{opacity:0.5;}}</style></head><body>";
  page += "<h1>Control de PoopConveyor</h1>";
  page += "<form action='/set' method='get'>";
  page += "<label>Velocidad (0-255):</label><input name='speed' value='" + String(motorSpeed) + "'><br>";
  page += "<label>Tiempo de detección (ms):</label><input name='detection' value='" + String(detectionTime) + "'><br>";
  page += "<label>Velocidad parpadeo led detección (ms):</label><input name='blink' value='" + String(blinkInterval) + "'><br>";
  page += "<button type='submit'>Guardar</button></form>";
  page += "</body></html>";
  return page;
}

// Guardar valores en memoria
void savePreferences() {
  preferences.putInt("speed", motorSpeed);
  preferences.putInt("detection", detectionTime);
  preferences.putInt("blink", blinkInterval);
  Serial.println("Datos guardados en memoria.");
}

// Cargar valores desde memoria
void loadPreferences() {
  motorSpeed = preferences.getInt("speed", 200); // Valor predeterminado: 200
  detectionTime = preferences.getInt("detection", 5000); // Predeterminado: 5000 ms
  blinkInterval = preferences.getInt("blink", 500); // Predeterminado: 500 ms
  Serial.println("Datos cargados desde memoria.");
}

// Configuración inicial
void setup() {
  Serial.begin(115200);

  pinMode(pinPIR, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);

  digitalWrite(ledGreen, HIGH);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
  Serial.println("WiFi AP iniciado. IP:");
  Serial.println(WiFi.softAPIP());

  preferences.begin("PoopConveyor", false); // Inicializar memoria con el espacio "PoopConveyor"
  loadPreferences(); // Cargar valores guardados

  server.on("/", []() {
    server.send(200, "text/html", webPage());
  });

  server.on("/set", []() {
    if (server.hasArg("speed")) motorSpeed = server.arg("speed").toInt();
    if (server.hasArg("detection")) detectionTime = server.arg("detection").toInt();
    if (server.hasArg("blink")) blinkInterval = server.arg("blink").toInt();
    savePreferences(); // Guardar los valores modificados
    server.send(200, "text/html", webPage());
  });

  server.begin();
}

void loop() {
  server.handleClient();
  // Lógica del programa sigue igual...
}
