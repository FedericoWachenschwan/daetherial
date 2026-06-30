#include "Camara.h"

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
Camara::Camara(float ancho_de_la_ventana, float alto_de_la_ventana) {
    _vista_de_la_camara.reset(sf::FloatRect(0.f, 0.f, ancho_de_la_ventana, alto_de_la_ventana));
    _zoom_minimo = 400.f;
    _zoom_maximo = 1600.f;
    _tiene_limites_establecidos = false;
}

///=============================================================///
///   SETTERS
///=============================================================///
void Camara::setLimites_del_mundo(const sf::FloatRect& limites) {
    _limites_del_mundo = limites;
    _tiene_limites_establecidos = true;
}

///=============================================================///
///   SEGUIR AL OBJETIVO
///=============================================================///
void Camara::seguir_al_objetivo(sf::Vector2f posicion_del_objetivo, float tiempo_transcurrido) {

    sf::Vector2f posicion_actual_de_la_camara = _vista_de_la_camara.getCenter();
    float velocidad_del_suavizado = 10.0f;

    float nueva_posicion_x = posicion_actual_de_la_camara.x + (posicion_del_objetivo.x - posicion_actual_de_la_camara.x) * velocidad_del_suavizado * tiempo_transcurrido;
    float nueva_posicion_y = posicion_actual_de_la_camara.y + (posicion_del_objetivo.y - posicion_actual_de_la_camara.y) * velocidad_del_suavizado * tiempo_transcurrido;

    if (_tiene_limites_establecidos == true) {

        float mitad_del_ancho_visible = _vista_de_la_camara.getSize().x / 2.f;
        float mitad_del_alto_visible = _vista_de_la_camara.getSize().y / 2.f;

        float limite_x_minimo = _limites_del_mundo.left + mitad_del_ancho_visible;
        float limite_x_maximo = _limites_del_mundo.left + _limites_del_mundo.width - mitad_del_ancho_visible;
        float limite_y_minimo = _limites_del_mundo.top + mitad_del_alto_visible;
        float limite_y_maximo = _limites_del_mundo.top + _limites_del_mundo.height - mitad_del_alto_visible;

        if (nueva_posicion_x < limite_x_minimo) nueva_posicion_x = limite_x_minimo;
        if (nueva_posicion_x > limite_x_maximo) nueva_posicion_x = limite_x_maximo;
        if (nueva_posicion_y < limite_y_minimo) nueva_posicion_y = limite_y_minimo;
        if (nueva_posicion_y > limite_y_maximo) nueva_posicion_y = limite_y_maximo;
    }

    _vista_de_la_camara.setCenter(nueva_posicion_x, nueva_posicion_y);
}

///=============================================================///
///   PROCESAR ZOOM
///=============================================================///
void Camara::procesar_zoom(const sf::Event& evento) {
    if (evento.type == sf::Event::MouseWheelScrolled && evento.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {

        float cantidad_de_scroll = evento.mouseWheelScroll.delta;

        if (cantidad_de_scroll > 0 && _vista_de_la_camara.getSize().x > _zoom_minimo) {
            _vista_de_la_camara.zoom(0.9f);
        }
        else if (cantidad_de_scroll < 0 && _vista_de_la_camara.getSize().x < _zoom_maximo) {
            _vista_de_la_camara.zoom(1.1f);
        }
    }
}