#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QFileSystemModel>
#include <QStringListModel>
#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>
#include <stack>
#include <QList>
#include <QString>
#include "mydialogue.h"
#include "optthread.h"
#include <QThread>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void showResults();
public slots:
    void update_global_path( std::stack<QString> stack_of_files );

private slots:
    void on_actionNew_Window_2_triggered();
    void on_searchButton_clicked();
    void displayFilePaths(std::stack<QString> stackOfFiles, Ui::MainWindow * ui);
    void on_fileView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    MyDialogue *myDialogue;
    QStringListModel *list_view_model;
    QFileSystemModel *dirmodel;
    std::stack<QString> global_stack;
    QString global_path;
    QString global_search_word;
    QList<QStringList> global_thread_list;
    int number_of_threads;
    optThread *ptThread_0;
    optThread *ptThread_1;
    QThread *main_thread;
    QThread *main2_thread;
    optThread *ptThread_2;
};

#endif // MAINWINDOW_H
