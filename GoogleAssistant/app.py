# coding: utf-8
#gactions update --action_package PACKAGE_NAME --project smartbees-6224f
#"url": "https://172.105.77.74:4443"

# import flask dependencies
from flask import Flask, request
import logging
import json
import requests
import sys
import jwt
import socket
import google.oauth2
# initialize the flask app
app = Flask(__name__)
app.config['DEBUG'] = True

logging.basicConfig(filename='logFile.log', level=logging.DEBUG, filemode='w')

# Хранилище данных о сессиях.
sessionStorage = {}

# default route
@app.route('/', methods=['GET','POST'])

def main():
        logging.info('Request: %r', request.json)

        response = {
                "expectUserResponse": True,
                "expectedInputs": [
                        {
                                "inputPrompt": {
                                        "richInitialPrompt": {
                                                "items": [
                                                        {
                                                                "simpleResponse": {
                                                                        "textToSpeech": "Извините, я Вас не понимаю",
                                                                        "displayText": "Извините, я Вас не понимаю"
                                                                }
                                                        }
                                                ]
                                        }
                                }
                        }
                ]
        }

        handle_dialog(response)

        return json.dumps(response,ensure_ascii=False,indent=2)

def handle_dialog(response):
        data = request.json
        intent = data['inputs'][0]['intent']

        user_jwt = data['user']['idToken']

        print(user_jwt)
        jwt_data = jwt.decode(user_jwt, verify=False, algorithms=['RS256'])
        print(jwt_data)
        user_id = jwt_data['email']
        print(user_id)

        if intent == 'actions.intent.MAIN':
                query = data['inputs'][0]['rawInputs'][0]['query'].strip().lower()
                answer = requests.get(
                                'http://165.22.16.92:3200/gate_id?user_id={}'.format(user_id),
                                headers = {'Api-Key' : 'Manager12345'}
                        )
                if not answer.ok:
                        response["expectedInputs"][0]["inputPrompt"]["richInitialPrompt"]\
                                ["items"][0]["simpleResponse"]["textToSpeech"] = "Вы еще не зарегистрированы"
                        response["expectedInputs"][0]["inputPrompt"]["richInitialPrompt"]\
                                ["items"][0]["simpleResponse"]["displayText"] = "Вы еще не зарегистрированы"
                        return

                answer = requests.get(
                        'http://165.22.16.92:3200/sub_check?user_id={}'.format(user_id),
                        headers = {'Api-Key' : 'Manager12345'}
                )

                if not answer.ok:
                        response["expectedInputs"][0]["inputPrompt"]["richInitialPrompt"]\
                                ["items"][0]["simpleResponse"]["textToSpeech"] = "Необходимо продлить подписку для пользования сервисом"
                        response["expectedInputs"][0]["inputPrompt"]["richInitialPrompt"]\
                                ["items"][0]["simpleResponse"]["displayText"] = "Необходимо продлить подписку для пользования сервисом"
                        return
                answer = requests.get(
                        'http://165.22.16.92:3200/gate_check?user_id={}'.format(user_id),
                        headers = {'Api-Key' : 'Manager12345'}
                )
                if not answer.ok:
                        response["expectedInputs"][0]["inputPrompt"]["richInitialPrompt"]\
                                ["items"][0]["simpleResponse"]["textToSpeech"] = \
                                        "Кажется, ваш шлюз не подключен к сети"
                        response["expectedInputs"][0]["inputPrompt"]["richInitialPrompt"]\
                                ["items"][0]["simpleResponse"]["displayText"] = \
                                        "Кажется, ваш шлюз не подключен к сети"
                        return

                response["expectedInputs"][0]["inputPrompt"]["richInitialPrompt"]\
                        ["items"][0]["simpleResponse"]["textToSpeech"] = "Привет, что мне сделать?"
                response["expectedInputs"][0]["inputPrompt"]["richInitialPrompt"]\
                        ["items"][0]["simpleResponse"]["displayText"] = "Привет, что мне сделать?"

                return
        if intent == 'actions.intent.TEXT':

                print('text')
                query = data['inputs'][0]['rawInputs'][0]['query'].strip().lower()

                if query == 'войти':
                        response["expectedInputs"][0].setdefault("possibleIntents", [{"intent": "actions.intent.SIGN_IN", 
                                                                                      "inputValueData": 
                                                                                      {"@type": "type.googleapis.com/google.actions.v2.SignInValueSpec"}}])

                        response["expectedInputs"][0]["inputPrompt"]["richInitialPrompt"]["items"][0]["simpleResponse"]["textToSpeech"] = "Проверьте свою почту, Вам пришло уведомление?"
                        response["expectedInputs"][0]["inputPrompt"]["richInitialPrompt"]["items"][0]["simpleResponse"]["displayText"] = "Проверьте свою почту, Вам пришло уведомление?"

                # if query == 'да':

                #         token = request.headers["Authorization"]

                #         print('token ' + token)
                #         jwt_data = jwt.decode(token, verify=False, algorithms=['RS256'])

                #         print(jwt_data)

                #         print("NEXT JSON")
                #         print(request.json)

                if query == 'пока':
                        print('bye')
                        response["expectUserResponse"] = False
                        response.setdefault("finalResponse", {"richResponse": {"items": [{"simpleResponse": {"textToSpeech": "Приятного использования, до свидания."}}]}})
                        # response.setdefault("finalResponse", {"richResponse": {"items": [{"simpleResponse": {"displayText": "Приятного использования, до свидания."}}]}})
                        response["finalResponse"]["richResponse"]["items"][0]["simpleResponse"].setdefault("displayText", "Приятного использования, до свидания.")

                else:

                        j = """{"user_id": "%s", "command": "%s"}""" % (user_id, query )
                        r = requests.post('http://165.22.16.92:3200', 
                                        headers={'Api-Key': 'Manager12345'}, 
                                        json=json.loads(j))
                        logging.info('Gate answer: %s', r.text)
                        if (r.status_code == 200):
                                response["expectedInputs"][0]["inputPrompt"]["richInitialPrompt"]\
                                        ["items"][0]["simpleResponse"]["textToSpeech"] = "Готово!"
                                response["expectedInputs"][0]["inputPrompt"]["richInitialPrompt"]\
                                        ["items"][0]["simpleResponse"]["displayText"] = "Готово!"
                        else:
                                if 'error_message' in r.json():
                                        response["expectedInputs"][0]["inputPrompt"]["richInitialPrompt"]\
                                                ["items"][0]["simpleResponse"]["textToSpeech"] = r.json()['error_message']
                                        response["expectedInputs"][0]["inputPrompt"]["richInitialPrompt"]\
                                                ["items"][0]["simpleResponse"]["displayText"] = r.json()['error_message']
                                else:
                                        response["expectedInputs"][0]["inputPrompt"]["richInitialPrompt"]\
                                                ["items"][0]["simpleResponse"]["displayText"] = "Что-то пошло не так"

# run the app
if __name__ == '__main__':
   app.run()