import RPi.GPIO as GPIO
import time

# GPIO pin numarası
pwmPin = 18

# PWM frekansı (Hz)
pwmFrequency = 1000

# PWM duty cycle (0-100 arası)
dutyCycle = 50

# GPIO ayarları
GPIO.setmode(GPIO.BCM)
GPIO.setup(pwmPin, GPIO.OUT)

# PWM nesnesi oluştur
pwm = GPIO.PWM(pwmPin, pwmFrequency)

try:
    # PWM'yi başlat
    pwm.start(dutyCycle)
    
    # Sonsuz döngü
    while True:
        # Devam etmek için Ctrl+C'ye basılana kadar bekleyin
        time.sleep(1)

except KeyboardInterrupt:
    # Ctrl+C'ye basılınca PWM'yi durdur
    pwm.stop()
    GPIO.cleanup()