#include "logindialog.h"
#include "ui_logindialog.h"
#include "network/authorization.h"
#include <QSettings>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    count=0;
    connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(obtainCode()));

    EndPoint = "https://accounts.google.com/o/oauth2/auth";
    Scope = "https://www.googleapis.com/auth/drive";
    ClientID = "79129249048.apps.googleusercontent.com"; //normal
    RedirectURI = "urn:ietf:wg:oauth:2.0:oob";
    ResponseType = "code";

    QString str = QString("%1?client_id=%2&redirect_uri=%3&response_type=%4&scope=%5").arg(EndPoint).arg(ClientID).
            arg(RedirectURI).arg(ResponseType).arg(Scope);
    const QUrl url(str);
    ui->webView->setUrl(url);
    connect(this, SIGNAL(CodeObtained()),this,SLOT(close()));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

//This terrible workaround was made 'cause embeded webView doesn't want redirect to addres
//which contain as a host "localhost" and code for obtaining tokens. So this function parsing
//code from html. For user there is no difference.
void LoginDialog::obtainCode(){
    if(count>=4){
        QString htmlCode(ui->webView->page()->currentFrame()->toHtml());
        int lastIndex=htmlCode.lastIndexOf("value=\"");
        lastIndex+=7;
        Code = htmlCode.mid(lastIndex, 62);
        if(Code[0]!='\"'){
            ui->webView->hide();
            Authorization *auth = new Authorization();
            connect(auth,SIGNAL(tokenObtained()),this,SIGNAL(TokenObtained()));
            auth->Token(Code);
        }
    }
    count++;
}

