#include <iostream> // Biblioteca est�ndar de entrada/salida de C++
#include <fstream> // Biblioteca para ficheros
#include <string> // Biblioteca para cadenas
using namespace std;

const int FILA = 9, COLUMNA = 9; // Declaraci�n de constantes para definir la dimensi�n de los Arrays
typedef int tEnteros[FILA][COLUMNA]; // Array bidimensional de enteros de 9 * 9
typedef bool tBooleanos[FILA][COLUMNA]; // Array bidimensional de booleanos de 9 * 9

typedef struct { // Estructura del Sudoku
    int cVacias; // N�mero de casillas vac�as
    tEnteros iniciales; // Array de enteros donde se guarda la partida actual
    tBooleanos booleanos; // Array de booleanos para verificar si las casillas se pueden modificar o no
} tTablero;

bool abrirArchivos(ifstream &archivoInicial, ifstream &archivoSolucion); // Devuelve true si los ficheros se han abierto correctamente
bool abrirFicheros(ifstream &archivoInicial, ifstream &archivoSolucion); // Abre los ficheros por defecto si se ha pulsado '\n'
bool abrirOtrosFicheros(ifstream &archivoInicial, ifstream &archivoSolucion, string ficheroIni, string ficheroSol); // Abre otros ficheros en el caso de haber introducido el nombre
void inicializarEstructuras(tTablero &tablero, tEnteros solucion); // Inicializa la estructura y arrays del Sudoku a '0'
void cargarArchivoInicial(ifstream &archivoInicial, tTablero &tablero); // Guarda los n�meros del archivo inicial en el array tEnteros iniciales
void cargarArchivoSolucion(ifstream &archivoSolucion, tEnteros solucion); // Guarda los n�meros del archivo soluci�n en el array tEnteros solucion
void mostrarTablero(tTablero tablero, tEnteros solucion); // Muestra el tablero del Sudoku en pantalla
int Menu(); // Muestra el men� del Sudoku en pantalla y devuelve la opci�n elegida
void verValorCasilla(tTablero &tablero); // Busca los posibles valores del Sudoku, acorde a una determinada posici�n, bas�ndose en las funciones booleanas: mirarFilas(), mirarColumnas(), mirarRegion()
void pedirDatos(int &fila, int &columna); // Solicita una posici�n del Sudoku, determinando fila y columna
void candidatos(tTablero &tablero, int &fila, int &columna); // Mira si la casilla solicitada esta vac�a y verifica los posibles candidatos
bool mirarFilas(tTablero tablero, int num, int fila, int columna); // Recorre las filas del Sudoku para verificar que el n�mero buscado no se encuentra en la fila
bool mirarColumnas(tTablero tablero, int num, int fila, int columna); // Recorre las columnas del Sudoku para verificar que el n�mero buscado no se encuentra en la columna
bool mirarRegion(tTablero tablero, int num, int fila, int columna); // Recorre la regi�n del Sudoku para verificar que el n�mero buscado no se encuentra en la regi�n
int pedirNumero(int &num); // Pide un n�mero del 1 al 9 para colocar en el Sudoku y devuelve el n�mero introducido
void colocarValorCasilla(tTablero &tablero); // Procedimiento que colola un n�mero en el tablero
void borrarValorCasilla(tTablero &tablero); // Borra el valor de una casilla, es decir, sustituye el valor por '0'
void mostrarValoresIncorrectos(tTablero tablero, tEnteros solucion); // Muestra los valores incorrectos en pantalla, comparando con el array tEnteros solucion
void reiniciarTablero(tTablero &tablero, tEnteros solucion, ifstream &archivoInicial, ifstream &archivoSolucion); // Procedimiento que reinicia el tablero al estado inicial del archivo inicial
bool casillaVacia(tTablero tablero, int fila, int columna); // Devuelve true si una casilla se encunentra vac�a, es decir, contiene un '0'
void completarCasillas(tTablero &tablero); // Completa las casillas simples recorriendo fila, columna y regi�n
bool completado(tTablero tablero, tEnteros solucion); // Verifica que el sudoku ha sido completado correctamente comparando con el array tEnteros solucion
void cerrarPrograma(bool &terminar); // Procedimiento que determina el final del programa

