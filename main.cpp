
#define latch 3 //Cable verde
#define clock 2 //Cable azul
#define datos 5 //Cable amarillo


void setup() {
    pinMode(latch,OUTPUT);
    pinMode(clock,OUTPUT);
    pinMode(datos,OUTPUT);

    randomSeed(analogRead(0));
}

void loop() {

}

