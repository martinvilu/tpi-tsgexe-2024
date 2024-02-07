/*
Trabajo integrador final
TOMAS SANTIAGO GONZALEZ
Modalidad libre
Programacion 1 - Ingenieria en computacion
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int **tablero;
    int **descubiertas;
    int filas;
    int columnas;
    int minas;
    int puntos;
} EstadoJuego;

/** convertir_coordenadas
 * Funcion que convierte los datos ingresados (de tipo char) a datos que el
 * resto de funciones pueden interpretar correctamente (tipo int)
 * 
 * Se utiliza sscanf para tomar los numeros ingresados de formato char y 
 * se transforman en numeros enteros que el programa puede usar
 * 
 * @param coordenadas Cadena que representa las coordenadas en formato "fila:columna"
 * @param fila Puntero a la variable donde se almacenara el valor de la fila
 * @param columna Puntero a la variable donde se almacenara el valor de la columna.
 * 
 * PRECONDICIONES:
 * -Los datos ingresados deben seguir el formato "numero:numero"
 * POSTCONDICIONES:
 * -La funcion lee los datos ingresados y los convierte a datos que el resto del
 * codigo puede utilizar     
*/
void convertir_coordenadas(const char *coordenadas, int *fila, int *columna)
{
    sscanf(coordenadas, "%d:%d", fila, columna);
}

/** configurar_juego
 * Funcion con la que el usuario puede configurar a su gusto algunos parametros
 * del juego como cantidad de filas, columnas y minas que tendra el tablero
 * Tambien se inicializa en 0 la variable "puntos" que ira aumentando conforme
 * el usuario descubra casillas sin minas
 * 
 * @param estado Puntero que apunta al estado del juego, el cual se ira actualziando
 * conforme la partida avance
 * 
 * PRECONDICIONES: 
 * -La struct EstadoJuego debera estar definida correctamente, con todas las variables
 * necesarias
 * POSTCONDICIONES: 
 * -La struct EstadoJuego se actualiza con la configuracion ingresada por el usuario
 * -La variable "Puntos" se inicia en 0 para poder ser utilizada
 * -Si la configuracion ingresada no es valida, se reemplaza con valores predeterminados
*/
void configurar_juego(EstadoJuego *estado)
{
    printf("Ingrese la cantidad de filas: ");
    scanf("%d", &(estado->filas));

    printf("Ingrese la cantidad de columnas: ");
    scanf("%d", &(estado->columnas));

    printf("Ingrese la cantidad de minas: ");
    scanf("%d", &(estado->minas));

    estado->puntos = 0;

    if (estado->filas <= 0 || estado->columnas <= 0 || estado->minas <= 0)
    {
        printf("Configuracion no valida, se utilizaran valores predeterminados.\n");
        estado->filas = 10;
        estado->columnas = 10;
        estado->minas = 10;
    }
}

/** iniciar_tablero
 * Funcion que inicializa el tablero del buscaminas asignando la memoria necesaria
 * y configurando el tablero tanto principal como de casillas descubiertas
 * Tambien se colocan las minas de manera aleatoria por el tablero
 * 
 * @param estado Puntero que apunta al estado del juego, el cual se actualiza con
 * los datos ingresados
 * 
 * PRECONDICIONES: 
 * -Se asigna la memoria necesaria para los tableros principal y descubiertas
 * -Las casillas de ambos tableros se inician en 0 (vacias y no descubiertas)
 * -La cantidad de minas seleccionadas se coloca de manera aleatoria en el tablero
 * POSTCONDICIONES:
 * -Se inician de manera correcta ambos tableros, pricnipal y descubiertas
 * -Se colocan minas en el tablero principal de manera aleatoria
*/
void iniciar_tablero(EstadoJuego *estado)
{
    estado->tablero = (int **)malloc(estado->filas * sizeof(int *));
    estado->descubiertas = (int **)malloc(estado->filas * sizeof(int *));

    for (int i = 0; i < estado->filas; i++)
    {
        estado->tablero[i] = (int *)malloc(estado->columnas * sizeof(int));
        estado->descubiertas[i] = (int *)malloc(estado->columnas * sizeof(int));

        for (int j = 0; j < estado->columnas; j++)
        {
            estado->tablero[i][j] = 0;      // Iniciar casillas como vacias
            estado->descubiertas[i][j] = 0; // Iniciar casillas como no descubiertas
        }
    }

    // Colocar minas de forma aleatoria
    int minas_colocadas = 0;
    while (minas_colocadas < estado->minas)
    {
        int fila = rand() % estado->filas;
        int columna = rand() % estado->columnas;

        if (estado->tablero[fila][columna] == 0)
        {
            estado->tablero[fila][columna] = 1; // Colocar mina
            minas_colocadas++;
        }
    }
}

