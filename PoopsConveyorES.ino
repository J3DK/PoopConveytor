#include <WiFi.h>
#include <WebServer.h>

// Configuración WiFi (modo AP)
const char* ssid = "PoopConveyor";
const char* password = "12345678";
WebServer server(80);

// Pines
const int pinPIR = 13;
const int in1 = 25;
const int in2 = 26;
const int enablePin = 27;
const int ledGreen = 14;
const int ledYellow = 12;
const int ledRed = 33;

// Parámetros configurables
int motorSpeed = 200;
int motorRunTime = 10000;
int detectionTime = 5000; // Tiempo que el LED amarillo parpadea
int blinkInterval = 500; // Intervalo de parpadeo en ms, ajustable

// Estado del motor y LEDs
bool motorRunning = false;
bool detecting = false;
unsigned long detectionEndTime = 0;
unsigned long motorStopTime = 0;
unsigned long lastBlinkTime = 0;
bool ledState = false;

String webPage() {
  String page = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1'>";
  page += "<style>body{font-family:sans-serif;text-align:center;background:#FEF0D2;color:#5a3d2b;padding:20px;}";
  page += "input,button{margin:10px;padding:10px;width:90%;max-width:300px;border:2px solid #7b5e45;border-radius:5px;}";
  page += "button{background:#7b5e45;color:white;font-weight:bold;}";
  page += ".status{margin:20px 0;display:flex;justify-content:center;gap:20px;}";
  page += ".status span{padding:10px;border-radius:5px;font-weight:bold;font-size:18px;color:white;}";
  page += ".standby{background-color:#4caf50;}";
  page += ".detecting{background-color:#ffc107;animation:blink 1s infinite;}";
  page += ".unloading{background-color:#f44336;animation:blink 1s infinite;}";
  page += "@keyframes blink{50%{opacity:0.5;}}</style>";

  // Inserta el logo codificado en Base64
  page += "<img src='data:image/png;base64,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' alt='Logo'>";

  page += "<script>";
  page += "function updateStatus() {";
  page += "  fetch('/status').then(response => response.json()).then(data => {";
  page += "    document.getElementById('statusGreen').style.display = data.state === 'standby' ? 'block' : 'none';";
  page += "    document.getElementById('statusYellow').style.display = data.state === 'detecting' ? 'block' : 'none';";
  page += "    document.getElementById('statusRed').style.display = data.state === 'unloading' ? 'block' : 'none';";
  page += "  });";
  page += "}";
  page += "setInterval(updateStatus, 1000);"; // Actualiza cada segundo
  page += "</script></head><body>";
  page += "<h2>PoopConveyor Control</h2>";
  page += "<div class='status'>";
  page += "<span id='statusGreen' class='standby'>En espera</span>";
  page += "<span id='statusYellow' class='detecting' style='display:none;'>Detectando</span>";
  page += "<span id='statusRed' class='unloading' style='display:none;'>Descargando</span></div>";
  page += "<form action='/set' method='get'>";
  page += "<label>Velocidad cinta (0-255):</label><input name='speed' value='" + String(motorSpeed) + "'><br>";
  page += "<label>Tiempo de detección sensor (ms):</label><input name='detection' value='" + String(detectionTime) + "'><br>";
  page += "<label>Velocidad parpadeo led detección (ms):</label><input name='blink' value='" + String(blinkInterval) + "'><br>";
  page += "<button type='submit'>Guardar</button></form>";
  page += "<button onclick=\"location.href='/manual'\">Mover en Manual</button>";
  page += "<button onclick=\"location.href='/stop'\">Paro</button>";
  page += "<p><small>1 seg = 1000 ms | 1 min = 60000 ms</small></p></body></html>";
  return page;
}

// Devuelve el estado actual en formato JSON
void sendStatus() {
  String state = "standby";
  if (detecting) state = "detecting";
  else if (motorRunning) state = "unloading";

  String json = "{\"state\":\"" + state + "\"}";
  server.send(200, "application/json", json);
}

void startMotor() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enablePin, motorSpeed);
  digitalWrite(ledRed, HIGH);
  motorRunning = true;
  motorStopTime = millis() + motorRunTime;
  Serial.println("Motor activado.");
}

void stopMotor() {
  analogWrite(enablePin, 0);
  digitalWrite(ledRed, LOW);
  motorRunning = false;
  Serial.println("Motor detenido.");
  digitalWrite(ledGreen, HIGH); // Vuelve a estado "standby"
}

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
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0)); // Configuración manual de la IP
  Serial.println("WiFi AP iniciado. IP:");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() {
    server.send(200, "text/html", webPage());
  });

  server.on("/status", sendStatus);

  server.on("/set", []() {
    if (server.hasArg("speed")) motorSpeed = server.arg("speed").toInt();
    if (server.hasArg("detection")) detectionTime = server.arg("detection").toInt();
    if (server.hasArg("blink")) blinkInterval = server.arg("blink").toInt();
    server.send(200, "text/html", webPage());
  });

  server.on("/manual", []() {
    startMotor();
    server.send(200, "text/html", webPage());
  });

  server.on("/stop", []() {
    stopMotor();
    server.send(200, "text/html", webPage());
  });

  server.begin();
}

void loop() {
  server.handleClient();
  unsigned long currentMillis = millis();

  int motion = digitalRead(pinPIR);

  if (motion == HIGH && !motorRunning && !detecting) {
    Serial.println("Movimiento detectado");
    digitalWrite(ledGreen, LOW);
    detecting = true;
    detectionEndTime = currentMillis + detectionTime;
  }

  if (detecting && currentMillis < detectionEndTime) {
    if (currentMillis - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = currentMillis;
      ledState = !ledState;
      digitalWrite(ledYellow, ledState ? HIGH : LOW);
    }
  }

  if (detecting && currentMillis >= detectionEndTime) {
    detecting = false;
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, HIGH);
    startMotor();
  }

  if (motorRunning && currentMillis >= motorStopTime) {
    stopMotor();
    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, HIGH);
  }
}

