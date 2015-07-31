#include "scheduler.h"

Schduler s1();
/*
*/




setup() {
    pinMode(D7, OUTPUT);
    s1.create( "blinkd7", blinkD7, 10, 0, 10 );
    
}


loop() {
    s1.execute();
}

void blinkD7() {
    digitalWrite( D7, HIGH );
    delay(200);
    digitalWrite( D7, LOW );
    delay(200);
}
