from django.forms import ModelForm
from binding.models import gate_uid


class web_form(ModelForm):
    class Meta:
        model = gate_uid
        fields = ['uid']