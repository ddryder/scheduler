#include "scheduler.h"
//#include "scheduler/scheduler.h"

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
    s1.stop("d7on");
    s1.stop("d7off");
    ledD7Off();
}

void stopBlink() {
    Spark.publish( "stopBlink" );
	s1.stop( "blink");
}

void startBlink() {
    Spark.publish( "startBlink" );
	s1.start( "blink");
}

void ledMain() {
    RGB.control( true );
    RGB.brightness(64);
    RGB.color(255, 0, 0);
    delay(500);
    RGB.color(0, 255, 0);
    delay(500);
    RGB.color(0, 0, 255);
    delay(500);
    RGB.control(false);
}

void setup() {
    pinMode(D7, OUTPUT);
    s1.task( "blink",  blinkD7, 10, 0, 2 );
    s1.task( "ledMain", ledMain, 20, 0, 20 );
    s1.task( "stopBlink", stopBlink, 60);
    s1.task( "startBlink", startBlink, 120);
}


void loop() {
    s1.execute();
}
