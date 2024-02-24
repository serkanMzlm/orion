import RPi.GPIO as GPIO  
from time import sleep   
GPIO.setmode(GPIO.BOARD) 
pwmPin = 35

GPIO.setup(pwmPin,GPIO.OUT) 
p = GPIO.PWM(pwmPin, 50)    
p.start(0)                  

for i in range(4):
    p.ChangeDutyCycle(i)     
    sleep(1)                 

p.stop()         
GPIO.cleanup()   