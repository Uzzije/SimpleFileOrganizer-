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

int number_of_thread = QThread::idealThreadCount();
clock_t begin_of_clock;
clock_t end_of_clock;
double time_spent;


MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_Window_2_triggered()
{
    MyDialogue MyDialogue( this );
    myDialogue->show();
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

    dest = "~/";
    
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

    QFutureSynchronizer<void> synchronizer;
    QFuture<stack<QString>> test;
    QFuture<stack<QString>> test_place;
    QFuture<QList<QStringList>> test_two;
    QString search_word = ui->searchBar->text();
    QString searchPath = "/Users/Administrator/";
    QString start_path = "";
    QString end_path = "";
    QDir start_path_of_search = QDir( searchPath );

    clock_t begin = clock(), end;
    QFileInfoList entries = start_path_of_search.entryInfoList( QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot );
    number_of_thread = entries.size();
    for(int x = 0; x < number_of_thread; x++){
        QFileInfo fileInfo = entries.at( x ); //get the directory/file of each of its subdirectory by index
        QString temp_path = fileInfo.filePath();
        test = QtConcurrent::run(&this->ptjob, &optThread::start, temp_path, search_word, start_path, end_path);
        synchronizer.addFuture(test);
    }


    test.waitForFinished();
    synchronizer.waitForFinished();

    displayFilePaths(test.result(), ui);

    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;



}
