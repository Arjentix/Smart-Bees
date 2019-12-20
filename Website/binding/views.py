from django.forms import formset_factory
from django.http import HttpResponse
from django.shortcuts import render, redirect
#from binding.models import gate_uid
from binding.web_form import web_form
import requests


def index(request):
    return HttpResponse('AAAAAAAAAA')


def config_editor(request, gate=''):
    return render(request, context={'gate': gate}, template_name='config_editor.html')


def load_data_to_page(request, gate):
    #request.body()
    # изменить get запрос
    r = requests.get('http://127.0.0.1:8888/get_uids?gate_id=' + gate, headers={'Api-Key':'ID_service_12345'})
    if r.ok:
        records = []
        for record in r.json():
            # вроде это нужно оставить. Оно взаимодействует только с сайтом
            records.append({'uid': record['user_id'],
                            'url': 'unbind/a=' + gate + '&b=' + record['user_id']})
        return render(request, context={'gate': gate, 'form': web_form(), 'records': records},
                      template_name='binding.html')
    return HttpResponse('<h3>Что-то пошло не так</h3>')


def devices(request, gate=''):
    if request.method == 'GET':
        return load_data_to_page(request, gate)
    if request.method == 'POST':
        form = web_form(request.POST)
        # изменить пост запрос
        r = requests.post('http://127.0.0.1:8888/bind', headers={'Api-Key':'ID_service_12345'}, json={'gate_id': gate, 'user_id': form.data['uid']})
        if r.status_code != 200:
            return HttpResponse('bind failed')
        return redirect('/gate/' + gate)


def unbind(request, gate='', uid=''):
    if request.method == 'POST':
        # изменить delete запрос
        r = requests.delete('http://127.0.0.1:8888/unbind?gate_id=' + gate + '&user_id=' + uid, headers={'Api-Key':'ID_service_12345'})
        if r.status_code != 200:
            return HttpResponse('bind failed')
    return redirect('/gate/' + gate)


def subscription(request, gate=''):
    # запрос к сервису подписок
    # создание формы
    # вывод страницы
    return render(request, context={'gate': gate, 'text': 'Данные от Славы'}, template_name='subscription.html')

# def change_gate_id(request, gate=''):
#     return HttpResponse("<h3>УРА</h3>")