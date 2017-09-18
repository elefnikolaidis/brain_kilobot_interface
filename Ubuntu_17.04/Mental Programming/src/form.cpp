#include "form.h"
#include "ui_form.h"
#include "kilowindow.h"
#include "packet.h"

#include <QDebug>
#include <QTimer>
#include <QMessageBox>


Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::Any, 20000);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    ui->setupUi(this);
    labels[0]=ui->first;
    labels[1]=ui->second;
    labels[2]=ui->third;
    labels[3]=ui->fourth;
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
    QTextCursor cursor1 = ui->plainTextEdit->textCursor();
    cursor1.setPosition(0, QTextCursor::MoveAnchor);
    ui->plainTextEdit->setTextCursor(cursor1);

    if(buffer == "Bl"){
        commandsCount[4]++;
        QTimer::singleShot(4000, this, SLOT(blinkCheck()));
    }

    if(start){
        qDebug()<< "Test started";

        if(sStrength > 92 || lastStrength==1 || commandNum==1){
            qDebug()<<"Required signal strength reached";
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

            if(counter==10){
                qDebug()<<"Counter";
                for(uint8_t i=0;i<4;i++){
                    if(commandsCount[i]>=6){
                        kilobotCommand<<i;
                        previousCommand = i;
                        ui->plainTextEdit->insertPlainText("Sending " + command[i] + "\n");
                        cursorReplacement();

                       for(int e=0; e<kilobotCommand.size();e++){
                           labels[e]->setText(command[kilobotCommand[e]]);
                           labels[e]->setEnabled(true);
                           if(kilobotCommand.size()==1){
                               ui->resetTest->setEnabled(true);
                           }
                       }

                       if(kilobotCommand.size()==4){
                           on_stopButton_clicked();
                           ui->sendCommand->setEnabled(true);
                           ui->sendLabel->setText("If you accept these commands use command \"Forward\""
                                                  " to send them to the Kilobots or \"Neutral\" to Reset the "
                                                  "test, for over 10 tries.");
                           testCompleted = true;
//                           QMessageBox msgBox;
//                           msgBox.setWindowTitle("Test Completed");
//                           msgBox.setText("Do you accept the commands: " + command[kilobotCommand[0]] + " - "
//                                   + command[kilobotCommand[1]] + " - " + command[kilobotCommand[2]] + " - "
//                                   + command[kilobotCommand[3]]
//                                   + " ?");
//                           msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
//                           msgBox.setDefaultButton(QMessageBox::Yes);

//                           int ret = msgBox.exec();

//                           switch (ret) {
//                             case QMessageBox::Yes:
//                                    ui->sendCommand->setEnabled(true);
//                                 break;
//                             case QMessageBox::No:
//                               for(int e=0; e<4;e++){
//                                   labels[e]->clear();
//                                   labels[e]->setEnabled(false);
//                               }
//                                 break;
//                           }
                       }

                        for(int y=0;y<4;y++)
                            commandsCount[y]=0;

                        break;
                    }
                }
                counter=0;
                lastStrength=0;
            }

        }else{
            for(int y=0;y<4;y++)
                commandsCount[y]=0;
        }

    //end of "if(start)"
    }else if(testCompleted){

        if(sStrength > 92 || lastStrength==1 || commandNum==1){
            qDebug()<<"Required signal strength reached";
            lastStrength=1;
            counter++;
            if(commandNum == 1){
                commandsCount[0]++;
            }else if(commandNum == 2){
                commandsCount[1]++;
            }

            if(counter==10){
                qDebug()<<"Counter";
                counter=0;
                if(commandsCount[0]>=6){
                    on_resetTest_clicked();
                    ui->plainTextEdit->insertPlainText("Reseting test \n");
                    cursorReplacement();
                }else if(commandsCount[1]>=6){
                    on_sendCommand_clicked();
                    ui->plainTextEdit->insertPlainText("Sending to Kilobots \n");
                    cursorReplacement();
                }else{
                    for(int y=0;y<4;y++)
                        commandsCount[y]=0;
                }

            }
        }
    }//End of "else if(testCompleted)

}

void Form::on_startButton_clicked()
{
    ui->plainTextEdit->insertPlainText("Test started \n");
    cursorReplacement();
    start=true;
    ui->stopButton->setEnabled(true);
    ui->startButton->setEnabled(false);
    ui->label_5->setText("Test started - Waiting for commands");
}

void Form::on_stopButton_clicked()
{
    for(int i=0;i<4;i++)
        commandsCount[i] = 0;
    start = false;
    ui->plainTextEdit->insertPlainText("Test stopped \n");
    cursorReplacement();

    ui->stopButton->setEnabled(false);
    ui->startButton->setEnabled(true);
    ui->label_5->setText("Waiting to start a test");
}

void Form::sendCommand(){
    qDebug()<<"sendCommand";
    uint8_t payload[9] = {0,0,0,0,0,0,0,0,0};
    uint8_t type = kilobotCommand[sentComm];

    ui->plainTextEdit->insertPlainText("Sending: " + command[type] + "\n");
    cursorReplacement();

    kiloWin->sendDataMessage(payload, type);
    usleep(10000);//micro seconds
    kiloWin->sendMessage(250);
    sentComm++;
    if(sentComm==4){
        sentComm=0;
    }

}

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

void Form::on_sendCommand_clicked()
{
    testCompleted = false;
    QTimer::singleShot(2000, this, SLOT(sendCommand()));

    if(ui->threeButton->isChecked()){
        QTimer::singleShot(5000, this, SLOT(sendCommand()));
        QTimer::singleShot(8000, this, SLOT(sendCommand()));
        QTimer::singleShot(11000, this, SLOT(sendCommand()));
    }else{
        QTimer::singleShot(7000, this, SLOT(sendCommand()));
        QTimer::singleShot(12000, this, SLOT(sendCommand()));
        QTimer::singleShot(17000, this, SLOT(sendCommand()));
    }

}

void Form::on_resetTest_clicked()
{
    ui->sendLabel->setText("");
    for(int e=0; e<4;e++){
        labels[e]->clear();
        labels[e]->setEnabled(false);        
    }
    kilobotCommand.clear();
    qDebug()<<kilobotCommand.isEmpty();
    ui->sendCommand->setEnabled(false);
    testCompleted = false;
}

void Form::cursorReplacement(){
    QTextCursor cursor1 = ui->plainTextEdit->textCursor();
    cursor1.setPosition(0, QTextCursor::MoveAnchor);
    ui->plainTextEdit->setTextCursor(cursor1);
}

void Form::on_closeWinButton_clicked()
{
    socket->disconnect();
    socket->close();
    qDebug()<<"Destroy Form window";
    QWidget::close();
}
