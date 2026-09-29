enum states {STOP = 0, FORWARD};

// this state variable saves our state between calls to loop()
int state;
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
}

void loop() {
  switch (state) {
    case STOP: // case for STOP 
      // TO DO: Set your outputs for the motor to stop
      digitalWrite(two, LOW);
      digitalWrite(three, LOW);

      // TO DO: Decide whether to change states and update the state variable accordingly
      if (digitalRead(TOGGLE) == LOW){
        state = FORWARD;
      }
      break;

    case FORWARD: // case for forward 
      Serial.println("FORWARD");

      int now = millis();
      int interval = now - start;
      Serial.print("start "); 
      Serial.println(start);
      Serial.print("now ");
      Serial.println(now);

      if (0 <= interval && interval < INTERVAL_SHY) {
        Serial.println("SHY_CASE");

        //start moving slowly
        for (int i = 0; i <= 15; i++) {
          digitalWrite(three, LOW);
          digitalWrite(two, HIGH);
          delay(10);
          digitalWrite(three, LOW);
          digitalWrite(two, LOW);
          delay(30);
          Serial.println("forward");
        }

        //pause for half a sec
        digitalWrite(three, LOW);
        digitalWrite(two, LOW);
        delay(1500);
        Serial.println("stop");

        //continue moving
        for (int i = 0; i <= 15; i++){
          digitalWrite(three, LOW);
          digitalWrite(two, HIGH);
          delay(20);
          digitalWrite(three, LOW);
          digitalWrite(two, LOW);
          delay(10);
          Serial.println("forward");
        }
        digitalWrite(three, LOW);
        digitalWrite(two, HIGH);
        Serial.println("forward");
        delay(500);

        if (digitalRead(TOGGLE) == HIGH){
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
        digitalWrite(three, LOW);
        delay(500);

        if (digitalRead(TOGGLE) == HIGH){
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
        delay(1500);

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
          delay(1000);

          if (digitalRead(LIMIT) == HIGH){
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
