// ============================================================================
// MÓDULO: Pathfinder (IA de Navegación)
// DESCRIPCIÓN: Implementación del algoritmo de búsqueda de caminos A* (A-Star)
//              sobre una grilla bidimensional. Incluye heurísticas personalizadas 
//              de "Costo de Incomodidad" para evitar que los agentes rocen colisiones.
// ============================================================================

#include "Pathfinder.h"
#include <cmath>
#include <algorithm>
#include <iostream>

// Constante de escala espacial: Define la resolución de la grilla de navegación
const int TAMANO_TILE = 16;

// ============================================================================
// NÚCLEO DEL ALGORITMO: Búsqueda del Camino Óptimo
// ============================================================================

/**
 * @brief Calcula la ruta óptima sorteando obstáculos usando A*.
 * @param mapa Referencia al mapa de colisiones (Grafo implícito).
 * @param posInicio Coordenada espacial real (píxeles) del origen.
 * @param posDestino Coordenada espacial real (píxeles) del objetivo.
 * @return std::vector<sf::Vector2f> Lista ordenada de waypoints en píxeles. Vacía si no hay ruta.
 */
std::vector<sf::Vector2f> Pathfinder::calcularCamino(Map& mapa, sf::Vector2f posInicio, sf::Vector2f posDestino) {
    std::vector<sf::Vector2f> caminoFinal;

    // ------------------------------------------------------------------------
    // 1. TRADUCCIÓN ESPACIAL (Píxeles a Grilla)
    // ------------------------------------------------------------------------
    // Mapeamos el espacio continuo del motor físico al espacio discreto del algoritmo.
    sf::Vector2i inicioGrilla(static_cast<int>(posInicio.x) / TAMANO_TILE,
        static_cast<int>(posInicio.y) / TAMANO_TILE);

    sf::Vector2i destinoGrilla(static_cast<int>(posDestino.x) / TAMANO_TILE,
        static_cast<int>(posDestino.y) / TAMANO_TILE);

    // Validación temprana: Si el punto de destino está dentro de un sólido geométrico, se aborta.
    if (!esCaminable(mapa, destinoGrilla)) {
        return caminoFinal;
    }

    // ------------------------------------------------------------------------
    // 2. ESTRUCTURAS DE DATOS (Frontera y Explorados)
    // ------------------------------------------------------------------------
    // listaAbierta: Nodos descubiertos pendientes de evaluación (Frontera).
    // listaCerrada: Nodos ya evaluados y descartados para re-evaluación.
    std::vector<Nodo*> listaAbierta;
    std::vector<Nodo*> listaCerrada;

    Nodo* nodoInicial = new Nodo(inicioGrilla);
    listaAbierta.push_back(nodoInicial);

    // Vectores de expansión: Movimiento ortogonal (Norte, Sur, Este, Oeste). No diagonales.
    std::vector<sf::Vector2i> direcciones = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

    // ------------------------------------------------------------------------
    // 3. BUCLE DE EXPANSIÓN DE GRAFOS (Motor A*)
    // ------------------------------------------------------------------------
    while (!listaAbierta.empty()) {

        // A. Selección del nodo más prometedor (Menor Costo F = G + H)
        auto iteradorMejorNodo = listaAbierta.begin();
        Nodo* nodoActual = *iteradorMejorNodo;

        for (auto it = listaAbierta.begin(); it != listaAbierta.end(); ++it) {
            if ((*it)->costoF < nodoActual->costoF) {
                nodoActual = *it;
                iteradorMejorNodo = it;
            }
        }

        // B. Transición de estado: El nodo pasa de la frontera a los explorados
        listaCerrada.push_back(nodoActual);
        listaAbierta.erase(iteradorMejorNodo);

        // C. ¡CONDICIÓN DE ÉXITO! Se alcanzó el nodo objetivo
        if (nodoActual->posicionGrilla == destinoGrilla) {
            Nodo* actual = nodoActual;

            // Reconstrucción del camino mediante backtracking (Padre a Padre)
            while (actual != nullptr) {
                // Retraducción Espacial: De grilla a píxeles (centrado en la baldosa)
                float pixelX = (actual->posicionGrilla.x * TAMANO_TILE) + (TAMANO_TILE / 2.f);
                float pixelY = (actual->posicionGrilla.y * TAMANO_TILE) + (TAMANO_TILE / 2.f);

                caminoFinal.push_back(sf::Vector2f(pixelX, pixelY));
                actual = actual->padre;
            }

            // Inversión del vector ya que el backtracking lo arma desde el destino al origen
            std::reverse(caminoFinal.begin(), caminoFinal.end());
            break;
        }

        // D. Expansión de Nodos Vecinos (Adyacencia)
        for (const auto& dir : direcciones) {
            sf::Vector2i posVecino = nodoActual->posicionGrilla + dir;

            // Filtro 1: Descartar obstáculos duros (Paredes)
            if (!esCaminable(mapa, posVecino)) continue;

            // Filtro 2: Descartar nodos ya procesados (Evita bucles infinitos)
            bool enCerrada = false;
            for (Nodo* n : listaCerrada) {
                if (n->posicionGrilla == posVecino) {
                    enCerrada = true;
                    break;
                }
            }
            if (enCerrada) continue;

            // ----------------------------------------------------------------
            // 🌟 HEURÍSTICA PERSONALIZADA: Costo de Incomodidad (Discomfort Cost)
            // ----------------------------------------------------------------
            // Evita el comportamiento "Wall-Hugging" (Rozar las paredes) del A* estándar.
            int costoBase = 10;
            int penalizacionPared = 0;

            int radioEscaner = 2; // Matriz de escaneo 5x5 alrededor del nodo
            int castigoPorPared = 5; // Peso matemático añadido por proximidad a sólidos

            // Convolution-like scan para detectar encierro espacial
            for (int oy = -radioEscaner; oy <= radioEscaner; ++oy) {
                for (int ox = -radioEscaner; ox <= radioEscaner; ++ox) {
                    if (mapa.esSolido(posVecino.y + oy, posVecino.x + ox)) {
                        penalizacionPared += castigoPorPared;
                    }
                }
            }

            // Costo G acumulado: Esfuerzo de ruta + Castigo por estrechez del pasillo
            int nuevoCostoG = nodoActual->costoG + costoBase + penalizacionPared;

            // Búsqueda del vecino en la frontera actual
            Nodo* vecino = nullptr;
            bool enAbierta = false;
            for (Nodo* n : listaAbierta) {
                if (n->posicionGrilla == posVecino) {
                    vecino = n;
                    enAbierta = true;
                    break;
                }
            }

            // Inserción o Actualización de Nodos en la Frontera (Edge Relaxation)
            if (!enAbierta || nuevoCostoG < vecino->costoG) {
                if (!enAbierta) {
                    vecino = new Nodo(posVecino);
                    listaAbierta.push_back(vecino);
                }

                // Actualización de pesos (F = G + H)
                vecino->padre = nodoActual;
                vecino->costoG = nuevoCostoG;
                // Calculamos H usando Distancia Manhattan
                vecino->costoH = calcularDistanciaHeuristica(vecino->posicionGrilla, destinoGrilla);
                vecino->costoF = vecino->costoG + vecino->costoH;
            }
        }
    }

    // ------------------------------------------------------------------------
    // 4. SANITIZACIÓN DE MEMORIA DINÁMICA (Memory Leak Prevention)
    // ------------------------------------------------------------------------
    // Es crítico liberar los punteros instanciados en el Heap con 'new'.
    for (Nodo* n : listaAbierta) delete n;
    for (Nodo* n : listaCerrada) delete n;

    return caminoFinal;
}

// ============================================================================
// FUNCIONES HEURÍSTICAS Y DE VALIDACIÓN ESPACIAL
// ============================================================================

/**
 * @brief Calcula la distancia Manhattan entre dos nodos.
 * Se elige Manhattan sobre Euclídea porque el agente se mueve de forma ortogonal (4 direcciones).
 */
int Pathfinder::calcularDistanciaHeuristica(sf::Vector2i nodoA, sf::Vector2i nodoB) {
    // Escala x10 para mantenerse en proporción con el 'costoBase' entero de los desplazamientos
    return (std::abs(nodoA.x - nodoB.x) + std::abs(nodoA.y - nodoB.y)) * 10;
}

/**
 * @brief Evalúa la transitabilidad de una coordenada discreta.
 */
bool Pathfinder::esCaminable(Map& mapa, sf::Vector2i posGrilla) {
    // Conversión matricial estricta: Fila = Y, Columna = X
    return !mapa.esSolido(posGrilla.y, posGrilla.x);
}