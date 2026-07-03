#include "VisualFX.h"
#include <iostream>
#include <cstdlib>

///=============================================================///
///   #1 - CONSTRUCTOR
///=============================================================///
// #1
VisualFX::VisualFX() {
    _cantidad_de_particulas_de_accion_en_uso = 0; // Empieza sin particulas de accion
    _cantidad_de_particulas_de_ambiente_en_uso = 0; // Empieza sin particulas de ambiente
    _cantidad_de_portales_en_uso = 0; // Empieza sin portales

    if (_textura_de_particulas.loadFromFile("assets/particleFX.png") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA TEXTURA DE PARTICULAS." << std::endl;
    }
    if (_textura_de_portal.loadFromFile("assets/PortalSpawn.png") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA TEXTURA DEL PORTAL." << std::endl;
    }
    else {
        _textura_de_portal.setSmooth(true); // Suaviza la imagen del portal
    }
}

///=============================================================///
///   #2 - AGREGAR RASTRO - Un destello que queda donde estaba el sprite
///   y se desvanece rapido (lo usa el dash y la bola de fuego)
///=============================================================///
// #2
void VisualFX::agregarRastro(sf::Sprite sprite_base, sf::Color color, float velocidad_de_desvanecimiento, bool glow) {
    if (_cantidad_de_particulas_de_accion_en_uso >= CANTIDAD_MAXIMA_DE_PARTICULAS_DE_ACCION) return; // Array lleno, no hace nada

    Particula nueva_particula;
    nueva_particula.sprite_de_la_particula = sprite_base; // Copia exacta del sprite actual
    nueva_particula.sprite_de_la_particula.setColor(color); // Le aplica el color de rastro
    nueva_particula.velocidad_de_la_particula = sf::Vector2f(0.f, 0.f); // El rastro no se mueve
    nueva_particula.opacidad_actual = 255.f; // Empieza completamente visible
    nueva_particula.velocidad_de_desvanecimiento = velocidad_de_desvanecimiento; // A que velocidad se hace transparente
    nueva_particula.usar_modo_glow = glow; // Si brilla o no al dibujarse

    _particulas_de_accion[_cantidad_de_particulas_de_accion_en_uso] = nueva_particula; // La guarda en el array
    _cantidad_de_particulas_de_accion_en_uso++; // Hay una particula mas activa
}

///=============================================================///
///   #3 - AGREGAR PARTICULA DINAMICA - Una chispa con movimiento propio
///=============================================================///
// #3
void VisualFX::agregarParticulaDinamica(const sf::Texture& textura, sf::Vector2f posicion, sf::Vector2f velocidad, sf::Color color, float velocidad_de_desvanecimiento, bool glow) {
    if (_cantidad_de_particulas_de_accion_en_uso >= CANTIDAD_MAXIMA_DE_PARTICULAS_DE_ACCION) return; // Array lleno

    Particula nueva_particula;
    nueva_particula.sprite_de_la_particula.setTexture(textura); // Asigna la imagen
    nueva_particula.sprite_de_la_particula.setOrigin(textura.getSize().x / 2.f, textura.getSize().y / 2.f); // Centro del sprite
    nueva_particula.sprite_de_la_particula.setScale(0.08f, 0.08f); // La hace muy chiquita
    nueva_particula.sprite_de_la_particula.setPosition(posicion); // Aparece en esta posicion
    nueva_particula.sprite_de_la_particula.setColor(color); // Le aplica el color
    nueva_particula.velocidad_de_la_particula = velocidad; // Con esta velocidad y direccion
    nueva_particula.opacidad_actual = 150.f; // Empieza semitransparente
    nueva_particula.velocidad_de_desvanecimiento = velocidad_de_desvanecimiento; // A que velocidad se desvanece
    nueva_particula.usar_modo_glow = glow; // Si brilla al dibujarse

    _particulas_de_accion[_cantidad_de_particulas_de_accion_en_uso] = nueva_particula; // La guarda en el array
    _cantidad_de_particulas_de_accion_en_uso++; // Hay una particula mas activa
}