int main() {

    ifstream archivoInicial; // Archivo "sudoku1.txt"
    ifstream archivoSolucion; // Archivo "solsdk1.txt"
    tTablero tablero; // Array de la estructura del Sudoku
    tEnteros solucion; // Array que guarda la soluci�n del Sudoku, respecto al archivo soluci�n. Se utiliza expl�citamente en la opci�n "4.- Mostrar valores incorrectos"
    bool terminar = false; // Variable local que determina cu�ndo debe finalizar el programa

    if (abrirArchivos(archivoInicial, archivoSolucion)) { // Verifica si se han abierto correctamente los archivos Inicial y Soluci�n
        inicializarEstructuras(tablero, solucion);
        cargarArchivoInicial(archivoInicial, tablero);
        cargarArchivoSolucion(archivoSolucion, solucion);

    } else cerrarPrograma(terminar); // Si no se abren correctamente los archivos, se finaliza el programa

    while (!terminar) {

        mostrarTablero(tablero, solucion);

        if (tablero.cVacias == 0 && completado(tablero, solucion)) // Cuando el n� de casillas vac�as sea '0', se comprueba si el Sudoku ha sido resuelto correctamente
            cerrarPrograma(terminar);

        else {

            int option = Menu();

            switch (option) {

                case 0: cerrarPrograma(terminar);
                    break;
                case 1: verValorCasilla(tablero);
                    break;
                case 2: colocarValorCasilla(tablero);
                    break;
                case 3: borrarValorCasilla(tablero);
                    break;
                case 4: mostrarValoresIncorrectos(tablero, solucion);
                    break;
                case 5: reiniciarTablero(tablero, solucion, archivoInicial, archivoSolucion);
                    break;
                case 6: completarCasillas(tablero);
                    break;
            }

        }
    }

    return 0; // Fin del programa
}

bool abrirArchivos(ifstream &archivoInicial, ifstream &archivoSolucion) {

    bool exito;
    string inicial, solucion, ficheroIni, ficheroSol;
    inicial = "Nombre del sudoku (Intro -> archivo por defecto 'sudoku1.txt'): ";
    solucion = "Nombre del la solucion (Intro -> archivo por defecto 'solsdk1.txt'): ";

    cin.clear();
    cout << endl << inicial;
    getline(cin, ficheroIni); // Escribimos el nombre del archivo Inicial
    cout << endl << solucion;
    getline(cin, ficheroSol); // Escribimos el nombre del archivo Soluci�n

    if (ficheroIni == "" && ficheroSol == "")
        exito = abrirFicheros(archivoInicial, archivoSolucion);
    else
        exito = abrirOtrosFicheros(archivoInicial, archivoSolucion, ficheroIni, ficheroSol);

    return exito;
}

bool abrirFicheros(ifstream &archivoInicial, ifstream &archivoSolucion) {

    bool exito = true;
    archivoInicial.open("sudoku1.txt");
    archivoSolucion.open("solsdk1.txt");
    if (!archivoInicial.is_open() && !archivoSolucion.is_open()) {
        cout << "Fallo al cargar el archivo con el estado inicial del Sudoku" << endl;
        exito = false;
    }

    return exito;

}

bool abrirOtrosFicheros(ifstream &archivoInicial, ifstream &archivoSolucion, string ficheroIni, string ficheroSol) {

    bool exito = true;
    archivoInicial.open(ficheroIni);
    archivoSolucion.open(ficheroSol);
    if (!archivoInicial.is_open() && !archivoSolucion.is_open()) {
        cout << "Fallo al cargar el archivo con el estado inicial del Sudoku" << endl;
        exito = false;
    }

    return exito;
}

