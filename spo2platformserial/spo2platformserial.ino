/**
 * A BLE client example that is rich in capabilities.
 * There is a lot new capabilities implemented.
 * author unknown
 * updated by chegewara
 */

#include "BLEDevice.h"

#include <WiFi.h>
#include <HTTPClient.h>

#define ssid "PIERROT139"
#define password "milo0638149355"

const char* rootCACertificate = \
"-----BEGIN CERTIFICATE-----\n"
"MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n"
"TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n"
"cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n"
"WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n"
"ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n"
"MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n"
"h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n"
"0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n"
"A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n"
"T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n"
"B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n"
"B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n"
"KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n"
"OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n"
"jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n"
"qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n"
"rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n"
"HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n"
"hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n"
"ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n"
"3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n"
"NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n"
"ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n"
"TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n"
"jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n"
"oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n"
"4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n"
"mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n"
"emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n"
"-----END CERTIFICATE-----\n";

//Your Domain name with URL path or IP address with path
const char* serverName = "https://connexthings.io/device/QkkKJFrV1eeETLiHKehf/reportedStates";

// The remote service we wish to connect to.
static BLEUUID serviceUUID("0000fff0-0000-1000-8000-00805f9b34fb");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("0000fff1-0000-1000-8000-00805f9b34fb");
// The address 
static BLEAddress bluedot("00:1c:c2:55:ff:d0");

//flag doing connecting
static boolean doConnect = false;
//flag connected
static boolean connected = false;
//flag scanning device
static boolean doScan = false;
//characteristic of device
static BLERemoteCharacteristic* pRemoteCharacteristic;
//advertised of device
static BLEAdvertisedDevice* myDevice;

BLEClient*  pClient;

//value that will push to magellan 
int pr;
int spo2;
int pi;

//wifi client
WiFiClientSecure *client = new WiFiClientSecure;
int wificonnect;
int wificon_state = 0;

//data that sent from sensor in pData array
static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  //corrected value that match with sensor screen pData[2] must equal 129
  if (pData[2] == 0x81) {
    pr = pData[4];
    spo2 = pData[3];
    pi = (pData[8]*0.2588)*10;
  }
}

//check connection of device
class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println(F("onDisconnect"));
  }
};

//connect to spo2 sensor
bool connectToServer() {
    Serial.print(F("Forming a connection to "));
    Serial.println(myDevice->getAddress().toString().c_str());
    
    pClient  = BLEDevice::createClient();
    Serial.println(F(" - Created client"));

    //client call MyClinetCallback for chenking connection
    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print(F("Failed to find our service UUID: "));
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(F(" - Found our service"));


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    //if characteristic are null then disconnect 
    if (pRemoteCharacteristic == nullptr) {
      Serial.print(F("Failed to find our characteristic UUID: "));
      Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(F(" - Found our characteristic"));

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      Serial.print(F("The characteristic value was: "));
      Serial.println(value.c_str());
    }

    if(pRemoteCharacteristic->canNotify())
      pRemoteCharacteristic->registerForNotify(notifyCallback);
    
    connected = true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print(F("\nBLE Advertised Device found: "));
    Serial.println(advertisedDevice.toString().c_str());
    
    Serial.print(F("Address: "));
    Serial.println(advertisedDevice.getAddress().toString().c_str());
    //check service UUID Have?Haven't 
    if (advertisedDevice.haveServiceUUID()) {
      Serial.println(F("Device has Service UUID"));
      if (advertisedDevice.isAdvertisingService(serviceUUID)) {
        Serial.println(F("Device is advertising our Service UUID"));
      }
      else {
        Serial.println(F("Device is not advertising our Service UUID"));
      }
    }
    else {Serial.println(F("Device does not have Service UUID"));}
    
    // found device by using service UUID & address then connect to device
    if ((advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) || (advertisedDevice.getAddress().equals(bluedot))) {
      //stop searching device
      BLEDevice::getScan()->stop();
      //new device to connect
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

void setup() {
  Serial.begin(115200);
  
  BLEDevice::init("");
  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 300 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

} // End of setup.



// This is the Arduino main loop function.
void loop() {
  
    String blood = String(pr);
    String oxy = String(spo2);
    String p_index = String(pi);
    
    // If the flag "doConnect" is true then we have scanned for and found the desired
    // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
    // connected we set the connected flag to be true.
      if (doConnect == true) {
        Serial.println(F("Starting Arduino BLE Client application..."));
        if (connectToServer()) {
          Serial.println(F("We are now connected to the BLE Server."));
        } else {
          Serial.println(F("We have failed to connect to the server; there is nothin more we will do."));
        }
        doConnect = false;
      }
    
      // If we are connected to a peer BLE Server, update the characteristic each time we are reached
      // with the current time since boot.
      if (connected) {
        String newValue = "Time since boot: " + String(millis()/1000);
        //Serial.println("Setting new characteristic value to \"" + newValue + "\"");
        
        // Set the characteristic's value to be the array of bytes that is actually a string.
        pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
        Serial.print("PR: ");
        Serial.println(pr);
        Serial.print("SpO2: ");
        Serial.println(spo2);
        Serial.print("PI: ");
        Serial.println(pi);
        Serial.println("------------------------");
      }else if(doScan){
        BLEDevice::getScan()->start(0);  // this is just sample to start scan after disconnect, most likely there is better way to do it in arduino
      }
     delay(1100);
     
    WiFiClientSecure *client = new WiFiClientSecure;
    if(client) {
      client -> setCACert(rootCACertificate);
      {
        BLEDevice::deinit(false);
        HTTPClient http;

        // Your Domain name with URL path or IP address with path
        http.begin(*client, serverName);
    
        // Specify content-type header
        http.addHeader("Content-Type", "application/json");
        // Data to send with HTTP POST
        //if sensor have value(sensor still working) then send to magellan
        if(blood != "0" || oxy != "0"){
          //data payload<string> push to magellan
          String payload = "{\"bpm\":"+blood+",\"spo2\":"+oxy+",\"pi\":"+p_index+"}";
          // Send HTTP POST request
          int httpResponseCode = http.POST(payload);
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          String res = http.getString();
          Serial.println(res); 
          // Free resources
          http.end();
          /*WiFi.disconnect(true);
          delay(1); // disable WIFI altogether
          WiFi.mode(WIFI_OFF);*/
          //delay sending data for 5 sec
          delay(1000);
          delete(pClient);
        }
        if(blood == "0" && oxy == "0"){
          wificon_state += 1; 
        }
        if(wificon_state > 1){
          http.end();
          /*WiFi.disconnect(true);
          delay(1); // disable WIFI altogether
          WiFi.mode(WIFI_OFF);*/
          //delay sending data for 5 sec
          delay(1000);
          delete(pClient);
        }
       }
      }
    else {
      Serial.println(F("WiFi Disconnected"));
    }
} // End of loop
