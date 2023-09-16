#define latch 3 //Cable verde
#define clock 2 //Cable azul
#define datos 5 //Cable amarillo

void dibujar(char *fila, char *columna) {
    shiftOut(datos,clock,MSBFIRST,fila);  //catodos largo - columnas, estos funcionan inverso, para que el led se encienda debe ser 0 y no 1
    shiftOut(datos,clock,MSBFIRST,columna); //Anodos altura - filas
    digitalWrite(latch,HIGH);
    digitalWrite(latch,LOW);
}

void setup() {

    Serial.begin(9600);
    pinMode(latch,OUTPUT);
    pinMode(clock,OUTPUT);
    pinMode(datos,OUTPUT);
}

void loop() {



}