/** liberar_memoria
 * Funcion que libera la memoria ocupada por los tableros espacio por espacio 
 * Debe llamarse al final del juego
 * 
 * @param estado Puntero al estado del juego donde se liberara su memoria
 * 
 * -PRECONDICIONES:
 * -La struc EstadoJuego debe estar correctamente definida con la informacion
 * de los tableros y su memoria ocupada
 * -POSTCONDICIONES:
 * -Se libera la memoria asignada al tablero principal y al tablero de descubiertas
 * -Los punteros de los tableros se establecen en NULL
*/
void liberar_memoria(EstadoJuego *estado)
{
    for (int i = 0; i < estado->filas; i++)
    {
        free(estado->tablero[i]);
        free(estado->descubiertas[i]);
    }

    free(estado->tablero);
    free(estado->descubiertas);
}

/** contar_adyacentes
 * Funcion que analiza una posicion en el tablero y cuenta las minas adyacentes
 * a esa posicion
 * 
 * @param estado Puntero al estado del juego contenedor del tablero
 * @param fila Fila de la posicion del tablero
 * @param columna Columna de la posicion del tablero
 * 
 * @return El número de minas adyacentes a la posición dada
 * 
 * PRECONDICIONES:
 * -La struct EstadoJuego debe estar correctamente definida, con informacion sobre
 * el tablero y sus dimensiones
 * POSTCONDICIONES:
 * -Devuelve la cantidad de minas adyacentes a la posicion escojida sin
 * modificar el tablero
*/
int contar_adyacentes(const EstadoJuego *estado, int fila, int columna)
{
    int contador = 0;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int nueva_fila = fila + i;
            int nueva_columna = columna + j;

            if (nueva_fila >= 0 && nueva_fila < estado->filas && nueva_columna >= 0 && nueva_columna < estado->columnas)
            {
                contador += estado->tablero[nueva_fila][nueva_columna];
            }
        }
    }

    return contador;
}

/** tablero_final
 * Funcion que imprime en la consola el tablero final al terminar la partida
 * Mostrara las posiciones de las minas con el caracter "*"
 * Mostrara las posiciones de las casillas descubiertas como "X"
 * Mostrara el resto de casillas no descubiertas como "#"
 * 
 * @param estado Puntero al estado de juego donde se guardan todas las posiciones
 * y su informacion
 * 
 * PRECONDICIONES:
 * -La struct EstadoJuego debe estar correctamente definida con la informacion
 * del tablero del momento en que es llamada esta funcion
 * POSTCONDICIONES:
 * -La funcion imprime en la consola el tablero final al tener un "game over",
 * mostrando posiciones descubiertas, posiciones no descubiertas, y todas las minas
 * 
*/
void tablero_final(const EstadoJuego *estado)
{
    printf("  ");
    for (int i = 0; i < estado->columnas; i++)
    {
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < estado->filas; i++)
    {
        printf("%d ", i);

        for (int j = 0; j < estado->columnas; j++)
        {
            if (estado->tablero[i][j] == 1)
            {
                printf("* "); // Mina
            }
            else if (estado->descubiertas[i][j] == 1)
            {
                printf("X "); // Casilla descubierta por el usuario
            }
            else
            {
                printf("# "); // Casilla no descubierta
            }
        }
        printf("\n");
    }
}

/** descubrir_casilla
 * Funcion que descubre una casilla en el tablero
 * Revela el contenido de la casilla y actualiza el estado del juego en el acto
 * Si la casilla no contiene una mina, se llama a la funcion "contar_adyacentes"
 * y se muestra la cantidad de minas adyacentes a la posicion con un numero
 * 
 * @param estado Puntero al estado del juego que se actualizara al descubrir
 * una posicion
 * @param fila Fila de la casilla a descubrir
 * @param columna Columna de la casilla a descubrir
 * 
 * PRECONDICIONES:
 * -La struct EstadoJuego debe estar correctamente definida con informacion
 * sobre el tablero de descubiertas y el estado del juego
 * -Las coordenadas de fila y columna deben estar dentro de los limites
 * establecidos para el tablero
 * POSTCONDICIONES:
 * -La casilla seleccionada es marcada como descubierta
 * -Si la casilla no contiene mina, se coloca un numero para marcar
 * la cantidad de minas adyacentes
 * -Si la casilla contiene mina, se muestra un mensaje de fin de juego y 
 * se llama a la funcion "tablero_final" para imprimir el ultimo tablero
 * antes del game over con la informacion de la partida hasta el momento
 * -Si el usuario pierde, tambien mostrara su puntuacion final
*/
void descubrir_casilla(EstadoJuego *estado, int fila, int columna)
{
    if (estado->descubiertas[fila][columna] == 0)
    {
        estado->descubiertas[fila][columna] = 1;

        if (estado->tablero[fila][columna] == 1)
        {
            printf("¡Has encontrado una mina! Fin del juego.\n");
            tablero_final(estado);
            
            // Imprimir la puntuacion final del jugador
            printf("Puntuacion final: %d \n", estado->puntos);
            
            liberar_memoria(estado);
            exit(0);
        }
        else
        {
            int minas_adyacentes = contar_adyacentes(estado, fila, columna);
            printf("La casilla NO tiene mina. Minas adyacentes: %d \n", minas_adyacentes);

            // Otorgar 1 punto al jugador por cada casilla sin mina despejada
            estado->puntos++;

            // Imprimir la puntuacion actual del jugador
            printf("Puntuacion actual: %d\n", estado->puntos);
        }
    }
    else
    {
        printf("Casilla ya descubierta, elige otra casilla.\n");
    }
}

