/* 
--- Master Board ---

Please note that mentions to Serial in the master board code have been commented out and are only meant to be used for any debugging necessary

*/


//variables

//general variables
int lives = 3;
int lastGame = 0;
int gameSelect;
int clock = 30000;

//simon says variables                                       
    //Pin names
int ss_greenLed = 2;
int ss_redLed = 3;
int ss_blueLed = 4;
int ss_yellowLed = 5;

int ss_greenButton = 6;
int ss_redButton = 7;
int ss_blueButton = 8;
int ss_yellowButton = 9;
    //Game Variables
int solution [5];
int num_rounds = 7;
bool gameInProgress = false;

//motor game
#include <Stepper.h>
int val;
int encoderPos = 0;
int encoderPinALast = 0;
int n = 0;
int target;
int mvmt;
Stepper myStepper(512, A0, A1, A2, A3);//using analog ins as digital pins due to lack of pin space
//buzzer game variables

int bg_button = 0;
int buzzer = 1;
int countdown = 2;
int start_time = 5;
int finish_time = 15;


void setup()
{
  //general purpose and communication
  //Serial.begin(9600); 
  pinMode(11, INPUT);
  pinMode(12, OUTPUT);
  randomSeed(analogRead(A0));

  //Simon says setup
  pinMode(ss_greenLed, OUTPUT);
  pinMode(ss_redLed, OUTPUT);
  pinMode(ss_blueLed, OUTPUT);
  pinMode(ss_yellowLed, OUTPUT);

  pinMode(ss_greenButton, INPUT_PULLUP);
  pinMode(ss_redButton, INPUT_PULLUP);
  pinMode(ss_blueButton, INPUT_PULLUP);
  pinMode(ss_yellowButton, INPUT_PULLUP);

  //motor game setup
  pinMode (A4, INPUT);
  pinMode (A5, INPUT);
  //Serial.begin (9600);
  myStepper.setSpeed(60);

  //buzzer game setup
  pinMode(buzzer, OUTPUT); //pins 0 and 1 can be used by giving up Serial monitor capabilities
  pinMode(bg_button, INPUT_PULLUP);  

}

void loop()
{
  delay(100);
  if(millis()%8 == 1) {buzzerGame();} // buzzer game occurs ~ once every 8 games randomly
  gameSelect = random(1,4);
  while(gameSelect == lastGame) {gameSelect = random(1,4);} //prevents consecutive game repeats
  if (gameSelect == 1){motorGame();}
  else if (gameSelect == 2){mazeGame();}
  else {simonSaysGame();}
  lastGame = gameSelect;
}

void motorGame(){
  gameInProgress = true;
  unsigned long startTime = millis();
  //random direction and target chosen 
  if(random(0,2)) {
     target = encoderPos + random(50,100);
     mvmt = 1;
    }
  else{
    target = encoderPos - random(20,70);
    mvmt = -1;
    }

  //game loop
  while(gameInProgress) {
  //this code reads the rotary encoder to see in which direction its being turned in
  n = digitalRead(A4);
  if ((encoderPinALast == LOW) && (n == HIGH)) { //check if there is a difference
    if (digitalRead(A5) == LOW) { //check direction
      encoderPos--;
    } else {
      encoderPos++;
    }
    /*Serial.println (encoderPos);
    Serial.print ("Target: ");
    Serial.println (target); */
  }
  //checks if rotary position is close enough to target
  if(encoderPos-target < 10 && encoderPos-target > -10){gameInProgress = false;} //stops game if target is reached
  encoderPinALast = n;
  if(millis()%4 == 1){myStepper.step(mvmt);} //movement of the stepper motor is slow (performance wise) so only needs to be sent once in a while to keep up performance while still functioning well
  if(millis()-startTime>30000) {gameInProgress = false; lives--; 
if(lives == 0) {lives = 3; delay(10000);}} //failure mechanism
  }
}

void simonSaysGame(){
  //Serial.println("initializing Simon Says");

  //Starts the game
  gameInProgress = true;
  delay(300);
    
  //Green is 6, red is 5, blue is 4, yellow is 3
  
  //Number of colors to match
  for(int i = 0; i<num_rounds; i++) {
    //Random number from 3 to 6
    randomSeed(analogRead(0));
    int num = random(3,7);
    //Selected number is used to light a pin
    digitalWrite(num, HIGH);
    delay(1000);
    digitalWrite(num, LOW);
    delay(400);
    //Adds that pin to the solution array
    solution[i] = num;
  }
   
    //Whole sequence for every color  
      for(int round = 0; round < num_rounds; round++) {
        if (gameInProgress == false) return;
    //Waits for input
    while(digitalRead(ss_greenButton) == LOW && digitalRead(ss_redButton) == LOW && digitalRead(ss_blueButton) == LOW && digitalRead(ss_yellowButton) == LOW) { 
    //Do Nothing
      delay(10);
    }
    for(int x = 6; x<9 ;x++) {
      if((digitalRead(x) == HIGH) && (x == (solution[round] + 6))) {
        //Serial.println("SUCCESS for button: ");
        //Serial.println(x);
        digitalWrite(solution[round], HIGH);
        delay(200);
        digitalWrite(solution[round], LOW);
        //Serial.println(round);
        
        //Player wins!
        if(round == (num_rounds-1))
          gameInProgress = false;
        
      } else if ((digitalRead(x) == HIGH) && (x != (solution[round] + 6))) {
        //Serial.println("FAILURE for button: ");
        lives--;
        if(lives == 0) {lives = 3; delay(10000);}
        //Serial.println(x);
        
        //Player has failed
        //Stops the game
        gameInProgress = false;
      }
    }
        //Delay makes the button press not break
        delay(200);
      }
	
}

void mazeGame() {
	
  //Serial.println("sending maze");
  digitalWrite(12, HIGH);
  delay(1000);
  digitalWrite(12, LOW);
  while(digitalRead(11) == HIGH){
  }
  checkResult();
}

void buzzerGame() {
  //Randomly activates buzzer from start to finish time
  randomSeed(analogRead(0));
  int buzzer_time = random(start_time,finish_time);
  delay(buzzer_time * 1000);
  digitalWrite(buzzer, HIGH);

  //Starts countdown
  unsigned long StartTime = millis();
  while(digitalRead(bg_button) == LOW) {}
  unsigned long CurrentTime = millis();
  unsigned long ElapsedTime = CurrentTime - StartTime;
  if(ElapsedTime <= (countdown*1000)) {
    digitalWrite(buzzer, LOW); // we move on to next game on a success
      //Serial.println("success");
    //Serial.println(ElapsedTime);
    //Serial.println(countdown1000);
    } 
  
  else {//Serial.println("FAIL");
    lives--;
    digitalWrite(buzzer, LOW); 
    if(lives == 0) {lives = 3; delay(10000);} // game over signal is 10s of everything off
  }
}


void checkResult() {
  if(digitalRead(10) == HIGH) {lives--; } //Serial.println("life lost");}
  if(lives == 0) {lives = 3; delay(10000);} // game over signal is 10s of everything off
}
