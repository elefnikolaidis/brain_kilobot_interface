#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "form.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->label_2->setVisible(false);

    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::Any, 20000);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

}

MainWindow::~MainWindow()
{
    delete ui;    
}

void MainWindow::readyRead(){
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;

    socket->readDatagram(buffer.data(), buffer.size(),
                         &sender, &senderPort);

    if(buffer=="Hello from Ubuntu"){
        ui->label_2->setVisible(true);
        socket->close();
        socket->disconnect();

        Form *form = new Form;
        form->show();

        this->close();

    }
}
void MainWindow::on_pushButton_clicked()
{
    QString ip = ui->lineEdit->text();
    QByteArray Data;
    Data.append("Hello from Windows");
    socket->writeDatagram(Data, QHostAddress(ip), 20000);
}
