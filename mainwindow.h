#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QStringListModel>
#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>
#include <stack>
#include "mydialogue.h"
#include "optthread.h"
#include <QtConcurrent/QtConcurrent>
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
    void displayFilePaths(std::stack<QString> stackOfFiles, Ui::MainWindow * ui);
    //void set_up_thread_files(QString search_path, QString search_word);
    //void on_openFileButton_clicked();

    void on_fileView_clicked(const QModelIndex &index);

public slots:
    void add_to_stack(QString name);

private:
    Ui::MainWindow *ui;
    MyDialogue *myDialogue;
    QStringListModel *list_view_model;
    QFileSystemModel *dirmodel;
    optThread ptjob;
    optThread ptjobs;
    optThread ptjob3;
    MainWindow *main_stuff;
    std::stack<QString> global_stack;
    QList<QStringList> global_thread_list;
};

#endif // MAINWINDOW_H
