# app.py

from flask import Flask, jsonify, render_template, request
app = Flask(__name__)

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
    print("GET:", a + b)
    return jsonify(result=a + b)

@app.route('/put-test', methods=['PUT'])
def put_test():
    data = request.get_json()
    a = data['a']
    b = data['b']
    print("PUT:", a + b)
    return jsonify(result=a + b)

@app.route('/post-test', methods=['POST'])
def post_test():
    a = request.form['a']
    b = request.form['b']
    print("POST:", a + b)
    return jsonify(result=a + b)

@app.route('/')
def root():
    return render_template('index.html')

if __name__ == '__main__':
    app.run(host='0.0.0.0')
