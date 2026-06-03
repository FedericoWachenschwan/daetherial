#include "Enemy.h"
#include "PathFinder.h"
#include <iostream>
#include <cmath>

// ============================================================================
// ENTIDAD: Enemy (Gólem de Hielo)
// DESCRIPCIÓN: Implementación de la IA cinemática, navegación por waypoints
//              mediante A* y resolución física de colisiones (Wall-Sliding).
// 
// "Sicronizacion de origen": El sprite del Gólem tiene un setOrigin personalizado para que su punto de pivote esté alineado con sus pies, 
// lo que mejora la precisión de las colisiones y la sensación de peso al moverse.
// 
// "Throttling del Pathfinding": Para evitar sobrecargar la CPU con cálculos de A* en cada frame, 
// se implementa un sistema de throttling que limita la frecuencia de recálculo a un máximo de una vez cada 0.5 segundos, 
// o cuando el jugador se desvíe significativamente de la ruta actual.
// 
// "Wall-Sliding": El sistema de movimiento se descompone en ejes X e Y para permitir que el Gólem se deslice suavemente a lo largo de las paredes en lugar de quedar atascado,
// ============================================================================



// ============================================================================
// CONSTRUCTORES Y CONFIGURACIÓN INICIAL
// ============================================================================

/**
 * @brief Constructor por defecto.
 * Inicializa punteros seguros para evitar accesos nulos en memoria antes del spawn.
 */
Enemy::Enemy() : _mapaRef(nullptr) { _danio = 0; }

/**
 * @brief Constructor parametrizado de la entidad.
 * @param posInicial Coordenadas de spawn en el espacio bidimensional del mundo.
 * @param mapa Puntero al mapa de colisiones para la consulta de datos del entorno.
 */
Enemy::Enemy(sf::Vector2f posInicial, Map* mapa) : _mapaRef(mapa) {

    // Carga de recursos gráficos (Textura del Sprite)
    if (!_textura.loadFromFile("assets/icegolem.png")) {
        std::cout << "❌ Error crítico: No se pudo cargar la textura del Gólem de Hielo." << std::endl;
    }

    _sprite.setTexture(_textura);
    _sprite.setOrigin(64.f, 108.f); // Ajuste del origen al centro de los pies para mejorar colisiones y sensación de peso
    _sprite.setPosition(posInicial);

    // Asignacion de atributos específicos del enemigo (pueden ser balanceados luego)
    _velocidad = 55.f;
    _vidaMaxima = 250;
    _vidaActual = 250;
    _danio = 25;
    _cooldownAtaque = 1.5f;
    _rangoAtaque = 40.f;
   }
/**
 * @brief Actualiza la posición del objetivo que la IA debe perseguir.
 * @param posJugador Coordenadas dinámicas del personaje principal.
 */
void Enemy::setPosicionObjetivo(sf::Vector2f posJugador) {
    _posicionObjetivo = posJugador;
}

// ============================================================================
//              BUCLE DE ACTUALIZACIÓN (Orquestador Lógico)
// ============================================================================

/**
 * @brief Actualiza la lógica de la entidad en cada iteración del bucle de juego.
 * @param dt DeltaTime. Tiempo transcurrido desde el último frame para movimiento independiente del hardware.
 */
