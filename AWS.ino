
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient 
import time#To create delay

from datetime import date, datetime 

import Adafruit_CharLCD as LCD 

import Adafruit_DHT 



myMQTTClient = AWSIoTMQTTClient("new_Client")

myMQTTClient.configureEndpoint("a2b6vkjgdpftm1.iot.us-east-2.amazonaws.com", 8883)

myMQTTClient.configureCredentials("/home/pi/Desktop/ESP32_Thing certificates/CA.pem", "/home/pi/Desktop/ESP32_Thing certificates/private.pem.key", "/home/pi/Desktop/ESP32_Thing certificates/certificate.pem.crt")

myMQTTClient.configureOfflinePublishQueueing(-1) 
myMQTTClient.configureDrainingFrequency(2)  

myMQTTClient.configureConnectDisconnectTimeout(10) 

myMQTTClient.configureMQTTOperationTimeout(5) 



sensor_name = Adafruit_DHT.DHT11 #we are using the DHT11 sensor

sensor_pin = 17 



lcd_rs        = 7  
lcd_en        = 8 
lcd_d4        = 25 
lcd_d5        = 24 
lcd_d6        = 23 
lcd_d7        = 18 
lcd_backlight =  0  
lcd_columns = 16 
lcd_rows    = 2 


lcd = LCD.Adafruit_CharLCD(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6, lcd_d7, 

                           lcd_columns, lcd_rows, lcd_backlight)   

lcd.message('AWS-IoT with Pi \n -CircuitDigest') #Give a intro message

time.sleep(2) #wait for 2 secs

lcd.clear() #clear the LCD


connecting_time = time.time() + 10


if time.time() < connecting_time: 

    myMQTTClient.connect()

    myMQTTClient.publish("DHT11/info", "connected", 0)

    print "MQTT Client connection success!"

    lcd.message('Connected to \n AWS thing') 

else:

    print "Error: Check your AWS details in the program"

    lcd.message('Error: \nInvalid details')


    

time.sleep(2) 



while 1: #Infinite Loop

    now = datetime.utcnow() 

    current_time = now.strftime('%Y-%m-%dT%H:%M:%SZ') 

    

    humidity, temperature = Adafruit_DHT.read_retry(sensor_name, sensor_pin) 

    lcd.clear() #Clear the LCD screen

    lcd.message ('Temp = %.1f C' % temperature) 

    lcd.message ('\nHum = %.1f %%' % humidity) 

    time.sleep(2) #Wait for 2 sec then update the values


    #prepare the payload in string format 

    payload = '{ "timestamp": "' + current_time + '","temperature": ' + str(temperature) + ',"humidity": '+ str(humidity) + ' }'


    print payload #print payload for reference 

    myMQTTClient.publish("DHT11/data", payload, 0) #publish the payload

    

    lcd.clear() #Clear the LCD screen

    lcd.message ('Published to \n  AWS-IOT') 


    time.sleep(2) #Wait for 2 sec then update the values

