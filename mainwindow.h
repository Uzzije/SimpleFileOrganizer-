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
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    optThread *ptThread_1;
    optThread *ptThread_0;
    optThread *ptThread_2;
    optThread *ptThread_3;
    optThread *ptThread_4;
public slots:
    void update_global_path( std::stack<QString> stack_of_files );
private slots:
    void on_actionNew_Window_2_triggered();


    void on_searchButton_clicked();
    //void get_search_word(QFileSystemModel *search_path);
    void displayFilePaths(std::stack<QString> stackOfFiles, Ui::MainWindow * ui);

    //void on_openFileButton_clicked();

    void on_fileView_clicked(const QModelIndex &index);
    //stack<QString> get_search_word( QString search_path, QString search_word );

private:
    Ui::MainWindow *ui;
    MyDialogue *myDialogue;
    QStringListModel *list_view_model;
    QFileSystemModel *dirmodel;
    std::stack<QString> global_stack;
    QString global_path;
    QString global_search_word;
    QList<QList<QString>> global_thread_list;
    int number_of_threads;
};

#endif // MAINWINDOW_H
