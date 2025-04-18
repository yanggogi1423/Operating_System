/*
*   Language-Level Mechanism인 Monitor를 이용한 Producer-Consumer 함수 구현
*   With C++.
*/

#include <iostream>
using namespace std;

#pragma region ClassesForMonitor 
static int N = 100;

//  Condition Queue
class bufHasData{
public:
    static void signal() {
        cout << "bufHasData Signal!" << endl;
        //  원래는 데이터가 생성되었다고 wake_up을 시킴 (기다리는 소비자를 깨움)
    }

    static void wait(){
        cout << "bufHasData Wait!" << endl;
        //  원래는 대기시키는 코드 (Condition Queue에 삽입)
    }
};

class bufHasSpace{
public:
    static void signal() {
        cout << "bufHasSpace Signal!" << endl;
        //  원래는 공간이 생성되었다고 wake_up을 시킴 (기다리는 생산자를 깨움)
    }

    static void wait(){
        cout << "bufHasData Wait!" << endl;
        //  원래는 대기시키는 코드 (Condition Queue에 삽입)
    }
};

//  Critical Data (Shared Data) Region
class monitor{
public :
    //  원래는 버퍼가 Circle-Queue이지만 편의상 array로 구현
    static char * buffer;

    static int validBufs;   //  물건의 개수
    static int in;          //  삽입 부분의 Idx
    static int out;         //  인출 부분의 Idx
};

//  Initialization
char * monitor::buffer = new char[N];
int monitor::validBufs = 0;
int monitor::in = 0;
int monitor::out = 0;


//  쉬운 접근을 위한 static 객체 선언
static monitor Monitor;

#pragma endregion


//  여기서부터 중요함 (위에는 사실상 필요 없음)
//  Monitor Algorithm
void pushData(char data){

    if(monitor::validBufs == N){ //  물건이 가득 있어?
        bufHasData::wait();
    }

    //  먼저 buffer의 idx:in에 데이터를 삽입
    monitor::buffer[monitor::in] = data;

    //  사용되고 있는 공간의 수를 늘림
    monitor::validBufs++;

    //  idx를 증가시킴 (단, N만큼 나누어야 함 -> Circle Queue)
    monitor::in = (monitor::in + 1) % N;

    //  Data가 생성되었다. 혹시 기다리는 소비자가 있다면 일어나라.
    bufHasData::signal();
}

char popData(){
    if(monitor::validBufs == 0){ // 물건이 있긴해?
        bufHasSpace::wait();
    }

    //  먼저 buffer의 idx::out에서 데이터를 인출함
    char tmp = monitor::buffer[monitor::out];

    //  사용되고 있는 공간의 수를 줄임
    monitor::validBufs--;

    //  idx를 감소시킴 
    monitor::out = (monitor::out - 1);

    //  Space가 생성되었다. 혹시 기다리는 생산자가 있다면 일어나라.
    bufHasSpace::signal();

    return tmp;     //  요부분은 없음
}