void Enemy::actualizar(float dt) {
    if (!_mapaRef) return;
    sf::Vector2f posActual = _sprite.getPosition();
    
    // Heurística de proximidad: Distancia Euclídea respecto al jugador
    float distanciaAlJugador = std::hypot(_posicionObjetivo.x - posActual.x, _posicionObjetivo.y - posActual.y);

    // ==============================================
    //              LOGICA DE COMBATE
    // ==============================================
    if (distanciaAlJugador <= _rangoAtaque) {
        // Si EL NPC esta en rango
        if (_relojAtaque.getElapsedTime().asSeconds() >= _cooldownAtaque) {

            if (_jugadorVivoRef != nullptr) {
				// Ejecuta la lógica de ataque (aplicar daño al jugador)
				_jugadorVivoRef->recibirDanio(_danio);
                std::cout << "💥 ¡El Golem te pegó por " << _danio << " de daño!" << std::endl;
            }
            // Reiniciamos el reloj para que no pegue de nuevo instantáneamente
            _relojAtaque.restart();
        }
        _caminoActual.clear();
    }
    // =======================================================
    // SI NO ESTÁ A RANGO, LO PERSIGUE (Culling a 400px)
    // =======================================================
    else if (distanciaAlJugador <= 400.f) {
        activarPathfinder(dt, posActual);
    }
    else {
        _caminoActual.clear();
    }
}


// ============================================================================
// NÚCLEO CINEMÁTICO: SISTEMA DE NAVEGACIÓN Y STEERING
// ============================================================================

/**
 * @brief Administra el consumo de waypoints, el movimiento físico y la lógica de atascos.
 */
void Enemy::activarPathfinder(float dt, sf::Vector2f posActual) {

    // ------------------------------------------------------------------------
        // [SINCRONIZACIÓN DE ORIGEN]: Cálculo del Centro Geométrico Real
        // ------------------------------------------------------------------------
        // 🌟 MAGIA DEL POLIMORFISMO: Obtenemos el centro directamente de la clase base
	// para que funcione tanto con el sprite del Gólem como con el hitbox calibrado de los pies.
    sf::Vector2f centroFisico = getCentroFisico();

    // ------------------------------------------------------------------------
    // [CAPA 1: EL CEREBRO] - Planificación y Throttling del A*
    // ------------------------------------------------------------------------
    float distAlObjetivo = std::hypot(_posicionObjetivo.x - _ultimoDestinoConocido.x,
        _posicionObjetivo.y - _ultimoDestinoConocido.y);

    // Umbral de tolerancia de desvío del jugador antes de forzar un recálculo (150 píxeles)
    bool necesitaRecalculo = distAlObjetivo > 50.f;

    if (_caminoActual.empty() || necesitaRecalculo) {

        // Control de frecuencia (Throttling): Límite de un cálculo cada 0.5 segundos para proteger la CPU
        if (_relojPathfinding.getElapsedTime().asSeconds() > 0.1f) {

            // Invocación polimórfica al buscador de caminos desde el centro real del agente
            std::vector<sf::Vector2f> nuevoCamino = Pathfinder::calcularCamino(*_mapaRef, centroFisico, _posicionObjetivo);

            if (!nuevoCamino.empty()) {
                _caminoActual = nuevoCamino;
                _ultimoDestinoConocido = _posicionObjetivo;
                _relojPathfinding.restart();
            }
        }
    }

    // ------------------------------------------------------------------------
    // [CAPA 2: EL CUERPO] - Control Cinemático y Descomposición de Ejes
    // ------------------------------------------------------------------------
    if (!_caminoActual.empty()) {
        sf::Vector2f siguienteMiga = _caminoActual.front();

        // Vector de dirección y distancia hacia el siguiente nodo de la grilla
        sf::Vector2f direccion = siguienteMiga - centroFisico;
        float distANodo = std::hypot(direccion.x, direccion.y);

        if (distANodo > 5.f) {
            direccion /= distANodo; // Normalización del vector dirección

            // Separación ortogonal del movimiento (Base del Wall-Sliding)
            sf::Vector2f movX(direccion.x * _velocidad * dt, 0.f);
            sf::Vector2f movY(0.f, direccion.y * _velocidad * dt);

            bool seMovio = false;
            bool raspandoPared = false;

            // --- PASO 1: Simulación y ejecución en el Eje X ---
            sf::FloatRect hitboxX = getBounds();
            hitboxX.left += movX.x;
            if (std::abs(movX.x) > 0.01f) {
                if (!_mapaRef->hayColision(hitboxX)) {
                    _sprite.move(movX);
                    seMovio = true;
                }
                else {
                    raspandoPared = true; // Colisión detectada lateralmente
                }
            }

            // --- PASO 2: Simulación y ejecución en el Eje Y ---
            sf::FloatRect hitboxY = getBounds();
            hitboxY.top += movY.y;
            if (std::abs(movY.y) > 0.01f) {
                if (!_mapaRef->hayColision(hitboxY)) {
                    _sprite.move(movY);
                    seMovio = true;
                }
                else {
                    raspandoPared = true; // Colisión detectada verticalmente
                }
            }

            // --- PASO 3: Válvula de Escape Inteligente (Rutinas de re-evaluación) ---
            if (raspandoPared) {
                // Si el agente detecta ineficiencia en la trayectoria por fricción con el entorno:
                _caminoActual.clear();         // Se invalida la ruta ineficiente actual
                _relojPathfinding.restart();  // Se penaliza temporalmente con un delay de reacción (0.5s)
            }

        }
        else {
            // El agente llegó con éxito al radio de aceptación del waypoint actual: se consume el nodo
            _caminoActual.erase(_caminoActual.begin());
        }
    }
}

