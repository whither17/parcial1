#include <iostream>
using namespace std;
#define latch 3 //Cable verde
#define clock 2 //Cable azul
#define datos 5 //Cable amarillo



/*
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
*/

void loop() {

}

char *patron1(){

    char *patron = new char[8]; //creamos un arreglo dinamico de 8 posiciones tipo char
    char binario;     // binario será nuestra variable que representará un byte
    int l = 1;        // l será una variable espejo
    int bandera = 2;  // bandera nos indica cuantos bit 1 debemos generar
    int aumento = 3;  // aumento nos indica cuanto debemos desplazar los bits
    bool control;     //control nos permite cambiar el flujo de ejecución

    for(int fila = 0; fila < 8; fila++) {
        binario = 0B00000001;                           //nuestro binario empieza en 1
        control = true;
        if(fila < 3) {                                  //obtenemos los bytes de las primeras 3 filas

            for(int j = 0; j < aumento; j++) {
                if(control == true) {
                    for(int i = 1; i < bandera; i++) {
                        binario = binario << 1;         //manipulamos nuestro byte desplazando un bit a la izquierda, 1 -> 10
                        binario = binario | 0B1;        //agregamos un bit al principio de nuestro byte con OR, 10 -> 11
                    }
                    control = false;
                }
                binario = binario << 1;                 //desplazamos nuestros bits tanto que indique aumento
            }
            aumento--;
            bandera = bandera + 2;                      //nuestro proximo byte tendrá 2 bits extra
            binario = ~binario;                         //negamos el bit
            patron[fila] = binario;
        }

        if(fila == 3) {                                 //este bloque crea el byte 11111111

            for(int i = 0; i < 6; i++) {
                binario = binario << 1;
                binario = binario | 0B1;
            }
            binario = binario | 0b10000000;
            binario = ~binario;
            patron[fila] = binario;
        }
        if(fila > 3) {                                 //este bloque usa nuestra variable espejo para copiar los valores previamente claculados
            patron[fila] = patron[fila - l];
            l = l + 2;
        }
    }
    return patron;
}

char *patron2() {
    char *patron = new char[8];
    char binario;
    int desplazamiento = 7;                             //cuanto debemos de mover nusestro bit a la izquierda
    int cola = 0;                                       //cuanto debemos de mover nuestros bits a su posicion definitiva
    int l = 1;                                          //variable espejo

    for(int fila = 0; fila < 8; fila++) {
        binario = 0B00000001;                           //nuestro binario empieza en 1
        if(fila < 4) {
            for(int i = 0; i < desplazamiento; i++) {   //movemos nuestro binario a la izquierda tantas veces desplazamiento
                binario = binario << 1;
            }
            binario = binario | 0b1;                    //añadimos un 1 al principio del byte 1000 -> 1001
            for(int j = 0; j < cola; j++) {
                binario = binario << 1;
            }
            cola++;
            desplazamiento = desplazamiento - 2;        //en cada fila se reduce en dos la cantidad de ceros entre nuestros bits 1
            patron[fila] = ~binario;
        }
        else {
            patron[fila] = patron[fila - l];           //obtenemos el espejo para terminar nuestro arreglo
            l = l + 2;
        }
    }
    return patron;
}

char *patron3() {
    char *patron = new char[8];
    char binario;
    int desplazamiento = 3;

    for(int fila = 0; fila < 8; fila = fila + 2) {

        if(fila < 4) {
            if(fila == 0) {
                binario = 0B00000000;
                for(int i = 0; i < desplazamiento; i++) {
                    binario = binario << 3;
                    binario = binario | 0b11;
                }
                patron[fila] = ~binario;
                patron[fila + 1] = ~binario;
            }
            if(fila == 2) {
                binario = 0B00000000;
                for(int i = 0; i < (desplazamiento - 1); i++) {
                    binario = binario << 3;
                    binario = binario | 0b11;
                }
                binario = binario << 2;
                binario = binario | 0b1;

                patron[fila] = ~binario;
                patron[fila + 1] = ~binario;
            }
        }
        else {
            patron[fila] = patron[fila - 4];
            patron[fila + 1] = patron[fila - 4];
        }
    }
    return patron;
}

char *patron4() {
    char *patron = new char[8];
    char binario = 0b00000000;
    int desplazamiento = 4;
    int l = 1;

    for(int fila = 0; fila < 8; fila++) {
        if(fila < 4) {
            binario = 0b00000000;
            binario = binario | 0b1111;
            for(int i = 0; i < desplazamiento; i++) {
                binario = binario << 1;
            }
            desplazamiento--;
            patron[fila] = ~binario;
        }
        else {
            patron[fila] = patron[fila - l];
            l = l + 2;
        }
    }
    return patron;
}

char *patron0() {
    char *patron = new char[8];
    for(int i = 0; i < 8; i++) {
        patron[i] = 0b00000000;
    }
    return patron;
}

void borrarPatron(char *ptr) {
    delete []ptr;
}
int main() {

    char *ptr;

    ptr = patron4();

    for(int i = 0; i < 8; i++) {
        cout << *(ptr + i) << endl;
    }

}


