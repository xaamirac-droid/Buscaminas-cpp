#include <iostream>
#include <time.h>
#include <string>

using namespace std;

const int MAX_JUGADORES = 100; //Maximo numero de jugadores
const int MAX_PARTIDAS = 100; //Maximo numero de partidas
const int MAX_JUGADORES_INACTIVOS =100;

struct Jugador{
    string nombre;
    string apellido;
    string nickname;
    string ci; 
    string fechaNacimiento;
    string contrasena;
    bool activo;
    int partidasGanadas =0;
    int partidasPerdidas =0;
    int partidasAbandonadas =0;
};

struct Partida{
    string fecha;
    string nickname;
    string resultado; //Partida Ganada, Perdida, Abandono
};

//Arreglo para almacenar los jugadores y un contador de jugadores
Jugador jugadores[MAX_JUGADORES];
int numerodeJugadores = 0;

//Arreglo de jugador inactivos para cuando se da de baja
Jugador jugadoresInactivos[MAX_JUGADORES_INACTIVOS];
int numJugadoresInactivos=0;

//Arreglo para almacenar las partidas
Partida partidas[MAX_PARTIDAS];
int numerodePartidas= 0;

//Arreglo para asignar el resultado de las partidas
string resultadoPartida[3]={"ganada","perdida","abandonada"};


//FUNCIONES DE GESTION DE LOS JUGADORES Y DE LAS PARTIDAS
bool CIValida(string ci);
bool fechaValida(string fecha);
void altaJugador();
//Funcion para buscar la cedula del jugador en el arreglo de jugadores inactivos, para darle el alta sin pedirle los datos nuevamente
int buscarJugadorInactivoPorcedula(string ci);
void bajaJugador();
void modificarJugador();
void listarJugadores();
void listarPartidas();
void listarPartidasPorJugador();
void listarPartidasPorFecha();
void IniciarJuego();

void mostrarMenu();

//FUNCIONES DEL JUEGO
void inicializarTableroVisual(char t[8][8]);//Inicializamos el tablero de visualizacion con 'B' (Tablero que visualiza el jugador, le muestra que casillas ha descubierto y cuales no)
void imprimirTablero(char y[8][8]); //Imprime cualquier tablero
void inicializarTableroReal(char y[8][8]); //Inicializamos el tablero real con '0' (Tablero en el cual realizamos la logica del juego)
void generadordeMinas(char t[8][8]); //genera minas en el tablero
void jugada(int x, int y, char tableroReal[8][8], char tableroVisual[8][8], bool primerMovimiento); //Obtiene la entrada del jugador y revela/marca/desmarca casilla segun lo que puso el usuario
void revelarCasillaCuando0(int x, int y, char tableroReal[8][8], char tableroVisual[8][8]); //Revelamos las casillas adyacentes a una casilla '0'
void marcarCasilla(int x, int y, char tableroReal[8][8], char tableroVisual[8][8]); //Marcar casilla con una bandera
void desmarcarCasilla(int x, int y, char tableroReal[8][8], char tableroVisual[8][8]); //Descamrcar casilla 
void descubrirCasilla(int x, int y, char tableroReal[8][8], char tableroVisual[8][8], bool primerMovimiento); //Descubre una casilla
bool partidaGanada(char tableroReal[8][8], char tableroVisual[8][8]); // Comprueba si el jugador gano
bool partidaPerdida(char tableroReal[8][8], char tableroVisual[8][8]); //Comprueba si el jugador perdio
void controlarPrimerMovimiento(int x, int y, char tableroReal[8][8]); //Asegura que la casilla descubierta no tenga ninguna bomba, vuelve a hacer el tablero en caso que sea necesario
int marcadas=0; //contador para las casillas marcadas
bool perdio=false;
bool primerMovimiento= true; //al principio se establece en true, porque no se ha realizado ningun movimiento aun

int main() {
    mostrarMenu();
    IniciarJuego();

    return 0;
}

