#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_Loginbtn_clicked()
{
    if(ui->UserLineEdit->text().trimmed() == tr("316010") &&
               ui->pwdLineEdit->text() == tr("010613"))
        {
           accept();
        } else {
           QMessageBox::warning(this, tr("Waring"),
                                 tr("user name or password error!"),
                                 QMessageBox::Yes);
           ui->UserLineEdit->clear();
           ui->pwdLineEdit->clear();
           ui->UserLineEdit->setFocus();
        }
}
