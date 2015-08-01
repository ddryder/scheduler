#include "scheduler.h"

Scheduler s1;


void blinkD7() {
    digitalWrite( D7, HIGH );
    delay(200);
    digitalWrite( D7, LOW );
    delay(200);
}


void setup() {
    pinMode(D7, OUTPUT);
    s1.create( "blinkd7", blinkD7, 10, 0, 10 );
    
}


void loop() {
    s1.execute();
}

