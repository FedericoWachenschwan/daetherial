#pragma once
#include <SFML/Graphics.hpp>

///=================================================================///
///   CAMARA - Maneja la "vista" del juego: sigue al personaje con
///   un movimiento suave y no la deja salirse de los limites del mapa
///=================================================================///
class Camara {
private:

    sf::View _vista_de_la_camara;
    float _zoom_minimo;
    float _zoom_maximo;
    sf::FloatRect _limites_del_mundo;
    bool _tiene_limites_establecidos;

public:

    ///=============================================================///
    ///   CONSTRUCTOR
    ///=============================================================///
    Camara(float ancho_de_la_ventana, float alto_de_la_ventana);

    ///=============================================================///
    ///   GETTERS
    ///=============================================================///
    const sf::View& getVista() const { return _vista_de_la_camara; }

    ///=============================================================///
    ///   SETTERS
    ///=============================================================///
    void setLimites_del_mundo(const sf::FloatRect& limites);

    ///=============================================================///
    ///   OTROS METODOS
    ///=============================================================///
    void seguir_al_objetivo(sf::Vector2f posicion_del_objetivo, float tiempo_transcurrido);
    void procesar_zoom(const sf::Event& evento);
};