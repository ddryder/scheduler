#include "scheduler.h"
#include "application.h"
// wed 

Scheduler::Scheduler() {
    _nextTask = 0;
    _maxSize = SCHEDULER_MAX_TASKS; // Fixed number of tasks
    _now = Time.now();
    //taskList = new Item[ _maxSize ];
    reset();
}

Scheduler::~Scheduler() {
	// TODO Auto-generated destructor stub
}

void Scheduler::reset() {
	for (int i=0; i < _maxSize; i++) {
		taskList[i].h = 0;
		taskList[i].start = 0;
	}
}

void Scheduler::print() {
	for (int i=0; i < _maxSize; i++) {
		Item ti = (Item)taskList[i];
		if ( ti.h != 0) {
			//cout << ti.name<< " " << ti.start << endl;
		}
	}
}

int Scheduler::find(char * name ) {
	int i = _maxSize;
	for (i=0; i < _maxSize; i++) {
		if ( strcmp( taskList[i].name, name) == 0 ) {
			break; // matched existing task
		}
	}
	if (i >= _maxSize) { // not found
		for (i=0; i < _maxSize; i++) {
			if ( taskList[i].h == 0 ) {
				break; // available for new task
			}
		}
	}
	return i;
}


void Scheduler::task( char *  name, Handler h, int startSecs ) {
	// run once in number of seconds
	this->createTask( name, h, FIXED, timeFuture( startSecs ), 0, 9999, 1 );
}

void Scheduler::task( char *  name, Handler h, int start, int offset, int interval, int n ) {
	// repeat fixed number of times
	this->createTask( name, h, FIXED, timeFuture( start ), offset, interval, n );
}

void Scheduler::task( char *  name, Handler h, int start, int offset, int interval) {
	// run idefintely, first at start time and subsequently at set interval
	this->createTask( name, h, REPEAT, timeFuture( start ), offset, interval, 0 );
}

void Scheduler::createTask( char * name, Handler h, TaskType type, int start, int offset, int interval, int n ) {
	// create new or modify existing
	int i = find( name );
	if (i < _maxSize) {
		Item *ti = &taskList[i];
		ti->name = name;
		ti->h = h; // handler
		ti->type = type;
		ti->start = start; // number of seconds to start
		ti->offset = offset;
		ti->interval = interval; // every xx seconds
		ti->n = n; // repeat task n times
		ti->iterations = 0;
	} else {
		// out of space
	}
}

void Scheduler::start( char * name ) {
	// start all that match
	for (int i=0; i < _maxSize; i++) {
		Item *ti = &taskList[i];
		if ( strcmp( ti->name, name) == 0 ) {
			ti->start = timeRoundUp( ti->interval, ti->offset); // reschedule
		}
	}
}

void Scheduler::stop( char * name ) {
	// stop all that match
	for (int i=0; i < _maxSize; i++) {
		Item *ti = &taskList[i];
		if ( strcmp( ti->name, name) == 0 ) {
			ti->start = 0;
		}
	}
}

void Scheduler::remove( char * name ) {
	// remove all that match
	for (int i=0; i < _maxSize; i++) {
		if ( strcmp( taskList[i].name, name) == 0 ) {
			taskList[i].h = 0;
			taskList[i].start = 0;
			taskList[i].name = '\0';
		}
	}
}

int Scheduler::timeFuture(int iSec) {
	return  Time.now() + iSec;
}

int Scheduler::timeRoundUp(int iSec, int offset) {
  // Round now up / future to next iSec period exactly - add offset to adjust
  int now = Time.now();
  int o1 = now % iSec; // how far past
  return now - o1 + iSec + offset;
}



void Scheduler::execute() {
	_now = Time.now();
	Item *ti = &taskList[ _nextTask ];
	if ( ( ti->h != 0) and (ti->start > 0) ) {
		if (_now > ti->start ) {
			ti->h(); // run the task
			ti->iterations++;
			ti->start = timeRoundUp( ti->interval, ti->offset); // reschedule
			//ti->start = _now + (_now % ti->interval) + ti->interval + ti->offset;

			// Handle fixed number of times
			if ( ti->type == FIXED ) {
				ti->n--;
				if ( ti->n < 1 ) {
					stop( ti->name ); // stop
				}
			}
		}
	} // if
	_nextTask = (_nextTask + 1) % _maxSize;
} // execute


int Scheduler::nextTask(int maxTime ) {
	int nextTask = timeFuture( maxTime ); // maximum time to next task
    int now = Time.now();
	for (int i=0; i < _maxSize; i++) {
		Item *ti = &taskList[i];
		if ( ( ti->h != 0) and (ti->start > 0) ) {
			if ( ti->start < nextTask ) {
				nextTask = ti->start;
			}
		}
	}
	return abs( (long)(nextTask - now) ); // number of seconds until next task, may have past, or 0
}
