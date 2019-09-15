import os
import json
import DbHandler
from flask import Flask
from flask import request
app = Flask(__name__)

@app.route('/<filename>', methods=['GET', 'POST'])
def handler(filename):
	if filename in os.listdir('static'):
		print('File was founded')
		if request.method == 'GET':
			print('GET request {}'.format(filename))
			return app.send_static_file(filename)
		if request.method == 'POST':
			print('POST request: {}'.format(request.json))
			response = {}
			db_handler = DbHandler.DbHandler()
			db_res = db_handler.bind_gate_uid(filename[:-5], request.json['uID'])
			print('Stored in db')
			if db_res:
				responce = {
				"type" : "OK"
				}
			else:
				responce = {
				"type" : "ERROR"
				}

			return json.dumps(
				response,
				ensure_ascii=False,
				indent=2
			)
	else:
		return 'Error'

if __name__ == '__main__':
	app.run(host='0.0.0.0')
