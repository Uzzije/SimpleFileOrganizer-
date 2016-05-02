#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mydialogue.h"
#include <fstream>
#include <iostream>
#include <QList>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QFileSystemModel>
#include <QStringListModel>
#include <QListView>
#include <QtGlobal>
#include <QTreeView>
#include <QListView>
#include <QtWidgets>
#include <QStandardItemModel>
#include <QString>
#include <QStringListModel>
#include <sstream>
#include <QFileInfo>
#include <QFileInfoList>
#include <QStandardItemModel>
#include <QFileDialog>
#include <stack>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "optthread.h"


using namespace std;
 QList<QList<QString>> global_thread_list;
 int number_of_threads;
 std::stack<QString> global_stack;
MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );
    number_of_threads = 5;

}

QList<QString> path_start_end(int index){
    QList<QString> temp_list;
    if(index > global_thread_list.size()){
        global_thread_list[index][0].append("empty");
    }
    temp_list.append(global_thread_list[index][0]); // append first element;
    temp_list.append(global_thread_list[index][global_thread_list[index].size() - 1]); // append last element

    return temp_list;
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::update_global_path(stack<QString> stack_of_files){
    QString file_path_name;
    for(int start = 0; start < (int)stack_of_files.size(); start++){
        file_path_name = stack_of_files.top();
        global_stack.push(file_path_name);
        stack_of_files.pop();
    }

}

void MainWindow::on_actionNew_Window_2_triggered() //click a new window in menu
{
    MyDialogue MyDialogue( this );
    myDialogue->show();
}

void set_up_thread( QString search_path )
{

    QList<QList<QString>> all_file_path;
    QDir start_path = QDir( search_path ); // get initial start path
    QFileInfoList entries = start_path.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot ); //get the list of directories in initial start path
    int num = number_of_threads;
    int index = entries.size()/num; //get the count of how many directory will be worked on per thread (there might be remainders)
    if (entries.size() < number_of_threads){ // if folder entries is less than the number of threads available to use
        for(int big_entry = 0; big_entry < entries.size(); big_entry++){ // for each entry in start path
            QString temp_path = entries.at(big_entry).filePath(); // create a file path of that entry
            all_file_path[big_entry].append(temp_path); //assign each index of all_file_path a list object with exactly one element (file_path). Basically assigning one thread per folder
          }
     }
    else{ // If not the case that directories are more than folders from start path
        for(int entry = 0; entry < number_of_threads; entry++){ // go through each index of all_path_files that we intend to create a list of file path in
            for(int mul_path = 0; mul_path < index; mul_path++){ // go through the first sets of file_path to allocate to the first index of all_file_path and so forth
                QString temp_path = entries.at(mul_path).filePath(); // grab the string version of file paths
                all_file_path[entry].append(temp_path); // add those file paths to the index of all_file_path
            }
         }
            if((index*number_of_threads) < entries.size()){ // check if there are left over files that we didn't grab i.e remainder of the index initialization above
                for(int start = (index*number_of_threads + 1); start < entries.size(); start++){ // add the remainder file path to a new all_file_path index
                QString temp_path = entries.at(start).filePath(); // create directory path into a string
                all_file_path[index + 1].append( temp_path ); // pass the remainder into a single index in the all_file_path index
            }
         }
      }

    global_thread_list.append(all_file_path);
 }

void MainWindow::on_fileView_clicked(const QModelIndex &index)
{
    QString click_path = index.model()->data(index, Qt::DisplayRole).toString();

    QString fileName = QFileDialog::getOpenFileName(
                this, tr("Open File"),click_path,"All files (*.*);;Text File (*.txt);;Music file(*.mp3)"
                );
}
void MainWindow::displayFilePaths(stack<QString> stackOfFiles, Ui::MainWindow * ui)
{
    QString file_path;
    int count, start;
    
    count = static_cast<int>( stackOfFiles.size() );
    list_view_model = new QStringListModel();
    QStringList list_of_paths;
    for( start = 0; start < count; start++ )
    {
        file_path = stackOfFiles.top();
        list_of_paths.append( file_path );
        stackOfFiles.pop();
    }
    list_view_model->setStringList( list_of_paths );
    ui->fileView->setModel( list_view_model );
}

void createFolder( stack<QString> files, QDir dir )
{
    QString src, dest, file_name, file_num;
    QStringList list;
    int index, i = 1;
 
    dest = "~/"; //update to get path from UI
    
    while( !dir.mkdir( dest ) )
    {
        file_num = QString::number( i );
        if( i == 1 )
            dest += ( "_" + file_num );
        else
        {
            index = dest.lastIndexOf( "_" );
            dest.replace( index + 1, file_num.size(), file_num );
        }
        i++;
    }
    while( files.size() > 0 ){
        src = files.top();
        files.pop();
        list = src.split( "/" );
        file_name = list[ list.size() - 1 ];
        QFile::copy( src, dest + "/" + file_name );
    }
}

void MainWindow::on_searchButton_clicked()
{
    clock_t begin = clock(), end;
    QDir dir;
    QString string, root;
    QMessageBox msgBox;
    double time_spent;
    
    dirmodel = new QFileSystemModel( this );
    dirmodel->setRootPath( "/Financial" );
    ui->treeView->setModel( dirmodel );
    ui->treeView->setRootIndex( dirmodel->index( "~/" ) );
    root = dirmodel->rootPath();
    global_path = QDir::rootPath();
    set_up_thread( global_path );
    global_search_word = ui->searchBar->text();
    QString end_path = "/Applications";
    ptThread_0 = new optThread(this);
    ptThread_0->start_path_of_file = path_start_end(1)[0];
    ptThread_0->end_path_of_file = path_start_end(1)[1];
    ptThread_0->string_word = ui->searchBar->text();
    ptThread_0->start();
    stack<QString> new_stack( global_stack );
    connect(ptThread_0, SIGNAL(ReturnStackOfFile(stack<QString>)), this, SLOT(update_global_path(stack<QString>)));
    displayFilePaths(new_stack, ui);
    dir = dirmodel->rootPath();
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
}
