# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.shortcuts import render
from django.http import HttpResponse
from megatron_ctl import car_ops as ops

car_handle = ops.CarHandle()

# Create your views here.
def index(request):
    return render(request, 'index.html')

def car_action(request):
    act = request.GET['act']
    action = str(act)
    print action
    car_handle.motor_move(action)
    return HttpResponse('')

def car_servo(request):
    act = request.GET['act']
    action = str(act)
    print action
    res = car_handle.servo_move(action)
    return HttpResponse(str(res))
