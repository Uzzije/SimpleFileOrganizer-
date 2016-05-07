#include "optthread.h"
#include "mainwindow.h"
#include <QMetaType>
#include <stack>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDir>
#include <QMutex>
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

optThread::optThread(QObject *parent) : QObject(parent)
{

}

std::stack<QString> optThread::start(QString search_path, QString search_word, QString start_path_of_file, QString end_path_of_file)
{

    QString temp_path; //holds temporal paths variable for looping through
    std::stack<std::stack<QString>> stack_of_stacks;
    std::stack<QString> stack_of_files, temp_stack; //a stack that holds all file paths containing directory or file of search word
    QDir start_path = QDir( search_path ); //Initialize directory object to investigated directories content
    QFileInfoList entriestemp = start_path.entryInfoList( QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot );
    QFileInfo startIndex(start_path_of_file);

    if(start_path_of_file == "empty"){
        return stack_of_files;
    }
    int starts = entriestemp.indexOf(startIndex);
    int entry = starts;
    if(starts < 0){
       entry= 0;
    }
    if( !start_path.exists() && search_path != end_path_of_file ) // if the initialized directory is not a directory, check if it is a file
    {
        QFile q_file( search_path ); //initiliaze file object to investigate file
        if( q_file.exists() )
        {
            QString file_name = q_file.fileName(); //get file name
            if( file_name.contains( search_word, Qt::CaseInsensitive ) ) // check if file name contains search_word
            {
                 emit on_find(start_path.filePath( file_name ));
                stack_of_files.push( start_path.filePath( file_name ) ); // push it's path into stack
                return stack_of_files;
            }
        }
    }
    else if( start_path.exists() ) // if initialize directory is a directory
    {
        QFileInfoList entries = start_path.entryInfoList( QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot ); // filter out its subdirectory for only files and folders

        for(; entry < entries.size(); entry++ ) //loop through each subdirectories
        {

            QFileInfo fileInfo = entries.at( entry ); //get the directory/file of each of its subdirectory by index
            temp_path = fileInfo.filePath(); // get that subdirectory file/folder path
            //qDebug() << "Currently On:" << temp_path;
            QDir check_dir = QDir( temp_path ); // initialize a directory object for that subdirectory to investigate its content
            if( fileInfo.isDir() ) // if subdirectory is a directory
            {
                temp_stack = start( temp_path, search_word, start_path_of_file, end_path_of_file);
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
                    emit on_find(file_name);
                    //return stack_of_files;
                }
            }
            if(temp_path == end_path_of_file){
                break;
            }
        }
    }

    int count = static_cast<int>( stack_of_stacks.size() );


    for( int start = 0; start < count; start++ )
    {
        std::stack<QString> temp_stack = stack_of_stacks.top(); //use copy
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
