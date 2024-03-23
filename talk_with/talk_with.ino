// Talk with other boards

#include <ESP8266WiFi.h>
#include <espnow.h>

// The MAC addresses other boards to talk with.  Can be many of them.
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// I/O
#define thisLED D0
#define thatLED D1
#define thisSW D5

// how frequently to send/receive data
const long interval = 100;
unsigned long previousMillis = 0;

// data structure
typedef struct struct_message {
    bool sw;
} struct_message;

// Variable to store if sending data was successful
String success;

// Create a struct_message to hold incoming sensor readings
struct_message dataSend;
struct_message dataRead;

bool thatSW;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&dataRead, incomingData, sizeof(dataRead));
  Serial.print("Bytes received: ");
  Serial.println(len);
  thatSW = dataRead.sw;
}

void printDataRead(){
  // Display Readings in Serial Monitor
  Serial.println("INCOMING READINGS");
  Serial.print("SW: ");
  Serial.println(thatSW);
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  Serial.print("ESP8266 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  pinMode(thisSW, INPUT_PULLUP);
  pinMode(thisLED, OUTPUT);
  digitalWrite(thisLED, LOW);
  pinMode(thatLED, OUTPUT);
  digitalWrite(thatLED, LOW);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Set values to send
    dataSend.sw = digitalRead(thisSW);
    Serial.print("INFO SW: ");
    Serial.println(dataSend.sw);

    // local LED
    if (dataSend.sw == LOW) {
      digitalWrite(thisLED, HIGH);
    }
    else {
      digitalWrite(thisLED, LOW);
    }

    // remote controlled LED
    if (thatSW == LOW) {
      digitalWrite(thatLED, HIGH);
    }
    else {
      digitalWrite(thatLED, LOW);
    }

    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &dataSend, sizeof(dataSend));

    printDataRead();
  }
}

