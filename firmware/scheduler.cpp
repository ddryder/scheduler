#include "scheduler.h"

Scheduler::Scheduler() {
	// TODO Auto-generated constructor stub
    reset();
    _nItems = 0;
}

Scheduler::~Scheduler() {
	// TODO Auto-generated destructor stub
}

void Scheduler::reset() {
	for (int i=0; i < MAX_TASKS; i++) {
		taskList[i].h = 0;
	}
}

void Scheduler::print() {
	for (int i=0; i < MAX_TASKS; i++) {
		Item ti = (Item)taskList[i];
		if ( ti.h != 0) {
			//cout << ti.name<< " " << ti.start << endl;
		}
	}
}

int Scheduler::find(char * name ) {
	int i = MAX_TASKS;
	for (i=0; i < MAX_TASKS; i++) {
		if ( strcmp( taskList[i].name, name) == 0 ) {
			break; // matched existing task
		}
	}
	if (i >= MAX_TASKS) { // not found
		for (i=0; i < MAX_TASKS; i++) {
			if ( taskList[i].h == 0 ) {
				break; // available
			}
		}
	}
	return i;
}


void Scheduler::task( char *  name, Handler h, unsigned long startSecs ) {
	// run once in number of seconds
	this->createTask( name, h, FIXED, timeFuture( startSecs ), 0, 9999, 1 );
}

void Scheduler::task( char *  name, Handler h, unsigned long start, unsigned long offset, unsigned long interval) {
	// run idefintely first at start time and subsequently at set interval
	this->createTask( name, h, REPEAT, timeRoundUp( start, offset), offset, interval, 0 );
}

void Scheduler::task( char *  name, Handler h, unsigned long start, unsigned long offset, unsigned long interval, unsigned long n ) {
	// repeat fixed number of times
	this->createTask( name, h, FIXED, timeRoundUp( start, offset ), offset, interval, n );
}


void Scheduler::createTask( char * name, Handler h, TaskType type, unsigned long start, unsigned long offset, unsigned long interval, unsigned long n ) {
	// create new or modify existing
	int i = find( name );
	if (i < MAX_TASKS) {
		Item *ti = &taskList[i];
		ti->name = name;
		ti->h = h;
		ti->type = type;
		ti->start = start ; // number of seconds to start
		ti->offset = offset;
		ti->interval = interval; // every xx seconds
		ti->n = n;
		ti->iterations = 0;
	}
}

void Scheduler::stop( char *  name ) {
	// stop all that match
	for (int i=0; i < MAX_TASKS; i++) {
		Item *ti = &taskList[i];
		if ( strcmp( ti->name, name) == 0 ) {
			ti->start = 0;
		}
	}
}

void Scheduler::start( char *  name ) {
	// start all that match
	for (int i=0; i < MAX_TASKS; i++) {
		Item *ti = &taskList[i];
		if ( strcmp( ti->name, name) == 0 ) {
			ti->start = timeRoundUp( ti->interval, ti->offset); // reschedule
		}
	}
}

void Scheduler::remove( char *  name ) {
	// remove all that match
	for (int i=0; i < MAX_TASKS; i++) {
		if ( strcmp( taskList[i].name, name) == 0 ) {
			taskList[i].h = 0;
			taskList[i].start = 0;
			taskList[i].name = '\0';
		}
	}
}

unsigned long Scheduler::timeFuture(unsigned long iSec) {
	return  Time.now() + iSec;
}

unsigned long Scheduler::timeRoundUp(unsigned long iSec, unsigned long offset) {
  // Round now up / future to next iSec period exactly - add offset to adjust
  unsigned long now = Time.now();
  unsigned long o1 = now % iSec; // how far past
  return now - o1 + iSec + offset;
}

unsigned long Scheduler::nextTask(unsigned long maxTime ) {
	unsigned long nextTask = timeFuture( maxTime ); // maximum time to next task
    unsigned long now = Time.now();
	for (int i=0; i < MAX_TASKS; i++) {
		Item *ti = &taskList[i];
		if ( ( ti->h != 0) and (ti->start > 0) ) {
			if ( ti->start < nextTask ) {
				nextTask = ti->start;
			}
		}
	}
	return abs( (long)(nextTask - now) ); // number of seconds until next task, may have past, or 0
}


void Scheduler::execute() {
	unsigned long now = Time.now();
	for (int i=0; i < MAX_TASKS; i++) {
		Item *ti = &taskList[i];
		if ( ( ti->h != 0) and (ti->start > 0) ) {
			if ( now > ti->start ) {
				ti->h(); // run the task
				ti->iterations++;
				ti->start = timeRoundUp( ti->interval, ti->offset); // reschedule

				// Handle fixed number of times
				if ( ti->type == FIXED ) {
					ti->n--;
					if ( ti->n < 1 ) {
						stop( ti->name ); // stop
					}
				}
			}
		} // if
	} // for
} // execute
