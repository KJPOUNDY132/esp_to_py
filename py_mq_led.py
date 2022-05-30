import paho.mqtt.client as mqtt
import RPi.GPIO as GPIO
# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("/btn")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    if msg.topic == "btn":
        if msg.payload == "1":
            print("  Green on!")
            GPIO.output(11, 1)
        else:
            print("  Green off!")
            GPIO.output(11, 0)
    



client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("192.168.1.31", 1883, 60)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(11, GPIO.OUT)
client.loop_forever()