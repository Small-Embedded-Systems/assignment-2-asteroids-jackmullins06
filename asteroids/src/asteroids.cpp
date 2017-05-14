/* Asteroids
    Sample solution for assignment
    Semester 2 -- Small Embedded Systems
    Dr Alun Moon
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

/* Main game elements */
#include "model.h"
#include "view.h"
#include "controller.h"

/* Game state */
float elapsed_time; 
int   score;
int   lives;
struct ship player;

float Dt = 0.01f;

bool gameStart = false;
bool inPlay = false;

Ticker model, view, controller, rocks;

void timerHandler();
bool paused = true;

/* The single user button needs to have the PullUp resistor enabled */
DigitalIn userbutton(P2_10,PullUp);

/* Set game variables back to default */
void resetGame(void) {
	lives = 5;
	player.shield = 3;
	score = 0;

	player.x = 230;
	player.y = 120;
}

int main() {
	init_DBuffer();
	
	intialiseAsteroidHeap();
	intialiseMissileHeap();

    view.attach(draw, 0.025);
    model.attach(physics, Dt);
    controller.attach(controls, 0.1);
    rocks.attach(spawnAsteroid, 0.1);
    
  	lives = 5;
    
    while(userbutton.read()){ /* remember 1 is not pressed */
		paused=true;
    	wait_ms(100);
  	} paused = false;
    
	while(true) {
		if(gameStart == true && lives == 0) {
			view.detach();
			model.detach();
			controller.detach();
			rocks.detach();
			drawEndScreen();
			wait_ms(300);
		}
	}
}	