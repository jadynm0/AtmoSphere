from flask import Flask, request, jsonify

app = Flask(__name__)

# This is the "Data Bridge" variable that holds the light level
shared_data = {"light": 0}

@app.route('/update', methods=['GET'])
def update():
    global shared_data
    # Ball 1 sends data like: /update?l=450
    light_val = request.args.get('l', 0)
    shared_data["light"] = int(light_val)
    print(f">>> DATA IN from Ball 1: {light_val}")
    return "Data Received"

@app.route('/get_light', methods=['GET'])
def get_light():
    # Ball 2 asks for data here
    print(f"<<< DATA OUT to Ball 2: {shared_data['light']}")
    return jsonify(shared_data)

if __name__ == '__main__':
    # CRITICAL: host='0.0.0.0' allows the ESP8266 to connect!
    app.run(host='0.0.0.0', port=5000)