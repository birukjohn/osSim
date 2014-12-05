#pragma once

#include <mono/coherencyRequest.h>
#include <queue>
#include <list>

class Bus {
public:
    void enque(CoherencyRequest *req);
    CoherencyRequest* deque();
private:
    std::queue<CoherencyRequest *, std::list<CoherencyRequest *> > requestQueue;
};