void IniciarJuego() {
    string nickname, contrasena;
    cout << "Ingrese su nickname: ";
    cin >> nickname;
    cout << "Ingrese su contrasena: ";
    cin >> contrasena;

    bool jugadorEncontrado = false;
    string fechaJugada, resultado;
    int posicionJugador;
    for (int i = 0; i < numerodeJugadores; i++) {
        if (jugadores[i].nickname == nickname && jugadores[i].contrasena == contrasena && jugadores[i].activo) {
            cout << "Iniciando la partida..." << endl;
            posicionJugador = i;
            jugadorEncontrado = true;
            break;
        }
    }

    if (!jugadorEncontrado) {
        cout << "Autenticacion fallida. Nickname o contrasena incorrectos o jugador inactivo." << endl;
        return;
    }

    srand(time(0));
    do {
        cout << "Escriba la fecha en la que esta jugando, en este formato dd/mm/aaaa: " << endl;
        cin >> fechaJugada;
    } while (!fechaValida(fechaJugada));

    cout << endl;
    cout << " (☞ﾟヮﾟ)☞ BIENVENIDO AL BUSCAMINAS ☜(ﾟヮﾟ☜)" << endl;
    cout << "→→ El objetivo del juego es descubrir todas las casillas que no contienen minas." << endl;

    char tableroVisual[8][8];
    char tableroReal[8][8];

    inicializarTableroVisual(tableroVisual);
    inicializarTableroReal(tableroReal);
    generadordeMinas(tableroReal);
    primerMovimiento = true;
    perdio = false;

    while (true) {
        imprimirTablero(tableroVisual);

        // Opcion de rendirse
        cout << "Ingrese (R) para rendirse o cualquier otra tecla para continuar: ";
        char opcion;
        cin >> opcion;
        if (opcion == 'R' || opcion == 'r') {
            cout << "Te has rendido. Partida abandonada." << endl;
            resultado = resultadoPartida[2]; // Abandonada
            jugadores[posicionJugador].partidasAbandonadas++;
            break;
        }

        int x, y;
        jugada(x, y, tableroReal, tableroVisual, primerMovimiento);
        primerMovimiento = false;

        if (partidaGanada(tableroReal, tableroVisual)) {
            imprimirTablero(tableroVisual);
            cout << "ヾ(⌐■■)ノ♪ FELICIDADES, ¡HAS GANADO LA PARTIDA!ヾ(⌐■■)ノ♪" << endl;
            resultado = resultadoPartida[0];
            jugadores[posicionJugador].partidasGanadas++;
            break;
        } else if (perdio) {
            imprimirTablero(tableroReal);
            cout << "Has perdido la partida. Vuelve a intentarlo! ಥ_ಥ  ¯\\(ツ)/¯" << endl;
            resultado = resultadoPartida[1];
            jugadores[posicionJugador].partidasPerdidas++;
            break;
        }
    }

    partidas[numerodePartidas].fecha = fechaJugada;
    partidas[numerodePartidas].nickname = nickname;
    partidas[numerodePartidas].resultado = resultado;
    numerodePartidas++;
}
    
void mostrarMenu(){
    int opcion;
    do{
        cout<<"\nMenu principal\n";
        cout<<"1. Alta de jugador\n";
        cout<<"2. Baja de jugador\n";
        cout<<"3. Modificar jugador\n";
        cout<<"4. Listar jugadores\n";
        cout<<"5. Listar todas las partidas\n";
        cout<<"6. Listar partidas por jugador\n";
        cout<<"7. Listar partidas por fecha\n";
        cout<<"8. Jugar\n";
        cout<<"9. Salir\n";
        cout<<"Selecciona una opcion: ";
        cin>>opcion;

        switch(opcion){
            case 1:
                altaJugador();
                break;
            case 2:
                bajaJugador();
                break;
            case 3:
                modificarJugador();
                break;
            case 4:
                listarJugadores();
                break;
            case 5:
                listarPartidas();
                break;
            case 6:
                listarPartidasPorJugador();
                break;
            case 7:
                listarPartidasPorFecha();
                break;
            case 8:
                IniciarJuego();
                break;
            case 9:
                cout<<"Saliendo del juego..."<< endl;
                exit(0); //salir del mostrar menu y por ende salimos del juego
            default:
                cout<<"Opcion invalida. Intente de nuevo."<<endl;
        }
    } while (opcion!=9);
}

