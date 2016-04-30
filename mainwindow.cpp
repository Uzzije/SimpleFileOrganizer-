#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mydialogue.h"
#include <iostream>
#include <fstream>
#include <QtGlobal>
#include <QFileSystemModel>
#include <QTreeView>
#include <QtWidgets>
#include <stack>
#include <QString>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <time.h>


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


stack<QString> get_search_word(QString search_path, QString search_word){
    //for each path in current_file_path
    QString temp_path; //holds temporal paths variable for looping through
    QString file_path_string = "testing purpose"; //Print f testing purposes
    stack<stack<QString>> stack_of_stacks;
    stack<QString> stack_of_files; //a stack that holds all file paths containing directory or file of search word
    stack<QString> temp_stack;
    QDir start_path = QDir(search_path); //Initialize directory object to investigated directories content
    if(!start_path.exists()){ // if the initialized directory is not a directory, check if it is a file
        QFile q_file(search_path); //initiliaze file object to investigate file
        if(q_file.exists()){
           QString file_name = q_file.fileName(); //get file name
           if(file_name.contains(search_word, Qt::CaseInsensitive)){ // check if file name contains search_word
               stack_of_files.push(start_path.filePath(file_name)); // push it's path into stack
               return stack_of_files;
            }
        }
    }
    else if(start_path.exists()){ // if initialize directory is a directory
        QFileInfoList entries = start_path.entryInfoList(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot); // filter out its subdirectory for only files and folders        
        #pragma omp parallel for ordered schedule(dynamic)
        for(int entry = 0; entry < entries.size(); entry++){ //loop through each subdirectories
            QFileInfo fileInfo = entries.at(entry); //get the directory/file of each of its subdirectory by index
            temp_path = fileInfo.filePath(); // get that subdirectory file/folder path
            QDir check_dir = QDir(temp_path); // initialize a directory object for that subdirectory to investigate its content
            if(fileInfo.isDir()) // if subdirectory is a directory
            {
                temp_stack = get_search_word(temp_path, search_word);
                if (temp_stack.size() != 0)
                    stack_of_stacks.push(temp_stack); // recursively call get_search_word and add it to stack_of_files
                //stack_of_files.push(temp_path);
                //return stack_of_files;
            }
            else if(fileInfo.isFile()){
              QFile q_file(fileInfo.filePath());//use fileInfo.filePath
              QString file_name = q_file.fileName();
              if(file_name.contains(search_word, Qt::CaseInsensitive)){
                    stack_of_files.push(file_name);
                    //return stack_of_files;
               }
            }
        }
 }

    int count = static_cast<int>(stack_of_stacks.size());

    for(int start = 0; start < count; start++)
    {
      stack<QString> temp_stack = stack_of_stacks.top(); //use copy
      int stack_count = static_cast<int>(temp_stack.size());
      stack_of_stacks.pop();
      for(int stacks_count = 0; stacks_count < stack_count; stacks_count++){
          stack_of_files.push(temp_stack.top());
          temp_stack.pop();
      }
    }
    return stack_of_files;
}

void MainWindow::on_searchButton_clicked()
{
    QMessageBox msgBox;
    QString string;
    clock_t begin, end;
    double time_spent;
    begin = clock();
    QFileSystemModel *dirmodel = new QFileSystemModel(this);
    dirmodel->setRootPath("C:/TEST/");
    ui->treeView->setModel(dirmodel);
    ui->treeView->setRootIndex(dirmodel->index("C:/TEST/"));
    //QString root = QDir::rootPath();
    QString root = dirmodel->rootPath();
    QString new_string = "financial";
    stack<QString> new_stack (get_search_word(root, new_string));
    //QString string_stack = get_search_word(root, new_string);
    QString file_path_string = new_stack.top();
    //for(int start = 0; start < 1; start++){
        //file_path_string = file_path_string;
    //}
    QDir dir = QDir::root();
    QStringList list;
    QString src, dest, file_name, file_num;
    int i = 1, index;
    dest = "C:/AAA";
    while(!dir.mkdir(dest))
    {
        file_num = QString::number(i);
        if(i == 1)
            dest += ("_" + file_num);
        else{
            index = dest.lastIndexOf("_");
            dest.replace(index + 1, file_num.size(), file_num);
    }
        i++;
    }
    while(new_stack.size() > 0){
        src = new_stack.top();
        new_stack.pop();
        list = src.split("/");
        file_name = list[list.size() - 1];
        QFile::copy(src, dest + "/" + file_name);
    }
    ui->fileView->setText(file_path_string);
    //ui->searchBar->setText("works");
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
}
