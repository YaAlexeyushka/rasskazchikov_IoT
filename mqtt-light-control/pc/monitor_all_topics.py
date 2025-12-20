from paho.mqtt.client import Client
from paho.mqtt.enums import CallbackAPIVersion
import random

def on_message(client, userdata, msg):
    print(f"[{msg.topic}] {msg.payload.decode()}")

client = Client(
    callback_api_version=CallbackAPIVersion.VERSION2,
    client_id=f"monitor_{random.randint(1000,9999)}"
)
client.on_message = on_message
client.connect("broker.emqx.io")
client.subscribe("iot/light/#")
client.loop_forever()
