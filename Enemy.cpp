#include "Enemy.h"
#include "PathFinder.h"
#include <iostream>
#include <cmath>

// Configuración de las dimensiones del frame (Ajustar según escala GIMP u original)
const int FRAME_ANCHO = 152;
const int FRAME_ALTO = 147;

// ============================================================================
// ENTIDAD: Enemy (Gólem de Hielo)
// DESCRIPCIÓN: Implementación de la IA cinemática, navegación por waypoints
//              mediante A* y resolución física de colisiones (Wall-Sliding).
// 
// "Sincronizacion de origen": El sprite del Gólem tiene un setOrigin personalizado para que su punto de pivote esté alineado con sus pies, 
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
Enemy::Enemy() : _mapaRef(nullptr), _estadoActual(EnemyState::IDLE) {} // Constructor por defecto (no recomendado, pero necesario para ciertos contenedores o inicializaciones)

// ============================================================================
// Constructor principal: Carga recursos, configura sprite y atributos de combate
// ============================================================================
Enemy::Enemy(sf::Vector2f posInicial, Map* mapa) : _mapaRef(mapa) {

    // Carga de recursos gráficos (Textura del Sprite)
    if (!_textura.loadFromFile("assets/golemhielo.png")) {
        std::cout << "❌ Error crítico: No se pudo cargar la textura del Gólem de Hielo." << std::endl;
    }

    _sprite.setTexture(_textura);
    _sprite.setOrigin(76.f, 115.f);
    _sprite.setPosition(posInicial);

    // 🌟 Inicializamos las variables PROTECTED de la animación heredadas del padre
    _maxFrames = 3;
    _frameActual = 0;
    _velocidadAnimacion = 0.12f;
    _tiempoFrame = 0.f;

    _sprite.setTextureRect(sf::IntRect(0, 0, FRAME_ANCHO, FRAME_ALTO));

    // Asignacion de atributos específicos del enemigo (pueden ser balanceados luego)
    _estadoActual = EnemyState::IDLE;
    _velocidad = 55.f;
    _vidaMaxima = 250;
    _vidaActual = 250;
    _danio = 25;
    _cooldownAtaque = 1.5f;
    _rangoAtaque = 40.f;
   }

// ============================================================================
// FUNCIONES DE CONFIGURACIÓN Y SETTERS
// ============================================================================
void Enemy::setPosicionObjetivo(sf::Vector2f posJugador) {
    _posicionObjetivo = posJugador;
}

// ============================================================================
//              BUCLE DE ACTUALIZACIÓN (Orquestador Lógico)
// ============================================================================

// ============================================================================
// FUNCION CENTRAL: Orquesta la lógica de comportamiento del enemigo en cada frame
// ===========================================================================
void Enemy::actualizar(float dt) {
    if (!_mapaRef) return;
    sf::Vector2f posAntes = _sprite.getPosition();
    
    // Heurística de proximidad: Distancia Euclídea respecto al jugador
    float distanciaAlJugador = std::hypot(_posicionObjetivo.x - posAntes.x, _posicionObjetivo.y - posAntes.y);

    // ==============================================
    //              LOGICA DE COMBATE
    // ==============================================
    if (distanciaAlJugador <= _rangoAtaque) {
		_estadoActual = EnemyState::ATACANDO;
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
		_estadoActual = EnemyState::PERSIGUIENDO;
        activarPathfinder(dt, posAntes);
    }
    else {
		_estadoActual = EnemyState::IDLE;
        _caminoActual.clear();
    }
    // ==============================================
    //          CÁLCULO DE ANIMACIÓN DICTADO POR LOS PIES
    // ==============================================
    // 2. Capturamos la posición DESPUÉS del movimiento físico
    sf::Vector2f posDespues = _sprite.getPosition();

    // 3. Restamos las posiciones para obtener el vector de desplazamiento REAL del frame
    sf::Vector2f movimientoReal = posDespues - posAntes;

    // ¿El bicho se está moviendo de verdad? (Tolerancia de 0.1 píxeles para evitar ruido)
    if (std::hypot(movimientoReal.x, movimientoReal.y) > 0.1f) {
        // 🌟 Si se está moviendo, se anima hacia donde camina (vía Pathfinder o Wall-Sliding)
        actualizarAnimacion(dt, movimientoReal, _estadoActual);
    }
    else {
        // 🌟 Si está quieto (IDLE o ATACANDO), que se plante y mire fijo al jugador
        sf::Vector2f direccionAlJugador = _posicionObjetivo - posDespues;
        actualizarAnimacion(dt, direccionAlJugador, _estadoActual);
    }
}



