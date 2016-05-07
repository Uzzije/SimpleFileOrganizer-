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

    void start(QString search_path, QString search_word, QString start_path_of_file, QString end_path_of_file);

signals:
    void on_find(QString name);
};
Q_DECLARE_METATYPE(QString)
#endif // OPTTHREAD_H
