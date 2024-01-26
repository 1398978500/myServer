#ifndef __LSG_TIMER_H__
#define __LSG_TIMER_H__

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/uio.h>

#include <time.h>
#include <log.h>

class utilTimer;

struct clientData {
    sockaddr_in address;
    int sockfd;
    utilTimer *timer;
};

class utilTimer {
public:
    utilTimer() : prev(nullptr), next(nullptr) {}

public:
    time_t iExpire;
    void (*cbFunc)(clientData *);
    clientData *m_userData;
    utilTimer *prev;
    utilTimer *next;
};

class sortTimerLst{
public:
    sortTimerLst();
    ~sortTimerLst();

    void addTimer(utilTimer *timer);
    void adjustTimer(utilTimer *timer);
    void delTimer(utilTimer *timer);
    void tick();

private:
    void addTimer(utilTimer *timer, utilTimer *hear);

    utilTimer *head;
    utilTimer *tail;
};

class utils {
public:
    utils() {}
    ~utils() {}

    void init(int iTimeslot);
};


#endif

