#pragma once
#include <SFML/Graphics.hpp>

///=================================================================///
///   NIEBLA - Una capa semitransparente que se mueve despacio sobre
///   el mapa, dando sensacion de viento y ambiente
///=================================================================///
class Niebla {
private:
    sf::Texture _textura_de_la_niebla; // Imagen de la niebla cargada desde disco
    sf::Sprite _sprite_de_la_niebla; // Lo que se dibuja sobre el mapa
    float _velocidad_del_viento_en_x; // Cuanto se desplaza la niebla horizontalmente por segundo
    float _velocidad_del_viento_en_y; // Cuanto se desplaza la niebla verticalmente por segundo
    float _desplazamiento_acumulado_en_x; // Total desplazado en X desde el inicio
    float _desplazamiento_acumulado_en_y; // Total desplazado en Y desde el inicio
    float _tiempo_acumulado_para_la_onda; // Temporizador para el efecto de onda
    float _opacidad_actual = 30.f; // Opacidad que va cambiando con el tiempo
    bool _la_opacidad_esta_subiendo = true; // Controla si en este momento sube o baja

public:
 // #1
    Niebla();
 // #2
    void actualizar(float tiempo_transcurrido); // Mueve la niebla y anima su opacidad
 // #3
    void dibujar(sf::RenderWindow& ventana_del_juego, const sf::View& vista_de_la_camara); // La dibuja cubriendo lo que ve la camara
};
