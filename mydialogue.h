#ifndef MYDIALOGUE_H
#define MYDIALOGUE_H

#include <QDialog>

namespace Ui {
class MyDialogue;
}

class MyDialogue : public QDialog
{
    Q_OBJECT

public:
    explicit MyDialogue(QWidget *parent = 0);
    ~MyDialogue();

private slots:
    void on_pushButton_2_clicked();

    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::MyDialogue *ui;
};

#endif // MYDIALOGUE_H
