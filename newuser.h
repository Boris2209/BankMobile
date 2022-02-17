#ifndef NEWUSER_H
#define NEWUSER_H

#include <QMainWindow>
#include <QFile>

namespace Ui {
class NewUser;
}

class NewUser : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewUser(QWidget *parent = nullptr);
    ~NewUser();

private slots:
    void on_pushButton_clicked();

    void on_AddInfoFile_clicked();

private:
    Ui::NewUser *ui;

    void FalseEnabledWidget();
    void ErrorInform();
};
#endif // NEWUSER_H