// ============================================================================
// DIBUJADO Y SISTEMAS GRÁFICOS
// ============================================================================

/**
 * @brief Renderiza el sprite base del enemigo.
 */


// ============================================================================
// INTERFACES FÍSICAS (Colisionable)
// ============================================================================

/**
 * @brief Implementación estricta del contrato de la interfaz Colisionable.
 * @return sf::FloatRect Caja de colisión física (Hitbox) optimizada para el tamaño del mapa.
 */
sf::FloatRect Enemy::getBounds() const {
    sf::Vector2f pos = _sprite.getPosition();

    // Caja física de 32x32 píxeles adaptada al volumen del personaje.
    // El offset respecto a 'pos' está sincronizado con el setOrigin del constructor.
    return sf::FloatRect(pos.x - 16.f, pos.y - 16.f, 32.f, 32.f);
}

// ============================================================================
// HERRAMIENTAS DE DIAGNÓSTICO (Sistemas de Telemetría Visual)
// ============================================================================

/**
 * @brief Renderiza la ruta planificada por el algoritmo A* en tiempo real.
 * Dibuja un VertexArray de líneas uniendo los waypoints y círculos en los nodos exactos.
 */
void Enemy::dibujarPathFinder(sf::RenderWindow& ventana) const {
    if (_caminoActual.empty()) return;

    // 1. Renderizado de la trayectoria continua (LineStrip)
    sf::VertexArray linea(sf::LineStrip, _caminoActual.size() + 1);
    linea[0].position = _sprite.getPosition();
    linea[0].color = sf::Color::Red;

    for (size_t i = 0; i < _caminoActual.size(); ++i) {
        linea[i + 1].position = _caminoActual[i];
        linea[i + 1].color = sf::Color::Red;
    }
    ventana.draw(linea);

    // 2. Renderizado de los puntos de control discretos (Waypoints)
    for (const auto& nodo : _caminoActual) {
        sf::CircleShape puntito(3.f);
        puntito.setFillColor(sf::Color::Yellow);
        puntito.setOrigin(1.5f, 1.5f);
        puntito.setPosition(nodo);
        ventana.draw(puntito);
    }
}

void Enemy::dibujarHitboxEnemy(sf::RenderWindow& ventana) const {
    sf::FloatRect limites = getBounds();
    sf::RectangleShape caja(sf::Vector2f(limites.width, limites.height));
    caja.setPosition(limites.left, limites.top);
    caja.setFillColor(sf::Color(255, 0, 255, 80));
    caja.setOutlineColor(sf::Color::Magenta);
    caja.setOutlineThickness(1.f);
    ventana.draw(caja);
}

