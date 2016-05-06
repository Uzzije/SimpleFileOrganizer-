#ifndef OPTTHREAD_H
#define OPTTHREAD_H
#include <QThread>
#include <stack>
#include <QString>
#include <QMutex>

class optThread : public QObject
{
    Q_OBJECT
public:
    explicit optThread(QObject *parent = 0);
    QString start_path_of_file;
    QString end_path_of_file;
    QString string_word;
    QString head_dir;
    void abort();
    void requestWork();
private:
    bool _finished;
    QMutex mutex;
    bool _abort;

signals:
    void ReturnStackOfFiles(std::stack<QString>);
    void ReturnFoundWord(QString path);
    void workRequested();
    void finished();
public slots:
    void doWork();
};
Q_DECLARE_METATYPE(std::stack<QString>)
Q_DECLARE_METATYPE(QString)
#endif // OPTTHREAD_H