///=============================================================///
///   #4 - AGREGAR PARTICULAS DE AMBIENTE - Lucierngas o polvo que
///   bailan por el mapa para siempre, cambiando de direccion
///   cada cierto tiempo
///=============================================================///
// #4
void VisualFX::agregarParticulasAmbiente(sf::Vector2f area_de_aparicion, int cantidad, sf::Color color) {
    for (int i = 0; i < cantidad; i++) {
        if (_cantidad_de_particulas_de_ambiente_en_uso >= CANTIDAD_MAXIMA_DE_PARTICULAS_DE_AMBIENTE) break; // No agrega mas si el array esta lleno

        Particula nueva_particula;
        nueva_particula.sprite_de_la_particula.setTexture(_textura_de_particulas); // Imagen de particula compartida
        nueva_particula.sprite_de_la_particula.setOrigin(_textura_de_particulas.getSize().x / 2.f, _textura_de_particulas.getSize().y / 2.f); // Rota desde el centro
        nueva_particula.sprite_de_la_particula.setScale(0.8f, 0.8f); // Tamanio visible del puntito

        float pos_x = (float)(rand() % (int)area_de_aparicion.x); // Posicion aleatoria en X dentro del area
        float pos_y = (float)(rand() % (int)area_de_aparicion.y); // Posicion aleatoria en Y dentro del area
        nueva_particula.sprite_de_la_particula.setPosition(pos_x, pos_y); // La ubica en el mapa

        float vel_x = (float)(rand() % 80) - 40.f; // Velocidad aleatoria entre -40 y 40
        float vel_y = (float)(rand() % 80) - 40.f; // Velocidad aleatoria entre -40 y 40
        nueva_particula.velocidad_de_la_particula = sf::Vector2f(vel_x, vel_y); // La hace moverse en esa direccion

        nueva_particula.sprite_de_la_particula.setColor(color); // Le da el color del ambiente
        nueva_particula.opacidad_actual = 180.f; // Un poco transparente
        nueva_particula.velocidad_de_desvanecimiento = 0.f; // Nunca se va
        nueva_particula.usar_modo_glow = true; // Las luciernagras brillan
        nueva_particula.tiempo_hasta_cambio_de_direccion = 1.f + (float)(rand() % 200) / 100.f; // Entre 1 y 3 segundos

        _particulas_de_ambiente[_cantidad_de_particulas_de_ambiente_en_uso] = nueva_particula; // La guarda en el array
        _cantidad_de_particulas_de_ambiente_en_uso++; // Hay una particula de ambiente mas
    }
}

///=============================================================///
///   #5 - AGREGAR PORTAL
///=============================================================///
// #5
void VisualFX::agregarPortal(sf::Vector2f posicion, float duracion, int frames, bool glow) {
    if (_cantidad_de_portales_en_uso >= CANTIDAD_MAXIMA_DE_PORTALES) return; // No entra mas
    if (_textura_de_portal.getSize().x == 0) return; // La textura no cargo, no hace nada

    if (frames < 1) {
        frames = 1; // Minimo un frame
    }

    PortalSpawn nuevo_portal;
    nuevo_portal.tiempo_de_vida_actual = duracion; // Empieza con toda su vida
    nuevo_portal.tiempo_de_vida_maximo = duracion; // Guarda la duracion total para calcular progreso
    nuevo_portal.cantidad_de_frames = frames; // Cuantos fotogramas tiene la animacion
    nuevo_portal.usar_modo_glow = glow; // Si brilla al dibujarse

    int columnas = 3; // El spritesheet tiene 3 columnas
    int ancho_del_frame = _textura_de_portal.getSize().x / columnas; // Ancho de un fotograma
    int alto_del_frame = _textura_de_portal.getSize().y / 2; // Alto de un fotograma (dos filas)

    nuevo_portal.sprite_del_portal.setTexture(_textura_de_portal); // Asigna la imagen
    nuevo_portal.sprite_del_portal.setTextureRect(sf::IntRect(0, 0, ancho_del_frame, alto_del_frame)); // Muestra el primer frame
    nuevo_portal.sprite_del_portal.setOrigin(ancho_del_frame / 2.f, alto_del_frame / 2.f); // Centrado
    nuevo_portal.sprite_del_portal.setPosition(posicion); // Donde aparece en el mapa
    nuevo_portal.sprite_del_portal.setScale(2.5f, 2.5f); // Agrandado al doble y medio

    _portales[_cantidad_de_portales_en_uso] = nuevo_portal; // Lo guarda en el array
    _cantidad_de_portales_en_uso++; // Hay un portal mas activo
}

