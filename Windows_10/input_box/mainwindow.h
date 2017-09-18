#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QObject>
#include <QUdpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_textEdit_textChanged();

    void readyRead();

    void sendCommand(QString str, QString ip);

    void on_connect_button_clicked();

    void on_plainTextEdit_textChanged();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    QUdpSocket *socket;
    QString command;
    bool connection = false;
    QString ip;
    bool startReceiving = false;
};

#endif // MAINWINDOW_H
