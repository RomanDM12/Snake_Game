#include <ncurses.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>
#include <time.h>

#define MAXWIDTH 140
#define MAXHEIGHT 45

int type() {
				//не блокирующий ввод
	struct timeval tv;
	fd_set read_fd;

  	tv.tv_sec=0;
  	tv.tv_usec=0;
  	FD_ZERO(&read_fd);
  	FD_SET(0,&read_fd);

  	if(select(1, &read_fd, NULL, NULL, &tv) == -1) {
    		return 0;
	}
  	if(FD_ISSET(0,&read_fd)) {
   		return 1;
	}
	return 0;
}


int wall(int *snakeArray, int snakeLength) {
	int normx = *snakeArray;
 	int normy = *(snakeArray+1);

  	if (normx < 0 || normx > MAXWIDTH) {
    		return 1;
  	}
  	else if (normy < 0 || normy > MAXHEIGHT) {
    		return 1;
  	}

  	for (int i = 2; i < snakeLength*2+2; i+=2) {
    		if (normx == *(snakeArray+i) && normy == *(snakeArray+i+1)) {
      			return 1;
    		}
  	}
  	return 0;
}

void moveSnake(int *snakeArray, int x, int y, int speed, int snakeLength) {
  	int oldX = 0;
  	int oldY = 0;
  	int nowX = *snakeArray;
  	int nowY = *(snakeArray+1);
  	int newX = nowX + x;
  	int newY = nowY + y;

  	*(snakeArray) = newX;
  	*(snakeArray+1) = newY;
  	mvprintw(newY, newX, "S");
  	for (int i = 2; i < snakeLength*2; i += 2) {
    		oldX = nowX;
    		oldY = nowY;
    		nowX = *(snakeArray+i);
    		nowY = *(snakeArray+i+1);
    		newX = oldX;
    		newY = oldY;
    		*(snakeArray+i) = newX;
    		*(snakeArray+i+1) = newY;
   		mvprintw(newY, newX, "*");
  	}

  	if (y != 0) {
    		usleep(500000/speed);
  	}

  	usleep(1000000/speed);
}

void appleSpawn(int* appleX, int* appleY, int *flagAppleEat) {
  	if (*flagAppleEat) {
    		srand(time(0));
    		*appleX = (rand() % MAXWIDTH)+1;
    		*appleY = (rand() % MAXHEIGHT)+1;
    		*flagAppleEat = 0;
  	}
	mvprintw(*appleY, *appleX,"@");
}

void eatApple(int *snakeArray, int appleX, int appleY, int *flagAppleEat, int *snakeLength) {
  	int x = *snakeArray;
  	int y = *(snakeArray+1);
  	if (x == appleX && y == appleY) {
    		*flagAppleEat = 1;
    		*snakeLength += 1;
    		int snakeLen = *snakeLength;
    		int lastX = *(snakeArray + snakeLen*2-2);
    		int lastY = *(snakeArray + snakeLen*2-2+1);
    		*(snakeArray + snakeLen*2) = lastX;
    		*(snakeArray + snakeLen*2+1) = lastY;
  	}

  	mvprintw(0,0,"snakeLength: %d", *snakeLength);
}

int main() {
  	int snakeArray[100][2];
  	for (int i = 0; i < 100; i++) {
    		for (int j = 0; j < 2; j++) {
      			snakeArray[i][j] = 0;
    		}
 	}

  	snakeArray[0][0] = 3;
  	snakeArray[0][1] = 3;

 	int ch = 0;
  	int x = 1;
 	int y = 0;
  	int speed = 25; 	//можно поменять скорость
  	int appleX = 0;
  	int appleY = 0;
  	int flagAppleEat = 1;
  	int snakeLength = 10;

  	initscr();
  	curs_set(0);
  	noecho();

  	while (!wall(&snakeArray[0][0], snakeLength)) {
		erase();

    		appleSpawn(&appleX, &appleY, &flagAppleEat);
    		moveSnake(&snakeArray[0][0], x, y, speed, snakeLength);
    		eatApple(&snakeArray[0][0], appleX, appleY, &flagAppleEat, &snakeLength);


    		refresh();
		if (type()) {
    			ch = getch();

     			if (ch == 'w') {
        			y = -1;
        			x = 0;
      			}
      			if (ch == 's') {
        			y = 1;
        			x = 0;
      			}
      			if (ch == 'a') {
        			y = 0;
        			x = -1;
      			}
      			if (ch == 'd') {
        			y = 0;
        			x = 1;
      			}
		}
}

  	erase();
	mvprintw(0, 0,"Game Over! - Your score: %d", snakeLength);
  	refresh();
  	getch();
  	endwin();

  	return 0;
}
