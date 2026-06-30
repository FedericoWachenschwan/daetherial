#include "VisualFX.h"
#include <iostream>
#include <cstdlib>

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
VisualFX::VisualFX() {
    _cantidad_de_particulas_de_accion_en_uso = 0;
    _cantidad_de_particulas_de_ambiente_en_uso = 0;
    _cantidad_de_portales_en_uso = 0;

    if (_textura_de_particulas.loadFromFile("assets/particleFX.png") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA TEXTURA DE PARTICULAS." << std::endl;
    }
    if (_textura_de_portal.loadFromFile("assets/PortalSpawn.png") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA TEXTURA DEL PORTAL." << std::endl;
    }
    else {
        _textura_de_portal.setSmooth(true);
    }
}

///=============================================================///
///   AGREGAR RASTRO - Un destello que queda donde estaba el sprite
///   y se desvanece rapido (lo usa el dash y la bola de fuego)
///=============================================================///
void VisualFX::agregarRastro(sf::Sprite sprite_base, sf::Color color, float velocidad_de_desvanecimiento, bool glow) {
    if (_cantidad_de_particulas_de_accion_en_uso >= CANTIDAD_MAXIMA_DE_PARTICULAS_DE_ACCION) return;

    Particula nueva_particula;
    nueva_particula.sprite_de_la_particula = sprite_base;
    nueva_particula.sprite_de_la_particula.setColor(color);
    nueva_particula.velocidad_de_la_particula = sf::Vector2f(0.f, 0.f);
    nueva_particula.opacidad_actual = 255.f;
    nueva_particula.velocidad_de_desvanecimiento = velocidad_de_desvanecimiento;
    nueva_particula.usar_modo_glow = glow;

    _particulas_de_accion[_cantidad_de_particulas_de_accion_en_uso] = nueva_particula;
    _cantidad_de_particulas_de_accion_en_uso++;
}

///=============================================================///
///   AGREGAR PARTICULA DINAMICA - Una chispa con movimiento propio
///=============================================================///
void VisualFX::agregarParticulaDinamica(const sf::Texture& textura, sf::Vector2f posicion, sf::Vector2f velocidad, sf::Color color, float velocidad_de_desvanecimiento, bool glow) {
    if (_cantidad_de_particulas_de_accion_en_uso >= CANTIDAD_MAXIMA_DE_PARTICULAS_DE_ACCION) return;

    Particula nueva_particula;
    nueva_particula.sprite_de_la_particula.setTexture(textura);
    nueva_particula.sprite_de_la_particula.setOrigin(textura.getSize().x / 2.f, textura.getSize().y / 2.f);
    nueva_particula.sprite_de_la_particula.setScale(0.08f, 0.08f);
    nueva_particula.sprite_de_la_particula.setPosition(posicion);
    nueva_particula.sprite_de_la_particula.setColor(color);
    nueva_particula.velocidad_de_la_particula = velocidad;
    nueva_particula.opacidad_actual = 150.f;
    nueva_particula.velocidad_de_desvanecimiento = velocidad_de_desvanecimiento;
    nueva_particula.usar_modo_glow = glow;

    _particulas_de_accion[_cantidad_de_particulas_de_accion_en_uso] = nueva_particula;
    _cantidad_de_particulas_de_accion_en_uso++;
}

