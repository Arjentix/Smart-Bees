# coding: utf-8
# Импортирует поддержку UTF-8.
from __future__ import unicode_literals

# Импортируем модули для работы с JSON и логами.
import json
import logging
import requests
import sys
import socket
sys.path.insert(0, 'src')

# Импортируем подмодули Flask для запуска веб-сервиса.
from flask import Flask, request
app = Flask(__name__)

logging.basicConfig(filename='skill.log', level=logging.DEBUG, filemode='w')

# Хранилище данных о сессиях.
sessionStorage = {}

# Задаем параметры приложения Flask.
@app.route("/", methods=['POST'])

def main():
# Функция получает тело запроса и возвращает ответ.
	logging.info('Request: %r', request.json)

	response = {
		"version": request.json['version'],
		"session": request.json['session'],
		"response": {
			"end_session": False
		}
	}

	handle_dialog(request.json, response)

	logging.info('Response: %r', response)

	return json.dumps(
		response,
		ensure_ascii=False,
		indent=2
	)

# Функция для непосредственной обработки диалога.
def handle_dialog(req, res):
	user_id = req['session']['user_id']

	# Проверка существования соединения со шлюзом этого пользователя 
	# И НАЧИЯ У НЕГО ПОДПИСКИ
	if req['session']['new']:
		# Это новая сессия.
		answer = requests.get(
			'http://localhost:3200/gate_id?user_id={}'.format(user_id),
			headers = {'Api-Key' : 'Manager12345'}
		)
		if not answer.ok:
			res['response']['text'] = 'Вы еще не зарегистрированы'
			return

		answer = requests.get(
			'http://localhost:3200/sub_check?user_id={}'.format(user_id),
			headers = {'Api-Key' : 'Manager12345'}
		)
		if not answer.ok:
			res['response']['text'] = 'Необходимо продлить подписку для пользования сервисом'
			return

		answer = requests.get(
			'http://localhost:3200/gate_check?user_id={}'.format(user_id),
			headers = {'Api-Key' : 'Manager12345'}
		)
		if not answer.ok:
			res['response']['text'] = 'Кажется, ваш шлюз не подключен к сети'
			return

		res['response']['text'] = 'Привет! Что мне включить?'
	else:
		# Обрабатываем ответ пользователя.
		tokens = req['request']['nlu']['tokens']

		j = """{"user_id": "%s", "command": "%s"}""" % (user_id,' '.join(tokens) )
		r = requests.post('http://localhost:3200', 
				  headers={'Api-Key': '531'}, 
				  json=json.loads(j))
		logging.info('Gate answer: %s', r.text)
		if (r.status_code == 200):
			res['response']['text'] = 'Готово!'
		else:
			if 'error_message' in r.json():
				res['response']['text'] = r.json()['error_message']
			else:
    				res['response']['text'] = 'Что-то пошло не так'	

if __name__ == '__main__':
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.bind(('', 4443))
	port = sock.getsockname()[1]
	sock.close()
	app.run(ssl_context=('cert.pem', 'key.pem'), host='0.0.0.0', port=port)