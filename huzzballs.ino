#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

//sensor libs
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

/*--------------NETWORK-----------------------------*/
char ssid[] = "YOUR_SSID";          // your network SSID (name)
char pass[] = "YOUR_KEY";                    // your network password

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192,168,1,51);        // remote IP of your computer
const unsigned int outPort = 9999;          // remote port to receive OSC
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)

/*--------------SENSOR-----------------------------*/
/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

/*GENERAL*/
boolean SERIALMONITOR=true;
String skate = "1";
int DELAYTIME=20;


void setup() {
    Serial.begin(115200);

    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Starting UDP");
    Udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(Udp.localPort());

  /*ACCELEROMETER*/
  Serial.println("Accelerometer Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();

  /*MAGNETO*/
  Serial.println("Magnetometer Test"); Serial.println("");
  
  /* Enable auto-gain */
  mag.enableAutoRange(true);
  
  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
}

void loop() {
   /*ACCELEROMETER*/  
    /* Get a new sensor event */ 
    sensors_event_t event; 
    accel.getEvent(&event);
    displaySerialAccl(event);
    sendSensorOSC("acclx",event.acceleration.x);
    sendSensorOSC("accly",event.acceleration.y);
    sendSensorOSC("acclz",event.acceleration.z);
   
    /* MAGNETIC SENSOR*/ 
    sensors_event_t event2; 
    mag.getEvent(&event2);
    displaySerialMagn(event2);
 
    sendSensorOSC("magnx",event2.magnetic.x);
    sendSensorOSC("magny",event2.magnetic.y);
    sendSensorOSC("magnz",event2.magnetic.z);
    

    delay(DELAYTIME);
}
void sendSensorOSC(String valueName,float valueFloat){
    String missatge="/"+skate+"/"+valueName;
    int miss_len=missatge.length()+1;
    char osc_miss[miss_len];
    missatge.toCharArray(osc_miss,miss_len);
    OSCMessage msg(osc_miss);
    msg.add(valueFloat);
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
    delay(2);
}
void displaySerialMagn(sensors_event_t event){
  if(SERIALMONITOR){
    /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
    Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");
  }
}

void displaySerialAccl(sensors_event_t event){
  if(SERIALMONITOR){
    /* Display the results (acceleration is measured in m/s^2) */
    Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
  }
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

