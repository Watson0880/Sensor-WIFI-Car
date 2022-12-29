/*
    This sketch demonstrates how to set up a simple HTTP-like server.
    The server will set a GPIO pin depending on the request
      http://server_ip/gpio/0 will set the GPIO2 low,
      http://server_ip/gpio/1 will set the GPIO2 high
    server_ip is the IP address of the ESP8266 module, will be
    printed to Serial when the module is connected.
*/

#include <ESP8266WiFi.h>
#include <Servo.h>
Servo myservo;
int d0 = 16;
int d1 = 5;
int d2 = 4;
int d3 = 0;
int d4=2;
int w=0,a=0,d=0,s=0,speed=0;
int turnover = 0;
int speed_state=0;
const char* ssid = "Redmi";
const char* password = "watson0880";
String s0 = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now 0";
String s1 = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now 1";
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // prepare LED
  pinMode(d0, OUTPUT);
  //pinMode(d1, OUTPUT);
  myservo.attach(d1);
  //myservo.attach(d2);
  //pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, INPUT);
  digitalWrite(d0, 0);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    if (w==1){
    speed_state+=1;
    if (speed_state>256) speed_state=0;
    if (speed_state%8==0){
      digitalWrite(d0, 0);
      digitalWrite(d3, 0);
    }
    else if (speed==0 && speed_state%8==2){
      digitalWrite(d0, 0);
      digitalWrite(d3, w);
    }
    else if (speed==1 && speed_state%8==1){
      digitalWrite(d0, 0);
      digitalWrite(d3, w);
    }
    }

    if (s==1){
      speed_state+=1;
    if (speed_state>256) speed_state=0;
    if (speed_state%8==0){
      digitalWrite(d0, 0);
      digitalWrite(d3, 0);
    }
    else if (speed_state%8==1){
      digitalWrite(d0, s);
      digitalWrite(d3, 0);
    }
    }
    return;
  }
  Serial.println("new client");

  //client.setTimeout(5000); // default is 1000
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println("request: ");
  Serial.println(req);
  //client.flush();

  // Match the request
  
  if (req.indexOf(F("/d0/0")) != -1) {
    w = 0;
    digitalWrite(d0, w);
    digitalWrite(d3, w);
  } 
  else if (req.indexOf(F("/d0/1")) != -1) {
    w = 1;
    Serial.println("forward");
    digitalWrite(d0, 0);
    digitalWrite(d3, w);
  }

  if (req.indexOf(F("/d1/0")) != -1) {
    //a = 0;
    myservo.write(90);
    //digitalWrite(d1, a);
  } 
  else if (req.indexOf(F("/d1/1")) != -1) {
    //a = 1;
    Serial.println("left");
    myservo.write(143);
    //digitalWrite(d1, a);
  }

  if (req.indexOf(F("/d2/0")) != -1) {
    //d = 0;
    myservo.write(90);
    //digitalWrite(d2, d);
  } 
  else if (req.indexOf(F("/d2/1")) != -1) {
    //d = 1;
    Serial.println("right");
    myservo.write(37);
    //digitalWrite(d2, d);
  }

  if (req.indexOf(F("/d3/0")) != -1) {
    s = 0;
    digitalWrite(d0, s);
    digitalWrite(d3, s);
  } 
  else if (req.indexOf(F("/d3/1")) != -1) {
    s = 1;
    Serial.println("back");
    digitalWrite(d0, s);
    digitalWrite(d3, 0);
  }
  if (req.indexOf(F("/d4/0")) != -1) {
    speed = speed;
  } 
  else if (req.indexOf(F("/d4/1")) != -1) {
    speed = (speed+1)%2;
    Serial.println("speed");
  }
  if (req.indexOf(F("/d5/1")) != -1) {
    turnover = digitalRead(d4);
  }
  
  // Set LED according to the request
  // read/ignore the rest of the request
  // do not client.flush(): it is for output only, see below
  /*
  while (client.available()) {
    // byte by byte is not very efficient
    client.read();
  }
  */
  client.flush();
  // Send the response to the client
  // it is OK for multiple small client.print/write,
  // because nagle algorithm will group them into one single packet
  
  if (turnover == 1){
    client.print(s0);
  }
  else {
    client.print(s1);
    turnover = 1;
  }
  
  //client.print(WiFi.localIP());
  // The client will actually be *flushed* then disconnected
  // when the function returns and 'client' object is destroyed (out-of-scope)
  // flush = ensure written data are received by the other side
  Serial.println(F("Disconnecting from client"));
}
