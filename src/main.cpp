// Copyright 2021 GHA Test Team
#include "TimedDoor.h"
#include <iostream>

int main() {
    try {
        TimedDoor door(5);
        Timer timer;
        DoorTimerAdapter adapter(door);

        std::cout << "Opening door..." << std::endl;
        door.unlock();

        std::cout << "Door opened: "
                  << (door.isDoorOpened() ? "Yes" : "No") << std::endl;
        std::cout << "Timeout setting: " << door.getTimeOut()
                  << " seconds" << std::endl;

        std::cout << "Starting timer..." << std::endl;
        timer.tregister(door.getTimeOut(), &adapter);

        std::cout << "Door closed normally" << std::endl;
        door.lock();
    }
    catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    return 0;
}
