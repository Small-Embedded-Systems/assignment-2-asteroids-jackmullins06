/* Asteroids View */

/* support double buffering */
void init_DBuffer(void);
void swap_DBuffer(void);

/* Add draw functions */
void draw(void);
void drawEndScreen(void);
void fireMissile(void);
void drawMissiles(struct missile *list);
void drawAsteroids(struct asteroid *list);