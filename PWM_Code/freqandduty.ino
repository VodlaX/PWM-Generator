#include <Arduino.h>
#include <TM1637Display.h>

// Распиновка 
const uint8_t CLK = 2;
const uint8_t DIO = 3;
const uint8_t InputPWM = 4;
const uint8_t Button = 5;
bool choosing = false;

TM1637Display display(CLK, DIO);

void setup() {

 pinMode(Button, INPUT_PULLUP); // Кнопка управления
 display.setBrightness(6); // Яркость

}

void loop() {

uint8_t N = 5; // Кол-во замеров
uint8_t valid = N; // Кол-во успешных замеров

uint32_t sumduron = 0; // Сумма импульсов
uint32_t sumduroff = 0; // Сумма пауз

uint16_t L = 0; // Замер пауз
uint16_t H = 0; // Замер импульсов


for (uint8_t i = 0; i < N; i++) {

  H = pulseIn(InputPWM, HIGH, 200000);
  L = pulseIn(InputPWM, LOW, 200000);

  if (H == 0 || L == 0) {
valid--;    
continue;
}

else {
sumduron += H;
sumduroff += L;
}
}

if (sumduron == 0 || sumduroff == 0) {
display.showNumberDec(0, true);  
delay(100);
return;
}
uint16_t duty = 0;
duty = (sumduron * 100) / (sumduron + sumduroff);

sumduron = sumduron / valid;
sumduroff = sumduroff / valid;
uint16_t frq = 0;

frq = 1000000 / (sumduron +  sumduroff);
uint16_t cel = frq / 1000; // Целые
uint16_t ost = ((frq % 1000) + 5) / 10; // Остаток
frq = cel * 100 + ost;

if (digitalRead(Button) == LOW) {
delay(20);
if (digitalRead(Button) == LOW){
  choosing = !choosing;
  while (digitalRead(Button) == LOW) {
    delay(10);
  }
}
}
if (choosing == true){
  display.showNumberDecEx(frq, 0b01000000, true);
  }
else {
  display.showNumberDec(duty, false);    
}
delay(500); 
}
