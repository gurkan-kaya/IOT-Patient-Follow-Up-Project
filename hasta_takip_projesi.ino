#include <Wire.h>
#include <IR_Thermometer_Sensor_MLX90614.h>
#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <ThingSpeak.h>;
#define USE_ARDUINO_INTERRUPTS false
#include <PulseSensorPlayground.h>
PulseSensorPlayground pulseSensor;
const int OUTPUT_TYPE = SERIAL_PLOTTER;

const int NABIZ = A0;
const int PULSE_BLINK = 13;    
const int PULSE_FADE = 5;
const int THRESHOLD = 550;  //çevre gürültüsünden kaynaklanan değerleri ölçmesin diye eşik değeri tanımladım.
byte samplesUntilReport;

const byte SAMPLES_PER_SERIAL_SAMPLE = 10;
const char* ssid = "Kablonet Netmaster-B600-G"; 
const char* password = "7c8e5437"; 


WiFiClient client;

unsigned long myChannelNumber = 1256357 ;
unsigned long myChannelNumber2=1257675;
const char * myWriteAPIKey = "NKSV5DKSF15KKLCH"; 
const char * myWriteAPIKey2 ="IDWFBFL1U8M6L3T1";

IR_Thermometer_Sensor_MLX90614 MLX90614 = IR_Thermometer_Sensor_MLX90614();

void setup()
{
   Serial.begin(9600);
   MLX90614.begin();
   WiFi.begin(ssid, password);
   ThingSpeak.begin(client);  
   pulseSensor.analogInput(NABIZ);
   pulseSensor.blinkOnPulse(PULSE_BLINK);
   pulseSensor.fadeOnPulse(PULSE_FADE);
   pulseSensor.setSerial(Serial);
   pulseSensor.setOutputType(OUTPUT_TYPE);
   pulseSensor.setThreshold(THRESHOLD);

  samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;

   if (!pulseSensor.begin()) 
   {

    for(;;) 
    {
      // Flash the led to show things didn't work.
      digitalWrite(PULSE_BLINK, LOW);
      delay(50);
      digitalWrite(PULSE_BLINK, HIGH);
      delay(50);
    }
  }
}

void loop()
{
   int nabizDegeri = pulseSensor.getBeatsPerMinute();
   float ates;
   ates=MLX90614.GetObjectTemp_Celsius();
    
 if (pulseSensor.sawNewSample()) 
  {

    if (--samplesUntilReport == (byte) 0)
    {
      samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;

        if (pulseSensor.sawStartOfBeat()) 
        {          
        pulseSensor.outputBeat();
        
        }

    }
      Serial.print("Hastanin Nabzi : ");      
                   
      Serial.println(nabizDegeri); 
  }

  

  
  Serial.print("Hastanın Atesi : ");
  
  Serial.print(MLX90614.GetObjectTemp_Celsius());
  
  Serial.println(" *C");

  ThingSpeak.writeField(myChannelNumber2, 1,ates, myWriteAPIKey2); 
  
  ThingSpeak.writeField(myChannelNumber, 1,nabizDegeri, myWriteAPIKey); 
  
}