bool CIValida (string ci) {
    if (ci.length() != 11)
        return false;
    if (!(ci[1] == '.' && ci[5] == '.'))
        return false;
    if (ci[9] != '-')
        return false;
    for (int i = 0; i < ci.length(); i++) {
        if (i != 1 && i != 5 && i != 9) {
            if (!isdigit(ci[i]))
                return false;
        }
    }
    return true;
}

bool fechaValida(string fecha){
    //Verificar la longitud de la fecha
    int longitud= 0;
    while (fecha[longitud] != '\0'){
        longitud++;
    }
    if (longitud != 10){
        return false; //La longitud debe ser de 10 caracteres
    }

    //Verificamos que los separadores '/' este en las posiciones correctas
    if (fecha[2] != '/'){
        return false; //Los separadores deben estar 3 y 6
    }

    //Verificamos que los demas caracteres sean numeros validos
    for (int i=0; i<10; i++){
        if (i == 2 || i == 5){
            continue; //Ignoramos los separadores '/'
        }
        if (fecha[i] < '0' || fecha[i] > '9'){
            return false; //No es un digito valido
        }
    }

    //Verificamos rangos de dia, mes y año
    int dia = (fecha[0] - '0') * 10 + (fecha[1] - '0');
    int mes = (fecha[3] - '0') * 10 + (fecha[4] - '0');

    if (mes < 1 || mes > 12){
        return false; //El mes debe estar entre 1 y 12
    }
    return true;
}


int buscarJugadorInactivoPorcedula(string ci) {
    for (int i = 0; i < numJugadoresInactivos; ++i) {
        if (jugadoresInactivos[i].ci == ci ) {
            return i; //Jugador encontrado
        }
    }
    return -1; //No se encontro el jugador
}

void altaJugador() {
    if (numerodeJugadores >= MAX_JUGADORES) {
        cout << "No se pueden anadir mas jugadores. Se ha alcanzado el limite maximo." << endl;
        return;
    }

    cout << "Escribe 1 si ya te has registrado anteriormente: " << endl
         << "Escribe 2 si deseas registrarte: " << endl;
    int registro;
    cin >> registro;

    if (registro == 1) {
        cout << "Escribe tu cedula para recuperar tu usuario: " << endl;
        string cedula;
        cin >> cedula;
        int posicion = buscarJugadorInactivoPorcedula(cedula);
        if (posicion != -1) {
            jugadores[numerodeJugadores] = jugadoresInactivos[posicion];
            jugadores[numerodeJugadores].activo = true;
            numerodeJugadores++;

            // Remover el jugador del arreglo de inactivos
            for (int i = posicion; i < numJugadoresInactivos - 1; i++) {
                jugadoresInactivos[i] = jugadoresInactivos[i + 1];
            }
            numJugadoresInactivos--;

            cout << "Jugador reactivado exitosamente." << endl;
            return;
        } else {
            cout << "No se encontro ningun jugador con esa cedula." << endl;
            cout << "Por favor, registrese." << endl;
        }
    }

    Jugador nuevoJugador;
    do{
    cout << "Ingrese su numero de cedula en este formato X.XXX.XXX-X: ";
    cin >> nuevoJugador.ci;
    }while(!CIValida(nuevoJugador.ci));

    cout << "Ingrese el nombre: ";
    cin >> nuevoJugador.nombre;

    cout << "Ingrese el apellido: ";
    cin >> nuevoJugador.apellido;

    cout << "Ingrese el nickname: ";
    cin >> nuevoJugador.nickname;

    for (int i = 0; i < numerodeJugadores; i++) {
        if (jugadores[i].nickname == nuevoJugador.nickname) {
            cout << "El nickname ya esta en uso. Pruebe con otro nickname." << endl;
            return;
        }
    }
    do{
    cout << "Ingrese la fecha de nacimiento (dd/mm/aaaa): ";
    cin >> nuevoJugador.fechaNacimiento;
    }while(!fechaValida(nuevoJugador.fechaNacimiento));

    cout << "Ingrese la contrasena: ";
    cin >> nuevoJugador.contrasena;

    nuevoJugador.activo = true;
    jugadores[numerodeJugadores] = nuevoJugador;
    numerodeJugadores++;

    cout << "Jugador anadido exitosamente." << endl;
}

