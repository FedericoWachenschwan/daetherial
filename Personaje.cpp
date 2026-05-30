#include "Personaje.h"
#include <iostream>

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

// Lee las teclas W, A, S, D, cambia la textura según la dirección y mueve el personaje
void Personaje::manejarInput(Map& mapa, sf::RenderWindow& ventana) {
    sf::Vector2f movimiento(0.f, 0.f);

    // 1. CAPTURAMOS EL INPUT Y CONFIGURAMOS TEXTURAS / GIROS FIRST
    // Hacemos esto primero para que getGlobalBounds() se actualice con la dirección correcta
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        movimiento.y -= velocidad;
        sprite_del_personaje.setTexture(textura_arriba);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        movimiento.y += velocidad;
        sprite_del_personaje.setTexture(textura_abajo);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        movimiento.x += velocidad;
        sprite_del_personaje.setTexture(textura_derecha);
        sprite_del_personaje.setScale(1.f, 1.f);
        sprite_del_personaje.setOrigin(0, -15);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        movimiento.x -= velocidad;
        sprite_del_personaje.setTexture(textura_izquierda);
        sprite_del_personaje.setScale(-1.f, 1.f);
        sprite_del_personaje.setOrigin(64, -15);
    }

    // =================================================================================================================================
    // 2. RESOLUCIÓN DE COLISIONES POR EJES SEPARADOS (ADIÓS CLIPPING)
    // =================================================================================================================================

    // --- EJE X (Horizontal) ---
    if (movimiento.x != 0.f) {
        sprite_del_personaje.move(movimiento.x, 0.f); // Damos el paso fantasma en X

        // Chequeamos si el rectángulo completo choca contra alguna pared del mapa
        bool chocoX = false;
        for (const auto& bloque : mapa.getBloquesSolidos()) {
            if (this->chequearColision(bloque)) {
                chocoX = true;
                break;
            }
        }

        // Si la caja entera colisionó, lo obligamos a retroceder el paso en X
        if (chocoX) {
            sprite_del_personaje.move(-movimiento.x, 0.f);
        }
    }

    // --- EJE Y (Vertical) ---
    if (movimiento.y != 0.f) {
        sprite_del_personaje.move(0.f, movimiento.y); // Damos el paso fantasma en Y

        bool chocoY = false;
        for (const auto& bloque : mapa.getBloquesSolidos()) {
            if (this->chequearColision(bloque)) {
                chocoY = true;
                break;
            }
        }

        // Si la caja entera colisionó, lo obligamos a retroceder el paso en Y
        if (chocoY) {
            sprite_del_personaje.move(0.f, -movimiento.y);
        }
    }

    // 3. NACHO - Tecla de acción (Espacio) para lanzar habilidad 1
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) 
    {
		sf::Vector2f posicionPersonaje = getPosicion();
        sf::Vector2i mousePantalla = sf::Mouse::getPosition(ventana);
		sf::Vector2f mouseMundo = ventana.mapPixelToCoords(mousePantalla);
        // Llamada para activar la primera habilidad (Bola de fuego)
        // - activa: posiciona el sprite, calcula la dirección y dispara el cooldown
        // - NO mueve el proyectil en este momento; el movimiento ocurre en actualizar(dt)
        primeraHabilidad.activar(posicionPersonaje, mouseMundo);
    }
}

// NACHO - Actualiza las habilidades del personaje
void Personaje::actualizarHabilidades(float dt) 
{
    //  llamamos al método actualizar(dt) de la habilidad
    //  - Esto actualiza el cooldown (cdActualizar), mueve el proyectil si está activo
    //  - y desactiva el proyectil cuando alcanza su rango
    primeraHabilidad.actualizar(dt);
}

// NACHO - Devuelve la posición actual del personaje como par de coordenadas X e Y
sf::Vector2f Personaje::getPosicion() {
    return sprite_del_personaje.getPosition();
}

// NACHO - Dibuja el sprite del personaje en la ventana
void Personaje::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite_del_personaje);
	primeraHabilidad.dibujar(ventana);
}

// Función de debug para dibujar la hitbox real del personaje (caja 16x16) en verde con opacidad
void Personaje::dibujarDebug(sf::RenderWindow& ventana) {
    // 1. Le pedimos a nuestro propio método los límites reales del sprite (AABB)
    sf::FloatRect limites = this->getBounds();

    // 2. Creamos el rectángulo con el tamaño exacto de la hitbox
    sf::RectangleShape rectDebug(sf::Vector2f(limites.width, limites.height));
    rectDebug.setPosition(limites.left, limites.top);

    // 3. Le damos facha de Debug: Verde con 100 de opacidad (el máximo es 255)
    rectDebug.setFillColor(sf::Color(0, 255, 0, 100));
    rectDebug.setOutlineColor(sf::Color::Green);
    rectDebug.setOutlineThickness(-1.f); // Un borde finito para que se note el límite exacto

    // 4. Lo mandamos a la pantalla
    ventana.draw(rectDebug);
}

void Personaje::ajustarOrigenSprite(float x, float y) {
    sf::Vector2f origenActual = sprite_del_personaje.getOrigin();

    float nuevoX = origenActual.x + x;
    float nuevoY = origenActual.y + y;
    sprite_del_personaje.setOrigin(nuevoX, nuevoY);

    std::cout << "🧍 Personaje Origen -> X: " << nuevoX << " | Y: " << nuevoY << std::endl;
}