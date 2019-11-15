from django.urls import path
from .views import devices, index, change_gate_id, unbind

urlpatterns = [
    path('devices', devices),
    path('change_gate_id', change_gate_id),
    path('unbind/a=<str:gate>&b=<str:uid>', unbind),
    path('', index)
]
