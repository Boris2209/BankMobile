#ifndef NULLACCAUNT_H
#define NULLACCAUNT_H

#include <QMainWindow>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlTableModel>

namespace Ui {
class NullAccaunt;
}

class NullAccaunt : public QMainWindow
{
    Q_OBJECT

public:
    explicit NullAccaunt(QWidget *parent = nullptr);
    ~NullAccaunt();

private slots:
    void on_ButtonNull_clicked();

private:
    Ui::NullAccaunt *ui;

    QSqlDatabase db;//переменная для работы с БД

};

#endif // NULLACCAUNT_H
