import serial, time, random
from paho.mqtt.client import Client
from paho.mqtt.enums import CallbackAPIVersion

SERIAL_PORT = "COM5"
BROKER = "broker.emqx.io"
TOPIC_VALUE = "iot/light/sensor/value"
TOPIC_STATUS = "iot/light/sensor/status"

ser = serial.Serial(SERIAL_PORT, 9600, timeout=1)
time.sleep(2)

client = Client(
    callback_api_version=CallbackAPIVersion.VERSION2,
    client_id=f"sensor_pc_{random.randint(1000,9999)}"
)
client.connect(BROKER)
client.loop_start()

ser.write(b's')

while True:
    line = ser.readline().decode().strip()
    if line:
        print(line)
        if line.startswith("SENSOR_VALUE"):
            client.publish(TOPIC_VALUE, line)
        else:
            client.publish(TOPIC_STATUS, line)
