#include "Personaje.h"
#include <iostream>
#include "InputManager.h"

// Constructor: prepara el personaje antes de que empiece el juego
Personaje::Personaje() {

    /// CARGA DE TEXTURAS DEL PERSONAJE:

    // La textura es el archivo PNG cargado en memoria (la imagen cruda)
    // El sprite toma esa textura y la muestra en pantalla en una posición
    // Sin conectarlos, el sprite no sabe qué imagen dibujar
    if (!textura_abajo.loadFromFile("assets/Walk_Down-Sheet.png")) {
        return;
    }
    if (!textura_arriba.loadFromFile("assets/Walk_Up-Sheet.png")) {
        return;
    }

    // La textura de izquierda y derecha es la misma imagen
    // Para la izquierda la espejamos con setScale(-1, 1) más adelante
    if (!textura_derecha.loadFromFile("assets/Walk_Side-Sheet.png")) {
        return;
    }
    if (!textura_izquierda.loadFromFile("assets/Walk_Side-Sheet.png")) {
        return;
    }

    // Por defecto el personaje mira hacia abajo al iniciar
    sprite_del_personaje.setTexture(textura_abajo);

    // setTextureRect le dice al sprite qué parte de la imagen mostrar
    // El spritesheet tiene 6 frames juntos en una sola imagen de 384x64 píxeles
    // IntRect(x, y, ancho, alto) define el rectángulo a recortar
    // Con (0, 0, 64, 64) mostramos solo el primer frame: desde el píxel 0 hasta el 64
    sprite_del_personaje.setTextureRect(sf::IntRect(0, 0, 64, 64));

    // Coloca el personaje en la posición inicial
    sprite_del_personaje.setPosition(100.f, 100.f);

    // Define cuántos píxeles avanza el personaje por cada frame
    velocidad = 2.f;
}

// Esta función se llama cada frame desde el GameManager, le pasamos el manager de input para que pueda leer las teclas, 
// el mapa para que pueda colisionar y la ventana para convertir las coordenadas del mouse

void Personaje::manejarInput(const InputManager& input, Map& mapa, sf::RenderWindow& ventana) {

    // 1. OBTENEMOS EL VECTOR DIRECCIÓN DEL MANAGER (¡Ya viene normalizado!)
    sf::Vector2f direccion = input.getDireccionMovimiento();

    // Calculamos cuánto se va a mover en este frame
    sf::Vector2f movimiento(direccion.x * velocidad, direccion.y * velocidad);

    // 2. CONFIGURAMOS TEXTURAS Y GIROS SEGÚN LA DIRECCIÓN DEL VECTOR
    if (direccion.y < 0.f) { // Va hacia arriba (W)
        sprite_del_personaje.setTexture(textura_arriba);
    }
    else if (direccion.y > 0.f) { // Va hacia abajo (S)
        sprite_del_personaje.setTexture(textura_abajo);
    }

    if (direccion.x > 0.f) { // Va hacia la derecha (D)
        sprite_del_personaje.setTexture(textura_derecha);
        sprite_del_personaje.setScale(1.f, 1.f);
        sprite_del_personaje.setOrigin(0, -15);
    }
    else if (direccion.x < 0.f) { // Va hacia la izquierda (A)
        sprite_del_personaje.setTexture(textura_izquierda);
        sprite_del_personaje.setScale(-1.f, 1.f);
        sprite_del_personaje.setOrigin(64, -15);
    }

    // 3. HABILIDADES (La bola de fuego de Nacho unificada)
    // Usamos los booleanos del manager. Atacar = Clic Izquierdo, Saltar = Espacio
    if (input.quiereAtacar() || input.quiereSaltar()) {
        // Le pedimos la posición del mouse al manager directamente
        sf::Vector2i mousePantalla = input.getPosicionMouse();
        sf::Vector2f mouseMundo = ventana.mapPixelToCoords(mousePantalla, ventana.getView());

        _bolaDeFuego.activar(this->getPosicion(), mouseMundo);
    }

    // =========================================================================
    // 4. RESOLUCIÓN DE COLISIONES POR EJES SEPARADOS (INTACTO)
    // =========================================================================

    // --- EJE X (Horizontal) ---
    if (movimiento.x != 0.f) {
        sprite_del_personaje.move(movimiento.x, 0.f); // Paso fantasma

        bool chocoX = false;
        for (const auto& bloque : mapa.getBloquesSolidos()) {
            if (this->chequearColision(bloque)) {
                chocoX = true;
                break;
            }
        }

        if (chocoX) {
            sprite_del_personaje.move(-movimiento.x, 0.f); // Retrocede
        }
    }

    // --- EJE Y (Vertical) ---
    if (movimiento.y != 0.f) {
        sprite_del_personaje.move(0.f, movimiento.y); // Paso fantasma

        bool chocoY = false;
        for (const auto& bloque : mapa.getBloquesSolidos()) {
            if (this->chequearColision(bloque)) {
                chocoY = true;
                break;
            }
        }

        if (chocoY) {
            sprite_del_personaje.move(0.f, -movimiento.y); // Retrocede
        }
    }
}

// NACHO - Actualiza las habilidades del personaje
void Personaje::actualizarHabilidades(float dt)
{
    _bolaDeFuego.actualizar(dt);
}

// NACHO - Devuelve la posición actual del personaje como par de coordenadas X e Y
sf::Vector2f Personaje::getPosicion() {
    return sprite_del_personaje.getPosition();
}

// NACHO - Dibuja el sprite del personaje en la ventana
void Personaje::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite_del_personaje);
    _bolaDeFuego.dibujar(ventana);
}

// Función de debug para dibujar la hitbox real del personaje (caja 16x16) en verde con opacidad
void Personaje::dibujarDebug(sf::RenderWindow& ventana) {
    sf::FloatRect limites = this->getBounds();

    sf::RectangleShape rectDebug(sf::Vector2f(limites.width, limites.height));
    rectDebug.setPosition(limites.left, limites.top);

    rectDebug.setFillColor(sf::Color(0, 255, 0, 100));
    rectDebug.setOutlineColor(sf::Color::Green);
    rectDebug.setOutlineThickness(-1.f);

    ventana.draw(rectDebug);
}

void Personaje::ajustarOrigenSprite(float x, float y) {
    sf::Vector2f origenActual = sprite_del_personaje.getOrigin();

    float nuevoX = origenActual.x + x;
    float nuevoY = origenActual.y + y;
    sprite_del_personaje.setOrigin(nuevoX, nuevoY);

    std::cout << "🧍 Personaje Origen -> X: " << nuevoX << " | Y: " << nuevoY << std::endl;
}