///=============================================================///
///   #6 - ACTUALIZAR PARTICULAS DE ACCION - Las que nacen y mueren.
///   Cuando una muere, ponemos la ultima en su lugar para no
///   dejar huecos en el array
///=============================================================///
// #6
void VisualFX::actualizar_particulas_de_accion(float tiempo_transcurrido) {
    int i = 0;
    while (i < _cantidad_de_particulas_de_accion_en_uso) {

        _particulas_de_accion[i].sprite_de_la_particula.move(_particulas_de_accion[i].velocidad_de_la_particula * tiempo_transcurrido); // Mueve la particula
        _particulas_de_accion[i].opacidad_actual -= _particulas_de_accion[i].velocidad_de_desvanecimiento * tiempo_transcurrido; // Le quita opacidad

        if (_particulas_de_accion[i].opacidad_actual <= 0.f) {
            _particulas_de_accion[i] = _particulas_de_accion[_cantidad_de_particulas_de_accion_en_uso - 1]; // La ultima tapa el hueco
            _cantidad_de_particulas_de_accion_en_uso--; // Hay una particula menos
        }
        else {
            sf::Color color_actual = _particulas_de_accion[i].sprite_de_la_particula.getColor(); // Lee el color actual
            int opacidad_entera = _particulas_de_accion[i].opacidad_actual; // Convierte float a int
            if (opacidad_entera < 0) opacidad_entera = 0; // No baja de cero
            if (opacidad_entera > 255) opacidad_entera = 255; // No sube de 255
            color_actual.a = opacidad_entera; // Actualiza el canal alpha del color
            _particulas_de_accion[i].sprite_de_la_particula.setColor(color_actual); // Aplica el nuevo color

            sf::Vector2f escala_actual = _particulas_de_accion[i].sprite_de_la_particula.getScale(); // Lee la escala actual
            _particulas_de_accion[i].sprite_de_la_particula.setScale(escala_actual.x * 0.95f, escala_actual.y * 0.95f); // Se encoge un 5% por frame

            i++; // Avanza al siguiente solo si esta particula sobrevivio
        }
    }
}

///=============================================================///
///   #7 - ACTUALIZAR PARTICULAS DE AMBIENTE
///=============================================================///
// #7
void VisualFX::actualizar_particulas_de_ambiente(float tiempo_transcurrido) {
    for (int i = 0; i < _cantidad_de_particulas_de_ambiente_en_uso; i++) {

        _particulas_de_ambiente[i].sprite_de_la_particula.move(_particulas_de_ambiente[i].velocidad_de_la_particula * tiempo_transcurrido); // Mueve la luciernaga

        _particulas_de_ambiente[i].tiempo_hasta_cambio_de_direccion -= tiempo_transcurrido; // Descuenta el tiempo hasta cambiar

        if (_particulas_de_ambiente[i].tiempo_hasta_cambio_de_direccion <= 0.f) {
            float nueva_vel_x = (float)(rand() % 80) - 40.f; // Nueva velocidad horizontal aleatoria
            float nueva_vel_y = (float)(rand() % 80) - 40.f; // Nueva velocidad vertical aleatoria
            _particulas_de_ambiente[i].velocidad_de_la_particula = sf::Vector2f(nueva_vel_x, nueva_vel_y); // Cambia de direccion
            _particulas_de_ambiente[i].tiempo_hasta_cambio_de_direccion = 1.f + (float)(rand() % 200) / 100.f; // Nuevo tiempo de entre 1 y 3 segundos
        }
    }
}

