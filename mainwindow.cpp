#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //цвет сообщения о блокировке счета
    ui->Block->setStyleSheet("color: rgb(255, 0, 0)");

    //подключаемся к базе данных
    try {
        db = QSqlDatabase::addDatabase("QSQLITE");
        //проследить что бы проект лежал в указанной папке (или тут измменить)
        db.setDatabaseName("debug\\qt.db");
        db.open();
    } catch (QString Error) {
        Error = "Ошибка БД";
        ui->Block->setText(Error);
    }
    //значение для кнопки разблокировки инициализируем для правильного обновления
    pushBlockButton = false;


    tmr = new QTimer();//создаем новый таймер
    tmr->setInterval(50);//данне будут обновляться 20 раз в секнду
    //Соединим сигнал timeout() таймера tmr со слотом updateTime() объекта класса MainWindow
    connect(tmr, SIGNAL(timeout()), this, SLOT(updateTime()));

    //и запустим таймер
    tmr->start();
    //таким образом таймер 20 раз в секунду обновляет всю информацию по банковскому счету
}

//обновление информации по таймеру
void MainWindow::updateTime(){
    DataBase();
    DataMoneyBase();
    StyleWindow();
}

//этот метод берет данные из бд (по состоянию счета и БК)
void MainWindow::DataBase(){

    //берем данные из БД
    try {
        //данные о состоянии счета
        QSqlQuery query;//в этой переменной храним запрос к БД
        query.exec("SELECT Account, StateAccount, FIO FROM Bank");

        //а теперь берем данные
        while (query.next())
        {
        money = query.value(0).toDouble();
        account = query.value(1).toBool();
        fio = query.value(2).toString();
        }

        //данные по банковской карте
        QSqlQuery queryBK; //так же берем данные по банковской карте
        queryBK.exec("SELECT Number, Hour, Month, Holder FROM Card");
        while(queryBK.next()){
            numberCard = queryBK.value(0).toString();
            dataCardHour = queryBK.value(1).toString();
            dataCardMonth = queryBK.value(2).toString();
            holderCard = queryBK.value(3).toString();
        }
    } catch (QString Error) {
        Error = "Ошибка БД";
        ui->label->setText(Error);
    }

}

//этот метод устанавливает все видимые значения окна
void MainWindow::StyleWindow(){

    //данные по состоянию счета
    ui->Money->setText(QString::number(money)); //количество денег
    ui->Name->setText(fio);                     //фио держателя счета

    //данные по банковской карте
    ui->numberCard->setText(numberCard);
    ui->dataCard->setText(dataCardMonth + "/" + dataCardHour);
    ui->holderCard->setText(holderCard);

    //установка вида кнопок и надписей
    //если счет не заблокирован
    if(account){
        ui->BlockButton->setStyleSheet(QString::fromUtf8("background-color: rgb(250, 0, 0);"));
        ui->BlockButton->setText("Заблокировать счет");
        ui->Block->hide();
        ui->Money->setStyleSheet("color: rgb(0, 0, 0)");

        ui->pushPr->hide();
        ui->passPr->clear();
        ui->passPr->hide();
        ui->labelPr->hide();
        //показываем кнопку блокировки
        ui->BlockButton->show();
        //показываем кнопку аннулирования
        ui->NullButton->show();

        ui->pushButton->setStyleSheet("color: rgb(0, 0, 0)");
        ui->StartPay->setStyleSheet("color: rgb(0, 0, 0)");
    }

    //если счет заблокирован но кнопку "разблокировать" еще не нажали
    else if(account == false && pushBlockButton==false){
        ui->BlockButton->setStyleSheet("background-color: rgb(250, 250, 250);");
        ui->BlockButton->setText("Разблокировать счет");
        ui->Block->show();
        ui->Money->setStyleSheet("color: rgb(255, 0, 0)");

        ui->pushPr->hide();
        ui->passPr->clear();
        ui->passPr->hide();
        ui->labelPr->hide();
        //показываем кнопку блокировки
        ui->BlockButton->show();
        //не показываем кнопку аннулирования
        ui->NullButton->hide();

        ui->pushButton->setStyleSheet("color: rgb(255, 0, 0)");
        ui->StartPay->setStyleSheet("color: rgb(255, 0, 0)");
    }

    //если счет заблокирован и кнопку "разблокировать" нажали
    else if(account == false && pushBlockButton==true) {
        ui->BlockButton->setStyleSheet("background-color: rgb(250, 250, 250);");
        ui->BlockButton->setText("Разблокировать счет");
        ui->Block->show();
        ui->Money->setStyleSheet("color: rgb(255, 0, 0)");

        ui->pushPr->show();
        ui->passPr->show();
        ui->labelPr->show();
        //а кнопку блокировки убираем
        ui->BlockButton->hide();
        //не показываем кнопку аннулирования
        ui->NullButton->hide();

        ui->pushButton->setStyleSheet("color: rgb(255, 0, 0)");
        ui->StartPay->setStyleSheet("color: rgb(255, 0, 0)");
    }
}

