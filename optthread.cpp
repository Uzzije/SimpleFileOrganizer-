#include "optthread.h"
#include <stack>
#include <QString>
#include <QtCore>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QFile>


optThread::optThread(QObject *parent)
    : QThread(parent)
{
    //start_path_of_file = start_path;
    //end_path_of_file = end_path;
    //string_word = search_word;
}

void optThread::run()
{
    if(start_path_of_file == "empty"){
        return;
    }
    std::stack<QString> stacks_of_stacks;
    stacks_of_stacks.push("value");
    //std::stack<QString> found_files = find_word_paths(start_path_of_file, end_path_of_file, string_word);
    emit ReturnStackOfFiles(stacks_of_stacks);
}

std::stack<QString> find_word_paths( QString search_path, QString end_path, QString search_word )
{
    QString temp_path; //holds temporal paths variable for looping through
    std::stack<std::stack<QString>> stack_of_stacks;
    std::stack<QString> stack_of_files, temp_stack; //a stack that holds all file paths containing directory or file of search word
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
        QFileInfoList entries = start_path.entryInfoList( QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot ); // filter out its subdirectory for only files and folders

        for( int entry = 0; entry < entries.size(); entry++ ) //loop through each subdirectories
        {
            QFileInfo fileInfo = entries.at( entry ); //get the directory/file of each of its subdirectory by index
            temp_path = fileInfo.filePath(); // get that subdirectory file/folder path
            QDir check_dir = QDir( temp_path ); // initialize a directory object for that subdirectory to investigate its content
            if( fileInfo.isDir() ) // if subdirectory is a directory
            {
                if (fileInfo.filePath() == end_path){ // if processor has reached the last file it is suppose to access exist out.
                    return stack_of_files;
                }
                temp_stack = find_word_paths( temp_path, end_path, search_word );
                if ( temp_stack.size() != 0 )
                    stack_of_stacks.push( temp_stack ); // recursively call get_search_word and add it to stack_of_files
                //stack_of_files.push(temp_path);
                //return stack_of_files;
            }
            else if( fileInfo.isFile() )
            {
                QFile q_file( fileInfo.filePath() );
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
    if( count == 0 ){
        return stack_of_files;
    }
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