///=============================================================///
///   AGREGAR PARTICULAS DE AMBIENTE - Lucierngas o polvo que
///   bailan por el mapa para siempre, cambiando de direccion
///   cada cierto tiempo
///=============================================================///
void VisualFX::agregarParticulasAmbiente(sf::Vector2f area_de_aparicion, int cantidad, sf::Color color) {
    for (int i = 0; i < cantidad; i++) {
        if (_cantidad_de_particulas_de_ambiente_en_uso >= CANTIDAD_MAXIMA_DE_PARTICULAS_DE_AMBIENTE) break;

        Particula nueva_particula;
        nueva_particula.sprite_de_la_particula.setTexture(_textura_de_particulas);
        nueva_particula.sprite_de_la_particula.setOrigin(_textura_de_particulas.getSize().x / 2.f, _textura_de_particulas.getSize().y / 2.f);
        nueva_particula.sprite_de_la_particula.setScale(0.8f, 0.8f);

        float pos_x = (float)(rand() % (int)area_de_aparicion.x);
        float pos_y = (float)(rand() % (int)area_de_aparicion.y);
        nueva_particula.sprite_de_la_particula.setPosition(pos_x, pos_y);

        float vel_x = (float)(rand() % 80) - 40.f;
        float vel_y = (float)(rand() % 80) - 40.f;
        nueva_particula.velocidad_de_la_particula = sf::Vector2f(vel_x, vel_y);

        nueva_particula.sprite_de_la_particula.setColor(color);
        nueva_particula.opacidad_actual = 180.f;
        nueva_particula.velocidad_de_desvanecimiento = 0.f; // Nunca se va
        nueva_particula.usar_modo_glow = true;
        nueva_particula.tiempo_hasta_cambio_de_direccion = 1.f + (float)(rand() % 200) / 100.f; // Entre 1 y 3 segundos

        _particulas_de_ambiente[_cantidad_de_particulas_de_ambiente_en_uso] = nueva_particula;
        _cantidad_de_particulas_de_ambiente_en_uso++;
    }
}

///=============================================================///
///   AGREGAR PORTAL
///=============================================================///
void VisualFX::agregarPortal(sf::Vector2f posicion, float duracion, int frames, bool glow) {
    if (_cantidad_de_portales_en_uso >= CANTIDAD_MAXIMA_DE_PORTALES) return;
    if (_textura_de_portal.getSize().x == 0) return;

    if (frames < 1) {
        frames = 1;
    }

    PortalSpawn nuevo_portal;
    nuevo_portal.tiempo_de_vida_actual = duracion;
    nuevo_portal.tiempo_de_vida_maximo = duracion;
    nuevo_portal.cantidad_de_frames = frames;
    nuevo_portal.usar_modo_glow = glow;

    int columnas = 3;
    int ancho_del_frame = _textura_de_portal.getSize().x / columnas;
    int alto_del_frame = _textura_de_portal.getSize().y / 2;

    nuevo_portal.sprite_del_portal.setTexture(_textura_de_portal);
    nuevo_portal.sprite_del_portal.setTextureRect(sf::IntRect(0, 0, ancho_del_frame, alto_del_frame));
    nuevo_portal.sprite_del_portal.setOrigin(ancho_del_frame / 2.f, alto_del_frame / 2.f);
    nuevo_portal.sprite_del_portal.setPosition(posicion);
    nuevo_portal.sprite_del_portal.setScale(2.5f, 2.5f);

    _portales[_cantidad_de_portales_en_uso] = nuevo_portal;
    _cantidad_de_portales_en_uso++;
}

///=============================================================///
///   ACTUALIZAR PARTICULAS DE ACCION - Las que nacen y mueren.
///   Cuando una muere, ponemos la ultima en su lugar para no
///   dejar huecos en el array
///=============================================================///
void VisualFX::actualizar_particulas_de_accion(float tiempo_transcurrido) {
    int i = 0;
    while (i < _cantidad_de_particulas_de_accion_en_uso) {

        _particulas_de_accion[i].sprite_de_la_particula.move(_particulas_de_accion[i].velocidad_de_la_particula * tiempo_transcurrido);
        _particulas_de_accion[i].opacidad_actual -= _particulas_de_accion[i].velocidad_de_desvanecimiento * tiempo_transcurrido;

        if (_particulas_de_accion[i].opacidad_actual <= 0.f) {
            _particulas_de_accion[i] = _particulas_de_accion[_cantidad_de_particulas_de_accion_en_uso - 1];
            _cantidad_de_particulas_de_accion_en_uso--;
        }
        else {
            sf::Color color_actual = _particulas_de_accion[i].sprite_de_la_particula.getColor();
            int opacidad_entera = _particulas_de_accion[i].opacidad_actual;
            if (opacidad_entera < 0) opacidad_entera = 0;
            if (opacidad_entera > 255) opacidad_entera = 255;
            color_actual.a = opacidad_entera;
            _particulas_de_accion[i].sprite_de_la_particula.setColor(color_actual);

            sf::Vector2f escala_actual = _particulas_de_accion[i].sprite_de_la_particula.getScale();
            _particulas_de_accion[i].sprite_de_la_particula.setScale(escala_actual.x * 0.95f, escala_actual.y * 0.95f);

            i++;
        }
    }
}

