![Image](https://github.com/user-attachments/assets/e391a6e1-94c3-45d1-b8ee-b8c2a2a1f943)

# PoopsConveyor

Rediseño del proyecto de t0nyz (https://t0nyz.com/projects/bambuconveyor#overview), simplificando conexiones y software.

Los archivos STL son del creador t0nyz y se pueden encontrar sus repositorios

El trabajo y el esfuerzo en el diseño pueden ser recompensados comentando las publicaciones y apoyando al creador t0nyz.

## Descripción

PoopsConveyor es una aplicación diseñada para gestionar la salida de residuos de una impresora 3D para cualquier marca del mercado. 

Utiliza un sensor de detección de movimiento para supervisar  y controlar un motor que retira los residuos del área de impresión mediante una cinta de transporte adaptable por longitud.
## Repositorios STL

Caja electrónica: https://makerworld.com/en/models/1071359-bambu-poop-conveyor-version-2-esp32-housing#profileId-1063585

Cinta de transporte: https://makerworld.com/en/models/148083-a1-poop-conveyor#profileId-161573

Extensión cinta transporte: https://makerworld.com/en/models/1325715-conveyor-extension#profileId-1362873

#### Alternativas caja electrónica:

Caja contenedor:  https://makerworld.com/en/models/1081634-poop-conveyor-ir-controller-mod#profileId-1073487
## Material

Motor JGY-370 10rpm 12v: https://es.aliexpress.com/item/1005007487336275.html

Módulo motor L298N: https://es.aliexpress.com/item/32392774289.html

Placa expansión ESP32 30 pines: https://es.aliexpress.com/item/1005007968065380.html

Mini IR piroeléctrico infrarrojo PIR: https://es.aliexpress.com/item/1005007244048605.html

ESP32-WROOM-32S 30 pines : https://es.aliexpress.com/item/1005007024756008.html (Color: ESP32 Type C-Board
)

Transformador Controlador AC 100V-240V a DC 12V 3A: https://es.aliexpress.com/item/1005006317791960.html

Adaptador Transformador: https://es.aliexpress.com/item/4000497237064.html

Surtido resistencias: https://es.aliexpress.com/item/1005005855324735.html

Surtido cables dupont: https://es.aliexpress.com/item/1005003641187997.html

Surtido Leds: https://es.aliexpress.com/item/1005003320140965.html

Rodamientos 8mm X 22mm X 7mm: https://es.aliexpress.com/item/1005008627764470.html
## Installation

Diagrama eléctrico

![Image](https://github.com/user-attachments/assets/93832fed-541b-4cfe-b708-b0ecaa462fda)

```bash
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

```

### Notas:

Debemos quitar el jumper y conectar el PIN 27 en el pin inferior de ENA del contrrolador L298N

![Image](https://github.com/user-attachments/assets/6b0e0359-a1bc-4545-ad84-fb2133925c1b)

# Cómo conectarte por WiFi a la app desde tu smartphone

![Image](https://github.com/user-attachments/assets/2b9a73fa-cb37-4735-b02f-d09b010ab40d)

#### 1. Configurar el ESP32

Asegúrate de que tu ESP32 esté funcionando correctamente y que el código cargado incluya la configuración del punto de acceso WiFi con el SSID (nombre de la red) y contraseña. En nuestro caso:

Nombre de la red: PoopConveyor

Contraseña: 12345678

IP predeterminada: 192.168.4.1

#### Conectar el smartphone al WiFi

Abre la configuración WiFi del smartphone.

En Android: Ve a Configuración > Conexiones > WiFi.

En iOS: Ve a Ajustes > WiFi.

Busca y selecciona la red WiFi del ESP32.

Deberías ver la red con el nombre PoopConveyor en la lista de redes disponibles.

Introduce la contraseña.

Contraseña: 12345678.

Conéctate a la red.

Una vez conectado, el dispositivo móvil no tendrá acceso a internet, lo cual es normal porque el ESP32 no está conectado a un router.

#### 3. Acceder a la app desde el navegador

Abre el navegador web en tu smartphone.

Puedes usar cualquier navegador (Chrome, Safari, Edge, etc.).

Ingresa la dirección IP de la ESP32.

Escribe 192.168.4.1 en la barra de direcciones y presiona "Ir" o "Enter".

Carga la página.

La app web del ESP32 debería cargarse y mostrar la interfaz con los controles.

#### 4. Navegar por la app web

Ahora puedes:

Ajustar los parámetros como velocidad, tiempo de detección, etc.

Ver los estados dinámicos de los LEDs (en espera, detectando, descargando).

Mover el motor en manual o detenerlo usando los botones.
## Configuración Arduino

Descarga IDE Arduino: https://www.arduino.cc/en/software/

### Instalar Arduino ESP32 Boards by Arduino
Tools > Manage Libraries...

![Image](https://github.com/user-attachments/assets/dd12d55c-e795-47ff-9f3e-7baee06d7c95)

### Configuración Boards

![Image](https://github.com/user-attachments/assets/c6bcbdec-3696-4235-8049-eb4a265388dd)



