#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlTableModel>

#include<QTimer>

#include"bankcard.h"
#include"pay.h"
#include"nullaccaunt.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void on_BlockButton_clicked();

    void on_pushPr_clicked();

    void on_pushButton_clicked();

    void on_StartPay_clicked();

    void updateTime();  //слот для обновления данных

    void on_NullButton_clicked();

private:
    //данные из БД
    void DataBase();
    void DataMoneyBase();

    //метод установки цвета и значений
    void StyleWindow();

    QTimer *tmr;    //ссылка на таймер

    double money;   //количество денег на счету
    QString fio;    //ФИО держател счета
    bool account;   //состояние счета

    bool pushBlockButton;//нажатие кнопки "разблокировать" для правильного обновления

    //по карте
    QString numberCard; //номер карты
    QString dataCardMonth, dataCardHour;   //срок действия
    QString holderCard; //имя владельца карты

    Ui::MainWindow *ui;
    QSqlDatabase db;//переменная для работы с БД

    BankCard *B;    //окно БК
    Pay *P;         //окно проведения платежей
    NullAccaunt *N; //окно аннулирования счета

};

#endif // MAINWINDOW_H