void inicializarEstructuras(tTablero &tablero, tEnteros solucion) {

    for (int i = 0; i < FILA; i++) {
        for (int j = 0; j < COLUMNA; j++) {
            tablero.booleanos[i][j] = 0;
            tablero.iniciales[i][j] = 0;
            solucion[i][j] = 0;
        }
    }

    tablero.cVacias = 81; // Total de casillas vac�as en el sudoku de 9 * 9 = 81
}

void cargarArchivoInicial(ifstream &archivoInicial, tTablero &tablero) {

    int digito, fila, columna;

    archivoInicial >> fila >> columna >> digito;

    while (!archivoInicial.eof() && tablero.cVacias > 0 && (digito < 10 || digito > 0)) {

        tablero.iniciales[fila - 1][columna - 1] = digito;
        tablero.booleanos[fila - 1][columna - 1] = 1;
        tablero.cVacias = tablero.cVacias - 1;

        archivoInicial >> fila >> columna >> digito;
    }

    if (digito > 9 || digito < 1) cout << "Fallo al cargar el archivo con el estado inicial del Sudoku" << endl;

    archivoInicial.close();
}

void cargarArchivoSolucion(ifstream &archivoSolucion, tEnteros solucion) {

    int digito = 0, cont = 0;

    while (cont < 81) {

        for (int i = 0; i < FILA; i++) {
            for (int j = 0; j < COLUMNA; j++) {
                archivoSolucion >> digito;
                solucion[i][j] = digito;
                cont++;
            }
        }

    }

    if (cont < 81) // Eval�a que se hayan le�do las 81 casillas del Sudoku
        cout << "Fallo al cargar el archivo con el estado inicial del Sudoku" << endl;

    archivoSolucion.close();
}

void mostrarTablero(tTablero tablero, tEnteros solucion) {

    cout << "     1  2  3   4  5  6   7  8  9  " << endl;
    cout << "   -------------------------------" << endl;

    for (int i = 0; i < FILA; i++) {
        cout << " " << i + 1 << " | ";

        for (int j = 0; j < COLUMNA; j++) {

            if (j == 2 || j == 5 || j == 8) {
                if (tablero.iniciales[i][j] == 0) cout << "  ";
                else cout << tablero.iniciales[i][j] << " ";
                cout << "| ";
            } else {
                if (tablero.iniciales[i][j] == 0) cout << "   ";
                else cout << tablero.iniciales[i][j] << "  ";
            }
        }
        cout << endl;

        if (i == 2 || i == 5 || i == 8)
            cout << "   -------------------------------" << endl;
    }

}

int Menu() {

    cout << endl;
    cout << "0.- SALIR" << endl;
    cout << "1.- Ver posibles valores de casilla" << endl;
    cout << "2.- Colocar un valor de una casilla" << endl;
    cout << "3.- Borrar el valor de una casilla" << endl;
    cout << "4.- Mostrar valores incorrectos" << endl;
    cout << "5.- Reiniciar tablero" << endl;
    cout << "6.- Completar casillas simples" << endl;
	cout << "Opcion: ";

    int option;
    cin >> option;

    return option;
}

// OPCI�N 1 - Ver posibles valores de casilla

void verValorCasilla(tTablero &tablero) {

    int fila, columna;
    pedirDatos(fila, columna);
    candidatos(tablero, fila, columna);
}

void pedirDatos(int &fila, int &columna) {

    do {
        cout << endl << "Introduce fila y columna [1...9]: ";
        cin >> fila >> columna;
    } while (fila > 9 || fila < 1 || columna > 9 || columna < 1);

    fila = fila - 1;
    columna = columna - 1;
}

void candidatos(tTablero &tablero, int &fila, int &columna) {

    if (!casillaVacia(tablero, fila, columna))
        cout << "NO MODIFICABLE" << endl;

    else {

        for (int num = 1; num < 10; num++) {
            if (!mirarFilas(tablero, num, fila, columna) && !mirarColumnas(tablero, num, fila, columna) && !mirarRegion(tablero, num, fila, columna)) {
                cout << num << " ";
            }
        }

        cout << endl;
    }
}

bool casillaVacia(tTablero tablero, int fila, int columna) {
    bool vacia = true;
    if (tablero.iniciales[fila][columna] != 0) vacia = false;

    return vacia;
}

