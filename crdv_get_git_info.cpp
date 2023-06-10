#include "crdv_get_git_info.h"

bool CRdv_Get_Git_Info::instanceFlag = false;
CRdv_Get_Git_Info* CRdv_Get_Git_Info::instance = NULL;

CRdv_Get_Git_Info::CRdv_Get_Git_Info(void)
{
}

CRdv_Get_Git_Info::~CRdv_Get_Git_Info(void)
{
	instanceFlag = false;

	if( instance )
	{
		delete instance ;
		instance = NULL;
	}
}

// 싱글턴 인스턴스를 반환할 멤버함수
CRdv_Get_Git_Info* CRdv_Get_Git_Info::getInstance()
{
	if(instance == NULL)
	{
		instance = new CRdv_Get_Git_Info();
		instanceFlag = true;
	}
	return instance;
}

std::string CRdv_Get_Git_Info::GET_VERSION() {
  return std::string(VERSION_HASH) ;
}

std::string CRdv_Get_Git_Info::GET_COMMIT_DATE() {
  return std::string(COMMIT_DATE);
}



