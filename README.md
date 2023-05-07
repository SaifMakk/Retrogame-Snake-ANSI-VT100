# Snake Game

This repository contains a simple implementation of the classic Snake game in C, designed to be played on an AVR microcontroller.

## Table of Contents
- [Introduction](#introduction)
- [Instructions](#instructions)
  - [Requirements](#requirements)
  - [Installation](#installation)
  - [Playing the Game](#playing-the-game)
- [Implementation Details](#implementation-details)
  - [Application Logic](#application-logic)
  - [Low-Level Hardware Interaction](#low-level-hardware-interaction)

## Introduction

Snake is a classic arcade game where the player controls a snake that grows as it eats food. The goal is to eat as much food as possible without running into the walls or the snake's own tail. In this implementation, the game is played on an AVR microcontroller connected to a Serial display and a joystick.

## Instructions

### Requirements

In order to play the game, you will need:

1. An AVR microcontroller ATmega328
   - *Notice*: you can use an Arduino Uno.
2. Serial monitor
   - *Notice*: This implementation uses Ansi Escape Sequences. To run this game, you need to use a serial monitor that is compatible with all escape sequence commands. I recommend using Terra term.
3. A joystick
4. Visual Studio Code IDE with PlatformIO extension

### Installation

1. Install [Visual Studio Code](https://code.visualstudio.com/) and the [PlatformIO extension](https://platformio.org/platformio-ide).
2. Connect the joystick pins to the AVR board as follows: Vcc to 5V, JoyX to PC0, JoyY to PC1, and Gnd to Gnd.
3. Clone this repository and open it in Visual Studio Code.
4. Configure the PlatformIO project for your AVR microcontroller.
5. Upload the main.cpp file to your AVR microcontroller using the PlatformIO extension.

### Playing the Game

Use the joystick to move the snake around the grid. The game ends if the snake runs into the walls or its own tail. The score is displayed at the bottom of the screen.

## Implementation Details

The game logic is contained in main.cpp, which is divided into two parts: application logic and low-level hardware interaction.

### Application Logic

The application logic for the game is contained in HeaderDatei.h. This file provides several functions to initialize the USART interface, read the analog value from a pin using the ADC, and send strings over the USART interface. It also defines an enumeration for the parity setting of the USART interface.

main.cpp contains the game logic itself, including functions to initialize the game, update the screen, read input from the joystick, and check for collisions.

### Low-Level Hardware Interaction

main.cpp interacts directly with the AVR microcontroller's hardware to control the Serial Monitor and read input from the joystick. It uses the avr/io.h, util/delay.h and header files to access the necessary hardware registers.













