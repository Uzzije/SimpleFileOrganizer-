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
    /*
    qDebug() << "Currently On:" << thread()->currentThreadId() << start_path_of_file;
    QString temp_path; //holds temporal paths variable for looping through
    std::stack<std::stack<QString>> stack_of_stacks;
*/
    std::stack<QString> stack_of_files, temp_stack; //a stack that holds all file paths containing directory or file of search word

    QDir start_path = QDir( search_path ); //Initialize directory object to investigated directories content
    QDirIterator it(search_path, QDir::Files, QDirIterator::Subdirectories);
    //stack<QString> new_stack;

    //QString string_word = ui->searchBar->text();
    while (it.hasNext()){
        //new_stack.push(it.next());
        if( it.next().contains( search_word, Qt::CaseInsensitive ) ){
            //qDebug() << it.next();
            emit on_find(it.next());//displayFilePaths(new_stack, ui);
        }

    }
    /*
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
            //qDebug() << "Currently On:" << thread()->currentThreadId();
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
                    //emit on_find(file_name);
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
    */
    return stack_of_files;
}
std::stack<QString> optThread::set_up_thread_files( QString search_path, QString search_word){
    qDebug() << "Currently On:" << thread()->currentThreadId();
    std::stack<QString> stack_of_files;
    stack_of_files.push("");
    QDir start_path = QDir( search_path ); // get initial start path
    QString temp_path;
    QFileInfoList entries_file = start_path.entryInfoList(QDir::Files);
    for(int big_entry = 0; big_entry < entries_file.size(); big_entry++){
        QFileInfo fileInfo = entries_file.at( big_entry );
        QFile q_files( fileInfo.filePath() );//use fileInfo.filePath
        if( q_files.exists() )
        {
            QString file_name = q_files.fileName();
            if( file_name.contains( search_word, Qt::CaseInsensitive ) )
            {
                //stack_of_files.push( file_name );
                //emit on_find( file_name );
                //
            }
        }
  }
    return stack_of_files;
}

QList<QStringList> optThread::set_up_thread( QString search_path, int number_of_threads )
{
    qDebug() << "Currently On:" << thread()->currentThreadId();

    QDir start_path = QDir( search_path ); // get initial start path
    QString temp_path;
    QFileInfoList entries = start_path.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot ); //get the list of directories in initial start path
    QThread::usleep(5);
     qDebug() << "im sleep"<< thread()->currentThreadId();
    QList<QStringList> all_file_path;
    int index = entries.size()/number_of_threads; //get the count of how many directory will be worked on per thread (there might be remainders)
    if (entries.size() < number_of_threads){ // if folder entries is less than the number of threads available to use
        QStringList temp_list_single;
        for(int big_entry = 0; big_entry < entries.size(); big_entry++){ // for each entry in start path
            QThread::usleep(5);
             qDebug() << "im sleep"<< thread()->currentThreadId();
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
