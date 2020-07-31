# app.py

from flask import Flask, jsonify, render_template, request
app = Flask(__name__, template_folder='res/templates', static_folder='res/static', static_url_path='/static')

from ipcqueue import posixmq
from ipcqueue.serializers import RawSerializer
outQueue = posixmq.Queue('/nametag', maxsize=16, maxmsgsize=4096, serializer=RawSerializer)
inQueue = posixmq.Queue('/nametag-out', maxsize=16, maxmsgsize=4096, serializer=RawSerializer)

import json
import uuid

def transmit_message(type, message):
    json_str = json.dumps({
        'type': type,
        'content': message,
        'uuid': str(uuid.uuid4())
    }) + '\0'
    outQueue.put(json_str.encode('utf-8'))

def receive_message():
    # -1 to splice out null terminator
    return json.loads(inQueue.get()[:-1].decode('utf-8'))

@app.errorhandler(404)
def not_found(error):
    return render_template('404.html'), 404

@app.errorhandler(500)
def internal_error(error):
    return render_template('5xx.html'), 500

@app.route('/get-test', methods=['GET'])
def get_test():
    a = request.args.get('a')
    b = request.args.get('b')
    
    transmit_message('get', request.args)

    return jsonify(result=a + b)

@app.route('/add', methods=['POST'])
def add():
    transmit_message('post', request.form)
    response = receive_message()
    response["type"] = request.form["type"]

    return jsonify(result=response)

@app.route('/delete', methods=['PUT'])
def delete():
    data = request.get_json()
    transmit_message('put', data)
        
    return jsonify(result='ok')

@app.route('/')
def root():
    return render_template('index.html')

if __name__ == '__main__':
    app.run(host='0.0.0.0')
