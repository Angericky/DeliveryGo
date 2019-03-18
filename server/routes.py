# -*- coding: utf-8 -*-
from flask import Flask,jsonify,render_template,request
import json
import deliveryGo
from flask_cors import CORS

DEBUG = True

app = Flask(__name__)
app.config.from_object(__name__)

cors = CORS(app, resources={r"/*": {"origins": "*"}}, methods=['GET', 'HEAD', 'POST', 'OPTIONS'])



@app.route('/input', methods=[ 'POST','GET'])
def getInputFile():
	response_object = {'status': 'success'}
	if request.method == 'POST':
		post_data = request.get_json()['filePath']
		
		try:
			with open(post_data, 'r') as f:
				input = f.read()
			res = deliveryGo.run(post_data)
			response_object['post_data'] = post_data
			response_object['message'] = 'Input file has been uploaded'
			response_object['output'] =  res
			response_object['input'] =  input
		except FileNotFoundError:
			response_object['error'] = 'File not found!'

	else:
		response_object['message'] = 'Successful'

	return jsonify(response_object)

if __name__ == '__main__':
    app.run(host='0.0.0.0',#任何ip都可以访问
            port=5000,#端口
            debug=True
            )
