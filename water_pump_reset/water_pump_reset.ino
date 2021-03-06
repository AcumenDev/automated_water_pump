#define INPUT_PIN 9
#define VALVE_PIN 8
#define PUMP_PIN 7

#define VALVE_WORK_TIME 1000 * 1

enum State {OFF = 0, START = 1, WORK = 2, STOP = 3};

State current;
bool inputState;
bool pumpWorked;

void setup() {
  pinMode(INPUT_PIN, INPUT_PULLUP);
  pinMode(VALVE_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(VALVE_PIN, HIGH);
  digitalWrite(PUMP_PIN, HIGH);

  current = OFF;
  inputState = false;
  pumpWorked = false;
}

void loop() {
  inputState = getInputState(inputState);

  switch (current) {
    case OFF: {
        if (inputState) {
          current = START;
        }
        break;
      }
    case START: {
        valveOn();
        pumpWorked = false;
        current = WORK;
        break;
      }
    case WORK: {
        if (inputState) {
          setStatePump(true);
          pumpWorked = true;
        } else {
          setStatePump(false);
          current = STOP;
        }
        break;
      }
    case STOP: {
        if (pumpWorked) {
          valveOn();
        }
        current = OFF;
        break;
      }
  }
}

bool getInputState(bool currentInputState) {
  bool rawState = !digitalRead(INPUT_PIN);
  if (rawState != currentInputState) {
    delay(100);
    if (currentInputState != !digitalRead(INPUT_PIN))
      return !currentInputState;
  }
  return currentInputState;
}

void valveOn() {
  digitalWrite(VALVE_PIN, LOW);
  delay(VALVE_WORK_TIME);
  digitalWrite(VALVE_PIN, HIGH);
}

void setStatePump(bool state) {
  delay(300);
  if (state) {
    digitalWrite(PUMP_PIN, LOW);
  } else {
    digitalWrite(PUMP_PIN, HIGH);
    delay(2000);
  }
  delay(300);
}
