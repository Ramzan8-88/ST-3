// Copyright 2021 GHA Test Team

#include "TimedDoor.h"
#include <iostream>
#include <chrono>
#include <thread>

DoorTimerAdapter::DoorTimerAdapter(TimedDoor& doorRef) : door(doorRef) {}

void DoorTimerAdapter::Timeout() {
    door.throwState();
}

TimedDoor::TimedDoor(int timeout) : iTimeout(timeout), isOpened(false) {
    adapter = new DoorTimerAdapter(*this);
}

TimedDoor::~TimedDoor() {
    delete adapter;
}

bool TimedDoor::isDoorOpened() {
    return isOpened;
}

void TimedDoor::unlock() {
    isOpened = true;
}

void TimedDoor::lock() {
    isOpened = false;
}

int TimedDoor::getTimeOut() {
    return iTimeout;
}

void TimedDoor::throwState() {
    if (isOpened) {
        throw std::runtime_error("Door has been opened for too long!");
    }
}

Timer::Timer() : client(nullptr) {}

void Timer::sleep(int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void Timer::tregister(int timeout, TimerClient* clientPtr) {
    client = clientPtr;
    sleep(timeout);
    if (client != nullptr) {
        client->Timeout();
    }
}