//данные о платежах
void MainWindow::DataMoneyBase(){

    //очищаем предыдущую информация=ю
    ui->MoneyList->clear();

    QString str = "";

    QString type, data, receiver;
    double sum=0;

    //берем данные из БД
    try {
        QSqlQuery query;//в этой переменной храним запрос к БД
        query.exec("SELECT Type, Sum, Data, Receiver FROM Pay");

        //а теперь берем данные
        while (query.next())
        {
        type = query.value(0).toString();
        sum = query.value(1).toDouble();
        data = query.value(2).toString();
        receiver = query.value(3).toString();

        //записываем их в строку
        if (type == "out")
            str+="Исходящий";
        else if(type == "in") {
            str+="Входящий";
        }
        else{
            str+="Bank Card";
        }

        str+= + "  |  " + QString::number(sum)+ "  |  " + data +  "  |  "  + receiver;

        //добавляем новую строку в список
        ui->MoneyList->addItem(str);
        str = "";
        }
    }
    catch(QString Error){
        Error = "Ошибка подключения к БД!";
        ui->label_2->setText("Error");
    }

}

//кнопка блокировки
void MainWindow::on_BlockButton_clicked()
{
    //если счет не заблокирован
    if (account){
        //отправляем в БД информацию о том, что счет заблокирован
        try {
            QSqlQuery query;//в этой переменной храним запрос к БД
            query.exec("UPDATE Bank SET StateAccount = false");
        }
        catch (QString Error) {
            Error = "Ошибка БД";
            ui->Block->setText(Error);
        }
        account = false;    //состояние счета
    }

    //если заблокирован
    else{
        //установим значеие для отображения виджетов проверки пароля
        pushBlockButton = true;
    }

}

//кнопка подтверждения пароля
void MainWindow::on_pushPr_clicked()
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
    userPassword = ui->passPr->text();

    if (userPassword == password){

        //отправляем в БД информацию о том, что счет разблокирован
        try {
            QSqlQuery query;//в этой переменной храним запрос к БД
            query.exec("UPDATE Bank SET StateAccount = true");
        }
        catch (QString Error) {
            Error = "Ошибка БД";
            ui->Block->setText(Error);
        }

        pushBlockButton = false;
    }
    else {
        ui->labelPr->setText("Неправильный пароль");
    }
}

//кнопка проведения платежа по БК
void MainWindow::on_pushButton_clicked()
{
    if(account){
    B = new BankCard;
    B->show();
    }

}

//кнопка перевода денег
void MainWindow::on_StartPay_clicked()
{
    if(account){
    P = new Pay;
    P->show();
    }
}

//кнопка аннулирования счета
void MainWindow::on_NullButton_clicked()
{
    N = new NullAccaunt();
    N->show();
}

MainWindow::~MainWindow(){
    delete ui;
}
