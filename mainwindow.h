#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QFileSystemModel>
#include <QStringListModel>
#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>
#include <stack>
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
    //void displayFilePaths(std::stack<QString> stackOfFiles, Ui::MainWindow * ui);

    //void on_openFileButton_clicked();

private:
    Ui::MainWindow *ui;
    MyDialogue *myDialogue;
    QStringListModel *list_view_model;
};

#endif // MAINWINDOW_H
