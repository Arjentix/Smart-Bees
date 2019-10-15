from django.http import HttpResponse
from django.shortcuts import render
from binding.models import gate_uid
from binding.web_form import web_form


def index(request):
    return HttpResponse('AAAAAAAAAA')


def devices(request, gate=''):
    if request.method == 'GET':
        if gate == '':
            return HttpResponse('AAAAAAAAAA')

        print(gate)
        records = gate_uid.objects.filter(gate=gate)
        return render(request, context={'records': records, 'gate': gate, 'form': web_form()}, template_name='binding.html')
    if request.method == 'POST':
        form = web_form(request.POST)
        try:
            gate_uid.objects.get(uid=form.data['uid'])
            return render(request, template_name='gate_already_bind.html')
        except gate_uid.DoesNotExist:
            if form.is_valid():
                record = gate_uid(gate=gate, uid=form.data['uid'])
                record.save()
                return render(request, template_name='success.html')
            return render(request, context={'form': form}, template_name='binding.html')
        except:
            return render(request, context={'form': form}, template_name='binding.html')

    # except:
    #     return HttpResponse("EEE ROCK")

# def index(request, gate=''):
#     if request.method == 'POST':
#         form = web_form(request.POST)
#         try:
#             if form.is_valid():
#                 record = gate_uid.objects.get(gate=form.data['gate'])
#                 record.uid = form.data['uid']
#                 record.save()
#                 return render(request, template_name='success.html')
#             else:
#                 return render(request, context={'form': form}, template_name='binding.html')
#         except:
#             return render(request, context={'form': form}, template_name='binding.html')
#
#     if gate == '':
#         return render(request, template_name='gate_not_found.html')
#
#     try:
#         board = gate_uid.objects.get(gate=gate)
#         if board.uid == '':
#             return render(request, context={'form': web_form(instance=board)}, template_name='binding.html')
#         return render(request, template_name='gate_already_bind.html')
#     except:
#         return render(request, template_name='gate_not_found.html')


def change_gate_id(request, gate=''):
    return HttpResponse("<h3>УРА</h3>")