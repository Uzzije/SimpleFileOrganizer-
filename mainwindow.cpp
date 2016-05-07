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
#include <QObject>
#include <QDebug>
#include <QThread>

using namespace std;

QList<QStringList> global_thread_list;

int number_of_threads = 1;
//stack<QString> global_stack;
stack<stack<QString>> stacking_it;
MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    ptThread_0 = new optThread();

    main_thread = new QThread();
    ptThread_0->moveToThread(main_thread);
    qRegisterMetaType<std::stack<QString>>();
    qRegisterMetaType<QString>();

    connect(ptThread_0, SIGNAL(workRequested()), main_thread, SLOT(start()));
    connect(ptThread_0, SIGNAL(ReturnFoundWord(QString)), this, SLOT(update_global_stack(QString)));
    connect(main_thread, SIGNAL(started()), ptThread_0, SLOT(doWork()));
    connect(ptThread_0, SIGNAL(finished()), main_thread, SLOT(quit()), Qt::DirectConnection);

    dirmodel = new QFileSystemModel( this );
    dirmodel->setRootPath( "/Financial" );
    ui->treeView->setModel( dirmodel );
    ui->treeView->setRootIndex( dirmodel->index( "~/" ) );
}

QStringList path_start_end(int index){
    QStringList temp_list;
    if((index - 1) > global_thread_list.size()){
        temp_list.append("empty");
        temp_list.append("empty");
    }
    temp_list.append(global_thread_list[index][0]); // append first element;
    temp_list.append(global_thread_list[index][global_thread_list[index].size() - 1]); // append last element

    return temp_list;
}

MainWindow::~MainWindow()
{
    ptThread_0->abort();
    ptThread_1->abort();
    ptThread_2->abort();
    ptThread_3->abort();
    ptThread_4->abort();
    main_thread->wait();
    main2_thread->wait();
    main3_thread->wait();
    main4_thread->wait();
    main5_thread->wait();
    delete main_thread;
    delete ptThread_0;
    delete ptThread_1;
    delete ui;
}
void MainWindow::update_global_path(stack<QString> stack_of_files){
   // global_stack = stack_of_files;
    //QString testing = stack_of_files.top();
    //global_stack.push(testing);
    /*
    stack<QString> temp_stack (stack_of_files);
    for(int x = 0; x < temp_stack.size(); x++){
        QString path = temp_stack.top();
        global_stack.push(path);
        temp_stack.pop();
    }
    */
    displayFilePaths(stack_of_files, ui);
}
void MainWindow::update_global_stack(QString path){
    this->global_stack.push(path);
    qDebug() << "Added:" << path << "To Stack";
    displayFilePaths(global_stack, ui);
}
void MainWindow::on_actionNew_Window_2_triggered() //click a new window in menu
{
    MyDialogue MyDialogue( this );
    myDialogue->show();
}

void set_up_thread( QString search_path )
{

    QDir start_path = QDir( search_path ); // get initial start path
    QString temp_path;
    QFileInfoList entries = start_path.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot ); //get the list of directories in initial start path
    int num = number_of_threads;
    QList<QStringList> all_file_path;
    int index = entries.size()/num; //get the count of how many directory will be worked on per thread (there might be remainders)
    if (entries.size() < number_of_threads){ // if folder entries is less than the number of threads available to use
        QStringList temp_list_single;
        for(int big_entry = 0; big_entry < entries.size(); big_entry++){ // for each entry in start path
            all_file_path.append(temp_list_single);
            QFileInfo fileInfo = entries.at( big_entry ); //get the directory/file of each of its subdirectory by index
            temp_path = fileInfo.filePath(); // get that subdirectory file/folder path
            all_file_path[big_entry].append(temp_path); //assign each index of all_file_path a list object with exactly one element (file_path). Basically assigning one thread per folder
          }
     }
    else{
        int mul_path = 0;// If not the case that directories are more than folders from start path
        for(int entry = 0; entry < number_of_threads; entry++){ // go through each index of all_path_files that we intend to create a list of file path in
            QStringList temp_list;
            int next = entry + 1;
            for(;mul_path < (index*next); mul_path++){ // go through the first sets of file_path to allocate to the first index of all_file_path and so forth
                temp_list.append(entries.at(mul_path).filePath()); // add those file paths to the index of all_file_path
            }
            all_file_path.append(temp_list);

         }

            if((index*number_of_threads) < entries.size()){ // check if there are left over files that we didn't grab i.e remainder of the index initialization above
                QStringList temp_list_mod;
                for(int start = (index*number_of_threads); start < entries.size(); start++){ // add the remainder file path to a new all_file_path index
                    all_file_path[number_of_threads - 1].append(entries.at(start).filePath());
                   }
                //all_file_path[index].append( temp_list_mod ); // pass the remainder into a single index in the all_file_path index
             }
      }

    global_thread_list = all_file_path;
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
    global_path = QDir::rootPath();//QDir::rootPath();
    QString global_path2 = "/Users/Administrator/Desktop";
    set_up_thread( global_path2 );
    QString begin_search = path_start_end(0)[0];
    QString end_search = path_start_end(0)[1];
    ptThread_0->start_path_of_file = begin_search;
    ptThread_0->end_path_of_file = end_search;
    ptThread_0->head_dir = global_path2;
    ptThread_0->string_word = ui->searchBar->text();
    ptThread_0->requestWork();
    main_thread->wait();

    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    qDebug() << "Time Spent:"<< time_spent;
}
