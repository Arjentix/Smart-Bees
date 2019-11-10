from django import forms
from binding.models import gate_uid


class web_form(forms.Form):
    uid = forms.CharField(max_length=128)


