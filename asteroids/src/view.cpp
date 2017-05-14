/* Asteroids view
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

#include "asteroids.h"
#include "model.h"
#include "utils.h"

Display *graphics = Display::theDisplay();

const colour_t background = rgb(0,51,102); /* Midnight Blue */
const colour_t shieldColour = rgb(102, 178, 255);


const coordinate_t shape[] = {
	{10,0}, {-5,5}, {-5,-5}
};

int tickCt = 0;

/* double buffering functions */
void init_DBuffer(void) {   
	uint16_t *bufferbase = graphics->getFb();
  	uint16_t *nextbuffer = bufferbase+ (480*272);
  	LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}

void swap_DBuffer(void) {   
	uint16_t *buffer = graphics->getFb();
  	graphics->setFb( (uint16_t*) LPC_LCD->UPBASE);
  	LPC_LCD->UPBASE = (uint32_t)buffer;
}

void drawStartScreen(void) {
	graphics->setCursor(50, 50);
	graphics->setTextSize(2);
	graphics->printf("Press the center button to start.");
}

void drawArena(void) {
	graphics->fillScreen(background);
	graphics->drawRect(0, 0, 480, 270, WHITE);
	graphics->drawRect(0, 0, 480, 20, WHITE);
	
	graphics->setCursor(325, 5);
	graphics->printf("Asteroids by Jack Mullins");
}

void drawLives(void) {
	graphics->setCursor(100, 5);
	graphics->printf("Lives: %d", lives);
}

void drawPlayer(void) {
	graphics->drawLine(player.x + player.leftX, player.y + player.leftY, player.x + player.pointX, player.y + player.pointY, WHITE);
	graphics->drawLine(player.x + player.leftX, player.y + player.leftY, player.x + player.rightX, player.y + player.rightY, WHITE);
	graphics->drawLine(player.x + player.rightX, player.y + player.rightY, player.x + player.pointX, player.y + player.pointY, WHITE);
}

void drawShield(void) {
	if(player.shield == 3) {
		graphics->drawCircle(player.x, player.y, 15, shieldColour);
	  	graphics->drawCircle(player.x, player.y, 17, shieldColour);
		graphics->drawCircle(player.x, player.y, 19, shieldColour);
	} else if(player.shield == 2) {
		graphics->drawCircle(player.x, player.y, 15, shieldColour);
	  	graphics->drawCircle(player.x, player.y, 17, shieldColour);
	} else if(player.shield == 1) {
		graphics->drawCircle(player.x, player.y, 15, shieldColour);
	}
}

void drawMissiles(struct missile *list) {
	while(list) {
		graphics->fillCircle(list->x, list->y, 1, RED);
		list = list->next;
	}
}

void drawAsteroids(struct asteroid *list) {
	while(list) {
		//graphics->fillCircle(list->x, list->y, 15, WHITE);
		graphics->drawCircle(list->x, list->y, 15, WHITE);
		list = list->next;
	}
}

void drawScore(void) {
	score++;
	int screenScore = score / 32;
		
	graphics->setCursor(5, 5);
	graphics->printf("Score: %d", screenScore);
}

void drawEndScreen(void) {
	int screenScore = score / 32;
	
	graphics->setCursor(230, 160);
	graphics->setTextSize(3);
	graphics->printf("Score: %d", screenScore);
	
}

void draw(void) {
	if(gameStart == false) {
		drawStartScreen();
	} else if(gameStart == true) {
		drawArena();
		drawScore();
		drawLives();
		drawPlayer();
		
		drawAsteroids(asteroidActive);
  		drawMissiles(activeMissile);
	} else if(lives == 0) {
		drawEndScreen();
	}
	
  	swap_DBuffer();
}
