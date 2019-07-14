#include "logindialog.h"
#include "ui_logindialog.h"
#include <QTimer>
#include <QDateTime>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    //关联定时器溢出信号和相应的槽函数
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer->start(1000);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginbtn_clicked()
{
    if(ui->usrLineEdit->text() == tr("123456") &&
               ui->pwdLineEdit->text() == tr("123456"))
        {
           accept();
        } else {
           QMessageBox::warning(this, tr("Waring"),
                                 tr("user name or password error!"),
                                 QMessageBox::Yes);
        }
    ui->usrLineEdit->clear();
    ui->pwdLineEdit->clear();
    ui->usrLineEdit->setFocus();
}

void LoginDialog::timerUpdate()
{
   //获取系统现在的时间
   QDateTime time = QDateTime::currentDateTime();
   //设置系统时间显示格式
   QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
   //在标签上显示时间
   ui->lineEdit->setText(str);
}