// ============================================================================
// NÚCLEO CINEMÁTICO: SISTEMA DE NAVEGACIÓN Y STEERING
// ============================================================================
// 
//=============================================================================
// FUNCIONES DE NAVEGACIÓN Y CONTROL DE MOVIMIENTO
//=============================================================================
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
// ============================================================================
// INTERFACES FÍSICAS (Colisionable)
// ============================================================================

// Polimorfismo puro: Cada entidad viva devuelve su propia caja de colisión (AABB)
sf::FloatRect Enemy::getBounds() const { // Polimorfismo puro: Cada entidad viva devuelve su propia caja de colisión (AABB)
    sf::Vector2f pos = _sprite.getPosition();

    // Caja física de 32x32 píxeles adaptada al volumen del personaje.
    // El offset respecto a 'pos' está sincronizado con el setOrigin del constructor.
    return sf::FloatRect(pos.x - 16.f, pos.y - 16.f, 32.f, 32.f);
}

// ============================================================================
//      ||HERRAMIENTAS DE DIAGNÓSTICO (Sistemas de Telemetría Visual)||
// ============================================================================

//=====================================================================
// Dibuja la trayectoria calculada por el PathFinder con líneas y puntos de control.
//=====================================================================
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

//=====================================================================
// Dibuja la hitbox física del enemigo como un rectángulo semitransparente para calibración y debugging.
//=====================================================================
void Enemy::dibujarHitboxEnemy(sf::RenderWindow& ventana) const {
    sf::FloatRect limites = getBounds();
    sf::RectangleShape caja(sf::Vector2f(limites.width, limites.height));
    caja.setPosition(limites.left, limites.top);
    caja.setFillColor(sf::Color(255, 0, 255, 80));
    caja.setOutlineColor(sf::Color::Magenta);
    caja.setOutlineThickness(1.f);
    ventana.draw(caja);
}

// ============================================================================
// SISTEMA DE ANIMACIÓN POR DIRECCIÓN (Mapeo de la Grilla de GIMP)
// ============================================================================
void Enemy::actualizarAnimacion(float dt, sf::Vector2f direccion, EnemyState estado) {
    static int filaDireccion = 0; // Guarda la orientación para que no parpadee al frenar

    // 1. Si no está IDLE y se está moviendo una cantidad decente, calculamos hacia dónde mira
    if (estado != EnemyState::IDLE && (std::abs(direccion.x) > 0.1f || std::abs(direccion.y) > 0.1f)) {
        // ¿El movimiento es más horizontal que vertical?
        if (std::abs(direccion.x) > std::abs(direccion.y)) {
            filaDireccion = (direccion.x > 0.f) ? 2 : 1; // Fila 2: Derecha | Fila 1: Izquierda
        }
        else {
            filaDireccion = (direccion.y > 0.f) ? 0 : 3; // Fila 0: Abajo | Fila 3: Arriba
        }
    }

	// 2. Si el estado es PERSIGUIENDO, hacemos correr las columnas con efecto ida y vuelta. Si no, lo plantamos en el frame de guardia (columna 1)
    static int pasoAnimacion = 1; // Guarda en qué paso de la caminata quedó

    if (estado == EnemyState::PERSIGUIENDO) {
		_tiempoFrame += dt; // Incrementamos el tiempo acumulado para el cambio de frame
		if (_tiempoFrame >= _velocidadAnimacion) { // Si es hora de cambiar el frame
            // Mapea los índices de columnas de tu GIMP: 0 (izq), 1 (centro), 2 (der)
			int secuenciaFrames[] = { 0, 1, 2, 1 }; // Secuencia de animación: izquierda, centro, derecha, centro (va y viene)
            pasoAnimacion = (pasoAnimacion + 1) % 4; // Cicla perpetuamente entre 0, 1, 2, 3
            _frameActual = secuenciaFrames[pasoAnimacion]; // Traduce el paso al frame real
			_tiempoFrame = 0.f;// Reseteamos el tiempo para el próximo cambio
        }
    }
    else {
        // Si está quieto (IDLE o ATACANDO), lo plantamos en el frame de guardia (columna 1)
        _frameActual = 1;
        pasoAnimacion = 1; // Reseteamos el contador al centro para que cuando vuelva a caminar arranque impecable
    }

    // 3. Aplicamos el recorte matemático usando tus constantes globales FRAME_ANCHO y FRAME_ALTO
    _sprite.setTextureRect(sf::IntRect(
        _frameActual * FRAME_ANCHO,  // Desplazamiento X (Columnas)
        filaDireccion * FRAME_ALTO,  // Desplazamiento Y (Filas)
		FRAME_ANCHO, // Ancho del frame
		FRAME_ALTO // Alto del frame
    ));
}