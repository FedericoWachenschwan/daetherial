#pragma once
#include <SFML/Graphics.hpp>

///=================================================================///
///   NIEBLA - Una capa semitransparente que se mueve despacio sobre
///   el mapa, dando sensacion de viento y ambiente
///=================================================================///
class Niebla {
private:
    sf::Texture _textura_de_la_niebla;
    sf::Sprite _sprite_de_la_niebla;
    float _velocidad_del_viento_en_x;
    float _velocidad_del_viento_en_y;
    float _desplazamiento_acumulado_en_x;
    float _desplazamiento_acumulado_en_y;
    float _tiempo_acumulado_para_la_onda;
    float _opacidad_actual = 30.f;          // Opacidad que va cambiando con el tiempo
    bool _la_opacidad_esta_subiendo = true; // Controla si en este momento sube o baja

public:
    Niebla();
    void actualizar(float tiempo_transcurrido);
    void dibujar(sf::RenderWindow& ventana_del_juego, const sf::View& vista_de_la_camara);
};