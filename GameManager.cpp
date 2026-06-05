#include "GameManager.h"
#include <iostream>
#include <cmath> // Para funciones matemáticas
#include <cstdlib> // Para rand() y srand()

using namespace std;

// ============================================================================
// 1. INICIALIZACIÓN Y CONFIGURACIÓN
// ============================================================================
GameManager::GameManager()
    : _ventana(sf::VideoMode(1280, 720), "Daetherial - UTN"),
    _camara(1280.f, 720.f),
    _menu(1280.f, 720.f),
    _mapa(16, 1.0f),
	_estadoActual(new EstadoMenu()) // Inicializamos el estado actual apuntando al menú para que arranque ahí
{
    // --- Configuración del Motor ---
    _camara.setLimitesMundo(sf::FloatRect(0, 0, 2000, 2000));
    _ventana.setFramerateLimit(60);

   /// =================
   /// CURSOR VISUAL
   /// ==================
    _ventana.setMouseCursorVisible(false); // Ocultamos el cursor de Windows para que no se vea encima del nuestro

	// --- Configuración de Entidades ---
	_golem = new Enemy(sf::Vector2f(968.f, 380.f), &_mapa); // Creamos el Gólem con su posición inicial y referencia al mapa
	_golem->setObjetivoJugador(&_personaje); // Pasamos la referencia del jugador para que el Gólem pueda perseguirlo y atacarlo

    // --- Carga del Mundo ---
    if (!_mapa.cargarMapa("assets/collisions_mapa_v1_background.csv", "assets/mapa_v1_background.png")) {
        cout << "❌ Error crítico: No se pudo cargar el mapa." << endl;
        _ventana.close();
    }

    // --- Audio Inicial ---
    cambiarMusica(0);

    // ========================================================================
    // 🌟 SEED DE LA BASE DE DATOS Y SPAWN DE PRUEBA
    // ========================================================================
    if (_itemManager.contarRegistros() == 0) {
        cout << "💾 Base de datos vacia. Generando items de prueba..." << endl;

        ItemReg pocion = { 1, static_cast<int>(TipoItem::Consumible), "Pocion de Vida", 20, 10, static_cast<int>(RarezaItem::Comun), 0, true };
        ItemReg espada = { 2, static_cast<int>(TipoItem::Equipamiento), "Espada Corta", 15, 0, static_cast<int>(RarezaItem::Raro), 1, true };
        ItemReg horno = { 3, static_cast<int>(TipoItem::Mueble), "Horno de Fundicion", 2, 50, static_cast<int>(RarezaItem::Comun), 2, true };

        _itemManager.guardarRegistro(pocion);
        _itemManager.guardarRegistro(espada);
        _itemManager.guardarRegistro(horno);
    }

    Item* pocionPrueba = _itemManager.crearItemPorId(1);
    Item* espadaPrueba = _itemManager.crearItemPorId(2);
    Item* hornoPrueba = _itemManager.crearItemPorId(3);

    // Los tiramos al piso usando tu función segura (ahora sin textura)
    spawnearDropSeguro(pocionPrueba, 300.f, 300.f);
    spawnearDropSeguro(espadaPrueba, 350.f, 300.f);
    spawnearDropSeguro(hornoPrueba, 400.f, 300.f);
	
    // --- Interfaz de Créditos ---
    if (!_fontCreditos.loadFromFile("assets/NorthEternal.otf")) {
        cout << "❌ Error cargando fuente de créditos" << endl;
    }

    _textoCreditos.setFont(_fontCreditos);
    _textoCreditos.setCharacterSize(12);
    _textoCreditos.setFillColor(sf::Color::White);
    _textoCreditos.setString(
        "CREDITOS\n\n"
        "Desarrollado por:\n"
        "Grupo 18 - Programacion 2\n"
        "Turno noche | Comision 102 (Virtual)\n\n"
        "Integrantes:\n"
        "- Federico Wachenschwan\n"
        "- Juan Corbacho\n"
        "- Andres Ignacio Fernandez Escudero\n"
        "- Miguel Salazar\n\n"
        "Tipo de proyecto:\n"
        "Juego\n\n"
        "Descripcion:\n"
        "Juego de supervivencia contra\n"
        "oleadas de mobs con mejoras y logros.\n"
        "Presiona ESC para volver"
    );
    _textoCreditos.setPosition(120, 100);
}

// ============================================================================
// 2. BUCLE PRINCIPAL DEL JUEGO
// ============================================================================
void GameManager::run() {
    while (_ventana.isOpen()) {
        procesarEventos();
        actualizar();
        renderizar();
    }
}

