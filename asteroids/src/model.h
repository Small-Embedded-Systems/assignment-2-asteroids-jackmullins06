/* Asteroids Model */
struct point {
    float x,y;
};

typedef struct point coordinate_t;
typedef struct point vector_t;

/* Some insitial struct types if you want to usd them */
struct ship {
	float x, y;
	float leftX, leftY;
	float rightX, rightY;
	float pointX, pointY;
	float headingTrajectory, shipTrajectory;
	float speed;
	int shield;
}; 

/* initial struts for building linked lists */
typedef struct asteroid {
	float x, y;
	float dx, dy;
	float ttl;
	struct asteroid *next;
} asteroid_t;

typedef struct missile {
    float x, y;
	float dx, dy;
	float list;
	float ttl;
	struct missile *next;
} missile_t;

/* FUNCTIONS */
void physics(void);
void asteroidFreeNode(asteroid_t *i);
void missileFreeNode(missile_t *i);
void intialiseAsteroidHeap(void);
void intialiseMissileHeap(void);
void fireMissile(void);
void spawnAsteroid(void);

/* NODES */
asteroid_t *allocateNodeAsteroid(void);
missile_t *allocateNodeMissile(void);

/* EXTERNAL VARIABLES */
extern struct missile *activeMissile;
extern struct asteroid *asteroidActive;
extern const int asteroidHeapsize;
extern const int missileHeapsize;

