from flask import Blueprint, render_template, request,flash, session, redirect,url_for
from database.db_user import db_User
from business_logical.bolt_nut import  twh_factory, get_twh_factory
from logger import Logger
from operator import itemgetter

web_user = Blueprint('web_user', __name__,template_folder='templates')



@web_user.route('/twh/login')
def login():
    return render_template('login.html')

@web_user.route('/twh/login_real', methods=['POST'])
def login_real():
    #TODO: login record and history
    Logger.Print('web_user.sign_up_real()', request.form.to_dict())
    user = db_User.get_user(request.form.get('user_id'))
    password = request.form.get("password")
    if user is None:
        flash("没有该用户")
        return render_template('login.html')
    if user["password"] != password:
        flash("密码错误")
        return render_template('login.html')
    if user['user_id'] == 'FengXuming':
        session['user'] = request.form.get('user_id')
        return render_template('home_admin.html')
    
    user_id = request.form.get('user_id')
    session['user'] = db_User.get_user(user_id) # type: ignore
    session['user']['factory_name'] = twh_factory[session['user']['twh_id']]
    # Logger.Print('@web_user.route(/login_real)', session)
    # "user": {
    #     "twh_id": "221109",
    #     "user_id": "jigong",
    #     "user_name": "(模拟)技工",
    #     "password": "jigong",
    #     "position": "技工",
    #     "factory_name": "卷积分义齿加工厂"
    # }
    return redirect(url_for('web_user.home'))

@web_user.route('/twh/sign_up')
def sign_up():
    twh = get_twh_factory(request.args.get('twh_id'))
    return render_template('sign_up.html', twh=twh)

@web_user.route('/twh/sign_up_real', methods=['POST'])
def sign_up_real():
    user_in_db  = db_User.get_user(request.form.get('user_id'))
    if user_in_db is None:
        db_User.table_user.insert(request.form.to_dict())

        return render_template('sign_up_ok.html', twh=get_twh_factory(request.form.get('twh_id')))

    flash("该用户名已经被使用，请更换一个用户名", 'error')
    return render_template('sign_up.html', twh=get_twh_factory(request.form.get('twh_id')))

@web_user.route('/twh/logout')
def log_out():
    if 'user' in session:
        del session['user']
        flash('已经成功登出')
        # twh = get_twh_factory('221109')
    return render_template('login.html')

@web_user.route('/twh/update_position', methods=['POST'])
def update_position():
    user = request.form.to_dict()
    user['doc_id'] = int(request.form.get('doc_id'))
    db_User.update_position(user)
    flash('员工' + user['user_name'] +  ' 的职位已经更新')
    return redirect(url_for('web_user.view_users'))

@web_user.route('/twh/view_users')
def view_users():
    users = db_User.get_users_of_twh(session['user']['twh_id'])
    sorted_users = sorted(users, key=itemgetter('position')) 

    # Logger.Print('@web_user.route(/view_users)', users)
    return render_template('view_users.html', users=sorted_users)

@web_user.route('/twh')
def home():
    if "user" not in session:
        return redirect(url_for("web_user.login"))
    if session['user']['position'] == '主管':
        return render_template('home_factory.html')
    if session['user']['position'] == '技工':
        return render_template('home_worker.html')
    flash('您的账户已经被主管临时禁用')
    return redirect(url_for('web_user.login'))