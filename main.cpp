#define latch 3 //Cable verde
#define clock 2 //Cable azul
#define datos 5 //Cable amarillo
String mensaje0 = "1 - Verificar matriz, 2 - Ingresar una figura, 3 - Secuencia predefinida";
String mensaje1 = "Ingrese una opcion: ";
String mensaje2 = "Opcion ingresada: ";
int opcion = 0;
char matrizFil[] = {128,64,32,16,8, 4, 2, 1};
char colOff = 255;

void dibujar(char *fila, char *columna) {
    shiftOut(datos,clock,LSBFIRST,*columna);  //catodos largo - columnas, estos funcionan inverso, para que el led se encienda debe ser 0 y no 1
    shiftOut(datos,clock,MSBFIRST,*fila); //Anodos altura - filas
    digitalWrite(latch,HIGH);
    digitalWrite(latch,LOW);
    //delay(500);
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

void dibujarPrueba(int tiempo, int rep) {

    unsigned long punto1, punto2, punto3, punto4;
    int i = 0;
    char *col;
    col = patron4();

    while(i < rep) {
        punto1 = millis();
        punto2 = millis();
        Serial.print("Al arrancar: ");
        Serial.println(punto1);
        while((punto2 - punto1) <= 2*tiempo) {
            punto2 = millis();
            Serial.print("T transcurrido: ");
            Serial.println(punto2);
            while(true){
                punto3 = millis();
                if ((punto3 - punto2) <= tiempo) {
                    for(int i = 0; i < 8; i++) {
                        dibujar(&matrizFil[i], &col[i]);
                    }
                }
                else{
                    break;
                }
            }

            while(true){
                punto4 = millis();
                if ((punto4 - punto3) <= tiempo) {
                    dibujar(&matrizFil[0], &colOff);
                }
                else{
                    break;
                }
            }

        }
        i++;
        Serial.print("veces: ");
        Serial.println(i);
    }
}

void verificacion() {
    unsigned int tiempo, repeticiones;
    while(true) {
        Serial.println("Ingrese el tiempo de retardo (en segundos): ");
        while(Serial.available() == 0){}
        tiempo = Serial.parseInt();
        Serial.print("Tiempo elegido (s): ");
        Serial.println(tiempo);

        Serial.println("Ingrese la cantidad de repeticiones (veces): ");
        while(Serial.available() == 0){}
        repeticiones = Serial.parseInt();
        Serial.print("Rep: ");
        Serial.println(repeticiones);

        if((repeticiones > 0) && (tiempo > 0)) {
            break;
        }
        else {
            Serial.println("Valores Incorrectos");
        }
    }

    dibujarPrueba((tiempo*1000), repeticiones);
}


void setup() {

    Serial.begin(9600);
    pinMode(latch,OUTPUT);
    pinMode(clock,OUTPUT);
    pinMode(datos,OUTPUT);
}

void loop() {
    Serial.println(mensaje0);
    while(Serial.available() == 0){}
    opcion = Serial.parseInt();
    Serial.print(mensaje2);
    Serial.println(opcion);

    switch(opcion) {
    case 1:
        verificacion();
        break;
    case 2:
        break;
    case 3:
        break;
    default:
        Serial.println("Valor incorrecto, intente otra vez");
    }

}


int main() {

    char *ptr;

    ptr = patron4();

    for(int i = 0; i < 8; i++) {
        cout << *(ptr + i) << endl;
    }

}