void bajaJugador() {
    string nickname;
    cout << "Ingrese el nickname del jugador a dar de baja: ";
    cin >> nickname;

    for (int i = 0; i < numerodeJugadores; i++) {
        if (jugadores[i].nickname == nickname && jugadores[i].activo) {
            jugadores[i].activo = false;
            jugadoresInactivos[numJugadoresInactivos++] = jugadores[i];
            
            // Remover el jugador del arreglo de jugadores activos
            for (int j = i; j < numerodeJugadores - 1; j++) {
                jugadores[j] = jugadores[j + 1];
            }
            numerodeJugadores--;
            
            cout << "Jugador dado de baja exitosamente." << endl;
            return;
        }
    }
    cout << "Jugador no encontrado o ya está inactivo." << endl;
}

void modificarJugador() {
    string nickname;
    cout << "Ingresa el nickname del jugador a modificar: ";
    cin >> nickname;

    //Recorrer el arreglo de jugadores usando un indice
    for (int i = 0; i < numerodeJugadores; i++) {
        if (jugadores[i].nickname == nickname) {
            // Permitir la modificacion incluso si el jugador esta inactivo
            Jugador jugadorOriginal = jugadores[i]; // Guardar el jugador original

            int modificar;
            cout << "Desea modificar el nombre? (1) Si, (2) No: ";
            cin >> modificar;
            if (modificar == 1) {
                cout << "Ingrese el nuevo nombre: ";
                cin >> jugadores[i].nombre;
            }

            modificar = 0;
            cout << "Desea modificar el apellido? (1) Si, (2) No: ";
            cin >> modificar;
            if (modificar == 1) {
                cout << "Ingrese el nuevo apellido: ";
                cin >> jugadores[i].apellido;
            }

            modificar = 0;
            cout << "Desea modificar la contrasena? (1) Si, (2) No: ";
            cin >> modificar;
            if (modificar == 1) {
                cout << "Ingrese la nueva contrasena: ";
                cin >> jugadores[i].contrasena;
            }

            int guardar;
            cout << "Desea guardar los cambios? (1) Si, (2) No: ";
            cin >> guardar;
            if (guardar == 1) {
                cout << "Jugador modificado exitosamente." << endl;
            } else {
                jugadores[i] = jugadorOriginal; // Restaurar el jugador original
                cout << "Cambios cancelados." << endl;
            }
            return;
        }
    }

    cout << "Jugador no encontrado o inactivo." << endl;
}

void ordenarPorNickname(Jugador jugadores[], int n) {
    Jugador tmp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1; j++) {
            if (jugadores[j].nickname > jugadores[j+1].nickname) {
                tmp = jugadores[j];
                jugadores[j] = jugadores[j+1];
                jugadores[j+1] = tmp;
            }
        }
    }
}

