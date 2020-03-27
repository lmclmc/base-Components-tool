#include <iostream>
#include "threadpool/workqueue.h"
#include "timer/ltimer.h"
#include <unistd.h>
using namespace std;
using namespace lmc;

int main()
{
    LTimer t;
    t.setTimer(20, []{
        static int a = 0;
        a++;
        cout << "1 ==" << a << endl;
    });

    t.setTimer(1000, []{
        static int a = 0;
        a++;
        cout << "222 ==" << a << endl;
    });

    t.setTimer(300, []{
        static int a = 0;
        a++;
        cout << "33333 ==" <<  a << endl;
    });

    t.setTimer(700, []{
        static int a = 0;
        a++;
        cout << "4444444 ==" <<  a << endl;
    });

    t.setTimer(1500, []{
        static int a = 0;
        a++;
        cout << "6666666666666 ==" <<  a << endl;
    });

    t.setTimer(30000, []{
        static int a = 0;
        a++;
        cout << "88888888888888888888888 ==" <<  a << endl;
    });

    t.startTimer();
    int a = 0;
    while (1)
    {
         ::usleep(3000000);
         a++;
         cout << "asd" << endl;
    }

    return 0;
}