/** imprimir_tablero
 * Funcion que imprime el tablero de juego en su estado actual
 * en cada iteracion, mostrando las casillas descubiertas mas informacion
 * sobre minas y casillas adyacentes
 * 
 * @param estado Puntero al estado del juego con la informacion sobre el tablero
 * 
 * PRECONDICIONES:
 * -La struct EstadoJuego debe estar correctamente definida con la informacion
 * sobre el estado del juego
 * POSTCONDICIONES:
 * -Se imprime en la consola un tablero con el estado actual del juego
*/
void imprimir_tablero(const EstadoJuego *estado)
{
    for (int i = 0; i < estado->filas; i++)
    {
        for (int j = 0; j < estado->columnas; j++)
        {
            if (estado->descubiertas[i][j])
            {
                if (estado->tablero[i][j] == 1)
                {
                    printf("* "); // Mina
                }
                else
                {
                    int minas_adyacentes = contar_adyacentes(estado, i, j);
                    printf("%d ", minas_adyacentes); // Numero de minas adyacentes
                }
            }
            else
            {
                printf("# "); // Casilla no descubierta
            }
        }
        printf("\n");
    }
}

/** guardar_partida
 * Funcion que al ser llamada, guarda la partida del usuario en un archivo que contiene
 * la informacion sobre las dimensiones del tablero, el progreso hasta el momento
 * de guardado, y la cantidad de puntos acumulados
 * 
 * @param estado Puntero al estado actual del juego, donde se encuentra la informacion
 * que se guardara en el archivo
 * @param nombre_archivo Nombre del archivo donde se guardara la partida
 * 
 * PRECONDICIONES:
 * -"estado" debe dirigir a informacion valida que pueda ser guardada en el archivo
 * -El nombre de nombre_archivo tiene que poder ser leído por el programa
 * POSTCONDICIONES:
 * -Se crea un archivo con el nombre asignado a "nombre_archivo", conteniendo
 * todos los datos necesarios para cargar la partida luego
 * -Si el nombre del archivo no se encuentra o no se puede crear, se imprime un
 * mensaje de error
*/
void guardar_partida(const EstadoJuego *estado, const char *nombre_archivo)
{
    FILE *archivo = fopen(nombre_archivo, "w");

    if (archivo == NULL)
    {
        printf("Error al abrir el archivo para guardar la partida.\n");
        return;
    }

    // Guardar informacion de configuracion
    fprintf(archivo, "%d %d %d %d\n", estado->filas, estado->columnas, estado->minas, estado->puntos);

    // Guardar estado del tablero
    for (int i = 0; i < estado->filas; i++)
    {
        for (int j = 0; j < estado->columnas; j++)
        {
            fprintf(archivo, "%d ", estado->descubiertas[i][j]);
        }
        fprintf(archivo, "\n");
    }

    fclose(archivo);
    printf("Partida guardada con exito.\n");
}

