from django.urls import path
from .views import devices, index, unbind, subscription, config_editor

urlpatterns = [
    # path('devices', devices),
    # path('change_gate_id', change_gate_id),
    # path('unbind/a=<str:gate>&b=<str:uid>', unbind),
    # path('subscription', subscription),
    # path('', index)
    path('gate/<str:gate>',devices),
    path('unbind/a=<str:gate>&b=<str:uid>', unbind),
    path('subscribe/<str:gate>',subscription),
    path('config_editor/<str:gate>', config_editor)
]
