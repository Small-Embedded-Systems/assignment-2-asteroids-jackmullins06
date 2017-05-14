/* Asteroids model */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"
#include "utils.h"
#include "asteroids.h"

static const int missileHeapsize = 3; //3 missiles at a time.
static const int asteroidHeapsize = 8; //3 asteroids on the screen at anyone time.

static missile_t missileHeap[missileHeapsize];
static asteroid_t asteroidHeap[asteroidHeapsize];
static missile_t *missileFreeNodes;
static asteroid_t *asteroidFreeNodes;

//static const float Dt = 0.01;

struct missile *activeMissile = NULL;
struct asteroid *asteroidActive = NULL;

void shipWrapAround(void) {
	if(player.x > 480) {
		player.x = 5;
	} else if (player.x < 0) {
		player.x = 475;
	} else if (player.y > 260) {
		player.y = 30;
	} else if (player.y < 30) {
		player.y = 255;
	}
}

void asteroidWrapAround(struct asteroid *a) {
	for(; a ; a = a->next) {
		if(a->x > 480) {
			a->x = 5;
		} else if(a->x < 0) {
			a->x = 475;
		} else if(a->y > 260) {
			a->y = 30;
		} else if(a->y < 30) {
			a->y = 255;
		}
	}
}

void intialiseMissileHeap(void) {
	int i;
	
	for(i = 0; i < (missileHeapsize-1); i++) {
		missileHeap[i].next = &missileHeap[i+1];
	}
	
	missileHeap[i].next = NULL;
	missileFreeNodes = &missileHeap[0];
}

missile_t *allocateMissileNode(void) {
	missile_t *missileNode = NULL;
	
	if(missileFreeNodes) {
		missileNode = missileFreeNodes;
		missileFreeNodes = missileFreeNodes->next;
	} return missileNode;
}

void missileOrigin(struct missile *ignited) {
	int missileSpeed = 250;
	
	ignited->x = player.x;
	ignited->y = player.y;
	ignited->dx = missileSpeed * (cos(player.headingTrajectory));
	ignited->dy = missileSpeed * (sin(player.headingTrajectory));
	ignited->ttl = 200;
}

void fireMissile(void) {
	struct missile *fire = allocateMissileNode();

	if(fire) {
		fire->next = activeMissile;
		activeMissile = fire;
		missileOrigin(fire);		
	}
}

/* Code for asteroid linked list */
void intialiseAsteroidHeap(void) {
	int i;
	
	for(i = 0; i < (asteroidHeapsize-1); i++) {
		asteroidHeap[i].next = &asteroidHeap[i+1];
	}
	asteroidHeap[i].next = NULL;
	asteroidFreeNodes = &asteroidHeap[0];
}

asteroid_t *allocatedAsteroidNode(void) {
		asteroid_t *asteroidNode = NULL;
		if(asteroidFreeNodes) {
				asteroidNode = asteroidFreeNodes;
				asteroidFreeNodes = asteroidFreeNodes->next;
		}
		return asteroidNode;
}

void asteroidOrigin(struct asteroid *ignited) {
		int asteroidSpeed = 10;
		int protectRadius = 50;
		
		ignited->x = randrange(20,460);
		ignited->y = randrange(20,240);
		
		if(ignited->x > (player.x - protectRadius) && ignited->x < (player.x + protectRadius) 
		   && ignited->y > (player.y - protectRadius) && ignited->y < (player.y + protectRadius)) {
			ignited->x = randrange(20,460);
			ignited->y = randrange(20,240);
		}		   
		   
		
		ignited->dx = randrange(-asteroidSpeed,asteroidSpeed);
		ignited->dy = randrange(-asteroidSpeed,asteroidSpeed);
		ignited->ttl = randrange(900,1100);
}

