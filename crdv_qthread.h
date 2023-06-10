#ifndef CRDV_QTHREAD_H
#define CRDV_QTHREAD_H

#include <QThread>
#include <QObject>

class CRdvThread : public QThread
{
    Q_OBJECT

public:
    CRdvThread();
    bool m_start;

private:
    void run();

signals:
    void Update(void);
};


#endif

