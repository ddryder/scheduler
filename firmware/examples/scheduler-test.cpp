#include "scheduler/scheduler.h"

Scheduler s1;


void blinkD7() {
    digitalWrite( D7, HIGH );
    delay(200);
    digitalWrite( D7, LOW );
    delay(200);
}

void ledD7On() {
    digitalWrite( D7, HIGH );
}

void ledD7Off() {
    digitalWrite( D7, LOW );
}

void ledD7Stop() {
    Spark.publish( "stopd7" );
    s1.remove("d7on");
    s1.remove("d7off");
    ledD7Off();
}

void ledMain() {
    RGB.control( true );
    RGB.brightness(64);
    RGB.color(255, 0, 0);
    delay(1000);
    RGB.color(0, 255, 0);
    delay(1000);
    RGB.color(0, 0, 255);
    delay(1000);
    RGB.control(false);
}

void setup() {
    pinMode(D7, OUTPUT);
    s1.task( "d7on",  ledD7On, 10, 0, 30 );
    s1.task( "d7off", ledD7Off, 10, 10, 30 );
    s1.task( "ledMain", ledMain, 20, 0, 20 );
    s1.task( "d7Stop", ledD7Stop, 30);
}


void loop() {
    s1.execute();
}
