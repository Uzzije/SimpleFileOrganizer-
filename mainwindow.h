#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QFileSystemModel>
#include "mydialogue.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNew_Window_2_triggered();


    void on_searchButton_clicked();
    //void get_search_word(QFileSystemModel *search_path);

private:
    Ui::MainWindow *ui;
    MyDialogue *myDialogue;
};

#endif // MAINWINDOW_H
