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
using namespace std;



QString seconds = "not in stack";

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
void MainWindow::add_to_stack(stack<QString> name){
    global_stack = name;
    global_stack.push("testing");
    seconds = "In add stack";
}
/*
stack<QString> get_search_word( QString search_path, QString search_word )
{
    QString temp_path; //holds temporal paths variable for looping through
    stack<stack<QString>> stack_of_stacks;
    stack<QString> stack_of_files, temp_stack; //a stack that holds all file paths containing directory or file of search word
    QDir start_path = QDir( search_path ); //Initialize directory object to investigated directories content
    if( !start_path.exists() ) // if the initialized directory is not a directory, check if it is a file
    {
        QFile q_file( search_path ); //initiliaze file object to investigate file
        if( q_file.exists() )
        {
            QString file_name = q_file.fileName(); //get file name
            if( file_name.contains( search_word, Qt::CaseInsensitive ) ) // check if file name contains search_word
            {
                stack_of_files.push( start_path.filePath( file_name ) ); // push it's path into stack
                return stack_of_files; // change this to emit
            }
        }
    }
    else if( start_path.exists() ) // if initialize directory is a directory
    {
       QString old_path = search_path;
       stack<QString> track_of_files;
       stack<int> track_of_index_searched;
       QFileInfoList entries = start_path.entryInfoList( QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot ); // filter out its subdirectory for only files and folders
       track_of_files.push(entries.at(0).filePath());
       int start = 0;
       QFileInfoList temp_entries = entries;
       int count = -1;
            while(count != 0){
                for(;start <temp_entries.size(); start++){
                    QFileInfo tempInfo = temp_entries.at(start);
                    QString tempFilePath = tempInfo.filePath();
                    QDir check_dir = QDir( tempFilePath );
                    if(tempInfo.isDir()){
                        track_of_files.push(old_path);
                        old_path = tempFilePath;
                        track_of_index_searched.push(start+1);
                        start = -1;
                        temp_entries = check_dir.entryInfoList( QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot );
                    }
                    else if( tempInfo.isFile() )
                    {
                        QFile q_file( tempInfo.filePath() );//use fileInfo.filePath
                        QString file_name = q_file.fileName();
                        if( file_name.contains( search_word, Qt::CaseInsensitive ) )
                        {
                            stack_of_files.push( check_dir.filePath( file_name ) );
                            //return stack_of_files;
                        }
                    }
                }
                int go_back_count = static_cast<int>( track_of_files.size() );
                if(go_back_count != 1){
                    QString tempFilePath_back = track_of_files.top();
                    QDir check_dir_back = QDir( tempFilePath_back );
                    temp_entries = check_dir_back.entryInfoList( QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot );
                    track_of_files.pop();
                    start = track_of_index_searched.top();
                    track_of_index_searched.pop();
                }
                else if(go_back_count == 1){
                    count = 0;
                }
            }
        }
    return stack_of_files;
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

void MainWindow::on_searchButton_clicked()
{
    clock_t begin = clock(), end;
    QDir dir;
    QString string, root;
    QMessageBox msgBox;
    double time_spent;
    dirmodel = new QFileSystemModel( this );
    dirmodel->setRootPath( "/" );
    ui->treeView->setModel( dirmodel );
    ui->treeView->setRootIndex( dirmodel->index( "/" ) );

    root = dirmodel->rootPath();
    qRegisterMetaType<stack<QString>>();
    connect(&ptjob, &optThread::on_find, this, &MainWindow::add_to_stack);

    //QDir start_path = QDir( root );
    //QFileInfoList entries = start_path.entryInfoList( QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot );

    QFuture<void> test;
    QString search_word = ui->searchBar->text();
    QString searchPath = "/Users/Administrator/Desktop";
    QThreadPool::globalInstance()->setMaxThreadCount(30);
        for(int i=0; i< 30; i++){
            test = QtConcurrent::run(&this->ptjob, &optThread::start, searchPath, search_word);
            qDebug() << "I am thread:"<< thread()->currentThreadId();
        }
        test.waitForFinished();


    ui->searchBar->setText("Running.." + seconds);
    stack<QString> new_stack = global_stack ;
    displayFilePaths(new_stack, ui);

    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    qDebug() << "Time is:" << time_spent;
}
