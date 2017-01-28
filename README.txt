KADENZE-MACHINE LEARNING ASS5
Jordi Sala 2017

GitHub: https://github.com/poperbu/kadenze_ml_ass5

INTRODUCTION:

I used HUZZAH ESP8266+ Ardafruit 10DOF to create a wireless sensor. ESP8266 sends sensors data via OSC over a WiFi network to the input.pd Pure Data patch. Once input.pd patch has adapted data, it sends over OSC to Wekinator where the features are extracted. Output.pd patch receive outputs from Wekinator, and a simple musical instrument is controlled by magnetic fields (synths) and accelerometers (percussion). The idea is integrate this hardware in a ball, so when we play with the ball we can play music and the same time.

	[HUZZAH ESP8266] -----OSC (wifi)-----> [input.pd] -----OSC (localhost)-----> [Wekinator] -----OSC(localhost)----->[output.pd]

It could be integrated to any object and detect its movements: skate, bike, human, drone, etc.

HARDWARE USED AS INPUT:

	·Ardafruit HUZZAH ESP8266 https://www.adafruit.com/product/2471

	·

	·Ardafruit 10-DOF IMU https://www.adafruit.com/products/1604

	·Arduino libraries: ESP8266 WifiUdp OSCMessage Adafruit_Sensor Adafruit_LSM303_U

HOWTO:

1-Hardware configuarion:

-Edit Huzzah_template.ino and change WiFi parameters, network addresses and ball ID.
-Load the file to your board.
-You can use ARDUINO IDE:  ESP8266 with Arduino IDE: https://learn.adafruit.com/adafruit-huzzah-esp8266-breakout/using-arduino-ide

2-input.pd (puredata)

-It works similar to Wekinator helper. In TRAINING_HELPER subpatch you can test diferent tools for filtering, smoothing, limiti, etc the input signals before send them to Wekinator.
-Once you have your settings ready, connect to wekinator (wekinator must be running). Check Connect in SEND_WEKINATOR subpatch.

3-Wekinator:

-huzzah_wekinator.zip contains a wekinator project , you can extract and use it or you can do your own training.

4-output.pd

-It receives the data from wekinator, and uses them as input for 3 instruments.
-You can modify the connections and adapt the instrument as you want.

