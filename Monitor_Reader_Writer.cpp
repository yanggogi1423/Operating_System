/*
 *  Language-Level Mechanism인 Monitor를 이용한 Reader-Writer 함수 구현
 *  With C++. - 교재에는 없음 (연습용)
 * 
 *  Condition - Reader 복수, Writer 복수인 경우
 */

#include <iostream>
using namespace std;

#pragma region ClassesForMonitor

//  Condition Queue
class writingAllowed {
public:
    static void signal() {
        cout << "writingAllowed Signal!" << endl;
        //  원래는 쓰기가 완료되었다고 wake_up을 시킴 (기다리는 소비자를 깨움)
    }

    static void wait() {
        cout << "writingAllowed Wait!" << endl;
        //  원래는 대기시키는 코드 (Condition Queue에 삽입)
    }

    static bool isEmpty(){
        //  사실 관계는 없지만 Writing을 하고 싶어하는 애가 있는 확인함. Returning Boolean
        return true;
    }
};

class readingAllowed {
public:
    static void signal() {
        cout << "readingAllowed Signal!" << endl;
        //  원래는 읽기가 완료되었다고 wake_up을 시킴 (기다리는 생산자를 깨움)
    }

    static void wait() {
        cout << "readingAllowed Wait!" << endl;
        //  원래는 대기시키는 코드 (Condition Queue에 삽입)
    }

    static bool isEmpty(){
        //  사실 관계는 없지만 Reading을 하고 싶어하는 애가 있는 확인함. Returning Boolean
        return true;
    }
};

//  Critical Data (Shared Data) Region
class monitor {
public:
    //  딘일 공간
    static char buffer;

    static int workingReader; //  물건의 개수
    static bool isWritten;
};

//  Initialization
char monitor::buffer = 0;
int monitor::workingReader = 0;
bool monitor::isWritten = false;

//  쉬운 접근을 위한 static 객체 선언
static monitor Monitor;

#pragma endregion


//  Monitor Methods
void beginReading(){    //  읽기 시작

    //  만일 누가 쓰고 있거나, 쓰고 싶어하는 애가 있다면 대기한다.
    if(monitor::isWritten || !writingAllowed::isEmpty()){
        readingAllowed::wait();
    }

    monitor::workingReader++;

    //  만일 읽고 싶어하는 애가 존재한다면 신호를 보낸다.
    if(!readingAllowed::isEmpty()){
        readingAllowed::signal();
    }
}

void finishReading(){
    monitor::workingReader--;

    //  만일 읽고 있는 애가 없다면 쓰기 허가를 내준다.
    if(monitor::workingReader == 0){
        writingAllowed::signal();
    }
}

void beginWriting(){
    //  읽고 있는 애가 존재하거나, 누군가 쓰는 중이라면 쓰기를 대기한다.
    if(monitor::workingReader > 0 || monitor::isWritten){ 
        writingAllowed::wait();
    }

    monitor::isWritten = true;
}

void finishWriting(){
    monitor::isWritten = false;
    
    //  만일 읽고 싶어하는 애가 있다면, 읽기 허가
    if(!readingAllowed::isEmpty()){
        readingAllowed::signal();
    }
    //  그게 아니라면 쓰기 허가
    else{
        writingAllowed::signal();
    }
}