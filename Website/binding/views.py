from django.forms import formset_factory
from django.http import HttpResponse
from django.shortcuts import render, redirect
from binding.models import gate_uid
from binding.web_form import web_form
import requests


def index(request):
    return HttpResponse('AAAAAAAAAA')


def config_editor(request, gate=''):
    return render(request, context={'gate': gate}, template_name='config_editor.html')


def load_data_to_page(request,gate):
    r = requests.get('http://127.0.0.1:8080/get_uids/' + gate)
    if r.ok:
        records = []
        for record in r.json():
            records.append({'uid': record['uid'],
                            'url': 'unbind/a=' + gate + '&b=' + record['uid']})
        return render(request, context={'gate': gate, 'form': web_form(), 'records': records},
                      template_name='binding.html')
    return HttpResponse('<h3>Бля</h3>')


def devices(request, gate=''):
    if request.method == 'GET':
        return load_data_to_page(request, gate)
    if request.method == 'POST':
        form = web_form(request.POST)
        requests.post('http://127.0.0.1:8080/bind/a=' + gate + '&b=' + form.data['uid'])
        return redirect('/gate/' + gate)


def unbind(request, gate='', uid=''):
    if request.method == 'POST':
        requests.delete('http://127.0.0.1:8080/unbind/a=' + gate + '&b=' + uid)
    return redirect('/gate/' + gate)


def subscription(request, gate=''):
    # запрос к сервису подписок
    # создание формы
    # вывод страницы
    return render(request, context={'gate': gate, 'text': 'Данные от Славы'}, template_name='subscription.html')

# def change_gate_id(request, gate=''):
#     return HttpResponse("<h3>УРА</h3>")