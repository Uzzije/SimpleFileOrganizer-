#include "optthread.h"
#include "mainwindow.h"
#include <QMetaType>
#include <stack>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDir>

optThread::optThread(QObject *parent) : QObject(parent)
{

}

void optThread::start(QString search_path, QString search_word)
{
    QString temp_path; //holds temporal paths variable for looping through
    std::stack<QString> stack_of_files, temp_stack; //a stack that holds all file paths containing directory or file of search word
    QDir start_path = QDir( search_path ); //Initialize directory object to investigated directories content
    //for(int i = 0; i < 1000; i++){
    if( !start_path.exists() ) // if the initialized directory is not a directory, check if it is a file
    {
        QFile q_file( search_path ); //initiliaze file object to investigate file
        if( q_file.exists() )
        {
            QString file_name = q_file.fileName(); //get file name
            if( file_name.contains( search_word, Qt::CaseInsensitive ) ) // check if file name contains search_word
            {
                stack_of_files.push( start_path.filePath( file_name ) ); // push it's path into stack

            }
        }
    }
    else if( start_path.exists() ) // if initialize directory is a directory
    {
       QString old_path = search_path;
       std::stack<QString> track_of_files;
       std::stack<int> track_of_index_searched;
       QFileInfoList entries = start_path.entryInfoList( QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot ); // filter out its subdirectory for only files and folders
       track_of_files.push(entries.at(0).filePath());
       int start = 0;
       QFileInfoList temp_entries = entries;
       int count = -1;
            while(count != 0){
                for(;start <temp_entries.size(); start++){
                    QFileInfo tempInfo = temp_entries.at(start);
                    QString tempFilePath = tempInfo.filePath();
                    qDebug() << "I am running" << tempFilePath << thread()->currentThreadId();
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
                            //emit on_find(check_dir.filePath( file_name ));
                        }
                    }
                }
                int go_back_count = static_cast<int>( track_of_files.size() );
                stack_of_files.push("anything");
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
    //}

    emit on_find(stack_of_files);
    //QThread::currentThread()->msleep(100);
}

