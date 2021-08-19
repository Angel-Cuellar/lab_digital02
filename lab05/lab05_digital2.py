import serial
import time
from Adafruit_IO import Client, RequestError, Feed

ADAFRUIT_IO_KEY = "aio_QBsC52QW6tQijfqqngqUxTIvZgqZ"
ADAFRUIT_IO_USERNAME = "Cue77ar"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

contpush_feed = aio.feeds('contador-push')
contUSART_feed = aio.feeds('contador')

cont = 0; 

pic = serial.Serial("COM2", 9600)

pic.timeout = 1

time.sleep(1)

while (True) :

    with pic:
        print('port conected'+'\n')

        print('Leyendo contador')

        pic.write(b'a')

        ser_bytes = pic.readline()
        decoded_bytes= int(ser_bytes[0:len(ser_bytes)-1].decode("utf-8"))

        print(decoded_bytes)

        print('\n')

        aio.send_data(contpush_feed.key, decoded_bytes)

        print('Enviando valor al pic')

        pic.write(b'b')

        cont =aio.receive(contUSART_feed.key)

        cont =str(int(cont.value))

        print(cont)

        pic.write(cont.encode("ascii"))

        print('\n')
        




