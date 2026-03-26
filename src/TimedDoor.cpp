// Copyright 2021 GHA Test Team
#include "TimedDoor.h"
#include <stdexcept>
#include <thread>
#include <chrono>


DoorTimerAdapter::DoorTimerAdapter(TimedDoor& d) : door(d) {}

void DoorTimerAdapter::Timeout() {
    Timer timer;
    timer.tregister(door.getTimeOut(), this);

    if (door.isDoorOpened()) {
        door.throwState();
    }
}


TimedDoor::TimedDoor(int timeout) : iTimeout(timeout), isOpened(false) {
    adapter = new DoorTimerAdapter(*this);
}

bool TimedDoor::isDoorOpened() {
    return isOpened;
}

void TimedDoor::unlock() {
    isOpened = true;
    adapter->Timeout();
}

void TimedDoor::lock() {
    isOpened = false;
}

int TimedDoor::getTimeOut() const {
    return iTimeout;
}

void TimedDoor::throwState() {
    throw std::runtime_error("Door opened too long!");
}


void Timer::sleep(int timeout) {
    std::this_thread::sleep_for(std::chrono::seconds(timeout));
}

void Timer::tregister(int timeout, TimerClient* c) {
    client = c;
    sleep(timeout);
}
