#ifndef CRDV_GET_GIT_INFO_H
#define CRDV_GET_GIT_INFO_H

#include <string>

#ifndef VERSION_HASH
#define VERSION_HASH "0000000"
#endif

#ifndef COMMIT_DATE
#define COMMIT_DATE ""
#endif

class CRdv_Get_Git_Info
{
public:
	
	// 싱글턴 인스턴스를 반환할 멤버함수
	static CRdv_Get_Git_Info* getInstance();

	// 소멸자, instanceFlag를 false로 변경
	virtual ~CRdv_Get_Git_Info() ;

private:
	/**
	 * 생성자
	 * 외부에서 인스턴스를 생성할 수 없도록 private 영역에 정의하고 구현함
	 * 원리: private 영역에 생성자가 존재하므로 외부에서 SingletonClass()를 호출할 수 없음
	 */
	CRdv_Get_Git_Info(void) ;
	
	// 싱글턴 인스턴스가 생성되었는지 여부
	static bool instanceFlag;

	// 싱글턴 인스턴스
	static CRdv_Get_Git_Info* instance;
	

public:
	std::string GET_VERSION() ; 
	std::string GET_COMMIT_DATE() ; 
} ;

#endif
