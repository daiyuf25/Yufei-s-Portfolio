enum states {STOP = 0, FORWARD, REVERSE};

// this state variable saves our state between calls to loop()
int state = STOP;
const byte TOGGLE = 8;
const byte LIMIT = 11;
const byte two = 2;
const byte three = 3;
const float INTERVAL_SHY = 10000;
const float INTERVAL_OKAY = 20000;
const float INTERVAL_UGH = 30000;
const float start = millis();


void setup() {
  Serial.begin(115200);
  Serial.println("Setup complete");
  // TO DO: 
  // intialize input and output pins
  pinMode(TOGGLE, INPUT_PULLUP);
  pinMode(LIMIT, INPUT_PULLUP);
  pinMode(two, OUTPUT);
  pinMode(three, OUTPUT);
  // intialize your initial states
  state = STOP;
  //testing state values
}

void loop() {
  // TODO read input pins to get status of toggle and limit
  int TOGGLE_1 = digitalRead(TOGGLE);
  int LIMIT_1 = digitalRead(LIMIT);

  switch (state) {
    case STOP: // case for STOP 
      // TO DO: Set your outputs for the motor to stop
      Serial.println("Stop");
      digitalWrite(two, LOW);
      Serial.println(digitalRead(two));
      digitalWrite(three, LOW);
      Serial.print(digitalRead(three));
      // TO DO: Decide whether to change states and update the state variable accordingly
      if (TOGGLE_1 == LOW){
        state = FORWARD;
      }
      break;
    case FORWARD: // case for forward 
      // TO DO: Set your outputs for the motor to move forward
      Serial.println("FORWARD");
      // TO DO: Decides whether to change states by updatin gth Transition for your states depending on your input
      int now = millis();
      int interval = now - start;
      Serial.print("start "); 
      Serial.println(start);
      Serial.print("now ");
      Serial.println(now);

      if (0 <= interval && interval < INTERVAL_SHY) {
        Serial.println("SHY_CASE");
        for (int i = 0; i <= 10; i++) {
          digitalWrite(3, LOW);
          digitalWrite(2, HIGH);
          delay(10);
          digitalWrite(3, LOW);
          digitalWrite(2, LOW);
          delay(30);
          Serial.println("forward");
        }

        //pause for half a sec
        digitalWrite(3, LOW);
        digitalWrite(2, LOW);
        delay(1500);
        Serial.println("stop");
        //continue moving
        for (int i = 0; i <= 20; i++){
          digitalWrite(3, LOW);
          digitalWrite(2, HIGH);
          delay(10);
          digitalWrite(3, LOW);
          digitalWrite(2, LOW);
          delay(50);
          Serial.println("forward");
        }
        digitalWrite(3, LOW);
        digitalWrite(2, HIGH);
        Serial.println("forward");
        delay(1000);

        if (digitalRead(TOGGLE) == HIGH){
          Serial.println("reversE");
          digitalWrite(two, LOW);
          digitalWrite(three, HIGH);
          delay(1000);
          if (digitalRead(LIMIT) == HIGH){
            Serial.println("stop");
            state = STOP;
          }
        }
      }
      else if (INTERVAL_SHY <= interval && interval < INTERVAL_OKAY) { 
        digitalWrite(two, HIGH);
        Serial.print(digitalRead(two));
        digitalWrite(three, LOW);
        Serial.print(digitalRead(three));
        delay(500);
        if (digitalRead(TOGGLE) == HIGH){
          Serial.println("reversE");
          digitalWrite(two, LOW);
          digitalWrite(three, HIGH);
          delay(1000);
          if (digitalRead(LIMIT) == HIGH){
            Serial.println("stop");
            state = STOP;
          }
        }              
      }

      else if (INTERVAL_OKAY <= interval && interval < INTERVAL_UGH) {       
        digitalWrite(two, HIGH);
        Serial.print(digitalRead(two));
        digitalWrite(three, LOW);
        Serial.print(digitalRead(three));
        delay(3000);
        if (digitalRead(TOGGLE) == HIGH){
          Serial.println("reversE");
          digitalWrite(two, LOW);
          digitalWrite(three, HIGH);
          delay(50);

          digitalWrite(two, LOW);
          digitalWrite(three, LOW);
          delay(2000);

          digitalWrite(two, LOW);
          digitalWrite(three, HIGH);
          delay(500);

          if (digitalRead(LIMIT) == HIGH){
            Serial.println("stop");
            state = STOP;
          }
        }              
      }
      
      break;

    default: 
      Serial.println("Invalid state");
      break;
  }; // end switch
}
