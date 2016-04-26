#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mydialogue.h"
#include <iostream>
#include <fstream>
#include <QFileSystemModel>
#include <QTreeView>
#include <QtWidgets>
#include <stack>
#include <QString>
#include <string>
#include <string>
#include <sstream>
#include <iostream>


using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),
            //ui->progressBar, SLOT(setValue(int)));
    //connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),
            //ui->progressBar_2, SLOT(setValue(int)));
    //setCentralWidget(ui->plainTextEdit);
    //connect(ui->searchButton, SIGNAL(clicked()), ui->searchBar, SLOT(on_searchButton_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_Window_2_triggered() //click a new window in menu
{
    MyDialogue MyDialogue(this);
    myDialogue->show();
}


QString get_search_word(QString search_path, string search_word){
    //for each path in current_file_path
    QString file_path_string = "";
    stack<QString> stack_of_files;
    QDir start_path = QDir(search_path);
    if(!start_path.exists()){
        QFile q_file = QFile(search_path);
        if(q_file.exists()){
            QString file_name = q_file.fileName();
            if(file_name.find(search_word)){
                return fil
            }
        }
    }
    else if(start_path.exists()){

    }
    QStringList entries = start_path.entryList();
    for(int entry = 0; entry < entries.size(); entry++){
        //stack_of_files.push(entries.at(entry));
        //
        if()
    }
    int count = static_cast<int>(stack_of_files.size());

    for(int start = 0; start < count; start++)
    {
      file_path_string = file_path_string + stack_of_files.top();
      stack_of_files.pop();
    }

    return file_path_string;
    //if that path is a file:
      //check if it contains "search_word"
         // add path string to list_of_path arrays
            // continue
       // else if path is a directory:
                // call get_search_word(on that path)
 }

void MainWindow::on_searchButton_clicked()
{
    QFileSystemModel *dirmodel = new QFileSystemModel(this);
    dirmodel->setRootPath("~/");
    ui->treeView->setModel(dirmodel);
    ui->treeView->setRootIndex(dirmodel->index("~/"));
    QString root = QDir::rootPath();
    //QString string_stack = get_search_word(root);
    //QString file_path_string = stack_of_files.top();
    //for(int start = 0; start < 1; start++){
        //file_path_string = file_path_string;
    //}
    ui->fileView->setText("file_path_string");
    ui->searchBar->setText(string_stack);

}
