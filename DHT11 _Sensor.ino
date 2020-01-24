/* Auther name= Rohit Maurya
 *  Date- 12/1/2020
 *  Discription- This code use to print temperature and Humidity on serial port 
 *  (all code writen in c -  language)
 */




// difine DHT11 data pin here
#define dataPin D1

// variable to use calculate the temperature and Humididty
unsigned long int DHT11_data = 0; // DHT11 store data
uint8_t  cnt = 0;
long tout;
float Temperature = 0;
float Humidity = 0;
uint8_t bits[5];

// function 
void Start();
bool cheaksum();
void responseTime();
void data_receive();
bool flag = false;

void setup()
{
  //set the baud rate
  Serial.begin(9600);
  delay(1000); // delay 1 sec
}

void loop() {

  Start();
  responseTime();
  data_receive();
  //  if (cheaksum())
  //  {
  Serial.print("Temperature : ");
  Serial.print(String(Temperature));
  Serial.print("\t");
  Serial.print("Humidity : ");
  Serial.println(String(Humidity));
  delay(3000);
  //  }
}
 /******************************************************************
  * name- Start()
  * return - void 
  * parameter - 
  * 
  * discription- 1.send the high value to DHT11 for 1sec
  *              2. Than ,Low Value for 18 msec  
  *              3. After that DHT 11 start to send the data 
  *              4. change data pinMode to input and wait 40 msec delay
  */
void Start()
{
  pinMode(dataPin, OUTPUT);
  digitalWrite(dataPin, HIGH);
  delay(1000);
  digitalWrite(dataPin, LOW);
  delay(18);                    // process must take lest 18 ms time
  digitalWrite(dataPin, HIGH);
  pinMode(dataPin, INPUT);
  delayMicroseconds(40);



  Serial.println("Response : " + String(digitalRead(dataPin)) );

}
/*****************************************************************************************
 * Name- responseTime()
 * return- void
 * parameter - 
 * discription- cheak response of DHT11 sensor
 * 
 */
void responseTime()
{
  tout = micros();
  while (digitalRead(dataPin) == LOW);

  if ((micros() - tout) > 80)
  {
  
    Serial.println("tout-LOW : " + String((micros() - tout) ));
  }

  tout = micros();
  while (digitalRead(dataPin) == HIGH);

  if ((micros() - tout) > 80)
  {
    Serial.println("cheak error:at HIGH");

    Serial.println("tout-HIGH : " + String((micros() - tout) ));
  }
  Serial.println("response done:");
}

/*************************************************************************************
 * Name- data_receive()
 * return- void 
 * parametre - mask,idx
 * 
 *dis- DHT11 send 5 bytes of data and store bits variable  
 */
void data_receive()
{
  uint8_t  mask = 128;
  uint8_t idx = 0;
  //  DHT11_data = 0;
  for (uint8_t i = 40; i != 0; i--)
  {

    tout = micros();
    if (digitalRead(dataPin) == LOW)
    {
      if ((micros() - tout) > 50)
      {
        Serial.println("cheak data error:at low");
        Serial.println("tout-LOW in data : " + String((micros() - tout) ));

      }
    }

    tout = micros();
    while (digitalRead(dataPin) == HIGH)
    {
      if ((micros() - tout) > 80)
      {
        Serial.println("cheak data error:at  HIGH");
        Serial.println("tout-HIGH in data : " + String((micros() - tout) ));
        delay(5000);
      }

    }


    if ((micros() - tout) > 40)
    {
      bits[idx] |= mask;
    }
    mask >>= 1;
    if (mask == 0)   // next byte?
    {
      mask = 128;
      idx++;
    }
  } // while(cnt) loop ENDS HERE



  Temperature = ((bits[2] + bits[3]));
  Humidity =  ((bits[0] + bits[1]));
  Serial.println(String((int)bits[4]));
  Serial.println(String(bits[2] + bits[3] + bits[0] + bits[1]));
  Serial.println("data received:");

} // data receive
/*************************************************************************************
 * name- cheaksum()
 * return - bool
 */
//bool cheaksum()
//{
//  if ( (bits[4])&0xff  == (bits[0] + bits[1] + bits[2] + bits[3]) & 0xff)
//  {
//    //    Temperature = bits[2] + bits[3] * 0.1 ;
//    //    Humidity =  bits[0] + bits[1] * 0.1;
//    Serial.println("cheak sum done:");
//  }
//}
