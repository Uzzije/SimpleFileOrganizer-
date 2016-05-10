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

    std::stack<QString> stack_of_files, temp_stack; //a stack that holds all file paths containing directory or file of search word

    QDirIterator it(search_path, QDir::Files, QDirIterator::Subdirectories);
    std::stack<QString> new_stack;

    while (it.hasNext()){
        new_stack.push(it.next());
        if( it.next().contains( search_word, Qt::CaseInsensitive ) ){
            emit on_find(it.next());
        }

    }

    return new_stack;
}
