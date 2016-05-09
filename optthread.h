#ifndef OPTTHREAD_H
#define OPTTHREAD_H
#include <QThread>
#include <QObject>
#include <stack>
class optThread : public QObject
{
    Q_OBJECT
public:
    explicit optThread(QObject *parent = 0);
    std::stack<QString> get_search_word( QString search_path, QString search_word );
    std::stack<QString> start(QString search_path, QString search_word, QString start_parth_of_file, QString end_path_of_file);
    std::stack<QString> set_up_thread_files( QString search_path, QString search_word);
    QList<QStringList> set_up_thread( QString search_path, int number_of_threads );
signals:
    void on_find(QString name);
    void on_find_file(QString name);
private:
    std::stack<QString> stack_of_files;
};
Q_DECLARE_METATYPE(QString)
#endif // OPTTHREAD_H