// ============================================================================
// 3. CONTROLADOR DE EVENTOS (Input de teclado y ventana)
// ============================================================================
void GameManager::cambiarEstado(Estado* nuevoEstado) {
    if (_estadoActual != nullptr) {
        delete _estadoActual; // Liberamos la pantalla anterior
    }
    _estadoActual = nuevoEstado;
}

void GameManager::procesarEventos() {
    sf::Event evento;
    while (_ventana.pollEvent(evento)) {
        if (evento.type == sf::Event::Closed) {
            _ventana.close();
        }
        // Delegación polimórfica:
        if (_estadoActual != nullptr) {
            _estadoActual->procesarEventos(evento, *this);
        }
    }
}

void GameManager::actualizar() {
    _cursor.actualizar(_ventana);

    // Solo medimos el dt si no estamos en un menú pausado (opcional),
    // pero por ahora lo dejamos global como lo tenías:
    float dt = _reloj.restart().asSeconds();

    if (_estadoActual != nullptr) {
        _estadoActual->actualizar(dt, *this);
    }
}

void GameManager::renderizar() {
    _ventana.clear(sf::Color(30, 30, 30));

    if (_estadoActual != nullptr) {
        _estadoActual->renderizar(*this);
    }

    _cursor.dibujar(_ventana);
    _ventana.display();
}

// ============================================================================
// RESOLUCIÓN DE COLISIONES ENTRE ENTIDADES (Jugador vs NPCS)
// ============================================================================
void GameManager::colisionEntreEntidades(EntidadViva& jugador, EntidadViva& enemigo) {
    sf::FloatRect boundsJugador = jugador.getBounds();
    sf::FloatRect boundsEnemigo = enemigo.getBounds();
    sf::FloatRect interseccion;

    // Si la hitbox verde del mago toca la violeta del Gólem...
    if (boundsJugador.intersects(boundsEnemigo, interseccion)) {
        sf::Vector2f correccion(0.f, 0.f);

        // Buscamos el eje con menor penetración para saber de qué lado fue el choque
        if (interseccion.width < interseccion.height) {
            // Choque en el eje X
            if (boundsJugador.left < boundsEnemigo.left) {
                correccion.x = -interseccion.width; // Empujar a la izquierda
            }
            else {
                correccion.x = interseccion.width;  // Empujar a la derecha
            }
        }
        else {
            // Choque en el eje Y
            if (boundsJugador.top < boundsEnemigo.top) {
                correccion.y = -interseccion.height; // Empujar hacia arriba
            }
            else {
                correccion.y = interseccion.height;  // Empujar hacia abajo
            }
        }

        // Desplazamos al jugador usando los métodos polimórficos de EntidadViva
        sf::Vector2f posActual = jugador.getPosicion();
        jugador.setPosicion(sf::Vector2f(posActual.x + correccion.x, posActual.y + correccion.y));
    }
}

// ============================================================================
// FUNCIONES AUXILIARES (Utilidades del mundo)
// ============================================================================
void GameManager::spawnearDropSeguro(Item* item, float startX, float startY) {
    sf::FloatRect hitbox = item->getBounds();
    hitbox.left = startX;
    hitbox.top = startY;

    int intentos = 0;
    const int MAX_INTENTOS = 100;

    while (_mapa.hayColision(hitbox) && intentos < MAX_INTENTOS) {
        startX += (rand() % 21 - 10);
        startY += (rand() % 21 - 10);
        hitbox.left = startX;
        hitbox.top = startY;
        intentos++;
    }

    if (intentos >= MAX_INTENTOS) {
        std::cout << "⚠️ Advertencia: Drop bloqueado en pared: " << item->getNombre() << std::endl;
    }

    _objectsManager.agregarItemAlMundo(item, startX, startY);
}

// ============================================================================
// CAMBIO DE MÚSICA DE FONDO SEGÚN EL ESTADO DEL JUEGO
// ============================================================================
void GameManager::cambiarMusica(int musicaID) {
    _musicaAmbiente.stop();

	if (musicaID == 0) { // 0=Menú
        _musicaAmbiente.openFromFile("assets/menu_song.ogg");
    }
	else if (musicaID == 1) { // 1=Jugando
        _musicaAmbiente.openFromFile("assets/ambient.wav");
    }

    _musicaAmbiente.setLoop(true);
    _musicaAmbiente.play();
}

// ============================================================================
// FIN DE GameManager.cpp DESTRUCTOR Y LIMPIEZA DE MEMORIA
// ============================================================================
GameManager::~GameManager() {
    if (_golem != nullptr) {
        delete _golem;
        _golem = nullptr;
    }
}