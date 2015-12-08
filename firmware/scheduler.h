/*
 * scheduler.h
 *
 *  Created on: Jul 19, 2015
 *      Author: davidryder
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#define SCHEDULER_MAX_TASKS 10
#define MAX_NAME_LEN 16

enum TaskType {
    REPEAT = 1,
    FIXED = 2,
	TRIGGER = 3
};


typedef void (* Handler)();

typedef struct {
    char *  name;
    Handler h;
    TaskType type;
    int start;
    int interval;
    int offset;
    int n;
    int iterations;
    int ran;
    int triggerOn;
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
    void task( char *  name, Handler h, int start );
    void task( char *  name, Handler h, int start, int offset, int interval );
    void task( char *  name, Handler h, int start, int offset, int interval, int n );
    void trigger( char * name, char * trigger, Handler h, int startSecs );
    void remove( char *  name );
    void execute();
    int nextTask(int maxTime );
    int timeFuture(int iSec);
    int timeRoundUp(int iSec, int offset);
    TaskType type;
	virtual ~Scheduler();

private:
    void createTask( char *  name, Handler h, TaskType type, int start, int offset, int interval, int n, int triggerOn );
	Item taskList[ SCHEDULER_MAX_TASKS ];
    int _nextTask;
    int _maxSize;
    int _now;
};


#endif /* SCHEDULER_H_ */
