/*
  Simon Battle station
  by Sine, Richy, Justin  
 */

const int BUTTON_COUNT = 4;

const int buttonPins[] = {2, 3, 4, 5};
int buttonStates[] = {0, 0, 0, 0};
int lastButtonStates[] = {LOW, LOW, LOW, LOW};

long lastDebounceTimes[] = {0, 0, 0, 0};
long debounceDelay = 50;

const int ledPins[] = {8, 9, 10, 11};

const int resetButtonPin = 6;
int resetButtonState = 0;

const int MOVE_MAX_COUNT = 100;
int moves[MOVE_MAX_COUNT];
int currentMovePosition = -1;

int currentCheckPosition = -1;

const int ledPin =  13;

enum GameState {
  Startup,
  Showing,
  Playing,
  GameOver,
  Count
};

GameState gameState = Startup;
long currentStateStartTime = -1;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0)); // Works if analog 0 is not connected 
  
  pinMode(ledPin, OUTPUT);      
  
  for (int i = 0; i < BUTTON_COUNT; i = i + 1) {
    pinMode(buttonPins[i], INPUT);
    pinMode(ledPins[i], OUTPUT);
  }

  pinMode(resetButtonPin, INPUT);

  changeState(Startup);
}

void loop(){

  int oldResetButtonState = resetButtonState;
  resetButtonState = digitalRead(resetButtonPin);

  if (resetButtonState == LOW && oldResetButtonState == HIGH) {
    changeState(Startup);
  }


  // -------- Game State ---------
  updateForGameState();
}

void updateForGameState() {
  long currentTime = millis();
  long timeSinceStateChange = currentTime - currentStateStartTime;
  
  switch(gameState) {
    case Startup:
      if (timeSinceStateChange > 100) {
        changeState(Showing);
      } else {
        int randomLight = random(BUTTON_COUNT); // 0-4 inclusive-exclusive 

        for (int i = 0; i < BUTTON_COUNT; i = i + 1) {
          digitalWrite(ledPins[i], randomLight == i ? HIGH : LOW);
        }
        delay(10);
      }
    break;
    case Showing:
    {
      const long blankTime = 1000;
      const long timePerLed = 1000;
      const long gapTimePerLed = 500;
      const long timePerMove = timePerLed + gapTimePerLed;
      
      const int currentLed = moves[currentMovePosition];
      const long showTime = (currentMovePosition+1) * timePerMove;
      
      if (timeSinceStateChange > blankTime + showTime) {

          for (int i = 0; i < currentMovePosition+1; i = i + 1) {
            Serial.print("move: ");
            Serial.print(i);
            Serial.print(" moveButton: ");
            Serial.print(moves[i]);
            Serial.print("\n");
          }

          
          changeState(Playing);
      } else if (timeSinceStateChange > blankTime) {

        

        int showingLedPos = ((timeSinceStateChange - blankTime) / timePerMove);
        int showingLed = moves[showingLedPos];
        
        long thisMoveTime = (timeSinceStateChange - blankTime) % timePerMove;
        if (thisMoveTime > timePerLed) {
          // show gap, no leds on
          for (int i = 0; i < BUTTON_COUNT; i = i + 1) {
            digitalWrite(ledPins[i], LOW);
          }
        } else {
          for (int i = 0; i < BUTTON_COUNT; i = i + 1) {
            digitalWrite(ledPins[i], showingLed == i ? HIGH : LOW);
          }
        }
      } else {
        for (int i = 0; i < BUTTON_COUNT; i = i + 1) {
          digitalWrite(ledPins[i], LOW);
        }
      }
    }
    break;
    case Playing:
    {

      
      
      // Check if there was a button down, if so store it
      int lastButtonDown = -1;
      for (int i = 0; i < BUTTON_COUNT; i = i + 1) {
        if (buttonStates[i] == HIGH) {
          lastButtonDown = i;
        }
      }




      // update button states
      int currentButtonDown = -1;
      for (int i = 0; i < BUTTON_COUNT; i = i + 1) {

        // Debounce!
        
        int reading = digitalRead(buttonPins[i]);
        
        if (reading != lastButtonStates[i]) {
          lastDebounceTimes[i] = millis();
        }

        if ((millis() - lastDebounceTimes[i]) > debounceDelay) {
          if (reading != buttonStates[i]) {
            buttonStates[i] = reading;

            
          }
        }
        
        
        
        digitalWrite(ledPins[i], buttonStates[i]);

        if (buttonStates[i] == HIGH) {
          currentButtonDown = i;
        }

        lastButtonStates[i] = reading;
      }


      // if all buttons are up then check if it's a correct button
      if (currentButtonDown < 0) {
        if (lastButtonDown >= 0) {
  
          // get the currect check button
          int currentCheckButton = moves[currentCheckPosition];
          
          Serial.print("checking pos: ");
          Serial.print(currentCheckPosition);
          Serial.print(" currentCheckButton: ");
          Serial.print(currentCheckButton);
          Serial.print(" lastButtonDown: ");
          Serial.print(lastButtonDown);
          Serial.print("\n");
  
          if (lastButtonDown == currentCheckButton) {
            // if it's correct then increase the current check button position
            currentCheckPosition += 1;
  
            
            // if current check button position is greater than current move position then goto Showing state
            if (currentCheckPosition > currentMovePosition) {
              changeState(Showing);
            }
            
          } else {
  
            // if it's incorrect game over
            
            changeState(GameOver);
          }
      
          
  
        }
      }

    }
    break;
    case GameOver:
      if (timeSinceStateChange > 3000) {
        changeState(Startup);
      } else {
        for (int i = 0; i < BUTTON_COUNT; i = i + 1) {
          digitalWrite(ledPins[i], HIGH);
        }
      }
    break;
    case Count:
    default:
    break;
  }
}

bool anyButtonDown() {
  for (int i = 0; i < BUTTON_COUNT; i = i + 1) {
    int state = digitalRead(buttonPins[i]);
    if (state == HIGH) {
      return true;
    }
  }
  return false;
}

// Don't call this directly, call changeState(Startup);
void reset() {
  Serial.print("---- Reset! ----\n");       // prints a label
  
  currentMovePosition = -1;
  for (int i = 0; i < MOVE_MAX_COUNT; i = i + 1) {
    moves[i] = -1;
  }
}

void changeState(GameState g) {
  gameState = g;
  currentStateStartTime = millis();

  switch(gameState) {
    case Startup:
      reset();
    break;
    case Showing:
      pickNextMove();
    break;
    case Playing:
      // reset to first check position 0
      currentCheckPosition = 0;
    break;
    case GameOver:
      Serial.print("---- game over ----\n");
    break;
    case Count:
    default:
    break;
  }
}

// ---- Game ----

void pickNextMove() {
  currentMovePosition += 1;
  moves[currentMovePosition] = random(BUTTON_COUNT);

  Serial.print("pos ");
  Serial.print(currentMovePosition);
  Serial.print(", led: ");
  Serial.print(moves[currentMovePosition]);
  Serial.print("\n");
}





