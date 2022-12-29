import smtplib
from email.mime.text import MIMEText
import time
import requests
from bs4 import BeautifulSoup

def send():
    # Google login
    gmail_id = 's09350116@go.thu.edu.tw'
    pa = ['your password']
    gmail_passwd = ''
    for i in pa:
        gmail_passwd += chr(i)
    to_address = 'jo1030147@gmail.com'
    msg = MIMEText('車子翻了 在 {0}'.format(time.ctime()), 'plain', 'utf-8') # 郵件內文
    msg['Subject'] = '車子翻覆'            # 郵件標題
    msg['From'] = 'car system'                  # 暱稱或是 email
    msg['To'] = to_address   # 收件人 email
    smtp = smtplib.SMTP('smtp.gmail.com', 587)
    smtp.ehlo()
    smtp.starttls()
    smtp.login(gmail_id,gmail_passwd)
    status = smtp.send_message(msg) 
    # terminating the session
    smtp.quit()

ed = 0
while(True):
    time.sleep(1)
    r = requests.get("http://192.168.43.67/d5/1")
    sp = BeautifulSoup(r.text,'lxml')
    down = sp.text.split(' ')[-1]
    print(sp.text.split(' ')[-1],ed)
    if down == '0':
        ed = 0
    if down=='1' and ed == 0:
        send()
        ed = 1
