#include <WiFi.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char* ssid = "ROVER-ESP32";  // Enter SSID here
const char* password = NULL; //"12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

// Motor A
int enable1Pin = 32; 
int motor1Pin1 = 25; 
int motor1Pin2 = 33; 


// Motor B
int motor2Pin1 = 27; 
int motor2Pin2 = 26; 
int enable2Pin = 14; 

int sonicTx = 16;
int sonicRx = 5;


// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;

bool LED1status;
bool LED2status;
typedef enum MotorControl { STOP, FWD, BACK, LEFT, RIGHT};
MotorControl motorControl = STOP;


void setup() {
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  pinMode(sonicTx, OUTPUT);
  pinMode(sonicRx, OUTPUT);
  
  // configure LED PWM functionalitites
//  ledcSetup(pwmChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
//  ledcAttachPin(enable1Pin, pwmChannel);
//  ledcAttachPin(enable2Pin, pwmChannel);
  digitalWrite(enable1Pin, HIGH); // full speed
  digitalWrite(enable2Pin, HIGH);

  Serial.begin(115200);

  // testing
  Serial.print("Testing DC Motor...");
  
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/stop", handle_stop);
  server.on("/fwd", handle_fwd);
  server.on("/back", handle_back);
  server.on("/left", handle_left);
  server.on("/right", handle_right);
  server.onNotFound(handle_NotFound);
  
  server.begin();
}

void loop() {
  server.handleClient();
    
  // Move the DC motor forward at maximum speed
  if (motorControl == FWD)
  {
    Serial.println("Moving Forward");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW); 
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW); 
  }
  else if (motorControl == BACK)
  {
    Serial.println("Moving Backwards");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH); 
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH); 
  }
  else if (motorControl == LEFT)
  {
    Serial.println("Moving Left");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH); 
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW); 
  }
  else if (motorControl == RIGHT)
  {
    Serial.println("Moving Right");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW); 
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH); 
  }
  else
  {
    Serial.println("Motor stopped");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);    
  }

  delay(200);


  
  #if 0
  // Move DC motor forward with increasing speed
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  while (dutyCycle <= 255){
    ledcWrite(pwmChannel, dutyCycle);   
    Serial.print("Forward with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle = dutyCycle + 5;
    delay(500);
  }
  dutyCycle = 200;
  #endif
}

void handle_OnConnect() {

  Serial.println("onConnect");
  server.send(200, "text/html", SendHTML()); 
}

void handle_stop() {
  motorControl = STOP;
  Serial.println("STOP");
  server.send(200, "text/html", SendHTML()); 
}

void handle_fwd() {
  motorControl = FWD;
  Serial.println("FWD");
  server.send(200, "text/html", SendHTML()); 
}

void handle_back() {
  motorControl = BACK;
  Serial.println("BACK");
  server.send(200, "text/html", SendHTML()); 
}

void handle_left() {
  motorControl = LEFT;
  Serial.println("LEFT");
  server.send(200, "text/html", SendHTML()); 
}

void handle_right() {
  motorControl = RIGHT;
  Serial.println("RIGHT");
  server.send(200, "text/html", SendHTML()); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ROVER Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
//  ptr +=".button-fwd {background-color: #3498db;}\n";
//  ptr +=".button-fwq:active {background-color: #2980b9;}\n";
//  ptr +=".button-stop {background-color: #34495e;}\n";
//  ptr +=".button-stop:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ROVER Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
  if(motorControl == FWD)
  {
    ptr += "<p>Status: FORWARD</p>";
  }
  else if (motorControl == BACK)
  {
    ptr += "<p>Status: BACK</p>";
  }
  else if (motorControl == LEFT)
  {
    ptr += "<p>Status: LEFT</p>";
  }
    else if (motorControl == RIGHT)
  {
    ptr += "<p>Status: RIGHT</p>";
  }
  else
  {
    ptr += "<p>Status: STOPPED</p>";
  }

  ptr +="<a class=\"button button-fwd\" href=\"/fwd\">FWD</a>\n";
  ptr +="<a class=\"button button-stop\" href=\"/stop\">STOP</a>\n";
  ptr +="<a class=\"button button-back\" href=\"/back\">BACK</a>\n";
  ptr +="<a class=\"button button-left\" href=\"/left\">LEFT</a>\n";
  ptr +="<a class=\"button button-right\" href=\"/right\">RIGHT</a>\n";


  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
