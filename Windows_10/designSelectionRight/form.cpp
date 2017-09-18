#include "form.h"
#include "ui_form.h"
#include <QPixmap>
#include <QDebug>


Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    QPixmap pixmap("C:/Users/Eddie/Desktop/star.png");
    QIcon ButtonIcon(pixmap);
    ui->pushButton->setIcon(ButtonIcon);
    ui->pushButton->setIconSize(pixmap.size());

    QPixmap pixmap2("C:/Users/Eddie/Desktop/square.png");
    QIcon ButtonIcon2(pixmap2);
    ui->pushButton_2->setIcon(ButtonIcon2);
    ui->pushButton_2->setIconSize(pixmap2.size());

    QPixmap pixmap3("C:/Users/Eddie/Desktop/circle.png");
    QIcon ButtonIcon3(pixmap3);
    ui->pushButton_3->setIcon(ButtonIcon3);
    ui->pushButton_3->setIconSize(pixmap3.size());

    socket = new QUdpSocket(this);
}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_clicked()
{
    QByteArray Data;
    Data.append("1");
    socket->writeDatagram(Data, QHostAddress("143.167.49.116"), 20000);
}

void Form::on_pushButton_2_clicked()
{
    QByteArray Data;
    Data.append("2");
    socket->writeDatagram(Data, QHostAddress("143.167.49.116"), 20000);
}

void Form::on_pushButton_3_clicked()
{
    QByteArray Data;
    Data.append("3");
    socket->writeDatagram(Data, QHostAddress("143.167.49.116"), 20000);
}
