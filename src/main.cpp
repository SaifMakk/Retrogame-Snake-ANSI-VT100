/**
 * \file			main.cpp
 * \brief			application-logic-header
 *
 * This file provides the application-logic for the program.
 *
 * \author			Seifeddine Makhlouf
 * \date			2023
 * \version			1.0
 */




#include "HeaderDatei.h"
#include<stdio.h> 
#include <stdlib.h>



/**
* \brief		 
*  move the cursor on the screen using Ansi Escape Sequence .
*/
#define movecursor(buffer, format, ...) \
    snprintf(buffer, sizeof(buffer), format, ##__VA_ARGS__);  

/**
* \brief		 
*  input pins of the joystick.
*/
#define joyX PC0 
#define joyY PC1

void gameOver(void) ;
void placeFood(void) ;
void drawsnake(void);
void update(void);
void readInput(void);
void init(void);
uint16_t analogRead(int);
void drawfoodandscore();
void updatescreen();
void initUsart(uint32_t baudrate, uint8_t databits, Parity parity, uint8_t stopbits);
/**
* \brief		 
*  function to send escape sequences to draw the grid, the score, and save the screen.
*/
void usart_putstring (char *s); 

/**
* \brief		 
*  //the dimensions of the grid
*/
const int16_t ROWS = 20;
const int16_t COLS = 20;
int16_t **snake;
int16_t snake_length = 1;
int16_t food_x, food_y,lastfoodposx,lastfoodposy;
int16_t direction = 1;
/**
* \brief		 
*  buffer to store formatted strings
*/
char buffer[30];

 int main(void)
{
 /**
* \brief		 
*  initialize the USART interface 
*\param 9600 Baudrate
*\param 8 Data bits
*\param none Parity
*\param 1 Stopbit
*/
 initUsart (9600, 8, none, 1);  
/**
* \brief		 
*  make the cursor  invisible 
*/
usart_putstring("\033[?25l"); 

/**
* \brief		 
*  Clear bit 0 of DDRC to set joyx as input
* make the cursor  invisible
* Clear 1 of DDRC to set joyy as input 
*/
DDRC &= ~(1 << PC0);  
    DDRC &= ~(1 << joyY);  
/**
* \brief		 
*initializes the game state by allocating memory for the snake 
*and placing it at the center of the screen.
*/
init(); 
/**
* \brief		 
*  draw the initialisierte field 
*/
 usart_putstring("\033[48;5;225m"); // set the field color
for (int i = 0; i < 21; i++)
  {for (int j = 0; j < 21; j++)
   {movecursor(buffer, "\033[%d;%dH ",i,j);
   usart_putstring(buffer);}
    }


/**
* \brief		 
* draw the score
*/
usart_putstring("\033[48;5;0m"); //Black background
 usart_putstring("\033[38;5;97m");// score foreground
 movecursor(buffer, "\033[%d;%dHScore:", ROWS+1, 2 );
  usart_putstring(buffer);

/**
* \brief		 
* save screen 
*/
usart_putstring("\033[?47h");  
     
   /**
* \brief		 
* main loop
*/ 
while (1){         
_delay_ms(300);// wait for 300 sec
  readInput(); 
  update();
  drawsnake();
}}
/**
* \fn init()
* \brief		 
* allocate memory for the snake
* initialize the snake at the center of the screen
* place the food randomly on the screen 
*/
void init() {
  // allocate memory for the snake
  snake = (int16_t**) malloc(ROWS * sizeof(int16_t*));
  for (int i = 0; i < ROWS; i++) {
    snake[i] = (int16_t*) malloc(COLS * sizeof(int16_t));
  }

  // initialize the snake at the center of the screen
  snake[0][0] = ROWS/2;
  snake[0][1] = COLS/2;

  // place the food randomly on the screen
  placeFood();
}

/**
*  readInput()
* \brief		 
* reads the input from the joystick and sets the direction of movement based on the input.
* Read the X value
* Determine the direction based on the joystick inputs 
*/
void readInput() {
  // Read the X value
 int16_t xValue = analogRead(joyX);
int16_t yValue =analogRead(joyY);
  // Determine the direction based on the joystick inputs
  if (xValue < 400 && direction != 2 && direction != 0) {
    direction = 0;
  } else if (xValue > 600 && direction != 0 && direction != 2) {
    direction = 2;
  } else if (yValue < 400 && direction != 3 && direction != 1) {
    direction = 1;
  } else if (yValue > 600 && direction != 1 && direction != 3) {
    direction = 3;
  }
}

/**
* *\fn		 updatescreen()
* \brief		 
* This function is responsible for updating the screen after every game loop iteration.
*/
 void updatescreen(){
  usart_putstring("\033[?47l"); //  &restore
 drawfoodandscore();
 usart_putstring("\033[?47h");

}

/**
*\fn		update()
* \brief		 
* This function updates the game state by moving the snake,
*checking for collision with the food or the walls,
*and updating the score.
*/
void update() {
  // move the snake
  for (int i = snake_length-1; i > 0; i--) {
    snake[i][0] = snake[i-1][0];
    snake[i][1] = snake[i-1][1];
  }

  if (direction == 0) {
    snake[0][0]--;
  } else if (direction == 1) {
    snake[0][1]++;
  } else if (direction == 2) {
    snake[0][0]++;
  } else if (direction == 3) {
    snake[0][1]--;
  }

  // check for collision with food
  if (snake[0][0] == food_x && snake[0][1] == food_y) {
    snake_length++;
    placeFood();
    
  }

  // check for collision with walls or own tail
  if (snake[0][0] < 0 || snake[0][0] >= ROWS ||
      snake[0][1] < 0 || snake[0][1] >= COLS) {
    gameOver();
  } else {
    for (int i = 1; i < snake_length; i++) {
      if (snake[0][0] == snake[i][0] && snake[0][1] == snake[i][1]) {
        gameOver();
        break;
      }
    }
  }
}

 // 



/**
*\fn		drawsnake()
* \brief		 
* draw the snake,
*/
void drawsnake() {
updatescreen();
 // draw the snake
usart_putstring("\033[48;5;197m"); // snake color
for (int i = 0; i < snake_length; i++) {
movecursor(buffer, "\033[%d;%dH ", snake[i][0]+1, snake[i][1]+1);
usart_putstring(buffer);
}

}

/**
*\fn		drawfoodandscore()
* \brief		 
*  draw the food and the score ,
*/
void drawfoodandscore() {
// draw the food
usart_putstring("\033[48;5;20m");// foodcolor
movecursor(buffer, "\033[%d;%dH ", food_x+1, food_y+1);
usart_putstring(buffer);
// delete the last food
usart_putstring("\033[48;5;225m");// foodcolor
movecursor(buffer, "\033[%d;%dH ", lastfoodposx+1, lastfoodposy+1);
usart_putstring(buffer);

// draw the score
usart_putstring("\033[48;5;0m"); // black background
usart_putstring("\033[38;5;97m"); // score color
movecursor(buffer, "\033[%d;%dH%d", ROWS+1, 9,snake_length-1);
usart_putstring(buffer);

}
/**
*\fn		placefood()
* \brief		 
*save the last place  of the food to delete it 
*generate random coordinates for the new food
*check if the food is not placed on the snake 
*/

void placeFood() {
//save the last place  of the food to delete it 
lastfoodposx=food_x;
lastfoodposy=food_y;
// generate random coordinates for the food
food_x = rand()%ROWS; 
food_y = rand()%COLS;
 

// check if the food is not placed on the snake
for (int i = 0; i < snake_length; i++) {
if (snake[i][0] == food_x && snake[i][1] == food_y) {
placeFood();
break;
}
}
}


/**
*\fn		gameover()
* \brief		 
* print the game over message
*/
void gameOver() {
  // print the game over message
  movecursor(buffer, "\033[%d;%dHGame Over!", ROWS/2, COLS/2-5);
  usart_putstring(buffer);

  // wait for button 2 to be pressed
  _delay_ms(5000);
  

  // reset the snake and food
  snake_length = 1;
  snake[0][0] = ROWS/2;
  snake[0][1] = COLS/2;
  placeFood();
}