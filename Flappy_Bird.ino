/* Switch debouncing.

 *

 * Tests a pushbutton switch. Increments a counter every time it

 * is pressed, and prints to Serial Monitor when it accepts or ignores an

 * apparent button press.

 *

 * == Setting up the Serial Monitor ==

 * The Serial Monitor must be configured (bottom-right corner of the screen) as

 * baud rate 115200.

 *

 * ENGR 40M

 * Stanford University

 * November 2015

 */


// Pins

const int BUTTON = A4;

// use the alive variable to stop the LEDs from running when the bird has died

bool alive = true;

const int DEBOUNCE_DELAY = 100; // in milliseconds

const int BAR_SHIFT_DELAY = 250;

long last_bar_shift = 0;

const int LOWER_BIRD_DELAY = 1500;

long last_bird_shift = 0;

int fallen = 0;

const int CHANGE_SCENE = 5;

static byte ledOn[8][8];

byte x = 0;

byte y = 0;

int hole = 0;

int bird_x = 3;

int star_x = 0;

int stars = 0;

const byte ANODE_PINS[8] = {13, 12, 11, 10, 9, 8, 7, 6};

const byte CATHODE_PINS[8] = {A3, A2, A1, A0, 5, 4, 3, 2};


void setup(){

  pinMode(BUTTON, INPUT_PULLUP);

  Serial.begin(115200);

  for(byte i = 0; i < 8; i++) {

    pinMode(ANODE_PINS[i], OUTPUT);

    pinMode(CATHODE_PINS[i], OUTPUT);

  }


  for (byte i = 0; i < 8; i++){

    digitalWrite(CATHODE_PINS[i], HIGH);

    digitalWrite(ANODE_PINS[i], HIGH);

  }


  //set where the bird is initally going to be

  x = bird_x;

  y = 0;

  ledOn[x][y] = !ledOn[x][y];


//create the first pipe

  hole = random(1, 4);

  for(byte j = 0; j < hole; j++){

    x = j;

    y = 7;

    ledOn[x][y] = !ledOn[x][y];

  }

  for(byte j = hole + 3; j < 8; j++){

    x = j;

    y = 7;

    ledOn[x][y] = !ledOn[x][y];

  }  

//create the first star

  star_x = hole + 1;

  x = star_x;

  y = 7 ; 

  ledOn[x][y] = !ledOn[x][y];

  display(ledOn);

}

bool check_up(byte pattern[8][8], int x, int y){

//if the LED is ON

  if (pattern[x][y] == 1 && x != 0){

//turn it off

    x = x;

    y = y;

    ledOn[x][y] = !ledOn[x][y];

//turn the one above it on

    x = x - 1;

//take note of where the bird is

    bird_x = x;

    y = y;

    ledOn[x][y] = !ledOn[x][y];

    display(ledOn);

//return true to show that the bird has already been moved up

    return true;

  }

  return false;

}

int check_down(byte pattern[8][8], int x, int y){

//if the LED is on

  if (pattern[x][y] == 1 && x != 7){

//turn it off

    x = x;

    y = y;

    ledOn[x][y] = !ledOn[x][y];

    if(pattern[x + 1][y] == 0){

  //turn the one below it off

      x = x + 1;

  //take note of where the bird is

      bird_x = x;

      y = y;

      ledOn[x][y] = !ledOn[x][y];

      display(ledOn);

    }

//say that you have already moved the bird down, so don't go moving more birds down in this round

//why would arduino return 1 even though it did not enter this if condition?

//does an int function always have to return something?

    return 1;

  }

//say that you have not moved the bird down

  return 0;

}


bool check_column(byte pattern[8][8], int x, int y){

  if (pattern[x][y] == 1){

    return true;

  }

  return false;

}

void display(byte pattern[8][8]) {


  for(byte i = 0; i < 8; i++){

    for (byte j = 0; j < 8; j++){

      if (pattern[i][j] == 1){

        if (digitalRead(CATHODE_PINS[j]) == LOW){

          digitalWrite(CATHODE_PINS[j], HIGH);

        }else{

          digitalWrite(CATHODE_PINS[j], LOW);

        }            

      }else{

        digitalWrite(CATHODE_PINS[j], HIGH);

      }

      digitalWrite(ANODE_PINS[i], LOW);

      digitalWrite(ANODE_PINS[i], HIGH);

      digitalWrite(CATHODE_PINS[j], HIGH);

    }

  }

}




