#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QUdpSocket>
#include <QObject>

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
    void show();
    void cursorReplacement();

private:
    Ui::Form *ui;

    QUdpSocket *socket;

    KiloWindow *kiloWin;
    bool start = false;
    int counter;
    int commandsCount[5]; // 0=Stop 1=Forward 2=Left 3=Right
    QString command[4] = {"Stop","Forward","Left","Right"};
    int previousCommand = 0; // Last command that was sent to the Kilobots
    int lastStrength = 0;

private slots:
    void readyRead();
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void sendCommand(uint8_t type);
    void blinkCheck();
    void on_stopMButton_clicked();
    void on_forwardMbutton_clicked();
    void on_leftMButton_clicked();
    void on_rightMButton_clicked();
    void on_closeWinButton_clicked();
};

#endif // FORM_H
