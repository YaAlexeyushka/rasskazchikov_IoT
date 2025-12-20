import serial
import time
import random
from paho.mqtt.client import Client
from paho.mqtt.enums import CallbackAPIVersion

SERIAL_PORT = "COM6"
BROKER = "broker.emqx.io"

TOPIC_SENSOR = "iot/light/sensor/value"
TOPIC_STATUS = "iot/light/actuator/status"  

THRESHOLD = 90
led_state = "OFF"  

ser = serial.Serial(SERIAL_PORT, 9600, timeout=1)
time.sleep(2)

client = Client(
    callback_api_version=CallbackAPIVersion.VERSION2,
    client_id=f"actuator_pc_{random.randint(1000,9999)}"
)
client.connect(BROKER)

def send_cmd(cmd, new_state):
    global led_state
    if led_state == new_state:
        return  
    ser.write(cmd.encode())
    led_state = new_state
    print(f"LED -> {new_state}")
    client.publish(TOPIC_STATUS, new_state, qos=2)

def on_message(client, userdata, msg):
    global led_state
    try:
        payload = msg.payload.decode().strip()
        if ":" in payload:
            value = int(payload.split(":")[1])
        else:
            value = int(payload)  
    except Exception as e:
        print(f"Ошибка при обработке сообщения: {e}")
        return

    if value < THRESHOLD:
        send_cmd('u', "ON")
    else:
        send_cmd('d', "OFF")

client.on_message = on_message
client.subscribe(TOPIC_SENSOR)

client.loop_forever()
