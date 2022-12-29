from pynput.keyboard import Key, Listener
import time
import requests
import os
def on_press(key):
    global w,a,s,d,speed
    if key == Key.esc:
        print(f"你按下瞭esc，監聽結束")
        return False
    if key==Key.shift:
        if speed == 0:
            requests.get("http://192.168.43.67/d4/1")
        speed = 1
    else:
        if key.char=='w':
            if w==0:
                requests.get("http://192.168.43.67/d0/1")
            w = 1
        if key.char=='a':
            if a==0:
                requests.get("http://192.168.43.67/d1/1")
            a = 1
        if key.char=='s':
            if s==0:
                requests.get("http://192.168.43.67/d3/1")
            s = 1
        if key.char=='d':
            if d==0:
                requests.get("http://192.168.43.67/d2/1")
            d = 1        
def on_release(key):
    global w,a,s,d,speed
    if key==Key.shift:
        speed = 0
        if speed==0:
            requests.get("http://192.168.43.67/d4/0")
    else:
        if key.char=='w':
            w = 0
            if w==0 and s==0:
                requests.get("http://192.168.43.67/d0/0")
                os.system( 'cls' )
            elif s==1:
                requests.get("http://192.168.43.67/d3/1")
        if key.char=='a':
            a = 0
            if a==0 and d==0:
                requests.get("http://192.168.43.67/d1/0")
            elif d == 1:
                requests.get("http://192.168.43.67/d2/1")
        if key.char=='s':
            s = 0
            if w==0 and s==0:
                requests.get("http://192.168.43.67/d3/0")
            elif w==1:
                requests.get("http://192.168.43.67/d0/1")
        if key.char=='d':
            d = 0
            if a==0 and d==0:
                requests.get("http://192.168.43.67/d1/0")
            elif a == 1:
                requests.get("http://192.168.43.67/d1/1")
global w,a,s,d,speed
w=0
a=0
s=0
d=0
speed = 0
listener = Listener(on_press=on_press, on_release=on_release)
listener.start()