void missileAsteroidCollision(struct asteroid *a, struct missile *m) {
	for(; m ; m = m->next) {
		for(; a ; a = a->next) {
			if((m->x > (a->x - 20) && m->x < (a->x + 20)) && (m->y > (a->y - 20) && m->y < (a->y + 20))) {
				m->ttl = 0;
				a->ttl = 0;
			}
		}
	}
}

void shipAsteroidCollision(struct asteroid *a) {
	int shipRadius;
	
	if(player.shield == 3) {
		shipRadius = 37;
	} else if(player.shield == 2) {
		shipRadius = 35;
	} else if(player.shield == 1) {
		shipRadius = 33;
	} else {
		shipRadius = 30;
	}
		
	for(; a ; a = a->next) {
		if((a->x) > (player.x - shipRadius) && (a->x) < (player.x + shipRadius)) {
			if(((a->y) > (player.y - shipRadius) && (a->y) < (player.y + shipRadius))) {
				if(player.shield > 0) {
					player.shield = player.shield -1;
					
					player.x = 230;
					player.y = 120;
				} else {
					lives = lives -1;
					
					player.x = 230;
					player.y = 120;
				}
			}
		}
	}
}

void spawnAsteroid(void) {
	struct asteroid *spawn = allocatedAsteroidNode();
	
	if(spawn) {
		spawn->next = asteroidActive;
		asteroidActive = spawn;
		asteroidOrigin(spawn);
	}
}

void missileStatus(struct missile *p) {
	for( ; p; p = p->next) {
		p->x += p->dx * Dt;
		p->y += p->dy * Dt;
		
		if(p->x < 0 || 480 < p->x) {
			p->ttl = 0;
		}
		
		if(p->y < 10 || 260 < p->y) {
			p->ttl = 0;
		} p->ttl -=Dt;
				
		
		if(p->x < 0) {
			p->x = 480;
		}
		
		if(p->next->ttl<=0) {
			struct missile *destroyed = p->next;
			p->next = p->next->next;
			missileFreeNode(destroyed);
		}
	}
}

void asteroidStatus (struct asteroid *p) {
	for(; p; p = p->next) {
		p->x += p-> dx * Dt;
		p->y += p-> dy * Dt;
			
		if(p->next->ttl <= 0) {
			struct asteroid *destroyed = p->next;
			p->next = p->next->next;
			asteroidFreeNode(destroyed);
		}
	}
}

void setMovement(void) {
	player.headingTrajectory = radians(player.shipTrajectory);
	player.x = player.x + player.speed * (cos(player.headingTrajectory));
	player.y = player.y + player.speed * (sin(player.headingTrajectory));
	
	player.pointX = 15 * (cos(player.headingTrajectory));
	player.pointY = 15 * (sin(player.headingTrajectory));
		
	player.leftX = (-8 *(cos(player.headingTrajectory))) - (8 * (sin(player.headingTrajectory)));
	player.leftY = (-8 *(sin(player.headingTrajectory))) + (8 * (cos(player.headingTrajectory)));
	
	player.rightX = (-8 *(cos(player.headingTrajectory))) - (-8 * (sin(player.headingTrajectory)));
	player.rightY = (-8 *(sin(player.headingTrajectory))) + (-8 * (cos(player.headingTrajectory)));
}

void setSpeed(void) {
	if(player.speed > 0.01) {
		player.speed = player.speed - 0.01;
	}
}

void missileFreeNode(missile_t *i) {
	i->next = missileFreeNodes;
	missileFreeNodes = i;
}

void asteroidFreeNode(asteroid_t *i) {
	i->next = asteroidFreeNodes;
	asteroidFreeNodes = i;
}

void physics(void){
	shipWrapAround();
	asteroidWrapAround(asteroidActive);
	
	setMovement();
	setSpeed();
		
	missileAsteroidCollision(asteroidActive, activeMissile);
	shipAsteroidCollision(asteroidActive);
	
	asteroidStatus(asteroidActive);
	missileStatus(activeMissile);
}

