from flask import Flask, request, jsonify
from flask_cors import CORS
import mysql.connector
from datetime import datetime


app = Flask(__name__)
CORS(app)  

db_config = {
    'user': 'root',
    'password': '123456',
    'host': 'localhost',
    'database': 'sys',
}
# truy vấn dữ liệu bảng sensor_data
@app.route('/api/sensor-data', methods=['GET'])
def get_sensor_data():
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor()
    try:
        cursor = conn.cursor(dictionary=True)
        query = "SELECT * FROM sensor_data ORDER BY id DESC LIMIT 1"
        cursor.execute(query)
        result = cursor.fetchone()
        cursor.close()
        conn.close()
        return jsonify(result), 200
    except Exception as e:
        return jsonify({"error": str(e)}), 500
# truy vấn dữ liệu và phan trang
@app.route('/api/sensor_stastic', methods=['GET'])
def get_sensordata():
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor()
    page = request.args.get('page', default=1, type=int) 
    limit = request.args.get('limit', default=10, type=int)  
    offset = (page - 1) * limit

    try:
        #tính toán số lượng bản ghi và trang
        cursor.execute("SELECT COUNT(*) FROM sensor_data")
        total_records = cursor.fetchone()[0]
        total_pages = (total_records + limit - 1) // limit  

        cursor.execute("SELECT * FROM sensor_data ORDER BY id DESC LIMIT %s OFFSET %s", (limit, offset))
        sensor_data = cursor.fetchall()

        sensor_list = []
        for row in sensor_data:
            sensor_list.append({
                'id': row[0],
                'temperature': row[1],
                'humidity': row[2],
                'light': row[3],
                'timestamp': row[4].strftime('%Y-%m-%d %H:%M:%S')
            })

        return jsonify({
            'sensorData': sensor_list,
            'totalPages': total_pages,
            'currentPage': page
        })
    except Exception as e:
        print(f"Error: {e}")
        return jsonify({'message': 'Failed to fetch sensor data!'}), 500
    finally:
        cursor.close()
        conn.close()
#tìm kiếm dữ liệu sensor qua tg
@app.route('/api/sensor_data_search', methods=['GET'])
def sensor_data_search():
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor()

    timestamp = request.args.get('timestamp')  # Thời gian cụ thể cần tìm kiếm

    try:
        # Truy vấn bản ghi khớp với thời gian
        query = """
            SELECT * FROM sensor_data 
            WHERE timestamp = %s
            LIMIT 1
        """
        cursor.execute(query, (timestamp,))
        result = cursor.fetchone()

        # Xử lý dữ liệu để trả về dưới dạng JSON
        if result:
            sensor_data = {
                'id': result[0],
                'temperature': result[1],
                'humidity': result[2],
                'light': result[3],
                'timestamp': result[4].strftime('%Y-%m-%d %H:%M:%S')
            }
            return jsonify({'sensorData': sensor_data})
        else:
            return jsonify({'message': 'No data found for the specified timestamp'}), 404

    except mysql.connector.Error as err:
        return jsonify({'error': str(err)}), 500
    finally:
        cursor.close()
        conn.close()
if __name__ == '__main__': 
    app.run(debug=True, host='0.0.0.0', port=5000)