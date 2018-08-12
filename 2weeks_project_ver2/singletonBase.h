#pragma once
/*
싱글톤 패턴은 static멤버로 하나의 인스턴스만 생성됨
프로그램 안에서 전역적으로 접근이 되며, 보통 클래스의 인스턴스를 하나만 유지하고자 할 때 사용함

전역변수를 이용해도 하나의 인스턴스를 유지할 수 있지만,
싱글톤 패턴을 사용하면 클래스 자신이 자기의 유일한 인스턴스로 접근하는 방법을 '캡슐화'하여 관리할 수 있음
상속, 객체생성, 객체소멸, 인스턴스의 개수 제어 등등 활용에 용이함

더 좋은 방법은 자기 스스로가 자기의 유일한 인스턴스로 접글하는 방법을 자체적으로 관리하는 방법임!
이 싱글톤 베이스는 다른 인스턴스가 생성되지 않도록 처리할 수 있고 접근 방법 또한 제한을 둘 수 있음ㄴ
*/

/*
getSingleton을 호출하는 그 순간에 만약 없다면 생성되고, static이므로 프로그램이 종료될때까지 유지
씬에서 다른씬으로 변경될때, 데이터를 유지하고 싶다면 싱글톤을 사용한다(유니티)
*/

//클래스 템플릿 정의 ===============================================================================================
template <typename T>
class SingletonBase
{
protected:
    //싱글톤 인스턴스 선언
    static T* singleton; //자기에 대한 포인터를 자신이 갖고있다 <- 이게 핵심
    SingletonBase() {};
    ~SingletonBase() {};
public:
    //싱글톤 객체 가져오기
    static T* getSingleton();
    void releaseSingleton();
};

//클래스 템플릿의 함수 외부 정의 =====================================================================================
//우리 추가
/*
클래스 템플릿도 멤버함수를 클래스 외부에 정의하는 것이 가능하다
간혹 함수를 외부에 정의하는 과정에서 template <typename T>를 빼먹는 경우가 있는데,
클래스 템플릿의 정의와 함수의 정의가 완전히 별개이기 때문에 '각각'에 대해서 문자 T가 무엇을 의미하는 지를 설명해줘야한다

아래의 함수 정의에서 SingletonBase<T>가 의미하는 바는
T에 대해 템플릿화 된 SingletonBase클래스 템플릿!
*/

//싱글톤 인스턴스 선언
template <typename T>
T* SingletonBase<T>::singleton = nullptr; //static 멤버 변수 초기화

                                          //싱글톤 객체 가져오기
template <typename T>
T* SingletonBase<T>::getSingleton()
{
    //싱글톤 객체가 없으면 새로 만들자
    if (!singleton) singleton = new T;
    return singleton;
}

template <typename T>
void SingletonBase<T>::releaseSingleton()
{
    //싱글톤 객체가 있다면 메모리에서 해제하자
    if (singleton)
    {
        delete singleton;
        singleton = nullptr;
    }
}