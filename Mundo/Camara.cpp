#include "Camara.h"

///=============================================================///
///   #1 - CONSTRUCTOR
///=============================================================///
// #1
Camara::Camara(float ancho_de_la_ventana, float alto_de_la_ventana) {
    _vista_de_la_camara.reset(sf::FloatRect(0.f, 0.f, ancho_de_la_ventana, alto_de_la_ventana)); // La camara empieza en la esquina superior izquierda
    _zoom_minimo = 400.f; // Limite de acercamiento en pixeles de ancho visible
    _zoom_maximo = 1600.f; // Limite de alejamiento en pixeles de ancho visible
    _tiene_limites_establecidos = false; // Sin limites hasta que se llame a setLimites_del_mundo
}

///=============================================================///
///   #2 - SETTERS
///=============================================================///
// #2
void Camara::setLimites_del_mundo(const sf::FloatRect& limites) {
    _limites_del_mundo = limites; // Guarda los bordes del mapa
    _tiene_limites_establecidos = true; // Ahora la camara respetara los bordes
}

///=============================================================///
///   #3 - SEGUIR AL OBJETIVO
///=============================================================///
// #3
void Camara::seguir_al_objetivo(sf::Vector2f posicion_del_objetivo, float tiempo_transcurrido) {

    sf::Vector2f posicion_actual_de_la_camara = _vista_de_la_camara.getCenter(); // Donde esta el centro de la camara ahora
    float velocidad_del_suavizado = 10.0f; // A mayor valor, mas rapido sigue al jugador

 // Interpolacion lineal: la camara se mueve una fraccion de la distancia al objetivo por frame
    float nueva_posicion_x = posicion_actual_de_la_camara.x + (posicion_del_objetivo.x - posicion_actual_de_la_camara.x) * velocidad_del_suavizado * tiempo_transcurrido;
    float nueva_posicion_y = posicion_actual_de_la_camara.y + (posicion_del_objetivo.y - posicion_actual_de_la_camara.y) * velocidad_del_suavizado * tiempo_transcurrido;

    if (_tiene_limites_establecidos == true) {

        float mitad_del_ancho_visible = _vista_de_la_camara.getSize().x / 2.f; // Cuanto espacio hay a cada lado del centro
        float mitad_del_alto_visible = _vista_de_la_camara.getSize().y / 2.f; // Cuanto espacio hay arriba y abajo del centro

        float limite_x_minimo = _limites_del_mundo.left + mitad_del_ancho_visible; // No puede ir mas a la izquierda que esto
        float limite_x_maximo = _limites_del_mundo.left + _limites_del_mundo.width - mitad_del_ancho_visible; // No puede ir mas a la derecha que esto
        float limite_y_minimo = _limites_del_mundo.top + mitad_del_alto_visible; // No puede subir mas que esto
        float limite_y_maximo = _limites_del_mundo.top + _limites_del_mundo.height - mitad_del_alto_visible; // No puede bajar mas que esto

        if (nueva_posicion_x < limite_x_minimo) nueva_posicion_x = limite_x_minimo; // No se va por la izquierda
        if (nueva_posicion_x > limite_x_maximo) nueva_posicion_x = limite_x_maximo; // No se va por la derecha
        if (nueva_posicion_y < limite_y_minimo) nueva_posicion_y = limite_y_minimo; // No se va por arriba
        if (nueva_posicion_y > limite_y_maximo) nueva_posicion_y = limite_y_maximo; // No se va por abajo
    }

    _vista_de_la_camara.setCenter(nueva_posicion_x, nueva_posicion_y); // Aplica la nueva posicion a la camara
}

///=============================================================///
///   #4 - PROCESAR ZOOM
///=============================================================///
// #4
void Camara::procesar_zoom(const sf::Event& evento) {
    if (evento.type == sf::Event::MouseWheelScrolled && evento.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {

        float cantidad_de_scroll = evento.mouseWheelScroll.delta; // Cuanto giro la rueda (positivo = arriba, negativo = abajo)

        if (cantidad_de_scroll > 0 && _vista_de_la_camara.getSize().x > _zoom_minimo) {
            _vista_de_la_camara.zoom(0.9f); // Scroll hacia arriba: acerca la vista un 10%
        }
        else if (cantidad_de_scroll < 0 && _vista_de_la_camara.getSize().x < _zoom_maximo) {
            _vista_de_la_camara.zoom(1.1f); // Scroll hacia abajo: aleja la vista un 10%
        }
    }
}