void loop() {

  // Terminology: "reading" refers to a raw reading of the input pin. A "reading

  // change" is when the reading is different from last time we took a reading

  // (i.e., last loop iteration). The "button_state" refers to the (debounced)

  // state of the button.

  if (alive){

    display(ledOn);

    static int counter = 0;              // count (debounced) button presses

    static byte button_state = HIGH;

    static byte last_reading = HIGH;

    static long last_reading_change = 0;

    static char message[50];             // buffer for sprintfs


  //read the state of the button

    byte reading = digitalRead(BUTTON);

    unsigned long now = millis();

    // Ignore button_state changes within DEBOUNCE_DELAY milliseconds of the last

    // reading change, otherwise accept.

    if (now - last_reading_change > DEBOUNCE_DELAY) {

      if (reading == LOW && button_state == HIGH) { // button pressed down (HIGH to LOW)

        sprintf(message, "accepted [%d], %ld ms since last\n", ++counter,

            now - last_reading_change);

        Serial.print(message);

        //if the button is pressed find the bird and move it up one

        //if the x value of the bird is at 7, then the for loop will run without ever moving the bird

        for(byte i = 0; i < 8; i++){

          x = i;

          y = 0;

          if (check_column(ledOn, 0, 0) == false){

            check_up(ledOn, x, y);

          }

        }

      }

      button_state = reading;

    }


    // For educational purposes, mention when we ignore a reading change. You

    // (probably) don't need this code when you incorporate switch bouncing into

    // your cool program.

    else if (reading == LOW && last_reading == HIGH) {

      sprintf(message, "ignored, %ld ms since last\n", now - last_reading_change);

      Serial.print(message);

    }

    // Prepare for next loop

    //rest reading to high if button is relased

    if (reading != last_reading) last_reading_change = now;

    last_reading = reading;

  //enter the loop when it is time to shift the bird down

    if (now - last_bird_shift > LOWER_BIRD_DELAY){

  //set the last time the bird was shifted to now

      last_bird_shift = now;

      for(byte i = 0; i < 8; i++){

        x = i;

        y = 0;

        //as long as you haven't moved the bird down check if this LED represents the bird and move it down

        //don't move down if the pipe is in the zeroth column because then you might accidentally land on a star or a pipe and turn it off and then when the pipe and star is getting removed they will appear instead

        if (fallen == 0 and check_column(ledOn, 0, 0) == false and check_column(ledOn, 0, 7) == false){

          fallen = check_down(ledOn, x, y);

        }

      }

      //reset fallen for the next round

      fallen = 0;

    }

    //when it is time to shift the pipe over

    if ((now - last_bar_shift) > BAR_SHIFT_DELAY){

    //set the last time the pole was shifted to now

      last_bar_shift = now;

      for (byte i = 0; i < 8; i++){

        //find the cathode that is low

        if (check_column (ledOn, 0, i)){

          //turn all of the LEDs in that column off

          for(byte j = 0; j < hole; j++){

            x = j;

            //if you are turing off the zeroth column and the anode pin that you are turning off is the same anode pin that is on to light up your bird, then the bird is dead

            if (x == bird_x and i == 0){

              alive = false; 

            }           

            y = i;

            ledOn[x][y] = !ledOn[x][y];

          }

          for(byte j = hole + 3; j < 8; j++){

            x = j;

            //if you are turing off the zeroth column and the anode pin that you are turning off is the same anode pin that is on to light up your bird, then the bird is dead

            if (x == bird_x and i == 0){

              alive = false; 

            }            

            y = i;

            ledOn[x][y] = !ledOn[x][y];

            display(ledOn);

          }        


          x = star_x;

          y = i;

          //if you are about to turn off a star but then you realize that that is where the bird is increase the amount of stars that the bird has gotten and set the bird position to the star position instead of removing the star

          if (star_x == bird_x and i == 0){

            stars += 1;

            bird_x = star_x;

          }else{

            ledOn[x][y] = !ledOn[x][y];

            display(ledOn);

          }


          //as long as the column that you are turning off is not the zeroth column, turn on the lights that are adjacent to the lights that you just turned off

          if (i != 0){

            for(byte j = 0; j < hole; j++){

              x = j;

              //if you are turning on the zeroth column and the anode pin that you are turning on is the same anode pin that is on to light up your bird, then the bird is dead

              if (x == bird_x and i - 1== 0){

                alive = false;

              }

              y = i - 1;

              ledOn[x][y] = !ledOn[x][y];

            }

            for(byte j = hole + 3; j < 8; j++){

              x = j;

              //if you are turning on the zeroth column and the anode pin that you are turning on is the same anode pin that is on to light up your bird, then the bird is dead

              if (x == bird_x and i - 1 == 0){

                alive = false;

              }

              y = i - 1;

              ledOn[x][y] = !ledOn[x][y];

              display(ledOn);

            }  

            //move star

            x = star_x;

            y = i - 1;

            //don't move the star if it is going to land on the bird because then it will turn the bird off. the bird will still catch the star through because the x position of the star is saved

            if (i - 1 != 0 or star_x != bird_x){

              ledOn[x][y] = !ledOn[x][y];

              display(ledOn);

            }      

            break;

          }else{

            //if the bar that you are turning off is the zeroth column, then make a new bar

            hole = random(1,4);

            for(byte j = 0; j < hole; j++){

              x = j;

              y = 7;

              ledOn[x][y] = !ledOn[x][y];

            }

            for(byte j = hole + 3; j < 8; j++){

              x = j;

              y = 7;

              ledOn[x][y] = !ledOn[x][y];

              display(ledOn);

            }  

            //create a new star

            star_x = hole + 1;

            x = star_x;

            y = 7;

            ledOn[x][y] = !ledOn[x][y];

            display(ledOn);

            break;

          }

        }

      }

    } 

  }else if(alive == false){

    Serial.println("Total Stars: ");

    Serial.println(stars);

  }

}
