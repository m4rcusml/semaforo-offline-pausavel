class Semaforo {
  const char* availableColors[3] = { "GREEN", "YELLOW", "RED" };
  const int availablePins[3] = { 8, 9, 10 };
  int currentColorIndex = 0;

  public:
    Semaforo() {}
    
    int getPinFromColor(const char* color) {
      	int pin = -1;
        for (int i = 0; i < 3; i++) {
          if(availableColors[i] == color) {
          	pin = availablePins[i];
            break;
          }
        }
    	return pin;
    }
  
    int getColor() const {
      return currentColorIndex;
    }
    
    void changeColor(const bool isButtonPressed) {
      if(isButtonPressed == true) {
      	return;
      }
      
      digitalWrite(getPinFromColor(availableColors[currentColorIndex]), HIGH);
      currentColorIndex++;
      
      if (currentColorIndex >= 3) currentColorIndex = 0;
    }
};

Semaforo semaforo;
int currentColorIndex = semaforo.getColor();
int currentDelay = 4000;
bool isButtonPressed = false;

void setup() {
  pinMode(semaforo.getPinFromColor("GREEN"), OUTPUT); // red
  pinMode(semaforo.getPinFromColor("YELLOW"), OUTPUT); // yellow
  pinMode(semaforo.getPinFromColor("RED"), OUTPUT); // green
  pinMode(2, INPUT_PULLUP); // button

}

void loop() {
  currentColorIndex = semaforo.getColor();
  isButtonPressed = digitalRead(2) != HIGH;
  
  if(isButtonPressed == false) {
    digitalWrite(semaforo.getPinFromColor("GREEN"), LOW);
    digitalWrite(semaforo.getPinFromColor("YELLOW"), LOW);
    digitalWrite(semaforo.getPinFromColor("RED"), LOW);
  }
  
  // muda a cor do semaforo no led
  switch(currentColorIndex) {
    case 2:
      // vermelho
      semaforo.changeColor(isButtonPressed);
      currentDelay = 6000;
      break;
    case 1:
      // amarelo
      semaforo.changeColor(isButtonPressed);
      currentDelay = 2000;
      break;
    case 0:
    default:
      // verde
      semaforo.changeColor(isButtonPressed);
      currentDelay = 4000;
    break;
  };
  
  delay(currentDelay);
}
