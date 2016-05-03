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

    void start(QString search_path, QString search_word);

signals:
    void on_find(std::stack<QString> name);
};
Q_DECLARE_METATYPE(std::stack<QString>)
#endif // OPTTHREAD_H
