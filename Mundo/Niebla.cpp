#include "Niebla.h"

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
Niebla::Niebla() {
    if (_textura_de_la_niebla.loadFromFile("assets/fog.png") == true) {
        _textura_de_la_niebla.setRepeated(true); // SFML: hace que la imagen se repita sola como un azulejo
        _sprite_de_la_niebla.setTexture(_textura_de_la_niebla);
        _sprite_de_la_niebla.setColor(sf::Color(255, 255, 255, 80));
    }

    _velocidad_del_viento_en_x = 15.f;
    _velocidad_del_viento_en_y = 5.f;
    _desplazamiento_acumulado_en_x = 0.f;
    _desplazamiento_acumulado_en_y = 0.f;
    _tiempo_acumulado_para_la_onda = 0.f;
}

///=============================================================///
///   ACTUALIZAR - Mueve la niebla y hace que la opacidad suba y
///   baje despacio, como un rebote entre 5 y 55 de transparencia
///=============================================================///
void Niebla::actualizar(float tiempo_transcurrido) {

    _desplazamiento_acumulado_en_x += _velocidad_del_viento_en_x * tiempo_transcurrido;
    _desplazamiento_acumulado_en_y += _velocidad_del_viento_en_y * tiempo_transcurrido;
    _tiempo_acumulado_para_la_onda += tiempo_transcurrido;

    ///=========================================================///
    ///   OPACIDAD QUE SUBE Y BAJA COMO UNA RESPIRACION
    ///   En vez de una onda matematica, usamos un contador que
    ///   rebota entre 5 y 55
    ///=========================================================///
    int opacidad_minima = 5;
    int opacidad_maxima = 55;
    float velocidad_de_la_onda = 25.f; // Cuanto cambia la opacidad por segundo

    if (_la_opacidad_esta_subiendo == true) {
        _opacidad_actual += velocidad_de_la_onda * tiempo_transcurrido;
        if (_opacidad_actual >= opacidad_maxima) {
            _opacidad_actual = opacidad_maxima;
            _la_opacidad_esta_subiendo = false; // Llego al maximo, ahora baja
        }
    }
    else {
        _opacidad_actual -= velocidad_de_la_onda * tiempo_transcurrido;
        if (_opacidad_actual <= opacidad_minima) {
            _opacidad_actual = opacidad_minima;
            _la_opacidad_esta_subiendo = true; // Llego al minimo, ahora sube
        }
    }

    int opacidad_entera = _opacidad_actual; // Convertimos el float a int simplemente guardandolo en un int
    _sprite_de_la_niebla.setColor(sf::Color(255, 255, 255, opacidad_entera));
}

///=============================================================///
///   DIBUJAR - Recorta la niebla para que cubra exactamente lo
///   que ve la camara en este momento
///=============================================================///
void Niebla::dibujar(sf::RenderWindow& ventana_del_juego, const sf::View& vista_de_la_camara) {

    sf::Vector2f tamano_de_la_camara = vista_de_la_camara.getSize();
    sf::Vector2f centro_de_la_camara = vista_de_la_camara.getCenter();

    int recorte_x = _desplazamiento_acumulado_en_x; // Guardamos el float en un int directamente
    int recorte_y = _desplazamiento_acumulado_en_y;
    int recorte_ancho = tamano_de_la_camara.x;
    int recorte_alto = tamano_de_la_camara.y;

    _sprite_de_la_niebla.setTextureRect(sf::IntRect(recorte_x, recorte_y, recorte_ancho, recorte_alto));

    _sprite_de_la_niebla.setPosition(
        centro_de_la_camara.x - (tamano_de_la_camara.x / 2.f),
        centro_de_la_camara.y - (tamano_de_la_camara.y / 2.f)
    );

    ventana_del_juego.draw(_sprite_de_la_niebla);
}