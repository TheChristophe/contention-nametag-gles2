# app.py

from flask import Flask, jsonify, render_template, request
app = Flask(__name__)

from ipcqueue import posixmq
from ipcqueue.serializers import RawSerializer
queue = posixmq.Queue('/nametag', maxsize=16, maxmsgsize=4096, serializer=RawSerializer)

import json

def transmit_message(type, message):
    json_str = json.dumps({
        'type': type,
        'content': message
    }) + '\0'
    queue.put(json_str.encode('utf-8'))

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

@app.route('/put-test', methods=['PUT'])
def put_test():
    data = request.get_json()
    a = data['a']
    b = data['b']

    transmit_message('put', data)
        
    return jsonify(result=a + b)

@app.route('/post-test', methods=['POST'])
def post_test():
    a = request.form['a']
    b = request.form['b']

    transmit_message('post', request.form)

    return jsonify(result=a + b)

@app.route('/')
def root():
    return render_template('index.html')

if __name__ == '__main__':
    app.run(host='0.0.0.0')