///=============================================================///
///   ACTUALIZAR PARTICULAS DE AMBIENTE
///=============================================================///
void VisualFX::actualizar_particulas_de_ambiente(float tiempo_transcurrido) {
    for (int i = 0; i < _cantidad_de_particulas_de_ambiente_en_uso; i++) {

        _particulas_de_ambiente[i].sprite_de_la_particula.move(_particulas_de_ambiente[i].velocidad_de_la_particula * tiempo_transcurrido);

        _particulas_de_ambiente[i].tiempo_hasta_cambio_de_direccion -= tiempo_transcurrido;

        if (_particulas_de_ambiente[i].tiempo_hasta_cambio_de_direccion <= 0.f) {
            float nueva_vel_x = (float)(rand() % 80) - 40.f;
            float nueva_vel_y = (float)(rand() % 80) - 40.f;
            _particulas_de_ambiente[i].velocidad_de_la_particula = sf::Vector2f(nueva_vel_x, nueva_vel_y);
            _particulas_de_ambiente[i].tiempo_hasta_cambio_de_direccion = 1.f + (float)(rand() % 200) / 100.f;
        }
    }
}

///=============================================================///
///   ACTUALIZAR PORTALES
///=============================================================///
void VisualFX::actualizar_portales(float tiempo_transcurrido) {
    int i = 0;
    while (i < _cantidad_de_portales_en_uso) {

        _portales[i].tiempo_de_vida_actual -= tiempo_transcurrido;

        if (_portales[i].tiempo_de_vida_actual <= 0.f) {
            _portales[i] = _portales[_cantidad_de_portales_en_uso - 1];
            _cantidad_de_portales_en_uso--;
        }
        else {
            float progreso = 1.f - (_portales[i].tiempo_de_vida_actual / _portales[i].tiempo_de_vida_maximo);
            int frame_actual = progreso * _portales[i].cantidad_de_frames;
            if (frame_actual >= _portales[i].cantidad_de_frames) {
                frame_actual = _portales[i].cantidad_de_frames - 1;
            }

            int columnas = 3;
            int ancho_del_frame = _textura_de_portal.getSize().x / columnas;
            int alto_del_frame = _textura_de_portal.getSize().y / 2;
            int columna_del_frame = frame_actual % columnas;
            int fila_del_frame = frame_actual / columnas;

            _portales[i].sprite_del_portal.setTextureRect(sf::IntRect(columna_del_frame * ancho_del_frame, fila_del_frame * alto_del_frame, ancho_del_frame, alto_del_frame));
            i++;
        }
    }
}

///=============================================================///
///   ACTUALIZAR Y DIBUJAR
///=============================================================///
void VisualFX::actualizar(float tiempo_transcurrido) {
    actualizar_particulas_de_accion(tiempo_transcurrido);
    actualizar_particulas_de_ambiente(tiempo_transcurrido);
    actualizar_portales(tiempo_transcurrido);
}

void VisualFX::dibujar(sf::RenderWindow& ventana_del_juego) {
    for (int i = 0; i < _cantidad_de_particulas_de_ambiente_en_uso; i++) {
        if (_particulas_de_ambiente[i].usar_modo_glow == true) {
            ventana_del_juego.draw(_particulas_de_ambiente[i].sprite_de_la_particula, sf::BlendAdd);
        }
        else {
            ventana_del_juego.draw(_particulas_de_ambiente[i].sprite_de_la_particula);
        }
    }

    for (int i = 0; i < _cantidad_de_particulas_de_accion_en_uso; i++) {
        if (_particulas_de_accion[i].usar_modo_glow == true) {
            ventana_del_juego.draw(_particulas_de_accion[i].sprite_de_la_particula, sf::BlendAdd);
        }
        else {
            ventana_del_juego.draw(_particulas_de_accion[i].sprite_de_la_particula);
        }
    }

    for (int i = 0; i < _cantidad_de_portales_en_uso; i++) {
        if (_portales[i].usar_modo_glow == true) {
            ventana_del_juego.draw(_portales[i].sprite_del_portal, sf::BlendAdd);
        }
        else {
            ventana_del_juego.draw(_portales[i].sprite_del_portal);
        }
    }
}