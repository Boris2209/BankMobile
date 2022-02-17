#include "newuser.h"
#include "ui_newuser.h"

NewUser::NewUser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewUser)
{
    ui->setupUi(this);
}

NewUser::~NewUser()
{
    delete ui;
}

//кнопка зарегистрировать
void NewUser::on_pushButton_clicked()
{
    //проверяем не пустые ли значения полей:
    if((ui->FIO->text()) == ""){
        ErrorInform();
        return;
    }

    if((ui->Account->text()) == ""){
        ErrorInform();
        return;
    }

    if((ui->Login->text()) == ""){
        ErrorInform();
        return;
    }

    if((ui->Password->text()) == ""){
        ErrorInform();
        return;
    }

    //проверка того, что в Account только числа
    try {

    QString acc = ui->Account->text();

    //предусмотрим счетчик точек и запятых тк число не может содержать более
    // одного знака разделения дробной и целой части
    int count = 0;

    for (int i=0; i<acc.size(); i++){
        if (acc[i]=='.' || acc[i]==','){
            count++;
        }
        if (acc[i]!='0' && acc[i]!='1' && acc[i]!='2'&& acc[i]!='3'&& acc[i]!='4'&& acc[i]!='5'&& acc[i]!='6'
                && acc[i]!='7'&& acc[i]!='8'&& acc[i]!='9'&& acc[i]!='.'&& acc[i]!=','&& acc[i]!='\n'&& acc[i]!='\r'&& acc[i]!=' '){
            throw 1;
        }
    }
    //если более одного раза встретилась разделительная то ошибка
    if(count>1){
        throw 2;
    }
    //если она встретилась в самом начале или конце
    if(acc[0]=='.' || acc[0]==',' || acc[acc.size()-1]=='.' || acc[acc.size()-1]==','){
        throw 3;
    }

    } catch (int Err) {
        Err = 0;
        ErrorInform();
        return;
    }

    FalseEnabledWidget();
    ui->Info->setText("Заявка направлена на рассмотрение");
}

//сообщение о некорректной информации
void NewUser::ErrorInform(){
    ui->Info->setText("Введена некорректная информация");
}

//делает виджеты неактивными
void NewUser::FalseEnabledWidget(){
    ui->FIO->setEnabled(false);
    ui->Account->setEnabled(false);
    ui->StateAccount->setEnabled(false);
    ui->Login->setEnabled(false);
    ui->Password->setEnabled(false);
    ui->pushButton->setEnabled(false);

    ui->AddInfoFile->setEnabled(false);
    ui->Adress->setEnabled(false);
}


//берет информацию из фалйла указанного по ссылке
void NewUser::on_AddInfoFile_clicked()
{

    try {
        //для правильного чтения русских символов файл должен быть в кодировке UTF-8 !!!
        QString adress = ui->Adress->text();
        QFile file(adress);

        if (!file.open(QIODevice::ReadOnly))
            throw 1;
        int count = 0;
        while (!file.atEnd())
        {
             QByteArray line = file.readLine();
             QString str = line.data();// <- прочитанная строка

             //устанавливаем ФИО
             if (count == 0){
                 ui->FIO->setText(str);
                 count++;
             }
             //сумма на счету
             else if(count == 1) {
                 ui->Account->setText(str);
                 count++;
             }
             //состояние счета
             else if(count == 2){
                 if(str == "1\r\n"){
                     ui->StateAccount->setChecked(true);
                 }
                 count++;
             }
             //логин
             else if(count == 3) {
                 ui->Login->setText(str);
                 count++;
             }
             //пароль
             else if(count == 4) {
                 ui->Password->setText(str);
                 count++;
             }
        }


    } catch (int err) {
        err = 0;
        ui->Info->setText("Ошибка чтения файла");
        return;
    }
}
