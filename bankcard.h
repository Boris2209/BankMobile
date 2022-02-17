#ifndef BANKCARD_H
#define BANKCARD_H

#include <QMainWindow>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlTableModel>
#include<QString>

#include<QDateTime>

namespace Ui {
class BankCard;
}

class BankCard : public QMainWindow
{
    Q_OBJECT

public:
    explicit BankCard(QWidget *parent = nullptr);
    ~BankCard();

private slots:
    void on_pushButton_clicked();

    void on_Start_clicked();

private:
    Ui::BankCard *ui;

    void DataBase();

    //по карте

    QString numberCard; //номер карты
    QString dataCardMonth, dataCardHour;   //срок действия
    QString holderCard; //имя владельца карты

    QSqlDatabase db;//переменная для работы с БД

};

#endif // BANKCARD_H
