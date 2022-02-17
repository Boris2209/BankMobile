#include "nullaccaunt.h"
#include "ui_nullaccaunt.h"

NullAccaunt::NullAccaunt(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NullAccaunt)
{
    ui->setupUi(this);

    //подключаемся к базе данных
    try {
        db = QSqlDatabase::addDatabase("QSQLITE");
        //проследить что бы проект лежал в указанной папке (или тут измменить)
        db.setDatabaseName("debug\\qt.db");
        db.open();
    } catch (QString Error) {
        Error = "Ошибка БД";
        //ui->Block->setText(Error);
    }
}

NullAccaunt::~NullAccaunt()
{
    delete ui;
}

void NullAccaunt::on_ButtonNull_clicked()
{
    QSqlQuery query;//в этой переменной храним запрос к БД
    query.exec("SELECT Password FROM Bank");

    QString password, userPassword;

    //а теперь берем данные
    while (query.next())
    {
       password = query.value(0).toString();
    }

    //введенные пользователем
    userPassword = ui->Pass->text();

    if (userPassword == password){
        //отправляем в БД информацию о том, что счет обнулен
        try {
            QSqlQuery query2;//в этой переменной храним запрос к БД
            query2.exec("UPDATE Bank SET Account = 0");
        }
        catch (QString Error) {
            Error = "Ошибка БД";
            ui->label->setText(Error);
        }

        close();
    }
    else {
        ui->label_2->setText("Неправильный пароль");
    }
}
