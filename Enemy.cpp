#include "Enemy.h"
#include "PathFinder.h"
#include <iostream>
#include <cmath>

// 1. EL CONSTRUCTOR VACÍO (Con 0.f para que SFML no se queje)
Enemy::Enemy() : _mapaRef(nullptr), _danio(0) {}

Enemy::Enemy(sf::Vector2f posInicial, Map* mapa) : _mapaRef(mapa) { // Guardamos el puntero al mapa para usarlo en la IA y las colisiones
    if (!_textura.loadFromFile("assets/icegolem.png")) {
        std::cout << "❌ Error al cargar la textura del Enemy (Gólem de Hielo)." << std::endl;
    }

    _sprite.setTexture(_textura);
    _sprite.setOrigin(46.f, 94.f);
    _sprite.setPosition(posInicial);

    _velocidad = 55.f;
    _vidaMaxima = 250;
    _vidaActual = 250;
    _danio = 25;
}
void Enemy::setPosicionObjetivo(sf::Vector2f posJugador) {
    _posicionObjetivo = posJugador;
}


// =======================================================
// EL DIRECTOR DE ORQUESTA (Actualizar)
// =======================================================
void Enemy::actualizar(float dt) {
    if (!_mapaRef) return;

    sf::Vector2f posActual = _sprite.getPosition();

    // Calculamos distancia real usando X e Y (separados por coma)
    float distanciaAlJugador = std::hypot(_posicionObjetivo.x - posActual.x,
        _posicionObjetivo.y - posActual.y);

    // Si estás a menos de 400 píxeles, arranca la máquina
    if (distanciaAlJugador <= 400.f) {
        activarPathfinder(dt, posActual);
    }
    else {
        // Si no, borra la memoria y frena
        _caminoActual.clear();
    }
}

// =======================================================
// EL TRABAJO PESADO (Tu función modular)
// =======================================================
void Enemy::activarPathfinder(float dt, sf::Vector2f posActual) {
    // =======================================================
    // 1. EL CEREBRO (Lógica optimizada)
    // =======================================================
    float distAlObjetivo = std::hypot(_posicionObjetivo.x - _ultimoDestinoConocido.x,
        _posicionObjetivo.y - _ultimoDestinoConocido.y);

    bool necesitaRecalculo = distAlObjetivo > 150.f;

    if (_caminoActual.empty() || necesitaRecalculo) {

        if (_relojPathfinding.getElapsedTime().asSeconds() > 0.5f) {

            // 🌟 CAMBIO 1: SIEMPRE calculamos desde donde estamos parados FÍSICAMENTE
            // Ya no usamos el punto de anclaje de la memoria, porque podemos estar atascados.
            std::vector<sf::Vector2f> nuevoCamino = Pathfinder::calcularCamino(*_mapaRef, posActual, _posicionObjetivo);

            if (!nuevoCamino.empty()) {
                _caminoActual = nuevoCamino;
                _ultimoDestinoConocido = _posicionObjetivo;
                _relojPathfinding.restart();
            }
        }
    }

    // =======================================================
    // 2. EL CUERPO (Movimiento con freno inteligente)
    // =======================================================
    if (!_caminoActual.empty()) {
        sf::Vector2f siguienteMiga = _caminoActual.front();
        sf::Vector2f direccion = siguienteMiga - posActual;
        float distANodo = std::hypot(direccion.x, direccion.y);

        if (distANodo > 5.f) {
            direccion /= distANodo;

            // Separamos el movimiento propuesto en X e Y
            sf::Vector2f movX(direccion.x * _velocidad * dt, 0.f);
            sf::Vector2f movY(0.f, direccion.y * _velocidad * dt);

            bool seMovio = false;

            // 1. INTENTAR MOVER SOLO EN X
            sf::FloatRect hitboxX = getBounds();
            hitboxX.left += movX.x;
            if (std::abs(movX.x) > 0.01f && !_mapaRef->hayColision(hitboxX)) {
                _sprite.move(movX);
                seMovio = true;
            }

            // 2. INTENTAR MOVER SOLO EN Y
            sf::FloatRect hitboxY = getBounds();
            hitboxY.top += movY.y;
            if (std::abs(movY.y) > 0.01f && !_mapaRef->hayColision(hitboxY)) {
                _sprite.move(movY);
                seMovio = true;
            }

            // 3. LA VÁLVULA DE ESCAPE
            // Si estaba bloqueado en ambos ejes perfectos (ej. chocó de frente 100%)
            if (!seMovio) {
                // En vez de borrar TODO el camino y hacer un bucle, descartamos solo 
                // este nodo problemático para que el Gólem apunte al siguiente.
                _caminoActual.erase(_caminoActual.begin());
            }

        }
        else {
            // Llegamos a la baldosa limpia, pasamos a la siguiente
            _caminoActual.erase(_caminoActual.begin());
        }
    }
}


void Enemy::dibujar(sf::RenderWindow& ventana) {
    EntidadViva::dibujar(ventana);
}

sf::FloatRect Enemy::getBounds() const {
    sf::Vector2f pos = _sprite.getPosition();
    // Hitbox de 12x12 (centrada). Le da 2 píxeles de "aire" de cada lado 
    // en un mapa de 16x16 para doblar las esquinas sin engancharse.
    return sf::FloatRect(pos.x - 0.f, pos.y - 0.f, 32.f, 32.f);
}

void Enemy::dibujarPathFinder(sf::RenderWindow& ventana) const {
    // Si la memoria está vacía y no tiene a dónde ir, no dibujamos nada
    if (_caminoActual.empty()) return;

    // =======================================================
    // 1. DIBUJAMOS LA LÍNEA LÁSER ROJA (VertexArray es muy rápido)
    // =======================================================
    sf::VertexArray linea(sf::LineStrip, _caminoActual.size() + 1);

    // El punto de inicio (0) es el pecho del Gólem
    linea[0].position = _sprite.getPosition();
    linea[0].color = sf::Color::Red;

    // Conectamos la línea roja por todos los puntos guardados en su memoria
    for (size_t i = 0; i < _caminoActual.size(); ++i) {
        linea[i + 1].position = _caminoActual[i];
        linea[i + 1].color = sf::Color::Red;
    }

    ventana.draw(linea);

    // =======================================================
    // 2. DIBUJAMOS PUNTITOS AMARILLOS (Los nodos exactos del A*)
    // =======================================================
    for (const auto& nodo : _caminoActual) {
        sf::CircleShape puntito(3.f); // Círculo chiquito de 3 píxeles
        puntito.setFillColor(sf::Color::Yellow);
        puntito.setOrigin(1.5f, 1.5f); // Lo centramos para que coincida con la línea
        puntito.setPosition(nodo);

        ventana.draw(puntito);
    }
}

void Enemy::dibujarHitboxEnemy(sf::RenderWindow& ventana) const {
    // 1. Pedimos la hitbox real física
    sf::FloatRect limites = getBounds();

    // 2. Creamos un rectángulo visual con esas medidas exactas
    sf::RectangleShape cajaDebug(sf::Vector2f(limites.width, limites.height));

    // 3. Lo posicionamos donde está la hitbox
    cajaDebug.setPosition(limites.left, limites.top);

    // 4. Lo pintamos estilo "Neon" para que resalte
    cajaDebug.setFillColor(sf::Color(255, 0, 255, 80));   // Magenta semitransparente para el fondo
    cajaDebug.setOutlineColor(sf::Color::Magenta);        // Borde Magenta sólido
    cajaDebug.setOutlineThickness(1.f);                   // Grosor del borde

    // 5. Lo dibujamos
    ventana.draw(cajaDebug);
}