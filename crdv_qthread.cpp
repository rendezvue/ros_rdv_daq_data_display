#include "crdv_qthread.h"
#include "iostream"

CRdvThread::CRdvThread()
{
    m_start = false;
}

void CRdvThread::run()
{
    while(1)
	{
        emit Update() ;
        msleep(50);
    }
}
