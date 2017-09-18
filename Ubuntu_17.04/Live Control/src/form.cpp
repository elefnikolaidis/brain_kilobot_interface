#include "form.h"
#include "ui_form.h"
#include "kilowindow.h"
#include "packet.h"

#include <QDebug>
#include <QTimer>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
}

Form::~Form()
{
    delete ui;
}


void Form::show(){
    QWidget::show();

    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::Any, 20000);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

//When a UDP message arrives
void Form::readyRead(){

    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderport;
    int commandNum;
    int sStrength = 0;
    socket->readDatagram(buffer.data(), buffer.size(), &sender, &senderport);

    QString strBuffer(buffer);
    if(strBuffer.toInt() == 0 && buffer!=NULL){
        QString strCommandNum = strBuffer.split(",")[0];
        commandNum = strCommandNum.toInt();
        QString strStrength = strBuffer.split(",")[1];
        sStrength = strStrength.toInt();
        qDebug()<< sStrength;

    }else{
        commandNum = strBuffer.toInt();
    }

    ui->plainTextEdit->insertPlainText(buffer + "\n");
    cursorReplacement();

    if(buffer == "Bl"){
        commandsCount[4]++;
        QTimer::singleShot(4000, this, SLOT(blinkCheck()));
    }

    //If "start test" button is pressed
    if(start){
        qDebug()<< "Test Started";

        if(sStrength > 92 || lastStrength==1 || commandNum==1){
            qDebug()<<"Required strength reached";
            lastStrength=1;
            counter++;

            if(commandNum == 1){
                commandsCount[0]++;
            }else if(commandNum == 2){
                commandsCount[1]++;
            }else if(commandNum == 32){
                commandsCount[2]++;
            }else if(commandNum == 64){
                commandsCount[3]++;
            }

            if(counter==7){
                qDebug()<<"Counter";
                for(uint8_t i=0;i<4;i++){
                    if(commandsCount[i]>=5){
                        sendCommand(i);
                        previousCommand = i;
                        ui->plainTextEdit->insertPlainText("Sending " + command[i] + "\n");
                        cursorReplacement();

                        for(int y=0;y<4;y++)
                            commandsCount[y]=0;
                    }
                }
                counter=0;
                lastStrength=0;
            }

        }else{
            for(int y=0;y<4;y++)
                commandsCount[y]=0;
        }

    }

}

void Form::on_startButton_clicked()
{
    ui->plainTextEdit->insertPlainText("Test started \n");
    cursorReplacement();
    start=true;
    ui->stopButton->setEnabled(true);
    ui->startButton->setEnabled(false);
}

void Form::on_stopButton_clicked()
{
    for(int i=0;i<3;i++)
        commandsCount[i] = 0;
    start = false;
    ui->plainTextEdit->insertPlainText("Test stopped \n");
    cursorReplacement();
    ui->stopButton->setEnabled(false);
    ui->startButton->setEnabled(true);
}

void Form::sendCommand(uint8_t type){
       qDebug()<<"Sending command"<<command[type];
    uint8_t payload[9] = {0,0,0,0,0,0,0,0,0};

    kiloWin->sendDataMessage(payload, type);
    usleep(10000);//micro seconds
    kiloWin->sendMessage(250);

}

//Check if the user blinked 3 times within 4 seconds to start or stop the test
void Form::blinkCheck(){

    if(commandsCount[4]>=3){
        if(!start){
            on_startButton_clicked();
        }else{
            on_stopButton_clicked();
        }
    }
    commandsCount[3]=0;

}

void Form::on_stopMButton_clicked()
{
    uint8_t payload[9] = {0,0,0,0,0,0,0,0,0};
    kiloWin->sendDataMessage(payload, 0);
    usleep(10000);//micro seconds
    kiloWin->sendMessage(250);
}

void Form::on_forwardMbutton_clicked()
{
    uint8_t payload[9] = {0,0,0,0,0,0,0,0,0};
    kiloWin->sendDataMessage(payload, 1);
    usleep(10000);//micro seconds
    kiloWin->sendMessage(250);
}

void Form::on_leftMButton_clicked()
{
    uint8_t payload[9] = {0,0,0,0,0,0,0,0,0};
    kiloWin->sendDataMessage(payload, 2);
    usleep(10000);//micro seconds
    kiloWin->sendMessage(250);
}

void Form::on_rightMButton_clicked()
{
    uint8_t payload[9] = {0,0,0,0,0,0,0,0,0};
    kiloWin->sendDataMessage(payload, 3);
    usleep(10000);//micro seconds
    kiloWin->sendMessage(250);
}

void Form::on_closeWinButton_clicked()
{
    socket->disconnect();
    socket->close();
    qDebug()<<"Destroy Form window";
    QWidget::close();
}

void Form::cursorReplacement(){

    QTextCursor cursor1 = ui->plainTextEdit->textCursor();
    cursor1.setPosition(0, QTextCursor::MoveAnchor);
    ui->plainTextEdit->setTextCursor(cursor1);
}
