#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QUdpSocket>
#include <QObject>
#include <QVector>
#include <QLabel>

#include "kilowindow.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();
    void setKiloWindow(KiloWindow *kiloWin) {this->kiloWin=kiloWin;}
    void cursorReplacement();
    void show();


private:
    Ui::Form *ui;

    QUdpSocket *socket;

    KiloWindow *kiloWin;
    bool start = false;
    bool testCompleted = false;
    int counter;
    int sentComm=0;
    int commandsCount[5]; // 0=Stop 1=Forward 2=Left 3=Right
    QString command[4] = {"Stop","Forward","Left","Right"};
    QLabel* labels[4];
    QVector<uint8_t> kilobotCommand; // The commands that are displayed on the .ui and are about to be sent to the Kilobots
    int previousCommand = 0; // Last command that was sent to the Kilobots
    int lastStrength = 0;

private slots:
    void readyRead();
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void sendCommand();
    void blinkCheck();
    void on_stopMButton_clicked();
    void on_forwardMbutton_clicked();
    void on_leftMButton_clicked();
    void on_rightMButton_clicked();
    void on_sendCommand_clicked();
    void on_resetTest_clicked();
    void on_closeWinButton_clicked();
};

#endif // FORM_H
