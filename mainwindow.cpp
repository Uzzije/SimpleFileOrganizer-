#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mydialogue.h"
#include <fstream>
#include <iostream>
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
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QFutureSynchronizer>
#include <QFutureWatcher>
using namespace std;



QString seconds = "not in stack";
stack<QString> global_stack;
//stack<double> time_stack;
int number_of_thread = QThread::idealThreadCount();
clock_t begin_of_clock;
clock_t end_of_clock;
clock_t time_spent;
QFutureWatcher<void> synchronizer;
//double time_spents = 0.0;
MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    //connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),
            //ui->progressBar, SLOT(setValue(int)));
    //connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),
            //ui->progressBar_2, SLOT(setValue(int)));
    //setCentralWidget(ui->plainTextEdit);
    //connect(ui->searchButton, SIGNAL(clicked()), ui->searchBar, SLOT(on_searchButton_clicked()));
   // time_stack.push(time_spents);
    //connect(&synchronizer, SIGNAL(finished()), SLOT(rendererReady()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_Window_2_triggered() //click a new window in menu
{
    MyDialogue MyDialogue( this );
    myDialogue->show();
}
/*
QList<QStringList> set_up_thread( QString search_path, int number_of_threads )
{
    QDir start_path = QDir( search_path ); // get initial start path
    QString temp_path;
    QFileInfoList entries = start_path.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot ); //get the list of directories in initial start path
    QList<QStringList> all_file_path;
    int index = entries.size()/number_of_threads; //get the count of how many directory will be worked on per thread (there might be remainders)
    if (entries.size() < number_of_threads){ // if folder entries is less than the number of threads available to use
        QStringList temp_list_single;
        for(int big_entry = 0; big_entry < entries.size(); big_entry++){ // for each entry in start path
            all_file_path.append(temp_list_single);
            QFileInfo fileInfo = entries.at( big_entry ); //get the directory/file of each of its subdirectory by index
            temp_path = fileInfo.filePath(); // get that subdirectory file/folder path
            all_file_path[big_entry].append(temp_path); //assign each index of all_file_path a list object with exactly one element (file_path). Basically assigning one thread per folder
          }
        for(int rest = entries.size(); rest < number_of_threads; rest++){
            all_file_path.append(temp_list_single);
            all_file_path[rest].append("empty");
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
    return all_file_path;
 }
*/
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

void MainWindow::add_to_stack(QString name){

    global_stack.push(name);

    displayFilePaths(global_stack, ui);
    end_of_clock = clock();

    //time_spent = (double)(end_of_clock - begin_of_clock) / CLOCKS_PER_SEC;
    //time_spents = time_spents + time_stack.top();
    //time_stack.pop();
    //time_stack.push(time_spents);
    //time_spent = (double) begin_of_clock/CLOCKS_PER_SEC + time_spent;
    //qDebug() << "time:" << time_spent;

}

void MainWindow::on_searchButton_clicked()
{

    QDir dir;
    QString string, root;
    QMessageBox msgBox;

    dirmodel = new QFileSystemModel( this );
    dirmodel->setRootPath( "/" );
    ui->treeView->setModel( dirmodel );
    ui->treeView->setRootIndex( dirmodel->index( "/" ) );

    root = dirmodel->rootPath();
    qRegisterMetaType<QString>();
    connect(&ptjob, &optThread::on_find, this, &MainWindow::add_to_stack);
    connect(&ptjobs, &optThread::on_find, this, &MainWindow::add_to_stack);


    //QDir start_path = QDir( root );
    //QFileInfoList entries = start_path.entryInfoList( QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot );

    QFuture<stack<QString>> test;
    QFuture<stack<QString>> test_place;
    QFuture<QList<QStringList>> test_two;
    QString search_word = ui->searchBar->text();
    QString searchPath = "/Users/Administrator/";
    QString start_path = "";
    QString end_path = "";
    //QString end_file = "";
    /*
    QThreadPool::globalInstance()->setMaxThreadCount(30);
    for(int x = 0; x < number_of_thread; x++){
      test_two = QtConcurrent::run(&this->ptjob3, &optThread::set_up_thread, searchPath, number_of_thread);
    }
    test_two.waitForFinished();
    //synchronizer.addFuture(test_two);

    QList<QStringList> listofpath = test_two.result();//set_up_thread(searchPath, number_of_thread);
    test_place = QtConcurrent::run(&this->ptjobs, &optThread::set_up_thread_files, searchPath, search_word);
    //synchronizer.setFuture(test_place);
    */
    clock_t begin = clock(), end;

    for(int x = 0; x < number_of_thread; x++){
        //QStringList tempList = listofpath[x];
        //start_path = tempList[0];
        //end_path = tempList[tempList.size() - 1];
        test = QtConcurrent::run(&this->ptjob, &optThread::start, searchPath, search_word, start_path, end_path);
        //synchronizer.addFuture(test);
        //synchronizer.setFuture(test);
    }

    //test_place.waitForFinished();
    test.waitForFinished();
    //synchronizer.waitForFinished();

    //displayFilePaths(test.result(), ui);

    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

   qDebug() << "Time is:" << QThread::idealThreadCount() << time_spent;

}
