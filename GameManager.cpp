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
    _estado(MENU),
    _menu(1280.f, 720.f),
    _mapa(16, 1.0f)
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
    cambiarMusica(_estado);

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
            _debug.procesarEventos(evento, _ventana, _hudInventario, _personaje, _golem);
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

    /// =================
    /// CURSOR VISUAL
    /// ==================
    _cursor.actualizar(_ventana); // Movemos el sprite del cursor a donde está el mouse en este frame

    if (_estado != JUGANDO) return;

    float dt = _reloj.restart().asSeconds(); // Calculamos el delta time para que el juego corra a la misma velocidad sin importar el rendimiento de la máquina

    // 1. SISTEMAS CORE
    _input.actualizarEstadoTiempoReal(_ventana);
    _camara.seguir(_personaje.getPosicion(), dt);
    _ventana.setView(_camara.getVista()); // Aseguramos que la vista esté actualizada antes de procesar la lógica del juego para que las posiciones del mouse sean correctas en relación al mundo

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

    // =======================================================================
    // 3.               ACTUALIZACIÓN DE ENTIDADES
    // =======================================================================

    // 1. Primero movemos al jugador
    _personaje.manejarInput(_input, _mapa, _ventana, _hudInventario.isOpen(), dt);
    _personaje.actualizar(dt); // Actualizamos al personaje antes que a los NPCs para que su posición esté actualizada para la IA
    // 2. AHORA calculamos el centro, cuando ya está en su posición final del frame
    sf::Vector2f centroJugador = _personaje.getCentroFisico(); // Obtenemos el centro físico real del personaje para que la IA tenga un objetivo preciso y consistente.
    // 3. Pasamos la posición real y actualizada

    // Si _golem es nullptr (porque ya lo matamos), esto se ignora y no crashea.
    if (_golem != nullptr) {
        _golem->setPosicionObjetivo(centroJugador);
        _golem->actualizar(dt);
		colisionEntreEntidades(_personaje, *_golem); // Chequeamos Colision entre el jugador y el Gólem para aplicar daño si es necesario
    };

    _mascota.setPosicionObjetivo(centroJugador);
    _mascota.actualizar(dt);
    _niebla.actualizar(dt);

    // =======================================================================
    // 4. INTERACCIONES FÍSICAS MUNDO-PERSONAJE
    // =======================================================================
    _objectsManager.chequearInteracciones(_personaje, _input);
    _debug.actualizar(_hudInventario, _personaje, _golem);
    if (_golem != nullptr) {
    }

    //========================================================================
    // 5. COMBATE: MAGIA VS ENEMIGOS
    //========================================================================
    BolaDeFuego& magia = _personaje.getBolaDeFuego();

    // 1. Verificamos si la bola está actualmente volando por la pantalla
    if (magia.estaActiva() && _golem != nullptr) {

        // 2. Si la hitbox de la bola se cruza con la hitbox del Gólem
        if (magia.getBounds().intersects(_golem->getBounds())) {

            // ¡Impacto! Le restamos vida usando el daño del mago (heredado de EntidadViva)
            _golem->recibirDanio(_personaje.getDanio());

            // Destruimos/ocultamos la bola de fuego para que no siga de largo y pegue 2 veces
            magia.desactivar();

            std::cout << "🔥 ¡IMPACTO! El Gólem recibió " << _personaje.getDanio() << " de daño." << std::endl;
            // 3. Verificamos si este golpe en particular le bajó la vida a 0 o menos
            if (_golem->estaMuerto()) {
                std::cout << "💀 ¡EL GÓLEM HA SIDO DERROTADO! Liberando memoria..." << std::endl;

                // Lo borramos físicamente de la RAM
                delete _golem;

                // ⚠️ CRÍTICO: Ponemos el puntero en nulo. 
                // Si no hacés esto, C++ cree que el objeto sigue ahí y explota en el próximo frame.
                _golem = nullptr;
            }
        }
    }
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
        if (_golem != nullptr) {
            _golem->dibujar(_ventana);
        };
        _niebla.dibujar(_ventana, _camara.getVista());

        // --- CAPA 2: MODO DEBUG POLIMÓRFICO ---
        if (_debug.estaActivo()) {
            _mapa.dibujarDebug(_ventana);

            // Dibujado de colisiones con colores semánticos
            _debug.dibujarCajaColision(_ventana, _personaje, sf::Color::Green);
            if (_golem != nullptr) {
                _golem->dibujarPathFinder(_ventana);
                _debug.dibujarCajaColision(_ventana, *_golem, sf::Color::Magenta);
            }
            // _debug.dibujarCajaColision(_ventana, _mascota, sf::Color::Cyan); 
        }

        // --- CAPA 3: INTERFAZ Y HUD (Cámara Estática) ---
        _ventana.setView(_ventana.getDefaultView());
        _hudInventario.dibujar(_ventana, _personaje.getInventario());
    }
    //--- CAPA 4: MODO DEBUG: EXTRACTOR DE TEXTURAS (Solo si el debug está activo y el objetivo es EXTRACTOR) ---
    if (_debug.estaActivo() && _debug.getObjetivoActual() == ObjetivoDebug::EXTRACTOR) {
        _debug.dibujarExtractor(_ventana, _itemManager.getTexturaMaestra());
    } 
    
    /// =================
    /// CURSOR VISUAL
    /// ==================
    _cursor.dibujar(_ventana);      // Dibujamos el cursor al último para que quede encima de todo

    _ventana.display();
}

// ============================================================================
// 6. FUNCIONES AUXILIARES (Utilidades del mundo)
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
// CAMBIO DE MÚSICA DE FONDO SEGÚN EL ESTADO DEL JUEGO
// ============================================================================
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

// ============================================================================
// FIN DE GameManager.cpp DESTRUCTOR Y LIMPIEZA DE MEMORIA
// ============================================================================
GameManager::~GameManager() {
    if (_golem != nullptr) {
        delete _golem;
        _golem = nullptr;
    }
}