/** cargar_partida
 * Funcion que se complementa con "guardar_partida", dando la posibilidad de cargar
 * la partida anteriormente guardada para poder retomarla desde donde el usuario la dejo
 * 
 * @param estado
 * @param nombre_archivo
 * 
 * PRECONDICIONES:
 * -"estado" debe apuntar a una estructura valida
 * -El archivo a cargar tiene que poder ser leido por el programa para poder ser cargado
 * POSTCONDICIONES:
 * -Estado se actualiza con la informacion contenida en el archivo cargado
 * -Si no se encuentra el archivo con el guardado, se imprime un mensaje de error
*/
void cargar_partida(EstadoJuego *estado, const char *nombre_archivo)
{
    FILE *archivo = fopen(nombre_archivo, "r");

    if (archivo == NULL)
    {
        printf("No se encontro la partida guardada.\n");
        return;
    }

    // Leer informacion de configuracion
    fscanf(archivo, "%d %d %d %d", &(estado->filas), &(estado->columnas), &(estado->minas), &(estado->puntos));

    // Reiniciar tablero y descubiertas
    iniciar_tablero(estado);

    // Leer estado del tablero
    for (int i = 0; i < estado->filas; i++)
    {
        for (int j = 0; j < estado->columnas; j++)
        {
            fscanf(archivo, "%d", &(estado->descubiertas[i][j]));
        }
    }

    fclose(archivo);
}

int main()
{
    srand(1);

    EstadoJuego estado;

    int opcion;
    printf("--- BUSCAMINAS ---\n");
    printf("\n1. Nueva partida\n2. Cargar partida\n3. Salir \nSeleccione una opcion: ");
    scanf("%d", &opcion);

    switch (opcion)
    {
    case 1:
        configurar_juego(&estado);
        iniciar_tablero(&estado);

        char decisionGuardar;
        printf("\nPara guardar la partida en cualquier momento del juego, ingrese 's' en la consola.\n");

        while (1)
        {
            imprimir_tablero(&estado);

            char entrada[10];
            printf("Ingrese fila y columna para descubrir (ejemplo: 0:1): ");
            scanf("%s", entrada);

            if (entrada[0] == 's' || entrada[0] == 'S')
            {
                printf("Opciones de guardado:\n1. Guardar partida\n2. Cancelar\nSeleccione una opcion: ");
                
                int opcion_guardar;
                scanf("%d", &opcion_guardar);

                if (opcion_guardar == 1)
                {
                    char nombre_archivo[20];
                    printf("Ingrese el nombre de la partida a guardar: ");
                    scanf("%s", nombre_archivo);
                    guardar_partida(&estado, nombre_archivo);
                    liberar_memoria(&estado);
                    return 0;
                }
                else if (opcion_guardar == 2)
                {
                    printf("Guardado cancelado.\n");
                }
                else
                {
                    printf("Opcion no valida, volviendo al juego.\n");
                }
            }
            else
            {
                int fila, columna;
                convertir_coordenadas(entrada, &fila, &columna);

                if (fila >= 0 && fila < estado.filas && columna >= 0 && columna < estado.columnas)
                {
                    descubrir_casilla(&estado, fila, columna);
                }
                else
                {
                    printf("Coordenadas fuera de rango, intentelo de nuevo.\n");
                }
            }
        }

        liberar_memoria(&estado);
        break;

    case 2:
    {
        char nombre_archivo[20];
        printf("Ingrese el nombre de su partida guardada: ");
        scanf("%s", nombre_archivo);
        cargar_partida(&estado, nombre_archivo);
        printf("Partida cargada con exito.\n");

        while (1)
        {
            imprimir_tablero(&estado);

            char entrada[10];
            printf("Ingrese fila y columna para descubrir (ejemplo: 0:1): ");
            scanf("%s", entrada);

            if (entrada[0] == 's' || entrada[0] == 'S')
            {
                printf("Opciones de guardado:\n1. Guardar partida\n2. Cancelar\nSeleccione una opcion: ");
                
                int opcion_guardar;
                scanf("%d", &opcion_guardar);

                if (opcion_guardar == 1)
                {
                    char nombre_archivo[20];
                    printf("Ingrese el nombre de la partida a guardar: ");
                    scanf("%s", nombre_archivo);
                    guardar_partida(&estado, nombre_archivo);
                    liberar_memoria(&estado);
                    return 0;
                }
                else if (opcion_guardar == 2)
                {
                    printf("Guardado cancelado.\n");
                }
                else
                {
                    printf("Opcion no valida, volviendo al juego.\n");
                }
            }
            else
            {
                int fila, columna;
                convertir_coordenadas(entrada, &fila, &columna);

                if (fila >= 0 && fila < estado.filas && columna >= 0 && columna < estado.columnas)
                {
                    descubrir_casilla(&estado, fila, columna);
                }
                else
                {
                    printf("Coordenadas fuera de rango, intentelo de nuevo.\n");
                }
            }
        }

        liberar_memoria(&estado);
        break;
    }

    case 3:
    {
        printf("Saliendo del programa...");
        break;
    }

    default:
        printf("La opcion seleccionada no es valida, intente nuevamente.");
        break;
    }

    return 0;
}

