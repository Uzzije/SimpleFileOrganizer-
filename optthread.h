#ifndef OPTTHREAD_H
#define OPTTHREAD_H
#include <QThread>
#include <stack>
#include <QString>
class optThread : public QThread
{
    Q_OBJECT
public:
    explicit optThread(QObject *parent = 0);
    void run();
    bool Stop;
    QString start_path_of_file;
    QString end_path_of_file;
    QString string_word;
private:
    std::stack<QString> find_word_paths(QString start_path, QString end_path, QString search_word);
    bool inBounds;
signals:
    void ReturnStackOfFiles(std::stack<QString>);

public slots:
};

#endif // OPTTHREAD_H
