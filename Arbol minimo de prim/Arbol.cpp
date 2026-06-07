#include <iostream>
#include <vector>
#include <climits>  // Para usar INT_MAX
#include <cstdlib>  // Para usar system("cls")

using namespace std;

// Definimos el número de nodos del grafo (fijo para el mapa visual)
#define NODOS 5
#define INF INT_MAX

// Función auxiliar para redibujar el grafo mostrando las conexiones seleccionadas (Puntos Extras)
void dibujarGrafoPrim(int nodoActual, bool visitados[], int padre[], int grafo[NODOS][NODOS]) {
    // Limpia la consola en Windows para generar efecto de animación en Dev-C++
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    cout << "---------------------------------------------------------" << endl;
    cout << "     SIMULADOR GRAFICO: ARBOL PARCIAL MINIMO DE PRIM     " << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << " Ultimo nodo agregado al arbol: ";
    if (nodoActual == -1) cout << "NINGUNO (Inicio)";
    else cout << (char)('A' + nodoActual);
    cout << endl << endl;

    // Lógica para detectar si una arista ya pertenece al Árbol de Expansión Mínima
    bool AB = (padre[1] == 0 || padre[0] == 1);
    bool AC = (padre[2] == 0 || padre[0] == 2);
    bool BC = (padre[2] == 1 || padre[1] == 2);
    bool AD = (padre[3] == 0 || padre[0] == 3);
    bool CE = (padre[4] == 2 || padre[2] == 4);

    // Dibujo del Grafo en Arte ASCII dinámico
    // Si la arista fue seleccionada por Prim, se imprime "[===]" en lugar de "---"
    cout << "         ( B ) " << (visitados[1] ? "*Visitado*" : "") << endl;
    cout << "        " << (AB ? " // " : " /  ") << " " << (BC ? " \\\\ " : "  \\ ") << endl;
    cout << "     2 " << (AB ? "//   " : "/    ") << " " << (BC ? "   \\\\ 3" : "    \\") << endl;
    cout << "      " << (AB ? "//     " : "/      ") << " " << (BC ? "     \\\\" : "      \\") << endl;

    cout << " ( A ) " << (AC ? "[=======]" : "---------") << " ( C ) " << (visitados[2] ? "*Visitado*" : "") << endl;
    cout << (visitados[0] ? "*Vis*" : "     ") << "    4    " << (CE ? " \\\\ " : "  \\ ") << endl;
    
    cout << "   " << (AD ? "||" : " |") << "               " << (CE ? "   \\\\ 5" : "    \\") << endl;
    cout << " 7 " << (AD ? "||" : " |") << "                " << (CE ? "     \\\\" : "      \\") << endl;
    
    cout << " ( D ) " << (visitados[3] ? "*Visitado*" : "") << "       ( E ) " << (visitados[4] ? "*Visitado*" : "") << endl;
    cout << "---------------------------------------------------------" << endl;
}		

// Muestra el estado del proceso en cada paso
void mostrarEstadoPasos(bool visitados[], int padre[], int clave[]) {
    cout << "\nEstado de las conexiones en esta iteracion:\n";
    cout << "Nodo:\t\tA\tB\tC\tD\tE\n";
    cout << "En Arbol:\t";
    for (int i = 0; i < NODOS; i++) cout << (visitados[i] ? "SI\t" : "NO\t");
    
    cout << "\nCosto Min:\t";
    for (int i = 0; i < NODOS; i++) {
        if (clave[i] == INF) cout << "INF\t";
        else cout << clave[i] << "\t";
    }
    
    cout << "\nConectado a:\t";
    for (int i = 0; i < NODOS; i++) {
        if (padre[i] == -1) cout << "-\t";
        else cout << (char)('A' + padre[i]) << "\t";
    }
    cout << "\n---------------------------------------------------------\n";
    cout << "Presione ENTER para continuar al siguiente paso...";
    cin.get(); // Pausa interactiva para la defensa de laboratorio
}

