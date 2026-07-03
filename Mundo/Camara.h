#pragma once
#include <SFML/Graphics.hpp>

///=================================================================///
///   CAMARA - Maneja la "vista" del juego: sigue al personaje con
///   un movimiento suave y no la deja salirse de los limites del mapa
///=================================================================///
class Camara {
private:

    sf::View _vista_de_la_camara; // La ventana recortada que ve el jugador
    float _zoom_minimo; // Hasta que tamanio se puede acercar
    float _zoom_maximo; // Hasta que tamanio se puede alejar
    sf::FloatRect _limites_del_mundo; // Los bordes del mapa, para no salirse
    bool _tiene_limites_establecidos; // Si es falso, la camara puede ir a cualquier lado

public:

 ///=============================================================///
 ///   CONSTRUCTOR
 ///=============================================================///
 // #1
    Camara(float ancho_de_la_ventana, float alto_de_la_ventana);

 ///=============================================================///
 ///   GETTERS
 ///=============================================================///
 // #2
    const sf::View& getVista() const { return _vista_de_la_camara; } // Devuelve la vista para aplicarla a la ventana

 ///=============================================================///
 ///   SETTERS
 ///=============================================================///
 // #3
    void setLimites_del_mundo(const sf::FloatRect& limites); // Define hasta donde puede ir la camara

 ///=============================================================///
 ///   OTROS METODOS
 ///=============================================================///
 // #4
    void seguir_al_objetivo(sf::Vector2f posicion_del_objetivo, float tiempo_transcurrido); // Mueve la camara suavemente hacia el jugador
 // #5
    void procesar_zoom(const sf::Event& evento); // Acerca o aleja la vista con la rueda del mouse
};
