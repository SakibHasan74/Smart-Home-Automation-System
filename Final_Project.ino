#include <Keypad.h>
#include <Servo.h>
#include <EEPROM.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(51, 49);
char msg;
char call;

int Button = 10; // Push Button

const byte numRows = 4; // number of rows on the keypad
const byte numCols = 4; // number of columns on the keypad

char keymap[numRows][numCols] =
    {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}};

char keypressed;                    // Where the keys are stored it changes very often
char code[] = {'1', '2', '3', '4'}; // The default code, you can change it or make it a 'n' digits one

char check1[sizeof(code)]; // Where the new key is stored
char check2[sizeof(code)]; // Where the new key is stored again so it's compared to the previous one

short a = 0, i = 0, s = 0, j = 0; // Variables used later

byte rowPins[numRows] = {22, 24, 26, 28}; // Rows 0 to 3 //if you modify your pins you should modify this too
byte colPins[numCols] = {30, 32, 34, 36}; // Columns 0 to 3

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 4);

#define datapin 53 // Digital pin we're connected to
#define DHTTYPE DHT22
#define DHTTYPE DHT22
DHT dht(datapin, DHTTYPE);
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);
Servo myservo;

/*//******************Main Code*************************
******************************************************
*****************************************************
*****************************************************/
// LED
//  int ledpin = 12;
int Fan = 48;
int led = 50;
int ledblue = 46;
// Servo
Servo ultraservo;
// Ultrasonic
int trigPin = 10;
int echoPin = 11;
long duration;
int distance;
// Buzzer
int buzzer = 8;
int buzzer1 = 9;
// Flame
int IRSensor = 13;
int statusSensor = 1;
// SmokeSenor
int smokeA0 = A14;
// Your threshold value
int sensorThres = 400;

char Incoming_value = 0; // Variable for storing Incoming_value

