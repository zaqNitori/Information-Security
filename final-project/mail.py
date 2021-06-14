import os
import json
import random
import security as sec
from dotenv import load_dotenv
from flask import Flask
from flask_mail import Mail
from flask_mail import Message
from flask import render_template,request
from Crypto.PublicKey import RSA

app = Flask(__name__)
app.config.update(
    #  hotmail的設置
    MAIL_SERVER='smtp.gmail.com',
    MAIL_PROT=587,
    MAIL_USE_TLS=True,
    MAIL_USERNAME=os.environ.get('NTUT_MAIL_ACCOUNT'),
    MAIL_PASSWORD=os.environ.get('NTUT_MAIL_PWD'),
    MAIL_DEFAULT_SENDER='t107590026@ntut.org.tw'
)
#  記得先設置參數再做實作mail
mail = Mail(app)

@app.route("/send",methods=['POST','GET'])
def send():
    if request.method =='POST':
        with open("member.json","r") as file:
            users = json.load(file)
        if request.values['send']=='SEND':
            if not request.values['sender'] in users:
                return render_template('send.html', errorMsg="No Such Sender",name="")
            elif not request.values['receiver'] in users:
                return render_template('send.html', errorMsg="No Such Receiver",name="")
            else:
                plaintext = request.values['plaintext']
                receiver = request.values['receiver']
                sender = request.values['sender']
                pbk = users[receiver]['PublicKey'].encode()
                pvk = request.values['PrivateKey'].encode()
                mac = (int)(users[sender]['MAC'])
                cipher = sec.encrypt(plaintext, pbk, pvk, mac)
                sendEmail(request.values['sender']
                          , request.values['receiver']
                          , request.values['title']
                          , cipher)
                return render_template('send.html', name=request.values['send']
                                       , receiver=receiver)
    return render_template('send.html',name="")


@app.route("/decrypt",methods=['POST','GET'])
def decrypt():
    if request.method =='POST':
        if request.values['send']=='DECRYPT':
            ciphertext = request.values['CipherText']
            pvk = request.values['PrivateKey'].encode()
            plain = sec.decrypt(ciphertext,pvk)
            return render_template('decrypt.html'
                                   , name = request.values['send']
                                   , Contents=plain.decode())
        elif request.values['send']=='Authenticate':
            sender = request.values['sender']
            mac = int(request.values['mac'])
            with open('member.json','r') as file:
                users = json.load(file)
            pbk = users[sender]["PublicKey"].encode()
            number = sec.signature(mac,pbk)
            if number == int(users[sender]["MAC"]):
                IdentityMsg = "Identity Authentication Success !"
            else:
                IdentityMsg = "Identity Authentication Failed !"
            return render_template('decrypt.html'
                                   , name = request.values['send']
                                   , Contents =request.values['CipherText']
                                   , IdentityMsg = IdentityMsg)
    return render_template('decrypt.html',name="")

@app.route("/signUp",methods=['POST','GET'])
def signUp():
    with open('member.json','r') as file:
        users = json.load(file)
    if request.method =='POST':
        if request.values['send']=='SEND':
            if request.values['email'] in users:
                return render_template("signUp.html",name=""
                                       ,errorMsg="SameEmail")
            else:
                r = random.randint(1,10000)
                key = RSA.generate(1024)
                pvk = key.exportKey()
                pbk = key.publickey().exportKey()
                users[request.form['email']]={'PublicKey':pbk.decode(), 'MAC':r}
                with open('./member.json','w') as fw:
                    json.dump(users, fw)
                title = "Email Encryption App Sign Up"
                content = "Your PublicKey is :\n" + pbk.decode() + "\n\nYour PrivateKey is:\n" + pvk.decode()
                sendEmail(os.environ.get('NTUT_MAIL_ACCOUNT')
                          , request.values['email']
                          , title
                          , content)
            return render_template('init.html',success=1)
    return render_template('signUp.html',name="")

@app.route("/",methods=['POST','GET'])
def init():
    return render_template('init.html')

def sendEmail(sender, receiver, title, content):
    if sender == os.environ.get('NTUT_MAIL_ACCOUNT'):
        msg_title = title
    else:
        msg_title = "From " + sender + " ! Title: " + title
    msg_recipients = []
    msg_recipients.append(receiver)
    msg = Message(msg_title, recipients=msg_recipients)
    msg.body = content
    mail.send(msg)
    return;

if __name__ == "__main__":
    app.debug = True
    load_dotenv()
    app.run()
