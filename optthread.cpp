#include "optthread.h"
#include <stack>
#include <QString>
#include <QtCore>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QFile>
#include <QMutex>
#include <QThread>


optThread::optThread(QObject *parent)
    : QObject(parent)
{
    _finished = true;
    _abort = false;
}

void optThread::doWork()
{
    qDebug() << "I was called" << thread()->currentThreadId();
    std::stack<QString> stacks_of_stacks;
    QString temp_path; //holds temporal paths variable for looping through
    std::stack<std::stack<QString>> stack_of_stacks;
    std::stack<QString> stack_of_files, temp_stack; //a stack that holds all file paths containing directory or file of search word
    QDir start_path = QDir( head_dir ); //Initialize directory object to investigated directories content
    if( !start_path.exists() ) // if the initialized directory is not a directory, check if it is a file
    {  // mutex.lock();
        QFile q_file( start_path_of_file ); //initiliaze file object to investigate file
        if( q_file.exists() )
        {
            QString file_name = q_file.fileName(); //get file name
            if( file_name.contains( string_word, Qt::CaseInsensitive ) ) // check if file name contains string_word
            {
                _finished = true;

                stack_of_files.push( start_path.filePath( file_name ) ); // push it's path into stack
                emit ReturnFoundWord(start_path.filePath(file_name));
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
       track_of_files.push( head_dir );
       int start = entries.indexOf(startIndex);
       int count = -1;
       if(start < 0){
          count = 0;
       }
       QFileInfoList temp_entries = entries;
       // mutex.unlock();
            while(count != 0){
                bool aborting = _abort;
                if(aborting){
                    break;
                }
                for(;start <temp_entries.size(); start++){
                    //qDebug() << "I was called" << thread()->currentThreadId();
                    QFileInfo tempInfo = temp_entries.at(start); //get file info
                    QString tempFilePath = tempInfo.filePath();  // get path string value
                    QDir check_dir = QDir( tempFilePath );       // make dir object
                    //qDebug() << "I am working right now on" << tempFilePath << thread()->currentThreadId();
                    if(tempInfo.isDir()){
                        track_of_files.push(tempFilePath);
                        old_path = tempFilePath;
                        track_of_index_searched.push(start+1); // next time start from two
                        start = -1;
                        temp_entries = check_dir.entryInfoList( QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot );
                    }
                    else if( tempInfo.isFile() )
                    {
                        QFile q_file( tempInfo.filePath() );//use fileInfo.filePath
                        QString file_name = q_file.fileName();
                        if( file_name.contains( string_word, Qt::CaseInsensitive ) )
                        {

                            stack_of_files.push( check_dir.filePath( file_name ) );
                            emit ReturnFoundWord(check_dir.filePath(file_name));
                            //return stack_of_files;

                        }
                    }

              }

                if(old_path == end_path_of_file){
                    //mutex.lock();
                    count = 0;
                    qDebug() << "I Should Stop now" << thread()->currentThreadId();  
                    //break;
                   //mutex.unlock();
                }
                else{
                    count = static_cast<int>( track_of_files.size() );
                    //qDebug() << "I am still going" << thread()->currentThreadId();
                  }
                int go_back_count = count;
                qDebug() << count << "Count left" << old_path << thread()->currentThreadId();
                if(go_back_count != 0){
                    track_of_files.pop();
                    if(go_back_count > 1){
                        QString tempFilePath_back = track_of_files.top();
                        QDir check_dir_back = QDir( tempFilePath_back );
                        temp_entries = check_dir_back.entryInfoList( QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot );
                        start = track_of_index_searched.top();
                        track_of_index_searched.pop();

                    }
                }
                mutex.lock();
                if(count == 0){
                  _finished = true;
                  qDebug() << "Set me to finished"<< thread()->currentThreadId();
                }
                 mutex.unlock();
                 //QThread::msleep(100);
            }
   }

    if(_finished){
        emit finished();
        qDebug() << "I am done" << thread()->currentThreadId();
    }
}
void optThread::abort(){
    if(_finished){
        _abort = true;
    }
}
void optThread::requestWork(){
    mutex.lock();
    _finished = false;
    _abort = false;
    qDebug()<< "Request worker start in Thread "<< thread()->currentThreadId();
    mutex.unlock();
    emit workRequested();
}
void optThread::requestPartWork(){
    mutex.lock();
    _finished = false;
    _abort = false;
    qDebug()<< "Request worker start in Thread Part "<< thread()->currentThreadId();
    mutex.unlock();
    emit workRequested();
}
