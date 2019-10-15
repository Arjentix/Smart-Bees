from django.urls import path
from .views import devices, index, change_gate_id

urlpatterns = [
    path('devices', devices),
    path('change_gate_id', change_gate_id),
    path('', index)
]