void setup()
{
  lcd.begin();
  dht.begin();
  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("Hello, world!"); // initialize the lcd
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("*ENTER THE CODE*");
  lcd.setCursor(1, 1);

  lcd.print("TO _/_ (Door)!!"); // What's written on the LCD you can change
  pinMode(Button, INPUT);
  myservo.attach(12);
  myservo.write(0);
  //  for(i=0 ; i<sizeof(code);i++){        //When you upload the code the first time keep it commented
  //            EEPROM.get(i, code[i]);             //Upload the code and change it to store it in the EEPROM
  //           }                                  //Then uncomment this for loop and reupload the code (It's done only once)

  // Main Code******************************************
  /*//LED
 pinMode(ledpin,OUTPUT);*/
  pinMode(Fan, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(ledblue, OUTPUT);
  pinMode(A0, OUTPUT);
  // Servo
  ultraservo.attach(7);
  // Ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Buzzer
  pinMode(buzzer, OUTPUT);
  pinMode(buzzer1, OUTPUT);
  // Serial_Monitor
  mySerial.begin(9600);
  Serial.begin(9600);
  // Flame
  pinMode(IRSensor, INPUT);
  // Smoke
  pinMode(smokeA0, INPUT);
  //*****************************
}

/*char Incoming_value = 0;                //Variable for storing Incoming_value
      //Sets digital pin 13 as output pin

void loop()
{
  if(Serial.available() > 0)
  {
    Incoming_value = Serial.read();      //Read the incoming data and store it into variable Incoming_value
    Serial.print(Incoming_value);        //Print Value of Incoming_value in Serial monitor
    Serial.print("\n");        //New line
    if(Incoming_value == '1')            //Checks whether value of Incoming_value is equal to 1
      digitalWrite(13, HIGH);  //If value is 1 then LED turns ON
    else if(Incoming_value == '0')       //Checks whether value of Incoming_value is equal to 0
      digitalWrite(13, LOW);   //If value is 0 then LED turns OFF
  }

}  */
void loop()
{

  keypressed = myKeypad.getKey(); // Constantly waiting for a key to be pressed
  if (keypressed == '*')
  { // * to open the lock
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("*ENTER THE CODE*"); // Message to show
    ReadCode();                      // Getting code function
    if (a == sizeof(code))
    { // The ReadCode function assign a value to a (it's correct when it has the size of the code array)
      OpenDoor();
      delay(5000);
      myservo.write(0);
    } // Open lock function if code is correct
    else
    {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("CODE"); // Message to print when the code is wrong
      lcd.setCursor(6, 0);
      lcd.print("INCORRECT");
      lcd.setCursor(15, 1);
      lcd.print("  ");
      lcd.setCursor(4, 1);
      lcd.print("GET AWAY!!!");
    }
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*ENTER THE CODE*");
    lcd.setCursor(1, 1);

    lcd.print("TO _/_ (Door)!!");
    // Return to standby mode it's the message do display when waiting
  }

  if (keypressed == '#')
  { // To change the code it calls the changecode function
    ChangeCode();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*ENTER THE CODE*");
    lcd.setCursor(1, 1);

    lcd.print("TO _/_ (Door)!!"); // When done it returns to standby mode
  }

  if (digitalRead(Button) == HIGH)
  { // Opening by the push button
    myservo.write(0);
  }

  /*//***************************************************
  ******************************************************
  *******************Main Code***************************
  ****************************************************/
  // LED

  /* digitalWrite(ledpin,HIGH);
   delay(500);
   digitalWrite(ledpin,LOW);
   delay(500);*/
  /* //Servo
   for(int angle=0;angle<=180;angle+=20){
       servo.write(angle);
       delay(200);
   }
    for(int angle=180;angle>=0;angle-=20){
       servo.write(angle);
       delay(200);
   }*/

  if (Serial.available() > 0)
  {
    Incoming_value = Serial.read(); // Read the incoming data and store it into variable Incoming_value
    Serial.print(Incoming_value);   // Print Value of Incoming_value in Serial monitor
    Serial.print("\n");             // New line
    if (Incoming_value == '1')
    {
      digitalWrite(Fan, HIGH); // Checks whether value of Incoming_value is equal to 1
    }
    // If value is 1 then LED turns ON
    else if (Incoming_value == '0') // Checks whether value of Incoming_value is equal to 0
      digitalWrite(Fan, LOW);
    else if (Incoming_value == '2') // Checks whether value of Incoming_value is equal to 0
      digitalWrite(led, HIGH);
    else if (Incoming_value == '3') // Checks whether value of Incoming_value is equal to 0
      digitalWrite(led, LOW);
    else if (Incoming_value == '4') // Checks whether value of Incoming_value is equal to 0
      digitalWrite(ledblue, HIGH);
    else if (Incoming_value == '5') // Checks whether value of Incoming_value is equal to 0
      digitalWrite(ledblue, LOW);   // If value is 0 then LED turns OFF
  }
  Smoke();

  // Ultrasonic
  Temp();
  Ultrasonic();
  if (distance < 5)
  {
    ultraservo.write(90);
  }
  else
  {
    ultraservo.write(0);
  }

  // Buzzer
  // Buzzer();
  // Flame
  Flame();
  if (statusSensor == 1)
  {
    Serial.write("No Fire Detected!");
  }
  else
  {
    Serial.write("Fire Detected!!!");
    MakeCall();
    SendMessage1();
    Buzzer1();
  }
  /*//Potentiometer
   sensorValue = analogRead(sensorPin);*/

  /*//********************************************
  ******************************************/
}

/*//********************************************
*******************Main Code******************
**********************************************/
void Ultrasonic()
{
  digitalWrite(trigPin, LOW); // clear trigger pin
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(10); // Set trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  distance = (duration * 0.034) / 2;
  Serial.print("Distance(cm): ");
  Serial.println(distance);
  delay(999);
}

void Buzzer()
{
  tone(buzzer, 200);
  delay(999);
  noTone(buzzer);
  delay(1000);
}
void Buzzer1()
{
  tone(buzzer1, 200);
  delay(999);
  noTone(buzzer1);
  delay(1000);
}

void Flame()
{
  statusSensor = digitalRead(IRSensor);
}

void Smoke()
{
  int analogSensor = analogRead(smokeA0);

  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  // Checks if it has reached the threshold value
  if (analogSensor > sensorThres)
  {
    Serial.write("Leakage Track!!");
    MakeCall();
    SendMessage();
    Buzzer();
  }
  else
  {
    Serial.write("No Leaks");
  }
  delay(100);
}

/*void PIR(){

}

void Temp&Humidity(){

}
*/
/*//***********************************
*************************************
***************************************
*************************************/

void ReadCode()
{        // Getting code sequence
  i = 0; // All variables set to 0
  a = 0;
  j = 0;

  while (keypressed != 'A')
  { // The user press A to confirm the code otherwise he can keep typing
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A')
    {                      // If the char typed isn't A and neither "nothing"
      lcd.setCursor(j, 1); // This to write "*" on the LCD whenever a key is pressed it's position is controlled by j
      lcd.print("*");
      j++;
      if (keypressed == code[i] && i < sizeof(code))
      { // if the char typed is correct a and i increments to verify the next caracter
        a++;
        i++;
      }
      else
        a--; // if the character typed is wrong a decrements and cannot equal the size of code []
    }
  }
  keypressed = NO_KEY;
}

void ChangeCode()
{ // Change code sequence
  lcd.clear();
  lcd.print("Changing code");
  delay(1000);
  lcd.clear();
  lcd.print("Enter old code");
  ReadCode(); // verify the old code first so you can change it

  if (a == sizeof(code))
  { // again verifying the a value
    lcd.clear();
    lcd.print("Changing code");
    GetNewCode1(); // Get the new code
    GetNewCode2(); // Get the new code again to confirm it
    s = 0;
    for (i = 0; i < sizeof(code); i++)
    { // Compare codes in array 1 and array 2 from two previous functions
      if (check1[i] == check2[i])
        s++; // again this how we verifiy, increment s whenever codes are matching
    }
    if (s == sizeof(code))
    { // Correct is always the size of the array

      for (i = 0; i < sizeof(code); i++)
      {
        code[i] = check2[i];    // the code array now receives the new code
        EEPROM.put(i, code[i]); // And stores it in the EEPROM
      }
      lcd.clear();
      lcd.print("Code Changed");
      delay(2000);
    }
    else
    { // In case the new codes aren't matching
      lcd.clear();
      lcd.print("Codes are not");
      lcd.setCursor(0, 1);
      lcd.print("matching !!");
      delay(2000);
    }
  }

  else
  { // In case the old code is wrong you can't change it
    lcd.clear();
    lcd.print("Wrong");
    delay(2003);
  }
}

void GetNewCode1()
{
  i = 0;
  j = 0;
  lcd.clear();
  lcd.print("Enter new code"); // tell the user to enter the new code and press A
  lcd.setCursor(0, 1);
  lcd.print("and press A");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("and press: A"); // Press A keep showing while the top row print ***

  while (keypressed != 'A')
  { // A to confirm and quits the loop
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A')
    {
      lcd.setCursor(j, 0);
      lcd.print("*");         // On the new code you can show * as I did or change it to keypressed to show the keys
      check1[i] = keypressed; // Store caracters in the array
      i++;
      j++;
    }
  }
  keypressed = NO_KEY;
}

void GetNewCode2()
{ // This is exactly like the GetNewCode1 function but this time the code is stored in another array
  i = 0;
  j = 0;

  lcd.clear();
  lcd.print("Confirm code");
  lcd.setCursor(0, 1);
  lcd.print("and press A");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("and press A");

  while (keypressed != 'A')
  {
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A')
    {
      lcd.setCursor(j, 0);
      lcd.print("*");
      check2[i] = keypressed;
      i++;
      j++;
    }
  }
  keypressed = NO_KEY;
}

void OpenDoor()
{ // Lock opening function open for 3s
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Access Granted");
  lcd.setCursor(4, 1);
  lcd.print("WELCOME!!");
  myservo.write(90);
}

void Temp()
{

  /* Find Temperature & Humidity */
  float air_temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  /* Print Output on LCD Screen */
  lcd.setCursor(0, 3);
  lcd.print((int)air_temp);
  lcd.print("*Cel  ");
  // lcd.print(String("Temp.") + String(air_temp));

  lcd.print((int)humidity);
  lcd.print("Humidity");
  // lcd.print(String("Humidity") + String(humidity));

  delay(2000);
}

void SendMessage()
{
  mySerial.println("AT+CMGF=1");                    // Sets the GSM Module in Text Mode
  delay(1000);                                      // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+8801611164224\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Gas Leakage Alert!!"); // The SMS text you want to send
  delay(100);
  mySerial.println((char)26); // ASCII code of CTRL+Z
  delay(1000);
}
void SendMessage1()
{
  mySerial.println("AT+CMGF=1");                    // Sets the GSM Module in Text Mode
  delay(1000);                                      // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+8801611164224\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Fire Detected Alert!!"); // The SMS text you want to send
  delay(100);
  mySerial.println((char)26); // ASCII code of CTRL+Z
  delay(1000);
}

void ReceiveMessage()
{
  mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to recieve a live SMS
  delay(1000);
  if (mySerial.available() > 0)
  {
    msg = mySerial.read();
    Serial.print(msg);
  }
}

void MakeCall()
{
  mySerial.println("ATD+8801611164224;"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
  Serial.println("Calling  ");            // print response over serial port
  delay(1000);
}

void HangupCall()
{
  mySerial.println("ATH");
  Serial.println("Hangup Call");
  delay(1000);
}

void ReceiveCall()
{
  mySerial.println("ATA");
  delay(1000);
  {
    call = mySerial.read();
    Serial.print(call);
  }
}

void RedialCall()
{
  mySerial.println("ATDL");
  Serial.println("Redialing");
  delay(1000);
}
