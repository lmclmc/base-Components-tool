#include "ltimer.h"
#include "single/uuid.hpp"

using namespace std;

LTimer::LTimer() : bStatus(false),
                   w(make_shared<WorkQueue>()),
                   timeStamp(0),
                   tmpTimeStamp(1000000)
{
}

uint64_t LTimer::setTimer(uint64_t time, const function<void()> &f, int64_t count)
{
    uint64_t uuid = UUID::generateUuid();
    mutex.lock();
    taskList.emplace_back(time * 1000, time * 1000, f, count, uuid);
    tmpTimeStamp = tmpTimeStamp > time*1000 ? time*1000 : tmpTimeStamp;
    mutex.unlock();
    return uuid;
}

void LTimer::removeTimer(uint64_t uuid)
{
    mutex.lock();
    for (auto it = taskList.begin(); it != taskList.end();)
    {
        if (it->uuid == uuid)
        {
            it = taskList.erase(it);
        }
        else
        {
            it++;
        }
    }
    mutex.unlock();
}

void LTimer::clearTimer()
{
    mutex.lock();
    taskList.clear();
    mutex.unlock();
}

void LTimer::startTimer()
{
    bStatus = true;

    tvS = std::chrono::system_clock::now().time_since_epoch().count() / 1000;
    tvE = tvS;

    task();
}

void LTimer::stopTimer()
{
    bStatus = false;
}

void LTimer::task()
{
    w->addTask([this] {
        if (this->taskList.size() > 0)
        {
            mutex.lock();
            this->timeStamp = this->tmpTimeStamp;
            this->tmpTimeStamp = 10000000;

            for (auto it = this->taskList.begin();
                 it != this->taskList.end();)
            {
                it->time -= this->timeStamp;

                if (it->time <= 0)
                {
                    if (it->count > 0)
                        it->count--;
                    this->taskQueue.emplace_back(it->task);
                    it->time = it->maxTime;

                }

                if (it->time < this->tmpTimeStamp)
                    this->tmpTimeStamp = it->time;

                if (it->count == 0)
                {
                    it = this->taskList.erase(it);
                }
                else
                {
                    it++;
                }
            }

            mutex.unlock();

            this->tvS = std::chrono::system_clock::now().time_since_epoch().count() / 1000;

            std::this_thread::sleep_for(std::chrono::microseconds(this->timeStamp + this->tvE - this->tvS));

            this->tvE = std::chrono::system_clock::now().time_since_epoch().count() / 1000;

            for (auto &t : this->taskQueue)
            {
                t();
            }

            this->taskQueue.clear();
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1000));
        }

        if (!this->bStatus)
            return;

        this->task();
    });
}
