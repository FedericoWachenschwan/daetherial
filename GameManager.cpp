#include "GameManager.h"
#include <iostream>
#include <cmath> // Para usar cos() y sin()
#include <cstdlib>

using namespace std;

// ============================================================================
// 1. INICIALIZACIÓN Y CONFIGURACIÓN
// ============================================================================
GameManager::GameManager()
    : _ventana(sf::VideoMode(1280, 720), "Daetherial - UTN"),
    _camara(1280.f, 720.f),
    _estado(MENU),
    _menu(1280.f, 720.f),
    _mapa(16, 1.0f),
    _golem(sf::Vector2f(500.f, 400.f), &_mapa) // Posición inicial del Gólem con referencia al mapa
{
    // --- Configuración del Motor ---
    _camara.setLimitesMundo(sf::FloatRect(0, 0, 2000, 2000));
    _ventana.setFramerateLimit(60);

    // --- Carga del Mundo ---
    if (!_mapa.cargarMapa("assets/collisions_mapa_v1_background.csv", "assets/mapa_v1_background.png")) {
        cout << "❌ Error crítico: No se pudo cargar el mapa." << endl;
        _ventana.close();
    }

    // --- Audio Inicial ---
    cambiarMusica(_estado);

    // --- Spawns de Prueba ---
    Item* pocionDePrueba = _itemManager.crearPocionVida();
    spawnearDropSeguro(pocionDePrueba, _itemManager.getTexturaPocionVida(), 400.f, 300.f);

    Item* hornoDePrueba = _itemManager.crearHorno();
    _objectsManager.agregarItemAlMundo(hornoDePrueba, _itemManager.getTexturaHorno(), 550.f, 350.f);

    // --- Interfaz de Créditos ---
    if (!_fontCreditos.loadFromFile("assets/NorthEternal-yYl4V.otf")) {
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
void GameManager::procesarEventos() {
    sf::Event evento;

    while (_ventana.pollEvent(evento)) {

        if (evento.type == sf::Event::Closed) {
            _ventana.close();
        }

        // Sistema global de input
        _input.procesarEvento(evento);

        // --- Lógica según el Estado ---
        switch (_estado) {

        case JUGANDO:
            _camara.procesarZoom(evento);

            // Toggle Debug
            if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::F3) {
                _debug.toggleDebug();
            }
            _debug.procesarEventos(evento, _hudInventario, _personaje, _golem);
            break;

        case MENU:
            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Up) _menu.moveUp();
                if (evento.key.code == sf::Keyboard::Down) _menu.moveDown();

                if (evento.key.code == sf::Keyboard::Enter) {
                    int selected = _menu.getSelectedIndex();
                    if (selected == 0) {
                        _estado = JUGANDO;
                        cambiarMusica(_estado);
                        _reloj.restart(); // Reiniciamos para evitar saltos bruscos de dt
                    }
                    else if (selected == 2) {
                        _estado = CREDITOS;
                    }
                    else if (selected == 3) {
                        _ventana.close();
                    }
                }
            }
            break;

        case CREDITOS:
            if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
                _estado = MENU;
            }
            break;
        }
    }
}