void listarJugadores() {
    ordenarPorNickname(jugadores, numerodeJugadores); // Llamamos a la funcion para ordenar los jugadores por nickname
    
    cout<< "\nLista de jugadores:\n";
    for (int i= 0; i < numerodeJugadores; i++){ //Recorremos todos los jugadores del arreglo
        cout<< "-------------------------------------"<< endl;
        cout<< "Nombre: "<< jugadores[i].nombre<< endl;
        cout<< "Apellido: "<< jugadores[i].apellido<< endl;
        cout<< "Nickname: "<< jugadores[i].nickname<< endl;
        cout<< "CI: "<< jugadores[i].ci<< endl;
        cout<< "Fecha de Nacimiento: "<< jugadores[i].fechaNacimiento<< endl;
        cout<< "Partidas ganadas: " << jugadores[i].partidasGanadas<<endl;
        cout<< "Partidas perdidas: " << jugadores[i].partidasPerdidas<<endl;
        cout<< "Partidas abandonadas: " << jugadores[i].partidasAbandonadas<<endl;
        cout<< "Estado: ";
        
        //Utilizamos if-else para dar el estado
        if(jugadores[i].activo){
            cout<< "Activo";
        }else{
            cout<< "Inactivo";
        }
        cout<< endl;
        cout<< "-------------------------------------"<< endl;
    }
}

void listarPartidas() {
    cout<<"\nLista de partidas: \n";
    cout<< "-------------------------------------"<< endl;
    for (int i= 0; i < numerodePartidas; i++){
        cout<< "Fecha: "<< partidas[i].fecha<< endl;
        cout<< "Jugador: "<< partidas[i].nickname<< endl;
        cout<< "Resultado: "<< partidas[i].resultado<< endl;
        cout<< "-------------------------------------"<< endl;
    }
}

void listarPartidasPorJugador() {
    string nickname;
    cout<< "Ingrese el nickname del jugador: ";
    cin>> nickname;
    cout<< "\nPartidas del jugador "<< nickname << "\n";

    //Busacamos si se encuentra alguna partida
    bool buscando = false;

    //Recorremos las partidas
    for(int i=0; i < numerodePartidas; i++){
        if(partidas[i].nickname == nickname){
            buscando = true; //Marcamos que se encontro al menos una partida
            cout<< "Fecha: "<< partidas[i].fecha<< endl;
            cout<< "Resultado: "<< partidas[i].resultado<< endl;
        }
    }

    if(!buscando){
        cout<< "No se encontraron partidas para el jugador " << nickname << "." <<endl;
    }
}

void listarPartidasPorFecha() {
    string fecha;
    cout << "Ingrese la fecha (dd/mm/aaaa): ";
    cin >> fecha;

    if (!fechaValida(fecha)) {
        cout << "Fecha invalida." << endl;
        return;
    }

    bool encontrado = false;
    cout << "Partidas del " << fecha << ":" << endl;
    cout << "---------------------------------------------" << endl;
    cout << "Nickname\tResultado" << endl;
    cout << "---------------------------------------------" << endl;
    for (int i = 0; i < numerodePartidas; i++) {
        if (partidas[i].fecha == fecha) {
            encontrado = true;
            cout << partidas[i].nickname << "\t\t" << partidas[i].resultado << endl;
        }
    }

    if (!encontrado) {
        cout << "No se encontraron partidas en la fecha " << fecha << "." << endl;
    }
    cout << "---------------------------------------------" << endl;
}

//Inicializamos el tablero en el cual realizamos la logica del juego
void inicializarTableroReal(char y[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            y[i][j] = '0'; // Inicializa el tablero real con '0'
        }
    }
}
//Inicializamos el tablero que ve el jugador
void inicializarTableroVisual(char t[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            t[i][j] = 'B'; //Inicializa el tablero visual con 'B'
        }
    }
}
//Imprime tablero
void imprimirTablero(char y[8][8]) {
    cout << endl << "    "; // Espacio inicial para los numeros de columna
    for (int j = 0; j < 8; j++) {
        cout << j << "   "; // Imprimir numeros de columna
    }
    cout << endl << "  +"; 
    for (int j = 0; j < 8; j++) {
        cout << "---+"; // Imprimir linea superior del tablero
    }
    cout << endl;

    for (int i = 0; i < 8; i++) {
        cout << i << " | "; // Imprimir numero de fila
        for (int j = 0; j < 8; j++) {
            cout << y[i][j] << " | "; // Imprimir contenido de la casilla
        }
        cout << endl << "  +";
        for (int j = 0; j < 8; j++) {
            cout << "---+"; // Imprimir linea separadora de filas
        }
        cout << endl;
    }
}

