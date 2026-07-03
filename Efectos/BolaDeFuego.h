#pragma once
#include <SFML/Graphics.hpp>
#include "VisualFX.h"

///=================================================================///
///   BOLA_DE_FUEGO - El hechizo del jugador. Vuela en linea recta
///   hasta alcanzar su rango y desaparece
///=================================================================///
class BolaDeFuego {
private:

    sf::Texture _textura_de_la_bola; // Imagen cargada desde disco
    sf::Sprite _sprite_de_la_bola; // Lo que se dibuja en pantalla

    bool _la_bola_esta_activa; // Si es falso, no se mueve ni dibuja
    sf::Vector2f _direccion_de_vuelo; // Vector normalizado de hacia donde vuela
    float _velocidad_de_vuelo; // Pixeles por segundo que avanza
    float _distancia_recorrida; // Cuanto lleva volado hasta ahora
    float _rango_maximo_de_vuelo; // Hasta donde puede llegar
    int _costo_de_mana; // Mana que consume lanzar esta bola

 ///=============================================================///
 ///   COOLDOWN
 ///=============================================================///
    bool _el_cooldown_ya_paso; // Verdadero cuando se puede lanzar de nuevo
    float _segundos_de_cooldown_restantes; // Tiempo que falta para poder tirar otra vez
    float _duracion_total_del_cooldown; // Tiempo total que dura el cooldown

    float _tiempo_desde_el_ultimo_rastro; // Controla cada cuando se genera una chispa

public:

 ///=============================================================///
 ///   CONSTRUCTOR
 ///=============================================================///
 // #1
    BolaDeFuego();

 ///=============================================================///
 ///   GETTERS
 ///=============================================================///
 // #2
    bool getEsta_activa() const { return _la_bola_esta_activa; } // Devuelve si la bola esta volando
 // #3
    int getCosto_de_mana() const { return _costo_de_mana; } // Devuelve cuanto mana consume lanzar la bola

 ///=============================================================///
 ///   OTROS METODOS
 ///=============================================================///
 // #5
    void activar(sf::Vector2f posicion_de_inicio, sf::Vector2f posicion_del_objetivo, float rango_en_pixeles);
 // #6
    void actualizar(float tiempo_transcurrido, VisualFX& efectos_visuales);
 // #7
    void dibujar(sf::RenderWindow& ventana_del_juego);
 // #8
    void desactivar() { _la_bola_esta_activa = false; } // Apaga la bola inmediatamente

 // Calcula el rectangulo cada vez que se llama (no es un atributo
 // guardado), por eso no lleva "get" adelante
 // #9
    sf::FloatRect calcular_caja_de_colision() const { return _sprite_de_la_bola.getGlobalBounds(); } // Rectangulo que ocupa la bola
};