///=============================================================///
///   #8 - ACTUALIZAR PORTALES
///=============================================================///
// #8
void VisualFX::actualizar_portales(float tiempo_transcurrido) {
    int i = 0;
    while (i < _cantidad_de_portales_en_uso) {

        _portales[i].tiempo_de_vida_actual -= tiempo_transcurrido; // Descuenta el tiempo de vida

        if (_portales[i].tiempo_de_vida_actual <= 0.f) {
            _portales[i] = _portales[_cantidad_de_portales_en_uso - 1]; // El ultimo tapa el hueco
            _cantidad_de_portales_en_uso--; // Hay un portal menos
        }
        else {
            float progreso = 1.f - (_portales[i].tiempo_de_vida_actual / _portales[i].tiempo_de_vida_maximo); // De 0.0 al comienzo a 1.0 al final
            int frame_actual = progreso * _portales[i].cantidad_de_frames; // Que fotograma mostrar ahora
            if (frame_actual >= _portales[i].cantidad_de_frames) {
                frame_actual = _portales[i].cantidad_de_frames - 1; // No pasa del ultimo frame
            }

            int columnas = 3; // El spritesheet tiene 3 columnas
            int ancho_del_frame = _textura_de_portal.getSize().x / columnas; // Ancho de un fotograma
            int alto_del_frame = _textura_de_portal.getSize().y / 2; // Alto de un fotograma
            int columna_del_frame = frame_actual % columnas; // En que columna esta este frame
            int fila_del_frame = frame_actual / columnas; // En que fila esta este frame

            _portales[i].sprite_del_portal.setTextureRect(sf::IntRect(columna_del_frame * ancho_del_frame, fila_del_frame * alto_del_frame, ancho_del_frame, alto_del_frame)); // Muestra el frame correcto
            i++; // Avanza al siguiente portal
        }
    }
}

///=============================================================///
///   #9 - ACTUALIZAR Y DIBUJAR
///=============================================================///
// #9
void VisualFX::actualizar(float tiempo_transcurrido) {
    actualizar_particulas_de_accion(tiempo_transcurrido); // Actualiza rastros y chispas
    actualizar_particulas_de_ambiente(tiempo_transcurrido); // Actualiza luciernagras
    actualizar_portales(tiempo_transcurrido); // Actualiza animaciones de portales
}

// #10
void VisualFX::dibujar(sf::RenderWindow& ventana_del_juego) {
    for (int i = 0; i < _cantidad_de_particulas_de_ambiente_en_uso; i++) {
        if (_particulas_de_ambiente[i].usar_modo_glow == true) {
            ventana_del_juego.draw(_particulas_de_ambiente[i].sprite_de_la_particula, sf::BlendAdd); // Brillo sobre el fondo
        }
        else {
            ventana_del_juego.draw(_particulas_de_ambiente[i].sprite_de_la_particula); // Dibujado normal
        }
    }

    for (int i = 0; i < _cantidad_de_particulas_de_accion_en_uso; i++) {
        if (_particulas_de_accion[i].usar_modo_glow == true) {
            ventana_del_juego.draw(_particulas_de_accion[i].sprite_de_la_particula, sf::BlendAdd); // Brillo sobre el fondo
        }
        else {
            ventana_del_juego.draw(_particulas_de_accion[i].sprite_de_la_particula); // Dibujado normal
        }
    }

    for (int i = 0; i < _cantidad_de_portales_en_uso; i++) {
        if (_portales[i].usar_modo_glow == true) {
            ventana_del_juego.draw(_portales[i].sprite_del_portal, sf::BlendAdd); // El portal brilla
        }
        else {
            ventana_del_juego.draw(_portales[i].sprite_del_portal); // Dibujado normal
        }
    }
}