bool mirarFilas(tTablero tablero, int num, int fila, int columna) {

    bool repetido = false;
    for (int j = 0; j < COLUMNA && !repetido; j++) {
        if (tablero.iniciales[fila][j] == num && columna != j) // Verifica si el n�mero buscado se encuentra en la fila, asumiendo que no est� en la posici�n seleccionada por el usuario
            repetido = true;
    }
    return repetido;
}

bool mirarColumnas(tTablero tablero, int num, int fila, int columna) {
    bool repetido = false;
    for (int i = 0; i < FILA && !repetido; i++) {
        if (tablero.iniciales[i][columna] == num && fila != i) // Verifica si el n�mero buscado se encuentra en la columna, asumiendo que no est� en la posici�n seleccionada por el usuario
            repetido = true;
    }
    return repetido;
}

bool mirarRegion(tTablero tablero, int num, int fila, int columna) {

    bool repetido = false;
    int filaInicial = 0, columnaInicial = 0;

    if (fila / 3 == 1) { // Si la divisi�n entera es == '1'
        filaInicial = 3; // Se encuentra a partir de la fila 4 (3 + 1)
    } else if (fila / 3 == 2) { // Si la divisi�n entera es == '2'
        filaInicial = 6; // Se encuentra a partir de la fila 7 (6 + 1)
    }
    if (columna / 3 == 1) { // Si la divisi�n entera es == '1'
        columnaInicial = 3; // Se encuentra a partir de la columna 4 (3 + 1)
    } else if (columna / 3 == 2) { // Si la divisi�n entera es == '2'
        columnaInicial = 6; // Se encuentra a partir de la columna 7 (6 + 1)
    }

    for (int i = filaInicial; i < filaInicial + 3 && !repetido; i++) { // Recorre desde el primer n�mero de la regi�n, hasta el �ltimo n�mero de la regi�n (n�mero inicial + 3)
        for (int j = columnaInicial; j < columnaInicial + 3 && !repetido; j++) {
            if (tablero.iniciales[i][j] == num)
                repetido = true;
        }
    }

    return repetido;
}

// OPCI�N 2 - Colocar un valor de una casilla

void colocarValorCasilla(tTablero &tablero) {

    int fila, columna, num;
    pedirDatos(fila, columna);

    if (!casillaVacia(tablero, fila, columna)) {
        if (tablero.booleanos[fila][columna] == 0) {
            cout << "CASILLA NO VACIA" << endl;

        } else if (tablero.booleanos[fila][columna] == 1) {
            cout << "NO MODIFICABLE" << endl;
        }

    } else if (casillaVacia(tablero, fila, columna)) {
        pedirNumero(num);
        if (num > 9 || num < 1 || mirarFilas(tablero, num, fila, columna) || mirarColumnas(tablero, num, fila, columna) || mirarRegion(tablero, num, fila, columna)) {
            cout << "DIGITO NO VALIDO" << endl;
        } else {
            tablero.iniciales[fila][columna] = num; // Insertamos el n�mero en el tablero
            tablero.cVacias--; // Decrementamos el n�mero de casillas vac�as
        }
    }
}

int pedirNumero(int &num) {

    cout << "Introduce un valor en el intervalo [1..9]: ";
    cin >> num;

    return num;
}

// OPCI�N 3 - Borrar el valor de una casilla

void borrarValorCasilla(tTablero &tablero) {
    int fila, columna, num;
    pedirDatos(fila, columna);

    if (casillaVacia(tablero, fila, columna))
        cout << "CASILLA VACIA" << endl;

    else {

        if (!casillaVacia(tablero, fila, columna)) {
            if (tablero.booleanos[fila][columna])
                cout << "NO MODIFICABLE" << endl;
            else {
                num = tablero.iniciales[fila][columna];
                tablero.iniciales[fila][columna] = 0; // Borramos el n�mero del tablero
                tablero.cVacias++; // Incrementamos el n�mero de casillas vac�as
            }
        }

    }

}