// ============================================================================
// 4. ACTUALIZACIÓN LÓGICA (Física, IA, Interacciones)
// ============================================================================
void GameManager::actualizar() {
    if (_estado != JUGANDO) return;

    float dt = _reloj.restart().asSeconds();

    // 1. SISTEMAS CORE
    _input.actualizarEstadoTiempoReal(_ventana);
    _camara.seguir(_personaje.getPosicion(), dt);

    // Alineamos la ventana con la cámara para que el mouse world-space funcione bien
    _ventana.setView(_camara.getVista());

    // 2. SISTEMAS DE INTERFAZ E INVENTARIO
    if (_input.quiereAbrirInventario()) {
        _hudInventario.toggle();
        std::cout << "🎮 Inventario: " << (_hudInventario.isOpen() ? "Abierto" : "Cerrado") << std::endl;
    }

    if (_input.quiereAtacar()) {
        _hudInventario.detectarClicCasillero(_input.getPosicionMouse(), _personaje.getInventario(), _ventana);
    }

    if (_input.quiereTirarItem()) {
        Item* itemATirar = _personaje.getInventario().extraerItemPorIndice();
        if (itemATirar != nullptr) {
            itemATirar->setPosicion(_personaje.getPosicion());
            _objectsManager.recibirItemSoltado(itemATirar);
            std::cout << "📥 Drop: " << itemATirar->getNombre() << std::endl;
        }
    }

    // 3. ACTUALIZACIÓN DE ENTIDADES
    _personaje.manejarInput(_input, _mapa, _ventana, _hudInventario.isOpen());
    _personaje.actualizar(dt);

    _golem.setPosicionObjetivo(_personaje.getPosicion());
    _golem.actualizar(dt);

	sf::Vector2f centroJugador = _personaje.getCentroFisico(); // Obtenemos el centro físico del jugador para que la mascota lo siga de forma más natural

    _mascota.setPosicionObjetivo(_personaje.getPosicion());
    _mascota.actualizar(dt);

    _niebla.actualizar(dt);

    // 4. INTERACCIONES FÍSICAS MUNDO-PERSONAJE
    _objectsManager.chequearInteracciones(_personaje, _input);

    _debug.actualizar(_hudInventario, _personaje, _golem);
}

// ============================================================================
// 5. RENDERIZADO (Dibujado en pantalla)
// ============================================================================
void GameManager::renderizar() {
    _ventana.clear(sf::Color(30, 30, 30));

    if (_estado == MENU) {
        _ventana.setView(_ventana.getDefaultView());
        _menu.draw(_ventana);
    }
    else if (_estado == CREDITOS) {
        _ventana.setView(_ventana.getDefaultView());
        _ventana.draw(_textoCreditos);
    }
    else if (_estado == JUGANDO) {

        // --- CAPA 1: MUNDO Y ENTIDADES (Camara del Jugador) ---
        _ventana.setView(_camara.getVista());

        _mapa.dibujarMapa(_ventana);
        _objectsManager.dibujarItems(_ventana);
        _personaje.dibujar(_ventana);
        _mascota.dibujar(_ventana);
        _golem.dibujar(_ventana);
        _niebla.dibujar(_ventana, _camara.getVista());

        // --- CAPA 2: MODO DEBUG POLIMÓRFICO ---
        if (_debug.estaActivo()) {
            _mapa.dibujarDebug(_ventana);
            _golem.dibujarPathFinder(_ventana);

            // Dibujado de colisiones con colores semánticos
            _debug.dibujarCajaColision(_ventana, _personaje, sf::Color::Green);
            _debug.dibujarCajaColision(_ventana, _golem, sf::Color::Magenta);
            // _debug.dibujarCajaColision(_ventana, _mascota, sf::Color::Cyan); 
        }

        // --- CAPA 3: INTERFAZ Y HUD (Cámara Estática) ---
        _ventana.setView(_ventana.getDefaultView());
        _hudInventario.dibujar(_ventana, _personaje.getInventario());
    }

    _ventana.display();
}

// ============================================================================
// 6. FUNCIONES AUXILIARES (Utilidades del mundo)
// ============================================================================
void GameManager::spawnearDropSeguro(Item* item, const sf::Texture& textura, float startX, float startY) {
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

    _objectsManager.agregarItemAlMundo(item, textura, startX, startY);
}

void GameManager::cambiarMusica(GameState nuevoEstado) {
    _musicaAmbiente.stop();

    if (nuevoEstado == MENU) {
        _musicaAmbiente.openFromFile("assets/menu_song.ogg");
    }
    else if (nuevoEstado == JUGANDO) {
        _musicaAmbiente.openFromFile("assets/ambient.wav");
    }

    _musicaAmbiente.setLoop(true);
    _musicaAmbiente.play();
}