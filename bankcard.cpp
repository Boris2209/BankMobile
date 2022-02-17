#include "bankcard.h"
#include "ui_bankcard.h"

BankCard::BankCard(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BankCard)
{
    ui->setupUi(this);

    DataBase();
}

//основная информация по карте
void BankCard::DataBase(){

    //подключаемся к базе данных
    try {
        db = QSqlDatabase::addDatabase("QSQLITE");
        //проследить что бы проект лежал в указанной папке (или тут измменить)
        db.setDatabaseName("debug\\qt.db");
        db.open();
    } catch (QString Error) {
        Error = "Ошибка БД";
        ui->numberCard->setText(Error);
    }

    try {
    QSqlQuery queryBK; //так же берем данные по банковской карте
    queryBK.exec("SELECT Number, Hour, Month, Holder, Cvc FROM Card");
    while(queryBK.next()){
        numberCard = queryBK.value(0).toString();
        dataCardHour = queryBK.value(1).toString();
        dataCardMonth = queryBK.value(2).toString();
        holderCard = queryBK.value(3).toString();
    }

    //устанавливаем видимые значения в виджеты
    ui->numberCard->setText(numberCard);
    ui->dataCard->setText(dataCardMonth + "/" + dataCardHour);
    ui->holderCard->setText(holderCard);

    }
    catch (QString Error) {
        Error = "Ошибка БД";
        ui->numberCard->setText(Error);
    }

}

//показать CVC код
void BankCard::on_pushButton_clicked()
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
    userPassword = ui->PassCVC->text();

    if (userPassword == password){
        ui->Cvc->setText("CVC: " + numberCard);

        QSqlQuery queryBK; //так же берем данные по банковской карте
        queryBK.exec("SELECT Cvc FROM Card");

        QString cvc;

        while(queryBK.next()){
            cvc = queryBK.value(0).toString();
        }

        ui->Cvc->setText("CVC: " + cvc);

    }

    else {
        ui->label_2->setText("Неправильный пароль!");
    }

}


//сделать платеж в магазине по БК
void BankCard::on_Start_clicked()
{
    //берем данные о состоянии счета в базе данных и пароль для проверки
    bool account=1;//состояние счета
    double money=0;//количество денег на счету
    QString pass;

    //берем данные из БД
    try {
        QSqlQuery query;//в этой переменной храним запрос к БД
        query.exec("SELECT Account, StateAccount, Password FROM Bank");

        //а теперь берем данные
        while (query.next())
        {
        money = query.value(0).toDouble();
        account = query.value(1).toBool();
        pass = query.value(2).toString();
        }
    }
    catch (QString Error) {
         Error = "Ошибка БД";
         ui->label->setText(Error);
    }

    //проверка пароля
    QString userPass = ui->Pass->text();//пароль от пользователя

    if(userPass != pass){
        ui->label->setText("Ошибка! Неправильный пароль!");
        return;
    }


    //если счет заблокирован то ошибка
    if(account == false){
        ui->label->setText("Ошибка! Счет заблокирован!");
        return;
    }

    //берем данные от пользоваетля
    double userMoney=0;
    QString adress;
    try {
          userMoney = (ui->Sum->text()).toDouble();
          adress = ui->NumberOut->text();
    } catch (QString Error) {
        Error = "Ошибка! Введены некооректные данные!";
        return;
    }

    //проверяем что после платежа счет не уходит в "минус"
    if((money - userMoney)<0){
        ui->label->setText("У вас недостаточно средств");
        return;
    }

    //Банк блокирует счет за превышение платежа
    if(userMoney>4000){
        ui->label->setText("Ваш счет заблокирован за превышение платежа!");
        ui->label->setStyleSheet("color: rgb(255, 0, 0)");
        try {
            QSqlQuery query2;//в этой переменной храним запрос к БД
            query2.exec("UPDATE Bank SET StateAccount = false");
        }
        catch (QString Error) {
            Error = "Ошибка БД";
            ui->label->setText(Error);
        }
        return;
    }

    //проверяем корректность введенной суммы
    if(userMoney<=0){
        ui->label->setText("Некорректные значения!");
        return;
    }

    //получаем системные дату и время
    QString dateAndTime = (QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss"));


    //изменяем счет в БД
    money = money - userMoney;
    try {
        QSqlQuery query2;//в этой переменной храним запрос к БД
        QString qur = "UPDATE Bank SET Account = " + QString::number(money);//формируем запрос
        query2.exec(qur);//отправляем его в БД
    }
    catch (QString Error) {
        Error = "Ошибка БД";
        ui->label->setText(Error);
    }

    //добавляем новую запись в историю платежей
    try {
        QSqlQuery query3;//в этой переменной храним запрос к БД
        QString qur2 =
                "INSERT INTO Pay(Type, Sum, Data, Receiver)"
                " VALUES ('BK', '" + QString::number(userMoney) +
                "', '" + dateAndTime + "', '" + adress +"')";  //формируем запрос
        query3.exec(qur2);//отправляем его в БД

    }
    catch (QString Error) {
        Error = "Ошибка БД";
        ui->label->setText(Error);
    }
    close();
}

BankCard::~BankCard()
{
    delete ui;
}


