#include <fstream>
#include <iostream>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QFileSystemModel>
#include <QListView>
#include <QtGlobal>
#include <QTreeView>
#include <QtWidgets>
#include <QStandardItemModel>
#include <QString>
#include <QStringListModel>
#include <sstream>
#include <stack>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mydialogue.h"

using namespace std;

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
                return stack_of_files;
            }
        }
    }
    else if( start_path.exists() ) // if initialize directory is a directory
    {    
        QFileInfoList entries = start_path.entryInfoList( QDir::Files|QDir::Dirs|QDir                                                 ::NoDotAndDotDot ); // filter out its subdirectory for only files and folders        
        for( int entry = 0; entry < entries.size(); entry++ ) //loop through each subdirectories
        {
            QFileInfo fileInfo = entries.at( entry ); //get the directory/file of each of its subdirectory by index
            temp_path = fileInfo.filePath(); // get that subdirectory file/folder path
            QDir check_dir = QDir( temp_path ); // initialize a directory object for that subdirectory to investigate its content
            if( fileInfo.isDir() ) // if subdirectory is a directory
            {
                temp_stack = get_search_word( temp_path, search_word );
                if ( temp_stack.size() != 0 )
                    stack_of_stacks.push( temp_stack ); // recursively call get_search_word and add it to stack_of_files
                //stack_of_files.push(temp_path);
                //return stack_of_files;
            }
            else if( fileInfo.isFile() )
            {
                QFile q_file( fileInfo.filePath() );//use fileInfo.filePath
                QString file_name = q_file.fileName();
                if( file_name.contains( search_word, Qt::CaseInsensitive ) )
                {
                    stack_of_files.push( file_name );
                    //return stack_of_files;
                }
            }
        }
    }

    int count = static_cast<int>( stack_of_stacks.size() );

    for( int start = 0; start < count; start++ )
    {
        stack<QString> temp_stack = stack_of_stacks.top(); //use copy
        int stack_count = static_cast<int>( temp_stack.size() );
        stack_of_stacks.pop();
        for( int stacks_count = 0; stacks_count < stack_count; stacks_count++ )
        {
            stack_of_files.push( temp_stack.top() );
            temp_stack.pop();
        }
    }
    return stack_of_files;
}

void displayFilePaths(stack<QString> stackOfFiles, Ui::MainWindow * ui)
{
    QStringListModel *list_view_model;
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
 
    dest = "C:/AAA"; //update to get path from UI 
    
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
    QFileSystemModel *dirmodel;
    QDir dir;
    QString string, root;
    QMessageBox msgBox;
    double time_spent;
    
    dirmodel = new QFileSystemModel( this );
    dirmodel->setRootPath( "C:/TEST/" );
    ui->treeView->setModel( dirmodel );
    ui->treeView->setRootIndex( dirmodel->index( "C:/TEST/" ) );
    root = dirmodel->rootPath();
    
    stack<QString> new_stack( get_search_word( root, ui->searchBar->text() ) );
    
    displayFilePaths(new_stack, ui);
    dir = dirmodel->rootPath(); //update to use UI path user chooses
    createFolder( new_stack, dir );
    
    // ui->fileView->setText( file_path_string );
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
}
