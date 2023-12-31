#define latch 3 //Cable verde
#define clock 2 //Cable azul
#define datos 5 //Cable amarillo

String mensaje0 = "1 - Verificar matriz, 2 - Ingresar una figura, 3 - Secuencia predefinida";
String mensaje1 = "Ingrese una opcion: ";
String mensaje2 = "Opcion ingresada: ";
unsigned char matrizFil[] = {128, 64, 32, 16, 8, 4, 2, 1, 0}; //Este array es fundamental para el dibujado de barrido

void dibujar(unsigned char *fila, unsigned char *columna) {

    shiftOut(datos,clock,LSBFIRST,*columna);  //catodos largo, columnas, estos funcionan inverso, para que el led se encienda debe ser 0 y no 1
    shiftOut(datos,clock,MSBFIRST,*fila);     //Anodos altura - filas informacion presente en matrizFil
    digitalWrite(latch,HIGH);                 //pasamos los datos a las salidas Q0 - Q7
    digitalWrite(latch,LOW);

}

unsigned int time() {

    unsigned int t;

    while(true) {
        Serial.println("Ingrese el tiempo de retardo (en segundos): ");
        while(Serial.available() == 0){}
        t = Serial.parseInt();

        if(t > 0) {
            Serial.print("Tiempo elegido (s): ");
            Serial.println(t);
            break;
        }
        else {
            Serial.println("Valor Incorrecto");
        }
    }
    return t*1000;

}

unsigned int repeticion() {

    unsigned int r;

    while(true) {
        Serial.println("Ingrese la cantidad de repeticiones (veces): ");
        while(Serial.available() == 0){}
        r = Serial.parseInt();

        if(r > 0) {
            Serial.print("Rep: ");
            Serial.println(r);
            break;
        }
        else {
            Serial.println("Valor Incorrecto");
        }
    }
    return r;

}

unsigned char *patron1(){

    unsigned char *patron = new unsigned char[8]; //creamos un arreglo dinamico de 8 posiciones tipo char
    unsigned char binario;     // binario será nuestra variable que representará un byte
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

unsigned char *patron2() {

    unsigned char *patron = new unsigned char[8];
    unsigned char binario;
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

unsigned char *patron3() {

    unsigned char *patron = new unsigned char[8];
    unsigned char binario;
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

unsigned char *patron4() {

    unsigned char *patron = new unsigned char[8];
    unsigned char binario = 0b00000000;
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

unsigned char *patron0() {
    unsigned char *patron = new unsigned char[8];
    for(int i = 0; i < 8; i++) {
        patron[i] = 0b00000000;
    }
    return patron;

}

void  patronUser() {

    unsigned int tiempo, rep, vez = 1;
    unsigned int *patronCrudo = new unsigned int[8];
    unsigned char *patron = new unsigned char[8];

    tiempo = time();
    rep = repeticion();
    for(int i = 0; i < 8; i++) {
        while(true) {
            Serial.print("(");
            Serial.print(vez);
            Serial.print(")");
            Serial.println("Ingrese un numero entre 0 y 255: ");
            while(Serial.available() == 0){}
            patronCrudo[i] = Serial.parseInt();

            if((patronCrudo[i] >= 0) && (patronCrudo[i] <= 255)) {
                vez++;
                break;
            }
            else {
                Serial.println("Valor Incorrecto");
            }
        }
    }
    patronArbitrario(patronCrudo, patron);
    delete []patronCrudo;
    dibujarEstatico(tiempo, rep, patron);
    delete []patron;

}

void borrarPatron(unsigned char *ptr) {
    delete []ptr;

}

void dibujarEstatico(int tiempo, int rep, unsigned char *col) {

    unsigned long punto1;                                   //nuestra variable que tomara una medida del tiempo
    int ciclo = 0;                                              //Nuestra variable contadora de ciclos
    bool estado = true;                                     //variable llave de paso

    punto1 = millis();                                      //medimos el tiempo antes del ciclo
    while(ciclo < rep) {
        if(millis() - punto1 < tiempo) {                    //comprobamos que el tiempo transcurrido es menor que el indicado
            if(estado){                                     //llave de paso
                for(int i = 0; i < 8; i++) {
                    dibujar(&matrizFil[i], &col[i]);        //funcion dibujo
                }
            }

        }
        else {
            dibujar(&matrizFil[8], &col[0]);                //apagar leds
            estado = !estado;
            punto1 = millis();

            if(estado) {
                ciclo++;
            }
        }
    }
}



void dibujarSecuencia(int tiempo, int rep) {

    unsigned long punto1;                                  //variable de tiempo
    unsigned char *pat1, *pat2, *pat3, *pat4;              //patrones predefinidos
    int ciclo = 0;                                         //variable de contol

    pat1 = patron1();                                      //cargamos los patrones
    pat2 = patron2();
    pat3 = patron3();
    pat4 = patron4();

    while(ciclo < rep) {
        punto1 = millis();                                 //medimos el tiempo
        while(true) {
            if(millis() - punto1 < tiempo) {               //dibujamos un patron
                for(int i = 0; i < 8; i++) {
                    dibujar(&matrizFil[i], &pat1[i]);
                }
            }
            else {
                break;
            }
        }
        punto1 = millis();
        while(true) {
            if(millis() - punto1 < tiempo) {
                for(int i = 0; i < 8; i++) {
                    dibujar(&matrizFil[i], &pat2[i]);
                }
            }
            else {
                break;
            }
        }
        punto1 = millis();
        while(true) {
            if(millis() - punto1 < tiempo) {
                for(int i = 0; i < 8; i++) {
                    dibujar(&matrizFil[i], &pat3[i]);
                }
            }
            else {
                break;
            }
        }
        punto1 = millis();
        while(true) {
            if(millis() - punto1 < tiempo) {
                for(int i = 0; i < 8; i++) {
                    dibujar(&matrizFil[i], &pat4[i]);
                }
            }
            else {
                ciclo++;
                break;
            }
        }
    }

    dibujar(&matrizFil[8], &matrizFil[8]);             //limpiamos la matriz
    borrarPatron(pat1);                                //liberamos la memoria
    borrarPatron(pat2);
    borrarPatron(pat3);
    borrarPatron(pat4);

}

void verificacion() {

    unsigned int tiempo, repeticiones;
    unsigned char *col;

    tiempo = time();
    repeticiones = repeticion();
    col = patron0();
    dibujarEstatico((tiempo), repeticiones, col);
    borrarPatron(col);

}

void patronArbitrario(unsigned int *ptr, unsigned char *ptchar) {

    for(int i = 0; i < 8; i++) {
        ptchar[i] = ~ptr[i];
    }

}



void secuencia() {

    unsigned int tiempo, repeticiones;

    tiempo = time();
    repeticiones = repeticion();

    dibujarSecuencia(tiempo, repeticiones);

}

void publik() {
    int opcion = 0;
    Serial.println(mensaje0);
    while((Serial.available()) == 0){}
    opcion = Serial.parseInt();
    Serial.print(mensaje2);
    Serial.println(opcion);

    switch(opcion) {
    case 1:
        verificacion();
        break;
    case 2:
        patronUser();
        break;
    case 3:
        secuencia();
        break;
    default:
        Serial.println("Valor incorrecto, intente otra vez");
    }

}

void setup() {

    Serial.begin(9600);                                          //inicializamos puerto serie
    pinMode(latch,OUTPUT);                                       //inicializamos los puertos de comunicacion con la matriz
    pinMode(clock,OUTPUT);
    pinMode(datos,OUTPUT);

}

void loop() {

    publik();

}
