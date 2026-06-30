#include "PathFinder.h"
#include <cmath>

///=============================================================///
///   CALCULAR CAMINO - El algoritmo A*
///=============================================================///
int PathFinder::calcular_camino(Map& mapa_del_juego, sf::Vector2f posicion_de_inicio, sf::Vector2f posicion_de_destino, sf::Vector2f camino_resultado[CANTIDAD_MAXIMA_DE_PASOS_EN_UN_CAMINO]) {

    const int TAMANO_DE_CADA_CASILLERO_EN_PIXELES = 16;

    sf::Vector2i casillero_de_inicio((int)posicion_de_inicio.x / TAMANO_DE_CADA_CASILLERO_EN_PIXELES, (int)posicion_de_inicio.y / TAMANO_DE_CADA_CASILLERO_EN_PIXELES);
    sf::Vector2i casillero_de_destino((int)posicion_de_destino.x / TAMANO_DE_CADA_CASILLERO_EN_PIXELES, (int)posicion_de_destino.y / TAMANO_DE_CADA_CASILLERO_EN_PIXELES);

    if (el_casillero_se_puede_caminar(mapa_del_juego, casillero_de_destino) == false) {
        return 0;
    }

    std::vector<Nodo> todos_los_nodos_que_exploramos;

    Nodo nodo_inicial;
    nodo_inicial.posicion_en_la_grilla = casillero_de_inicio;
    nodo_inicial.esta_en_la_lista_abierta = true;
    todos_los_nodos_que_exploramos.push_back(nodo_inicial);

    sf::Vector2i direcciones_posibles[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

    int indice_del_nodo_destino_encontrado = -1;

    while (true) {

        int indice_del_mejor_nodo_abierto = -1;

        for (int i = 0; i < (int)todos_los_nodos_que_exploramos.size(); i++) {
            if (todos_los_nodos_que_exploramos[i].esta_en_la_lista_abierta == true) {
                if (indice_del_mejor_nodo_abierto == -1 || todos_los_nodos_que_exploramos[i].costo_total < todos_los_nodos_que_exploramos[indice_del_mejor_nodo_abierto].costo_total) {
                    indice_del_mejor_nodo_abierto = i;
                }
            }
        }

        if (indice_del_mejor_nodo_abierto == -1) {
            break;
        }

        todos_los_nodos_que_exploramos[indice_del_mejor_nodo_abierto].esta_en_la_lista_abierta = false;
        todos_los_nodos_que_exploramos[indice_del_mejor_nodo_abierto].esta_en_la_lista_cerrada = true;

        if (todos_los_nodos_que_exploramos[indice_del_mejor_nodo_abierto].posicion_en_la_grilla == casillero_de_destino) {
            indice_del_nodo_destino_encontrado = indice_del_mejor_nodo_abierto;
            break;
        }

        for (int i = 0; i < 4; i++) {
            sf::Vector2i posicion_del_vecino = todos_los_nodos_que_exploramos[indice_del_mejor_nodo_abierto].posicion_en_la_grilla + direcciones_posibles[i];

            if (el_casillero_se_puede_caminar(mapa_del_juego, posicion_del_vecino) == false) {
                continue;
            }

            int castigo_por_estar_cerca_de_paredes = 0;
            for (int desplazamiento_y = -2; desplazamiento_y <= 2; desplazamiento_y++) {
                for (int desplazamiento_x = -2; desplazamiento_x <= 2; desplazamiento_x++) {
                    if (mapa_del_juego.getEs_solido(posicion_del_vecino.y + desplazamiento_y, posicion_del_vecino.x + desplazamiento_x)) {
                        castigo_por_estar_cerca_de_paredes += 5;
                    }
                }
            }

            int costo_de_llegar_hasta_el_vecino = todos_los_nodos_que_exploramos[indice_del_mejor_nodo_abierto].costo_de_caminos_recorridos + 10 + castigo_por_estar_cerca_de_paredes;

            int indice_del_vecino = -1;
            for (int j = 0; j < (int)todos_los_nodos_que_exploramos.size(); j++) {
                if (todos_los_nodos_que_exploramos[j].posicion_en_la_grilla == posicion_del_vecino) {
                    indice_del_vecino = j;
                    break;
                }
            }

            if (indice_del_vecino == -1) {
                Nodo nodo_nuevo;
                nodo_nuevo.posicion_en_la_grilla = posicion_del_vecino;
                nodo_nuevo.esta_en_la_lista_abierta = true;
                nodo_nuevo.indice_del_padre = indice_del_mejor_nodo_abierto;
                nodo_nuevo.costo_de_caminos_recorridos = costo_de_llegar_hasta_el_vecino;
                nodo_nuevo.costo_estimado_hasta_el_destino = calcular_distancia_estimada_entre_dos_casilleros(posicion_del_vecino, casillero_de_destino);
                nodo_nuevo.costo_total = nodo_nuevo.costo_de_caminos_recorridos + nodo_nuevo.costo_estimado_hasta_el_destino;
                todos_los_nodos_que_exploramos.push_back(nodo_nuevo);
            }
            else if (todos_los_nodos_que_exploramos[indice_del_vecino].esta_en_la_lista_cerrada == false && costo_de_llegar_hasta_el_vecino < todos_los_nodos_que_exploramos[indice_del_vecino].costo_de_caminos_recorridos) {
                todos_los_nodos_que_exploramos[indice_del_vecino].esta_en_la_lista_abierta = true;
                todos_los_nodos_que_exploramos[indice_del_vecino].indice_del_padre = indice_del_mejor_nodo_abierto;
                todos_los_nodos_que_exploramos[indice_del_vecino].costo_de_caminos_recorridos = costo_de_llegar_hasta_el_vecino;
                todos_los_nodos_que_exploramos[indice_del_vecino].costo_total = todos_los_nodos_que_exploramos[indice_del_vecino].costo_de_caminos_recorridos + todos_los_nodos_que_exploramos[indice_del_vecino].costo_estimado_hasta_el_destino;
            }
        }
    }

    if (indice_del_nodo_destino_encontrado == -1) {
        return 0;
    }

    int cantidad_de_pasos_encontrados = 0;
    int indice_actual_para_reconstruir = indice_del_nodo_destino_encontrado;

    while (indice_actual_para_reconstruir != -1 && cantidad_de_pasos_encontrados < CANTIDAD_MAXIMA_DE_PASOS_EN_UN_CAMINO) {

        const Nodo& nodo_actual = todos_los_nodos_que_exploramos[indice_actual_para_reconstruir];

        float posicion_x_en_pixeles = (nodo_actual.posicion_en_la_grilla.x * TAMANO_DE_CADA_CASILLERO_EN_PIXELES) + (TAMANO_DE_CADA_CASILLERO_EN_PIXELES / 2.f);
        float posicion_y_en_pixeles = (nodo_actual.posicion_en_la_grilla.y * TAMANO_DE_CADA_CASILLERO_EN_PIXELES) + (TAMANO_DE_CADA_CASILLERO_EN_PIXELES / 2.f);

        camino_resultado[cantidad_de_pasos_encontrados] = sf::Vector2f(posicion_x_en_pixeles, posicion_y_en_pixeles);
        cantidad_de_pasos_encontrados++;

        indice_actual_para_reconstruir = nodo_actual.indice_del_padre;
    }

    for (int i = 0; i < cantidad_de_pasos_encontrados / 2; i++) {
        sf::Vector2f temporal = camino_resultado[i];
        camino_resultado[i] = camino_resultado[cantidad_de_pasos_encontrados - 1 - i];
        camino_resultado[cantidad_de_pasos_encontrados - 1 - i] = temporal;
    }

    return cantidad_de_pasos_encontrados;
}

///=============================================================///
///   DISTANCIA ESTIMADA
///=============================================================///
int PathFinder::calcular_distancia_estimada_entre_dos_casilleros(sf::Vector2i casillero_a, sf::Vector2i casillero_b) {
    return (std::abs(casillero_a.x - casillero_b.x) + std::abs(casillero_a.y - casillero_b.y)) * 10;
}

///=============================================================///
///   SE PUEDE CAMINAR
///=============================================================///
bool PathFinder::el_casillero_se_puede_caminar(Map& mapa_del_juego, sf::Vector2i posicion_del_casillero) {
    return mapa_del_juego.getEs_solido(posicion_del_casillero.y, posicion_del_casillero.x) == false;
}