#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QList>
#include <QNetworkInterface>

#include <QAbstractSlider>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QUdpSocket(this);

    socket->bind(QHostAddress::Any, 20000);

//    QList<QHostAddress> list = QNetworkInterface::allAddresses();
//    ui->ipLabel->setText(list[1].toString());

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_textEdit_textChanged()
{
    command = ui->textEdit->toPlainText();
    QTextCursor cursor1 = ui->plainTextEdit->textCursor();
    ui->plainTextEdit->insertPlainText(command + "\n");
    cursor1.setPosition(0, QTextCursor::MoveAnchor);
    ui->plainTextEdit->setTextCursor(cursor1);


    QTextCursor cursor2 = ui->textEdit->textCursor();
    cursor2.setPosition(0, QTextCursor::KeepAnchor);
    ui->textEdit->setTextCursor(cursor2);
    sendCommand(command, ip);
}

void MainWindow::readyRead(){
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;

    socket->readDatagram(buffer.data(), buffer.size(),
                         &sender, &senderPort);

    if(buffer=="Hello from Ubuntu"){
        ui->label->setText("UDP connection established");
        ui->textEdit->setEnabled(true);
        ui->plainTextEdit->setEnabled(true);
        ip = sender.toString();
        sendCommand("Hello from Windows", ip);
        connection = true;
    }else if(connection && startReceiving){
        QString str1(buffer);
        QString num = str1.split(",")[0] + "," + str1.split(",")[1];
        ui->textEdit->setText(num);
    }
}

void MainWindow::sendCommand(QString str, QString ip){
    QByteArray Data;
    Data.append(str);
    socket->writeDatagram(Data, QHostAddress(ip), 20000);

}

void MainWindow::on_connect_button_clicked()
{
    ip = ui->ipEdit->toPlainText();
    sendCommand("Hello from Windows", ip);
}

void MainWindow::on_plainTextEdit_textChanged()
{

}

void MainWindow::on_pushButton_clicked()
{
    if(startReceiving){
        startReceiving = false;
        ui->pushButton->setText("Start Receiving");
    }else{
        startReceiving = true;
        ui->pushButton->setText("Stop Receiving");
    }
}
