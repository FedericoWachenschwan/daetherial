#pragma once
#include <SFML/Graphics.hpp>
#include "VisualFX.h"

///=================================================================///
///   BOLA_DE_FUEGO - El hechizo del jugador. Vuela en linea recta
///   hasta alcanzar su rango y desaparece
///=================================================================///
class BolaDeFuego {
private:

    sf::Texture _textura_de_la_bola;
    sf::Sprite _sprite_de_la_bola;

    bool _la_bola_esta_activa;
    sf::Vector2f _direccion_de_vuelo;
    float _velocidad_de_vuelo;
    float _distancia_recorrida;
    float _rango_maximo_de_vuelo;
    int _dano_de_la_bola;

    ///=============================================================///
    ///   COOLDOWN
    ///=============================================================///
    bool _el_cooldown_ya_paso;
    float _segundos_de_cooldown_restantes;
    float _duracion_total_del_cooldown;

    float _tiempo_desde_el_ultimo_rastro;

public:

    ///=============================================================///
    ///   CONSTRUCTOR
    ///=============================================================///
    BolaDeFuego();

    ///=============================================================///
    ///   GETTERS
    ///=============================================================///
    bool getEsta_activa() const { return _la_bola_esta_activa; }
    int getDano() const { return _dano_de_la_bola; }

    ///=============================================================///
    ///   OTROS METODOS
    ///=============================================================///
    void activar(sf::Vector2f posicion_de_inicio, sf::Vector2f posicion_del_objetivo, float rango_en_pixeles);
    void actualizar(float tiempo_transcurrido, VisualFX& efectos_visuales);
    void dibujar(sf::RenderWindow& ventana_del_juego);
    void desactivar() { _la_bola_esta_activa = false; }

    // Calcula el rectangulo cada vez que se llama (no es un atributo
    // guardado), por eso no lleva "get" adelante
    sf::FloatRect calcular_caja_de_colision() const { return _sprite_de_la_bola.getGlobalBounds(); }
};