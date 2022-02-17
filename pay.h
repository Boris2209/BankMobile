#ifndef PAY_H
#define PAY_H

#include <QMainWindow>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlTableModel>

#include<QDateTime>

namespace Ui {
class Pay;
}

class Pay : public QMainWindow
{
    Q_OBJECT

public:
    explicit Pay(QWidget *parent = nullptr);
    ~Pay();

private slots:
    void on_Start_clicked();

private:
    QSqlDatabase db;//переменная для работы с БД

    Ui::Pay *ui;
};

#endif // PAY_H
