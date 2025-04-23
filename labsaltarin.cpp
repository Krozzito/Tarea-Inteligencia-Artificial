#include <iostream>
#include "raylib.h"
#include <vector>
#include <string>
#include <queue>

struct Labyrinth
{
    int f, c; 
    int fin, cIn, fdest, cdest;
    std::vector<std::vector<int>> lab; // matriz del laberinto
    Labyrinth(int f, int c, int fin, int cIn, int fdest, int cdest) : f(f), c(c), fin(fin), 
    cIn(cIn), fdest(fdest), cdest(cdest), lab(f, std::vector<int>(c, 0)) {
    }
        
};

struct Node
{
    int coorX, coorY;
    Node(int x, int y) : coorX(x), coorY(y) {}
};

struct NodeU
{
    int x, y, cost;
    NodeU(int x, int y, int cost) : x(x), y(y), cost(cost) {}
    bool operator<(const NodeU& other) const {
        return cost > other.cost; 
    }
};

void uniformCostSearch() {

};

void Dfs(int x , int y, int fdest, int cdest, std::vector<Node>& actualPath,
     std::vector<Node>& minPath, std::vector<std::vector<bool>>& visited, std::vector<std::vector<int>>& lab, int f, int c) {
    
    if(x < 0 || x >= f || y < 0 || y >= c || visited[x][y]) return; // Fuera de los limites o ya visitado

    actualPath.push_back(Node(x, y)); // Agregar nodo al camino actual

    if(x == fdest  && y == cdest){
        if(minPath.empty() || actualPath.size() < minPath.size()){
            minPath = actualPath; // Actualizar el camino mas corto
        }
        actualPath.pop_back(); // Deshacer el paso
        return;
    }

    visited[x][y] = true; // Marcar como visitado

    int mov = lab[x][y]; // cantidad de movimientos
    int movX[] = {mov, -mov, 0, 0 }; // movimientos en x
    int movY[] = {0, 0, mov, -mov}; // movimientos en y
    for(int i = 0; i < 4; i++){
        int newX = x + movX[i];
        int newY = y + movY[i];
        Dfs(newX, newY, fdest, cdest, actualPath, minPath, visited, lab, f, c); 
    }
    
    visited[x][y] = false; // Desmarcar como visitado   
    actualPath.pop_back(); // Deshacer el paso
};


int main() {
    int f,c; // filas y columnas 
    int fin,cIn,fdest,cdest; // inicio y destino

    std::queue<std::string> results; // Cola para almacenar los laberintos
    
    std::vector<Node>actualPath; // Vector para almacenar el camino encontrado
    std::vector<Node>minPath; // Vector para almacenar el camino mas corto
    std::vector<Labyrinth>labs;
    

    std::cout << "Bienvenido al laberinto de Saltarin" << std::endl;

    while (true)
    {
        std::cout << "Ingrese el numero de filas y columnas: ";
        std::cin >> f;
        if(f == 0) break;
        std::cin >> c;

        std::cout << "Ingrese la posicion de inicio (fila y columna): ";
        std::cin >> fin >> cIn;
        std::cout << "Ingrese la posicion de destino (fila y columna): ";
        std::cin >> fdest >> cdest;

        labs.push_back(Labyrinth(f, c, fin, cIn, fdest, cdest)); // Agregar laberinto a la lista
        Labyrinth& Alab = labs.back(); // Obtener el laberinto actual

        std::cout << "Ingrese los caminos del laberinto:" << std::endl;
        for (int i = 0; i < f; i++) {
            for (int j = 0; j < c; j++) {
                std::cin >> Alab.lab[i][j]; // Ingresar caminos
            }
        }

    };

    
    while (!labs.empty()) 
    {

        Labyrinth& actLab = labs.front(); // Obtener el laberinto actual
        std::vector<std::vector<bool>> visited(actLab.f, std::vector<bool>(actLab.c, false)); // matriz para marcar las celdas visitadas

        const int cellSize = 50;
        const int screenWidth = actLab.c * cellSize;
        const int screenHeight = actLab.f * cellSize;
        // Inicializar la ventana de Raylib

        InitWindow(screenWidth, screenHeight, "Laberinto saltarin");

        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            // Dibujar matriz
            for (int i = 0; i < actLab.f; i++) {
                for (int j = 0; j < actLab.c; j++) {
                    Rectangle cell = { j * cellSize, i * cellSize, cellSize, cellSize };

                    // Colorear celda según su función
                    if (i == actLab.fin && j == actLab.cIn)
                        DrawRectangleRec(cell, GREEN);  // Inicio
                    else if (i == actLab.fdest && j == actLab.cdest)
                        DrawRectangleRec(cell, RED);    // Destino
                    else
                        DrawRectangleRec(cell, LIGHTGRAY); // Normal

                    DrawRectangleLinesEx(cell, 1, BLACK); // Bordes
                    DrawText(std::to_string(actLab.lab[i][j]).c_str(), j * cellSize + 15, i * cellSize + 15, 20, BLACK);
                }
            }

            EndDrawing();
          
        }
        CloseWindow();

            Dfs(actLab.fin, actLab.cIn, actLab.fdest, actLab.cdest, actualPath, minPath, visited, actLab.lab, actLab.f, actLab.c);
            if(minPath.empty())results.push("No hay solucion"); // Si no hay camino
            else{
                std::string result = std::to_string(minPath.size() - 1); // Guardar el resultado en la cola
                results.push(result);
            }


        labs.erase(labs.begin());
        actualPath.clear();
        minPath.clear(); 
        visited.clear(); 
    }
    // Mostrar resultados
    while (!results.empty()) {
        std::cout << results.front() << std::endl; // Mostrar resultados
        results.pop();
    }   
    
    return 0;

    
}