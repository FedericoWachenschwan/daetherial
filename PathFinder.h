#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "map.h" // Necesitamos tu mapa para leer dónde están las paredes

// ============================================================================
// 🌟 ESTRUCTURA NODO: Representa un cuadradito (baldosa) en la grilla del mapa
// ============================================================================
struct Nodo {
    sf::Vector2i posicionGrilla; // Coordenada (x, y) en la matriz lógica
    int costoG; // Esfuerzo desde el inicio
    int costoH; // Distancia estimada al destino
    int costoF; // G + H (El puntaje final)

    Nodo* padre; // El nodo anterior que pisamos para llegar a este (como migas de pan)

    // Constructor del nodo
    Nodo(sf::Vector2i pos, Nodo* p = nullptr)
        : posicionGrilla(pos), costoG(0), costoH(0), costoF(0), padre(p) {
    }
};

// ============================================================================
// 🌟 CLASE PATHFINDER: El GPS matemático
// ============================================================================
class Pathfinder {
public:
    // Es estático ('static') para que podamos llamarlo desde cualquier lado 
    // sin tener que crear una variable Pathfinder nueva todo el tiempo.
    static std::vector<sf::Vector2f> calcularCamino(Map& mapa, sf::Vector2f posInicio, sf::Vector2f posDestino);

private:
    // Funciones matemáticas internas para que el código quede limpio
    static int calcularDistanciaHeuristica(sf::Vector2i nodoA, sf::Vector2i nodoB);
    static bool esCaminable(Map& mapa, sf::Vector2i posGrilla);
};

/*Implementar A* (A-Star): El algoritmo de búsqueda de caminos más famoso y eficiente.
Para que este algoritmo funcione, tenemos que enseñarle a la computadora a pensar en "baldosas" (nodos) y a calcular costos. 
El Pathfinder no ve texturas bonitas, ve una grilla de Excel.
🧠 La Matemática del Algoritmo (El Secreto)Para decidir qué camino tomar, 
A* analiza los cuadraditos que tiene alrededor y les asigna un "puntaje" usando esta 
fórmula sagrada:$F = G + H$Costo $G$: Es el esfuerzo real que le tomó al Gólem llegar desde el inicio hasta esa baldosa (cuánta nafta gastó).
Costo $H$ (Heurística): Es una "adivinanza" matemática de la distancia en línea recta desde esa baldosa hasta vos (cuánta nafta falta).
Costo $F$: Es la suma de los dos. El algoritmo SIEMPRE va a elegir pisar la baldosa que tenga el $F$ más bajo.

Para que el A* funcione en C++, necesitamos dos listas fundamentales:

La Lista Abierta (Open List): Son las baldosas que el algoritmo "ve" pero todavía no pisó. 
Es como mirar el mapa y decir "puedo ir por acá o por allá".

La Lista Cerrada (Closed List): Son las baldosas que ya pisamos y evaluamos. 
Las guardamos acá para no volver atrás y quedarnos en un bucle infinito.

Además, tenemos que traducir los píxeles (coordenadas del mundo de SFML) a "baldosas" (coordenadas de la grilla del mapa). 
*/