#include "mydialogue.h"
#include "ui_mydialogue.h"

MyDialogue::MyDialogue(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyDialogue)
{
    ui->setupUi(this);
}

MyDialogue::~MyDialogue()
{
    delete ui;
}

void MyDialogue::on_pushButton_2_clicked()
{

}