// OPCI�N 4 - Mostrar valores incorrectos

void mostrarValoresIncorrectos(tTablero tablero, tEnteros solucion) {

    int num;
    bool error = false;


    for (int i = 0; i < FILA; i++) { // Bucle que verifica si hay alg�n n�mero mal colocado en el tablero respecto al archivo soluci�n
        for (int j = 0; j < COLUMNA; j++) {
            if ((tablero.iniciales[i][j] != solucion[i][j]) && (tablero.iniciales[i][j] != 0))
                error = true;
        }
    }

    if (error) { // Imprime errores por pantalla si los hay

        cout << "Errores:" << endl;

        for (int i = 0; i < FILA; i++) { // Bucle que imprime por pantalla los n�meros mal colocados en el tablero respecto al archivo soluci�n
            for (int j = 0; j < COLUMNA; j++) {
                if ((tablero.iniciales[i][j] != solucion[i][j]) && (tablero.iniciales[i][j] != 0)) {
                    num = tablero.iniciales[i][j];
                    cout << "[" << i + 1 << "," << j + 1 << "]" << " " << "(" << tablero.iniciales[i][j] << ")" << endl;
                }
            }
        }

    } else cout << "NO HAY ERRORES" << endl;

}

// OPCI�N 5 - Reiniciar tablero

void reiniciarTablero(tTablero &tablero, tEnteros solucion, ifstream &archivoInicial, ifstream &archivoSolucion) {

    abrirFicheros(archivoInicial, archivoSolucion);
    inicializarEstructuras(tablero, solucion);
    cargarArchivoInicial(archivoInicial, tablero);
    cargarArchivoSolucion(archivoSolucion, solucion);

}

// OPCI�N 6 - Completar casillas simples

void completarCasillas(tTablero &tablero) {

    int cont, fila, col, n, casillas = 0;

    //while (tablero.cVacias != 0) {

    for (int i = 0; i < FILA; i++) {
        for (int j = 0; j < COLUMNA; j++) {
            cont = 0;
            for (int num = 1; num < 10; num++) { // Va probando con los posibles n�meros
                if (!mirarFilas(tablero, num, i, j) && !mirarColumnas(tablero, num, i, j) && !mirarRegion(tablero, num, i, j) && casillaVacia(tablero, i, j)) {
                    cont++;
                    fila = i; // Guardamos la fila en 'fila' para no perder su posici�n
                    col = j; // Guardamos la columna en 'col' para no perder su posici�n
                    n = num; // Guardamos el n�mero en 'n' para no perder el n�mero
                }

            }

            if (cont == 1) { // Verifica que s�lo se ha encontrado un n�mero que posible respecto a fila, columna y regi�n
                tablero.iniciales[fila][col] = n;
                casillas++; // Incrementamos el n�mero de casillas rellenadas
                tablero.cVacias--; // Decrementamos las casillas vac�as
            }

        }

    }
    //}

    cout << " Se han rellenado " << casillas << " casillas " << endl;
}

bool completado(tTablero tablero, tEnteros solucion) {

    bool error = false, completado = false;

    for (int i = 0; i < FILA; i++) {
        for (int j = 0; j < COLUMNA; j++) {
            if ((tablero.iniciales[i][j] != solucion[i][j]) && (tablero.iniciales[i][j] != 0)) // Recorremos el Sudoku comparando el tablero con el array que contiene la soluci�n
                error = true;
        }
    }

    if (!error) { // Si no hay errores...
        completado = true;
        cout << "ENORABUENA HAS CONSEGUIDO SOLUCIONARLO" << endl;
    }

    return completado;
}

void cerrarPrograma(bool &terminar) {
    // Codigo para hacer que el programa no se cierre hasta que pulses una tecla
    cin.sync(); // Limpia el buffer de cin
    cout << "Pulsa Intro para terminar";
    char c;
    cin.get(c);
    terminar = true; // Modificamos la variable local 'terminar', para poder salir del bucle del programa principal y terminar la ejecuci�n del programa
}
