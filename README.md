# Arcade Games for Arduino
*Authors: Alexandre Hachey and Killian Hedou*

*Created in: Fall 2021*

This repository contains the code for a variety of arcade-style games that run on two Arduino boards. The master Arduino controls the overall game flow, while the slave Arduino manages the maze game.

## Completed Project


<img src="https://lh3.googleusercontent.com/drive-viewer/AFGJ81odTw_P_SjPzK72BSKt_A4bGL13nkM6WaaQfgcuXGv7D19jbBKBD5yJqjPukRkBLKLBzEm3DL_YXjzbn_vg1OXeuZJHhg=s1600" style="width:60%; margin:auto;"/>


## Games Included
1. Motor Game
2. Maze Game
3. Simon Says Game
4. Buzzer Game

## Hardware Requirements
- 2 Arduino boards
- Stepper motor
- Rotary encoder
- Buzzer
- LED lights (green, red, blue, yellow)
- Buttons (green, red, blue, yellow)
- 4x4 LED matrix (for the maze game)
- Analog joystick (for the maze game)
- Resistors and jumper wires

## Software Requirements
- [Arduino IDE](https://www.arduino.cc/en/software)
- Stepper motor library (included in the Arduino IDE)

## Installation
1. Download and install the Arduino IDE from the official website.
2. Clone or download this repository.
3. Open the `Master.ino` file in the Arduino IDE and upload it to the master Arduino board.
4. Open the `Slave.ino` file in the Arduino IDE and upload it to the slave Arduino board.

## How to Play
1. Connect the master and slave Arduino boards.
2. Power on the Arduinos.
3. The games will start automatically, and the player will have three lives. A game is randomly selected after each round. If the player loses all three lives, the system will reset after a 10-second delay.
4. To play a specific game, modify the `gameSelect` variable in the `Master.ino` file.

## Game Descriptions
### Motor Game
In the Motor Game, a rotary encoder is used to control a stepper motor. The player has to reach a random target position within a specific time limit to win the game.

### Maze Game
The Maze Game is played on a 4x4 LED matrix controlled by the slave Arduino. Players must navigate the matrix using an analog joystick to reach specific points within a time limit.

### Simon Says Game
In the Simon Says Game, players must memorize and repeat a sequence of colored lights. Each round adds a new color to the sequence. Players must successfully repeat the entire sequence to win the game.

### Buzzer Game
The Buzzer Game requires the player to press a button within a specific time window after the buzzer is activated. If the player successfully presses the button within the time window, they win the game.
