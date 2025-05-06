import paho.mqtt.client as mqtt
import mysql.connector
import json  

db = mysql.connector.connect(
    host="localhost",        
    user="root",   
    password="123456",
    database="sys"
)

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
        client.subscribe("sensor")
        client.subscribe("led")
    else:
        print(f"Connection failed with code {rc}")

def on_message(client, userdata, msg):
    try:
        payload = msg.payload.decode().strip()
        print(f"Received from topic '{msg.topic}': {payload}")
        data = json.loads(payload)
        cursor = db.cursor()

        if msg.topic == "sensor":
            try:
                # Kiểm tra và chuyển đổi dữ liệu
                temp_str = data.get("temperature", "").replace("C", "").strip()
                humidity_str = data.get("humidity", "").replace("%", "").strip()
                light_str = data.get("light", "").replace("lux", "").strip()

                # Kiểm tra dữ liệu hợp lệ trước khi chuyển đổi sang float
                if temp_str and humidity_str and light_str:
                    temperature = float(temp_str)
                    humidity = float(humidity_str)
                    light = float(light_str)

                    # Chèn dữ liệu vào MySQL
                    sql = "INSERT INTO sensor_data (temperature, humidity, light) VALUES (%s, %s, %s)"
                    values = (temperature, humidity, light)
                    cursor.execute(sql, values)
                    db.commit()
                    print("Sensor data inserted successfully!")
                else:
                    print("Invalid sensor data received!")

            except ValueError:
                print("Error converting sensor data to float!")

        elif msg.topic == "led":
            led1_status = data.get("led1", "off")
            led2_status = data.get("led2", "off")

            if "led1" in data:
                sql_led1 = "INSERT INTO device_status (Device, Status) VALUES (%s, %s)"
                cursor.execute(sql_led1, ("LED 1", led1_status))
                print(f"Inserted LED1 status: {led1_status}")

            if "led2" in data:
                sql_led2 = "INSERT INTO device_status (Device, Status) VALUES (%s, %s)"
                cursor.execute(sql_led2, ("LED 2", led2_status))
                print(f"Inserted LED2 status: {led2_status}")

            db.commit()

        cursor.close()

    except json.JSONDecodeError:
        print("Error decoding JSON!")
    except Exception as e:
        print(f"Unexpected error: {e}")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set("qwer", "123")
client.connect("192.168.180.26", 1885, 60)
client.loop_forever()
