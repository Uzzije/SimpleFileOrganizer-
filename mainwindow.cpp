#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mydialogue.h"
#include <iostream>
#include <fstream>
#include <QFileSystemModel>
#include <QStringListModel>
#include <QTreeView>
#include <QListView>
#include <QtWidgets>
#include <stack>
#include <QString>
#include <string>
#include <string>
#include <sstream>
#include <iostream>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QStandardItemModel>
#include <QFileDialog>



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

    QString file_path_string = "testing purpose"; //Print f testing purposes
    stack<stack<QString>> stack_of_stacks;
    stack<QString> stack_of_files; //a stack that holds all file paths containing directory or file of search word
    QString temp_path; //holds temporal paths variable for looping through
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
        for(int entry = 0; entry < entries.size(); entry++){ //loop through each subdirectories
            QFileInfo fileInfo = entries.at(entry); //get the directory/file of each of its subdirectory by index
            temp_path = fileInfo.filePath(); // get that subdirectory file/folder path
            QDir check_dir = QDir(temp_path); // initialize a directory object for that subdirectory to investigate its content
            if(fileInfo.isDir()) // if subdirectory is a directory
            {
                //#pragma omp task
                stack_of_stacks.push(get_search_word(temp_path, search_word)); // recursively call get_search_word and add it to stack_of_files
                //stack_of_files.push(temp_path);
                //return stack_of_files;
            }
            else if(fileInfo.isFile()){
              QFile q_file(fileInfo.filePath());//use fileInfo.filePath
              QString file_name = q_file.fileName();

              if(file_name.contains(search_word, Qt::CaseInsensitive)){
                    stack_of_files.push(fileInfo.filePath());
                    return stack_of_files;
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

void displayFilePaths(stack<QString> stackOfFiles, Ui::MainWindow * ui){
    int count = static_cast<int>(stackOfFiles.size());
    QStringListModel *list_view_model = new QStringListModel();
    QString file_path;
    QStringList list_of_paths;
    for(int start = 0; start < count; start++){
        file_path = stackOfFiles.top();
        list_of_paths.append(file_path);
        stackOfFiles.pop();
    }
    list_view_model->setStringList(list_of_paths);
    ui->fileView->setModel(list_view_model);
}

void MainWindow::on_searchButton_clicked()
{
    QFileSystemModel *dirmodel = new QFileSystemModel(this);
    dirmodel->setRootPath("~/");
    ui->treeView->setModel(dirmodel);
    ui->treeView->setRootIndex(dirmodel->index("~/"));
    QString root = dirmodel->rootPath();// QDir::rootPath();
    stack<QString> new_stack (get_search_word(root, ui->searchBar->text()));
    displayFilePaths(new_stack, ui);
}
/*
void MainWindow::on_openFileButton_clicked()
{
    QModelIndexList selected_item = ui->fileView->selectedIndexes();
    QString s
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("Open File"),"~/","All files (*.*);;Text File (*.txt);;Music file(*.mp3)"
                );
}
*/
