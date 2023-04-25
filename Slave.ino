// Slave Arduino \\

//declare variables

//communication variables
int com1 = 10; 
int com2 = 11;
int com3 = 12;


//Maze game variables
const int row[] = {2, 3, 4, 9};
const int col[] = {5, 6, 7, 8};
int curcol;
int currow;
int lastmove = 0;
const int level = 2; // needs to be done somewhere else
int curpos;
int blinker;
int curframe[4][4] = {  //default "screen" is set to all off
    {1,1,1,1},
    {1,1,1,1},
    {1,1,1,1},
    {1,1,1,1}        };
long startTime = 0;
bool gameStarted = false;
bool lost = false;

void setup()
{
    //general use & communication
    pinMode(com1, OUTPUT);
    pinMode(com2, INPUT);
  	pinMode(com3, OUTPUT);
    Serial.begin(9600);
	randomSeed(analogRead(A0));
    //for maze game
    for (int i=2;i<10;i++) {pinMode(i, OUTPUT);} 
    pinMode(A1, INPUT);
    pinMode(A2, INPUT); 
}

void loop()
{
  if(digitalRead(com2) == HIGH) {startGame();} //waits for signal to start maze game
}




void startGame() {
  digitalWrite(com3, HIGH); //signals to mother arduino that game is running
  Serial.println("playing game"); 
  lost = false;
  gameLoop(); //starts game loop
  delay(1);
  digitalWrite(com3, LOW); //signals to mother arduino that game is done
  Serial.println("Ending game");

}

void gameLoop() {
    
    //sets up points to light up and starting player position
    int randomdots[6];
  	curpos = random(1,17);
    for (int i=0;i<6;i++) {  
    randomdots[i] = random(1,17);
    Serial.println(randomdots[i]);
    curframe[(int)randomdots[i]/4][randomdots[i]%4 - 1] = 0;}
    gameStarted = true;
    startTime = millis();
    while(gameStarted) { 
        //check if time is up
        if(millis() - startTime > 30000) {digitalWrite(com1, HIGH);  digitalWrite(com3, LOW); delay(100); lost = true; digitalWrite(com1, LOW);Serial.println(startTime);Serial.println(millis());} //signals -1 life to master arduino and signals game end for slave board

  	    //check time since last movement
  	    if(millis() - lastmove > 500) {      
          	movement(); //executes movement
        	
  	    	int completion = 1; //checks if we won
  	    	for (int i=0;i<level;i++) {
        		if(randomdots[i] != 99) {
        		completion = 0;
        	}
  	    }
  	    if (completion == 1 || lost) {gameStarted = false; delay(1000); completion = 0; lost = false;}

        }
  	    //blink current position
  	    if(millis()- blinker > 250) {
          	int x = curframe[curpos/4][curpos%4 - 1];
          	curframe[curpos/4][curpos%4 - 1] = 1 - x;
          	blinker = millis();
        }
        //check if position is on an objective
        for (int i=0;i<level;i++) {
            //replace the random dot with arbitrary value if we hit it
            if(curpos == randomdots[i]) {randomdots[i] = 99;}

        }

        //draw frame
  	    drawFrame(curframe);        
    }
}

void movement(){
  //map x input
  int x = map(analogRead(A1), 0, 1023, -100, 100);
  //map y input
  int y = map(analogRead(A2), 0, 1023, -100, 100);
  
  if (x > 50 && curpos % 4 != 0) {
    //turns off previous position
    curframe[(int)curpos/4][curpos%4 - 1] = 1;
    
    //changes position
    curpos++;
    
    //turns on new position
    curframe[(int)curpos/4][curpos%4 - 1] = 0;
    
    //stores time in ms of last movement
    lastmove = millis();
    }
  
  if (x < -50 && curpos % 4 != 1) {
    //turns off previous position
    curframe[(int)curpos/4][curpos%4 - 1] = 1;
    //changes position
    curpos--;
    //turns on new position
    curframe[(int)curpos/4][curpos%4 - 1] = 0;
    //stores time in ms of last movement
    lastmove = millis();
  }
    
  if (y > 50 && curpos <13) {
    //turns off previous position
    curframe[(int)curpos/4][curpos%4 - 1] = 1;
    
    //changes position
    curpos = curpos +4;
    
    //turns on new position
    curframe[(int)curpos/4][curpos%4 - 1] = 0;  
    
    //stores time in ms of last movement
    lastmove = millis();
    }
  if (y < -50 && curpos >4) {
    //turns off previous position
    curframe[(int)curpos/4][curpos%4 - 1] = 1;
    
    //changes position
    curpos = curpos -4;
    
    //turns on new position
    curframe[(int)curpos/4][curpos%4 - 1] = 0;
    
    //stores time in ms of last movement
    lastmove = millis();
}}
//draws each frame
void  drawFrame(int frame[4][4]){
  for (int c=0; c<4; c++){
    digitalWrite(col[c], HIGH);
    for (int r = 0; r < 4; r++){
      digitalWrite(row[r], 255*frame[r][c]);
      delayMicroseconds(10);
    }
    for (int r = 0; r < 4; r++){
      digitalWrite(row[r], HIGH);
      delayMicroseconds(10);
    }
    digitalWrite(col[c], LOW);
  }
}