// Incrementa casilla solo si no es una mina
void incrementarCasilla(int x, int y, char arr[8][8]) {
    if (x >= 0 && x < 8 && y >= 0 && y < 8) {
        if (arr[x][y] != 'X') { // Incrementa solo si no es una mina
            arr[x][y] = arr[x][y] + 1;
        }
    }
}

//Generamos las minas
void generadordeMinas(char t[8][8]) {
    int cont = 0; //Contador de minas
    int a, b;
    srand(time(0));
    while (cont < 16) {
        a = rand() % 8; //Coordenada aleatoria x
        b = rand() % 8; //Coordenada aleatoria y
        if (t[a][b] != 'X') { // Solo coloca la mina si no esta ya en esa posicion
            t[a][b] = 'X'; //Colocar mina
            //Incrementar el conteo de minas en las casillas adyacentes
            incrementarCasilla(a, b + 1, t);
            incrementarCasilla(a, b - 1, t);
            incrementarCasilla(a - 1, b, t);
            incrementarCasilla(a - 1, b + 1, t);
            incrementarCasilla(a - 1, b - 1, t);
            incrementarCasilla(a + 1, b, t);
            incrementarCasilla(a + 1, b + 1, t);
            incrementarCasilla(a + 1, b - 1, t);
            cont++;
        }
    }
}

//Revelamos las casillas adyacentes cuando sea 0
void revelarCasillaCuando0(int x, int y, char tableroReal[8][8], char tableroVisual[8][8]) {
    if (x >= 0 && x < 8 && y >= 0 && y < 8 && tableroVisual[x][y] == 'B') {
        tableroVisual[x][y] = tableroReal[x][y]; //Revelar la casilla actual
        if (tableroReal[x][y] == '0') { //Si la casilla es 0, revelar adyacentes
            tableroVisual[x][y] = '-';
            revelarCasillaCuando0(x, y + 1, tableroReal, tableroVisual);
            revelarCasillaCuando0(x, y - 1, tableroReal, tableroVisual);
            revelarCasillaCuando0(x - 1, y + 1, tableroReal, tableroVisual);
            revelarCasillaCuando0(x - 1, y, tableroReal, tableroVisual);
            revelarCasillaCuando0(x - 1, y - 1, tableroReal, tableroVisual);
            revelarCasillaCuando0(x + 1, y + 1, tableroReal, tableroVisual);
            revelarCasillaCuando0(x + 1, y, tableroReal, tableroVisual);
            revelarCasillaCuando0(x + 1, y - 1, tableroReal, tableroVisual);
        }
    }
}


//Descubrir casillas en el tablero
void descubrirCasilla(int x, int y, char tableroReal[8][8], char tableroVisual[8][8], bool primerMovimiento) {
    cout << "Escribe el numero de fila: ";
    cin >> x;
    cout << "Escribe el numero de la columna: ";
    cin >> y;

    if (x >= 0 && x < 8 && y >= 0 && y < 8) {
        if(primerMovimiento){ //se verifica si es el primer movimiento del usuario
            controlarPrimerMovimiento(x, y, tableroReal); //si es el primer movimiento, llamamos a controlarPrimerMovimiento para garantizar que en la primera casilla descubierta no haya una bomba
            revelarCasillaCuando0(x, y, tableroReal, tableroVisual);
            return;
        }
        else  if (tableroVisual[x][y] != 'B') {
            cout <<endl<< "Esta casilla ya fue descubierta!" << endl;
        } else if (tableroVisual[x][y] == 'F') {
            cout <<endl<< "Esta casilla ya fue marcada!" << endl;
        } else if (tableroReal[x][y] == '0') {
                revelarCasillaCuando0(x, y, tableroReal, tableroVisual); //Revelar casilla adyacente si es 0
        } else if(tableroReal[x][y]== 'X'){
                    perdio=true;
        } else {
            tableroVisual[x][y]=tableroReal[x][y];
        }
        }else {
        cout <<endl<< "Esta casilla se encuentra fuera de rango!" << endl; 
    }
}

