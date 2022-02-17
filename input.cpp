#include "input.h"
#include "ui_input.h"

Input::Input(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Input)
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
        ui->ErrText->setText(Error);
    }

}
//кнопрка Войти
void Input::on_Start_clicked()
{

    try {
        //берем логин и пароль из БД
        QSqlQuery query;//в этой переменной храним запрос к БД
        query.exec("SELECT Login, Password FROM Bank");

        QString Login;//логин из БД
        QString Password;//пароль из БД

        QString UserLogin, UserPass;//логин и пароль введенные пользователем
        UserLogin = ui->LogText->text();
        UserPass = ui->PassText->text();

        //а теперь берем данные
        while (query.next())
        {
        Login = query.value(0).toString();
        Password = query.value(1).toString();
        }

        //сравниваем данные для входа
        if (UserLogin == Login && UserPass==Password){
            w = new MainWindow;
            close();//закрываем это окно
            w->show();//открываем окно MainWindow
        }
        else {
             ui->ErrText->setText("Неверный логин и/или пароль");
        }

    } catch (QString Error) {
        Error = "Ошибка БД";
        ui->ErrText->setText(Error);
    }

}

void Input::on_Regist_clicked()
{
    u = new NewUser();
    u->show();
}

Input::~Input()
{
    delete ui;
}
