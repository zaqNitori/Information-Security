import os
from dotenv import load_dotenv
from flask import Flask
from flask_mail import Mail
from flask_mail import Message
from flask import render_template,request

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
msg_recipients = []
msg_recipients.append('zaq81009@gmail.com')
#msg_recipients.append('t107590037@ntut.org.tw')

@app.route("/send",methods=['POST','GET'])
def send():
    if request.method =='POST':
        if request.values['send']=='送出':
            return render_template('send.html',name=request.values['user'])
    return render_template('send.html',name="")

    """#  主旨
    msg_title = 'Hello It is Flask-Mail'
    #  寄件者，若參數有設置就不需再另外設置
    # msg_sender = 't107590026@ntut.org.tw'
    #  收件者，格式為list，否則報錯
    #msg_recipients = ['Recipients@mail_domain.com']
    #  郵件內容
    msg_body = 'Hey, I am mail body!'
    #  也可以使用html
    #  msg_html = '<h1>Hey,Flask-mail Can Use HTML</h1>'
    msg = Message(msg_title,
                  recipients=msg_recipients)
    msg.body = msg_body
    #  msg.html = msg_html
    
    #  mail.send:寄出郵件
    mail.send(msg)
    return 'You Send Mail by Flask-Mail Success!!'"""

@app.route("/decrypt",methods=['POST','GET'])
def decrypt():
    if request.method =='POST':
        if request.values['send']=='送出':
            return render_template('decrypt.html',name=request.values['user'])
    return render_template('decrypt.html',name="")

@app.route("/signUp",methods=['POST','GET'])
def signUp():
    with open('./member.json','r') as file:
        users = json.load(file)
    if request.method =='POST':
        if request.values['send']=='送出':
            if request.form['user'] == None:
                return render_template("signUp.html",alert="Account should not be empty.")
            elif request.form['email'] == None:
                return render_template("signUp.html",alert="Email should not be empty.")
            elif request.form['user'] in users:
                return render_template("signUp.html",alert="This account has already been used."
                                       ,email=request.form['email'])
            else:
                users[request.form['user']]={'email':request.form['email']}
                with open('./member.json','w') as fw:
	    json.dump(users, fw)
            return render_template('init.html',name=request.values['user'])
    return render_template('signUp.html',name="")

@app.route("/",methods=['POST','GET'])
def init():
    return render_template('init.html')

def sendEmail(sender,receiver,content):
    

if __name__ == "__main__":
    app.debug = True
    load_dotenv()
    app.run()
