/* Including the SDK and WiFi library */
#include <Apollo.h>
#include <ESP8266WiFi.h>

/* Configurations */
String deviceID = "YOUR-DEVICE-ID";
String apiKey = "YOUR-APIKEY";
String token = "YOUR-ACCESS-TOKEN";

// /* WiFi credentials */
String ssid = "WIFI-SSID";
String password = "WIFI-PASSWORD";

/* New object of ApolloDevice class */
ApolloDevice device;

/* Function to check device's connection status */
void onConnection(JSONObject updateObject) {
  switch((int) updateObject["event"]) {
        case CONNECTED:
          /* Device connected to the cloud */
          
          Serial.println("Device is connected to the cloud.");
          return;

        case DISCONNECTED:
          /* Device disconnected from cloud */
          Serial.println("Device is disconnected from the cloud.");

          return;
  }
}

/* Function to handle parms update event  */
void handleParmsUpdate(JSONObject updateObject) {
    /* Get state */
    int state = (int) updateObject["state"];
    
    /* Print state */
    Serial.printf("Updated state is %d\n", state);
    
    /* Update pin level */
    digitalWrite(2, state);
}

/* Function to connect to WiFi */
void connectWiFi() {
    /* Set mode to station */
    WiFi.mode(WIFI_STA);

    /* Connect using the ssid and password */
    WiFi.begin(ssid, password);

    /* Block till WiFi connected */
    while (WiFi.status() != WL_CONNECTED) {
         delay(500);
         Serial.print(".");
    }
    
    /* Connected to WiFi so print message */
    Serial.println("");
    Serial.println("WiFi connected");

    /* and IP address */
    Serial.println(WiFi.localIP());
}

/* In setup */
void setup() {
    /* Begin the serial */
    Serial.begin(9600);

    /* Connect to WiFi */
    connectWiFi();
    
    /* Initializes the global object "apollo" with your configurations. */
    device = apollo.init(deviceID, apiKey, token);

    /* Sets connection state update handler */
    device.onConnection(onConnection);

    /* Subscribe to change of params */
    device.onParmsUpdated(handleParmsUpdate);

    /* Set mode of LED to output */
    pinMode(2, OUTPUT); 
    
    /* By default turn the LED off */
    digitalWrite(2, 0);    
}

/* Loop function */
void loop() {
    /* 
        Synchronizes the SDK with the cloud 
        SDK will loop till we are connected to WiFi
    */
    device.loop(WiFi.status() == WL_CONNECTED);
} 
