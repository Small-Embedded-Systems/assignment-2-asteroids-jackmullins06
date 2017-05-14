/* Controller */

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* hardware platform libraries */
#include <mbed.h>

/* asteroids */
#include "model.h"
#include "asteroids.h"

/* Joystick 5-way switch
*/
enum position { left,down,right,up,centre };
DigitalIn joystick[] = {P5_0, P5_1, P5_4, P5_2, P5_3};// LFT, RGHT, UP, DWN, CTR

bool joystickPress(enum position p) {
		return !joystick[p];
}

void joystickCentre(void) {
	if(!gameStart && joystickPress(centre)) {
		gameStart = true;
		resetGame();
	} else if(lives == 0 && joystickPress(centre)) {
		gameStart = false;
	}
		
	if(gameStart == true && lives > 0 && joystickPress(centre)) {
		fireMissile();
	}
}

void joystickUp(void)	{
	if(joystickPress(up) && player.speed <= 3.5) {
		player.speed = player.speed + 0.5;
	}
}


//Down button controls joystick and sets shield variable to 3.
void joystickDown(void) {
	if(joystickPress(down) && lives > 0 ) {
		player.shield = 3;
	}
}

//Right button rotates the ship.
void joystickRight(void) {
	if(joystickPress(right)) {
		player.shipTrajectory = player.shipTrajectory + 15;
	}
}
	
//Left button rotates the ship.
void joystickLeft(void) {
	if(joystickPress(left)) {
		player.shipTrajectory = player.shipTrajectory - 15;
	}
}

void controls(void){
	joystickCentre();
	joystickUp();
	joystickDown();
	joystickRight();
	joystickLeft();
}