//Marcar casillas en el tablero
void marcarCasilla(int x, int y, char tableroReal[8][8], char tableroVisual[8][8]) {
    if (marcadas >= 16) { //verifica si ya se han marcado 16 casillas, si se quiere marcar mas sale el mensaje
        cout <<endl<< "Ya has marcado 16 casillas, no puedes marcar más!" << endl;
        return;
    }

    cout << "Escribe el numero de fila: ";
    cin >> x;
    cout << "Escribe el numero de la columna: ";
    cin >> y;

    if (x >= 0 && x < 8 && y >= 0 && y < 8) {
            if (tableroVisual[x][y] == 'B') {
            tableroVisual[x][y] = 'F'; //Marcar la casilla con una bandera
            marcadas++; //Aumentar el contador de las casillas marcadas
            } else if (tableroVisual[x][y] == 'F') {
            cout << endl << "Esta casilla ya fue marcada!" << endl;
            }    
    } else {
        cout <<endl<< "Ya has marcado 16 casillas, no puedes marcar más!" << endl; 
    }
}

void desmarcarCasilla(int x, int y, char tableroReal[8][8], char tableroVisual[8][8]) {
    cout << "Escribe el numero de fila: ";
    cin >> x;
    cout << "Escribe el numero de la columna: ";
    cin >> y;

    if (x >= 0 && x < 8 && y >= 0 && y < 8) {
        if (tableroVisual[x][y] == 'F') {
            tableroVisual[x][y] = 'B'; //Desmarcar casilla
            marcadas--; //Disminuir el contador de las casillas marcadas
        } else if (tableroVisual[x][y] != 'F') {
            cout <<endl<< "Esta casilla no esta marcada!" << endl; 
        }
    } else {
        cout <<endl<< "Esta casilla se encuentra fuera de rango!" << endl; 
    }
}

//Jugada que quiere hacer el jugador
void jugada(int x, int y, char tableroReal[8][8], char tableroVisual[8][8], bool primerMovimiento) {
    int opciones;
    cout << endl << "1)Descubrir casilla" << endl;
    cout << "2)Marcar casilla" << endl;
    cout << "3)Desmarcar casilla" << endl;
    cout << "Elegir opcion: ";
    cin >> opciones;
    switch(opciones) {
        case 1:
            descubrirCasilla(x, y, tableroReal, tableroVisual, primerMovimiento);
            break;
        case 2:
            marcarCasilla(x, y, tableroReal, tableroVisual);
            break;
        case 3:
            desmarcarCasilla(x, y, tableroReal, tableroVisual);
            break;
        default:
            cout << "Opción inválida" << endl;
            break;
    }
}


//Booleano en caso de que el jugador gane la partida
bool partidaGanada(char tableroReal[8][8], char tableroVisual[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (tableroVisual[i][j] == 'B' && tableroReal[i][j] != 'X') {
                return false; // Aun hay casillas por descubrir
            }
        }
    }
    return true; // Ya estan todas las casillas descubiertas o marcadas
}

void controlarPrimerMovimiento(int x, int y, char tableroReal[8][8]){
    if (tableroReal[x][y] != '0'){ //verifica si es distinto a 0
        inicializarTableroReal(tableroReal); //se reinicializa el tablero
        generadordeMinas(tableroReal); //se vuelven a generar minas
        while(tableroReal[x][y]!= '0'){ //lo continua reinicializando hasta que sea distinto a 0
            inicializarTableroReal(tableroReal);
            generadordeMinas(tableroReal);
            
        }primerMovimiento=false;
    }
}