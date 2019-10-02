from django.urls import path
from .views import index

urlpatterns = [
    path('<str:gate>', index),
    path('', index)
]
