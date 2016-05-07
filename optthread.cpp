#include "optthread.h"
#include "mainwindow.h"
#include <QMetaType>
#include <stack>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDir>
#include <QMutex>

optThread::optThread(QObject *parent) : QObject(parent)
{

}

void optThread::start(QString search_path, QString search_word, QString start_path_of_file, QString end_path_of_file)
{
        QMutex mutex;
        qDebug() << "I was called" << thread()->currentThreadId() << "working on"<< start_path_of_file << end_path_of_file;
        int go_back_count;

        std::stack<QString> stacks_of_stacks;
        QString temp_path; //holds temporal paths variable for looping through
        std::stack<std::stack<QString>> stack_of_stacks;
        std::stack<QString> stack_of_files, temp_stack; //a stack that holds all file paths containing directory or file of search word
        QDir start_path = QDir( search_path ); //Initialize directory object to investigated directories content
        //QEventLoop loop;
        //QTimer::singleShot(1000, &loop, SLOT(quit()));
        //loop.exec();
        if( !start_path.exists() ) // if the initialized directory is not a directory, check if it is a file
        {
            QFile q_file( search_path ); //initiliaze file object to investigate file
            if( q_file.exists() )
            {
                QString file_name = q_file.fileName(); //get file name
                if( file_name.contains( search_word, Qt::CaseInsensitive ) ) // check if file name contains string_word
                {
                    //return stack_of_files; // change this to emit

                    stack_of_files.push( start_path.filePath( file_name ) ); // push it's path into stack
                    emit on_find(start_path.filePath(file_name));
                }
            }
        }
        else if( start_path.exists() ) // if initialize directory is a directory
        {
            QString old_path = start_path_of_file;
           std::stack<QString> track_of_files;
           std::stack<int> track_of_index_searched;
           QFileInfoList entries = start_path.entryInfoList( QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot ); // filter out its subdirectory for only files and folders
           QFileInfo startIndex(start_path_of_file);
           track_of_files.push( search_path );
           int start = entries.indexOf(startIndex);
           int count = -1;
           if(start < 0){
              count = 0;
           }
           QFileInfoList temp_entries = entries;
                while(count != 0){
                    for(;start <temp_entries.size(); start++){
                        QFileInfo tempInfo = temp_entries.at(start); //get file info
                        QString tempFilePath = tempInfo.filePath();  // get path string value
                        QDir check_dir = QDir( tempFilePath );       // make dir object
                        old_path = tempFilePath;
                        //qDebug() << "Old path" << old_path;
                        //qDebug() << "I am working right now on" << tempFilePath << thread()->currentThreadId();
                        if(tempInfo.isDir()){
                            track_of_files.push(tempFilePath);
                            //old_path = tempFilePath;
                            track_of_index_searched.push(start+1); // next time start from two
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
                                emit on_find(check_dir.filePath(file_name));
                                //return stack_of_files;

                            }
                        }
                  }
                    //qDebug() << "old path" << track_of_files.top() << thread()->currentThreadId();

                    if(track_of_files.top() == end_path_of_file){
                        count = 0;
                        go_back_count = 0;
                       qDebug() << "count" << "break"<< "I am done" << old_path << end_path_of_file << thread()->currentThreadId();
                       break;
                    }
                    else{
                        go_back_count = static_cast<int>( track_of_files.size() );
                    }

                  count = static_cast<int>( track_of_files.size() );
                    if(go_back_count != 0){
                        track_of_files.pop();
                        if(go_back_count > 1){
                            QString tempFilePath_back = track_of_files.top();
                            QDir check_dir_back = QDir( tempFilePath_back );
                            temp_entries = check_dir_back.entryInfoList( QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot );
                            //track_of_files.pop();
                            start = track_of_index_searched.top();
                            track_of_index_searched.pop();
                        }
                    }
                }
       }
        int file_count = (int)stack_of_files.size();
        //emit on_find(stack_of_files);
}

