#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
#define SensorPin A1           //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0; 
LiquidCrystal lcd(2,3,4,5,6,7);
float ph;
float tempreture;
float turbidity;
void setup(void)
{
  
  Serial.begin(9600);
  lcd.begin(16, 2);
  sensors.begin();
   //lcd.print("hello, world!");
  delay(1000);
}


void loop(void){ 
  tempsensor();
  delay(50);
  turbiditysensor();
  delay(50);
  phsensor();
  Serial.println("  ");
  LCD();
  delay(1500);
  for (int positionCounter = 0; positionCounter < 20; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(600);
  }
  delay(2000);
}
void phsensor()
{
   static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+Offset;
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
  Serial.print("Voltage:");
        Serial.print(voltage,2);
        Serial.print("    pH value: ");
  Serial.println(pHValue,2);
ph=pHValue;
        printTime=millis();
  }
}
double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}
void turbiditysensor()
{
  int sensorValue = analogRead(A0);// read the input on analog pin 0:
  float voltage = sensorValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.println(voltage);
  turbidity=voltage;// print out the value you read:
  delay(500);
}
void tempsensor()
{
  sensors.requestTemperatures(); 
  Serial.print("Celsius temperature: ");
  Serial.print(sensors.getTempCByIndex(0));
  tempreture=sensors.getTempCByIndex(0);
  Serial.print(" - Fahrenheit temperature: ");
  Serial.println(sensors.getTempFByIndex(0));
  delay(1000);
}
void LCD()
{
 lcd.setCursor(0, 0);
 lcd.print("PH=");
 lcd.print(ph);
 lcd.print(",");
 lcd.print("Temp=");
 lcd.print(tempreture);
 lcd.print(",");
 lcd.print("Turbidity=");
 lcd.print(turbidity);
 
}


