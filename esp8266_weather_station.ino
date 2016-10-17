#include <ESP8266WiFi.h>
#include <DHT.h>


// Router SSID and Password
#define NETW "MySSID"
#define PASS "MyPassword"

// Serial Baud Speed
#define BAUD 115200

// DHT22 Sensor
#define DHTTYPE DHT22
#define DHTPIN 2

// Panahon IoT Server
#define SERVER "armanvp.com"
#define ENDPOINT "/api/data/post"
#define PORT 3001
#define UPDATE_TIME 1000 * 60 * 1 // 5 mins
#define RETRY_TIME 1000 * 5 // 5 secs

DHT dht(DHTPIN, DHTTYPE, 11);
WiFiClient client;

// wifiConnect
void wifiConnect() {

  WiFi.begin(NETW, PASS);
  Serial.print("Connecting to ");
  Serial.print(NETW);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println();
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
}

// Gets MAC Address of ESP8266
String getMacAddr() {

  uint8_t macAddr[6];
  String macAddrStr;

  WiFi.macAddress(macAddr);
  for (uint8_t i = 0; i < 6; i++) {
    if (!macAddrStr.length()) {
      macAddrStr = String(macAddr[i],HEX);
    } else {
      macAddrStr += ":" + String(macAddr[i],HEX);
    }
  }
  
  return macAddrStr;
}

// Request Path for the IoT Sever
bool buildRequestPath(String *path) {

  // Get Sensor Data
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if both data were retrieved
  if (isnan(h) || isnan(t)) {
    return false;
  }

  *path = ENDPOINT + String("?key=") + getMacAddr()
    + String("&t=") + String(t) + String("&h=") + String(h);
    
  return true;
}

// Request String to be sent to the IoT Sever
String buildRequestStr(String host, String path) {

  return String("GET ") + path + " HTTP/1.1\r\n"
    + "Host: " + host + "\r\n"
    + "Connection: close\r\n\r\n";
  
}

void setup() {

  // Initialize Serial
  Serial.begin(BAUD);
  delay(1000);
  Serial.println();
  
  Serial.println("*---------------------------------*");
  Serial.println(" ESP8266 Weather Station ");
  Serial.println(" http://armanvp.com ");
  Serial.println("*---------------------------------*");

  Serial.print("MAC Address: ");
  Serial.println(getMacAddr());
  Serial.print("ID: ");
  Serial.println(ESP.getChipId());
  Serial.print("Size (KB): ");
  Serial.println(ESP.getFlashChipSize()/1024);
  Serial.print("Speed (HZ): ");
  Serial.println(ESP.getFlashChipSpeed());

  // Connect to WiFi
  wifiConnect();
  
}

void loop() {

  String path;

  Serial.println("Reading DHT22 sensor data...");

  // Build Path and Get Sensor Data
  if (!buildRequestPath(&path)) {
    Serial.println("Error retrieving sensor data, retry again..");
    delay(RETRY_TIME);
    return;
  }

  // Make sure we are connected!
  if (WiFi.status() != WL_CONNECTED) {
    wifiConnect();
  }
  
  Serial.println("Sending data...");

  // Connect to server
  if (client.connect(SERVER, PORT)) {
    // Send Request
    client.print(buildRequestStr(SERVER, path));
    Serial.println("Data sent!");
    Serial.println("*---------------------------------*");
    while (client.connected()) {
      if (client.available()) {
        Serial.println(client.readStringUntil('\n'));
      }
    }
    Serial.println("*---------------------------------*");
    client.stop();

  // Error connecting to server
  } else {
    Serial.println("Connection failed!");
    client.stop();
    Serial.println("Retrying to send data again...");
    delay(RETRY_TIME);
    return;
  }

  // Disconnect from WiFi
  Serial.println("Disconnecting from WiFi...");
  WiFi.disconnect();

  Serial.println("Waiting for next update...");
  delay(UPDATE_TIME);
}
