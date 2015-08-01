/*
 * scheduler.h
 *
 *  Created on: Jul 19, 2015
 *      Author: davidryder
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "application.h"
//#include <string>
//using namespace std;

#define MAX_TASKS    10
#define MAX_NAME_LEN 16

enum TaskType {
    REPEAT = 1,
    FIXED = 2
};


typedef void (* Handler)();

typedef struct {
    char *  name;
    Handler h;
    TaskType type;
    unsigned long start;
    unsigned long interval;
    unsigned long offset;
    unsigned long n;
    unsigned long iterations;
} Item;


class Scheduler
{
public:
    Scheduler();
    void reset();
    void print();
    int find( char * name );
    void start( char * name );
    void stop( char * name );
    void task( char *  name, Handler h, unsigned long start );
    void task( char *  name, Handler h, unsigned long start, unsigned long offset, unsigned long interval );
    void task( char *  name, Handler h, unsigned long start, unsigned long offset, unsigned long interval, unsigned long n );
    void remove( char *  name );
    void execute();
    unsigned long nextTask(unsigned long maxTime );
    unsigned long timeFuture(unsigned long iSec);
    unsigned long timeRoundUp(unsigned long iSec, unsigned long offset);
    TaskType type;
	virtual ~Scheduler();

private:
    void createTask( char *  name, Handler h, TaskType type, unsigned long start, unsigned long offset, unsigned long interval, unsigned long n );
	Item taskList[ MAX_TASKS ];
    int _nItems;
};


#endif /* SCHEDULER_H_ */