// Encuentra el nodo con la clave de peso mínima que no esté en el árbol
int minClave(int clave[], bool visitados[]) {
    int minimo = INF, indiceMinimo = -1;
    for (int v = 0; v < NODOS; v++) {
        if (!visitados[v] && clave[v] < minimo) {
            minimo = clave[v];
            indiceMinimo = v;
        }
    }
    return indiceMinimo;
}

// Algoritmo de Prim con simulación visual integrada
void primSimulador(int grafo[NODOS][NODOS]) {
    int padre[NODOS];   // Almacena el árbol de expansión mínima construido
    int clave[NODOS];   // Valores de peso mínimos para elegir la arista más barata
    bool visitados[NODOS]; // Nodos incluidos oficialmente en el árbol

    // Inicialización compatible con C++98
    for (int i = 0; i < NODOS; i++) {
        clave[i] = INF;
        visitados[i] = false;
        padre[i] = -1;
    }

    // Empezamos por el Nodo A (índice 0)
    clave[0] = 0;
    padre[0] = -1; 

    // Mostrar pantalla inicial de arranque antes de procesar
    dibujarGrafoPrim(-1, visitados, padre, grafo);
    mostrarEstadoPasos(visitados, padre, clave);

    for (int count = 0; count < NODOS; count++) {
        // Seleccionar el nodo con el peso mínimo que no esté en el árbol
        int u = minClave(clave, visitados);
        if (u == -1) break;

        // Agregar el nodo seleccionado al árbol
        visitados[u] = true;

        // Redibujar gráfico mostrando el nuevo nodo e hilo incorporado
        dibujarGrafoPrim(u, visitados, padre, grafo);
        cout << ">> Evaluando aristas salientes del nodo " << (char)('A' + u) << "..." << endl;

        // Actualizar los valores de clave y padre de los nodos vecinos
        for (int v = 0; v < NODOS; v++) {
            // El grafo[u][v] es distinto de cero para conexiones reales
            // visitados[v] debe ser falso para no hacer ciclos regresivos
            // El peso de la arista debe ser menor que la clave guardada actualmente
            if (grafo[u][v] != 0 && !visitados[v] && grafo[u][v] < clave[v]) {
                padre[v] = u;
                clave[v] = grafo[u][v];
                cout << "   * Encontrada conexion mas barata hacia " << (char)('A' + v) 
                     << " con peso: " << clave[v] << endl;
            }
        }

        // Mostrar la tabla de datos y congelar la pantalla temporalmente
        mostrarEstadoPasos(visitados, padre, clave);
    }

    // Pantalla final con el reporte consolidado
    dibujarGrafoPrim(-1, visitados, padre, grafo);
    cout << "---------------------------------------------------------" << endl;
    cout << "       SIMULACION FINALIZADA - ARBOL DE MINIMO COSTO     " << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << "Aristas que forman el Arbol Parcial Minimo:" << endl;
    int costoTotal = 0;
    for (int i = 1; i < NODOS; i++) {
        cout << "  Arista: " << (char)('A' + padre[i]) << " --- " << (char)('A' + i) 
             << "  (Peso: " << grafo[i][padre[i]] << ")" << endl;
        costoTotal += grafo[i][padre[i]];
    }
    cout << "---------------------------------------------------------" << endl;
    cout << " Costo total del Arbol de Expansion Minima = " << costoTotal << endl;
    cout << "---------------------------------------------------------" << endl;
}

int main() {
    // Definimos la matriz de adyacencia del grafo
    // Los números representan los costos de conectar los nodos. 0 significa sin conexión.
    // Mapeo: 0=A, 1=B, 2=C, 3=D, 4=E
    int grafo[NODOS][NODOS] = {
        {0, 2, 4, 7, 0},  // Conexiones desde A
        {2, 0, 3, 0, 0},  // Conexiones desde B
        {4, 3, 0, 0, 5},  // Conexiones desde C
        {7, 0, 0, 0, 0},  // Conexiones desde D
        {0, 0, 5, 0, 0}   // Conexiones desde E
    };

    // Ejecutar el simulador
    primSimulador(grafo);

    // Asegurar que la consola no se cierre al terminar en Dev-C++
    cout << "\nPresione cualquier tecla para finalizar el programa...";
    cin.get();

    return 0;
}

