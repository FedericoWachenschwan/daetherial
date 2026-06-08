#include "GameManager.h"
#include <iostream>
#include <cmath>
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
    _mapa(16, 1.0f)
{
    _camara.setLimitesMundo(sf::FloatRect(0, 0, 2000, 2000));
    _ventana.setFramerateLimit(60);

    ///=================
    /// CURSOR VISUAL
    ///=================
    _ventana.setMouseCursorVisible(false); // Ocultamos el cursor de Windows

    _golem = new Enemy(sf::Vector2f(968.f, 380.f), &_mapa); // Creamos el golem en su posición inicial
    _golem->setObjetivoJugador(&_personaje); // Le pasamos al golem la referencia del jugador

    if (!_mapa.cargarMapa("assets/collisions_mapa_v1_background.csv", "assets/mapa_v1_background.png")) {
        cout << "❌ Error crítico: No se pudo cargar el mapa." << endl;
        _ventana.close();
    }

    cambiarMusica(_estado); // Arrancamos con la música del menú

    // ========================================================================
    // SEED DE LA BASE DE DATOS Y SPAWN DE PRUEBA
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

    spawnearDropSeguro(pocionPrueba, 300.f, 300.f);
    spawnearDropSeguro(espadaPrueba, 350.f, 300.f);
    spawnearDropSeguro(hornoPrueba, 400.f, 300.f);

    ///=====================================================================================///
    ///     TIENDA -  POSICIÓN DEL SPRITE → Creamos la tienda cerca del inicio del jugador
    ///=====================================================================================///
    Item* item_para_vender = _itemManager.crearItemPorId(1); // Intentamos crear la poción para vender

    if (item_para_vender == nullptr) {
        std::cout << "ERROR: NO SE PUDO CREAR EL ITEM PARA LA TIENDA" << std::endl; // AVISAMOS SI FALLA
    }
    else {
        _tienda = new Tienda(sf::Vector2f(100, 260.f), item_para_vender, 10); // Creamos la tienda
        _tienda->cargar_fuente_y_cartel(); // Cargamos la fuente y el cartel una sola vez al arrancar
    }

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
// 3. CONTROLADOR DE EVENTOS
// ============================================================================
void GameManager::procesarEventos() {
    sf::Event evento;

    while (_ventana.pollEvent(evento)) {

        if (evento.type == sf::Event::Closed) {
            _ventana.close();
        }

        switch (_estado) {

        case JUGANDO:
            _input.procesarEvento(evento);
            _camara.procesarZoom(evento);

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
                        _reloj.restart();
                    }
                    else if (selected == 1) {
                        _estado = CREADOR_ITEMS;
                        _uiCreadorItems.actualizarSprite(_itemManager.getTexturaMaestra());
                    }
                    else if (selected == 2) {
                        std::cout << "🏆 Pantalla de Logros en construccion..." << std::endl;
                    }
                    else if (selected == 3) {
                        _estado = CREDITOS;
                    }
                    else if (selected == 4) {
                        _ventana.close();
                    }
                }
            }
            break;

        case CREADOR_ITEMS:
            _uiCreadorItems.procesarEventos(evento, _itemManager);

            if (evento.type == sf::Event::KeyPressed &&
                (evento.key.code == sf::Keyboard::Left || evento.key.code == sf::Keyboard::Right)) {
                _uiCreadorItems.actualizarSprite(_itemManager.getTexturaMaestra());
            }

            if (_uiCreadorItems.quiereGuardar()) {
                ItemReg nuevoItem = _uiCreadorItems.generarRegistro();

                if (_itemManager.guardarRegistro(nuevoItem)) {
                    std::cout << "✅ ÍTEM GUARDADO EN LA BASE DE DATOS: " << nuevoItem.nombre << std::endl;
                }
                else {
                    std::cout << "❌ Error al guardar el ítem." << std::endl;
                }

                _uiCreadorItems.confirmarGuardado();
            }

            if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
                _estado = MENU;
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
// 4. ACTUALIZACIÓN LÓGICA
// ============================================================================
void GameManager::actualizar() {

    ///=================
    /// CURSOR VISUAL
    ///=================
    _cursor.actualizar(_ventana); // Actualizamos el cursor en todos los estados

    if (_estado != JUGANDO) return;

    float dt = _reloj.restart().asSeconds(); // Tiempo entre frames

    _input.actualizarEstadoTiempoReal(_ventana);
    _camara.seguir(_personaje.getPosicion(), dt);
    _ventana.setView(_camara.getVista());

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

    _personaje.manejarInput(_input, _mapa, _ventana, _hudInventario.isOpen(), dt);
    _personaje.actualizar(dt);
    sf::Vector2f centroJugador = _personaje.getCentroFisico();

    if (_golem != nullptr) {
        _golem->setPosicionObjetivo(centroJugador);
        _golem->actualizar(dt);
        colisionEntreEntidades(_personaje, *_golem);
    }

    _mascota.setPosicionObjetivo(centroJugador);
    _mascota.actualizar(dt);
    _niebla.actualizar(dt);

    ///============================================================///
    ///     TIENDA - Actualizamos y chequeamos si el jugador compra
    ///============================================================///
    if (_tienda != nullptr) {
        _tienda->actualizar_tienda(_personaje.getPosicion()); // Actualizamos la tienda con la posición del jugador

        bool jugador_quiere_comprar = sf::Keyboard::isKeyPressed(sf::Keyboard::E); // Chequeamos si el jugador presiona E
        bool jugador_esta_cerca_de_la_tienda = _tienda->getJugadorEstaCercaDeLaTienda(); // Chequeamos si el jugador está cerca

        if (jugador_esta_cerca_de_la_tienda == true && jugador_quiere_comprar == true) {
            _tienda->intentar_comprar_item(_personaje); // Si está cerca y presiona E, intentamos comprar
        }
    }
    

    _objectsManager.chequearInteracciones(_personaje, _input);
    _debug.actualizar(_hudInventario, _personaje, _golem);

    BolaDeFuego& magia = _personaje.getBolaDeFuego();

    if (magia.estaActiva() && _golem != nullptr) {
        if (magia.getBounds().intersects(_golem->getBounds())) {
            _golem->recibirDanio(_personaje.getDanio());
            magia.desactivar();
            std::cout << "🔥 ¡IMPACTO! El Gólem recibió " << _personaje.getDanio() << " de daño." << std::endl;

            if (_golem->estaMuerto()) {
                std::cout << "💀 ¡EL GÓLEM HA SIDO DERROTADO! Liberando memoria..." << std::endl;
                delete _golem;
                _golem = nullptr;
            }
        }
    }
}

// ============================================================================
// 5. RENDERIZADO
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
    else if (_estado == CREADOR_ITEMS) {
        _ventana.setView(_ventana.getDefaultView());
        _uiCreadorItems.dibujar(_ventana);
    }
    else if (_estado == JUGANDO) {

        _ventana.setView(_camara.getVista());

        _mapa.dibujarMapa(_ventana);
        _objectsManager.dibujarItems(_ventana);

        ///============================================================///
        ///     TIENDA - Dibujamos la tienda solo si existe
        ///============================================================///
        if (_tienda != nullptr) {
            _tienda->dibujar_tienda(_ventana, _debug.estaActivo()); // Pasamos si el debug está activo para mostrar la zona verde con F3 
        }

        _personaje.dibujar(_ventana);
        _mascota.dibujar(_ventana);
        if (_golem != nullptr) {
            _golem->dibujar(_ventana);
        }
        _niebla.dibujar(_ventana, _camara.getVista());

        if (_debug.estaActivo()) {
            _mapa.dibujarDebug(_ventana);
            _debug.dibujarCajaColision(_ventana, _personaje, sf::Color::Green);
            if (_golem != nullptr) {
                _golem->dibujarPathFinder(_ventana);
                _debug.dibujarCajaColision(_ventana, *_golem, sf::Color::Magenta);
            }
        }

        _ventana.setView(_ventana.getDefaultView());
        _hudInventario.dibujar(_ventana, _personaje.getInventario());
    }

    if (_debug.estaActivo() && _debug.getObjetivoActual() == ObjetivoDebug::EXTRACTOR) {
        _debug.dibujarExtractor(_ventana, _itemManager.getTexturaMaestra());
    }

    ///=================
    /// CURSOR VISUAL
    ///=================
    _cursor.dibujar(_ventana); // Dibujamos el cursor encima de todo

    _ventana.display();
}

// ============================================================================
// 6. FUNCIONES AUXILIARES
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

void GameManager::colisionEntreEntidades(EntidadViva& jugador, EntidadViva& enemigo) {
    sf::FloatRect boundsJugador = jugador.getBounds();
    sf::FloatRect boundsEnemigo = enemigo.getBounds();
    sf::FloatRect interseccion;

    if (boundsJugador.intersects(boundsEnemigo, interseccion)) {
        sf::Vector2f correccion(0.f, 0.f);

        if (interseccion.width < interseccion.height) {
            if (boundsJugador.left < boundsEnemigo.left) {
                correccion.x = -interseccion.width;
            }
            else {
                correccion.x = interseccion.width;
            }
        }
        else {
            if (boundsJugador.top < boundsEnemigo.top) {
                correccion.y = -interseccion.height;
            }
            else {
                correccion.y = interseccion.height;
            }
        }

        sf::Vector2f posActual = jugador.getPosicion();
        jugador.setPosicion(sf::Vector2f(posActual.x + correccion.x, posActual.y + correccion.y));
    }
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

// ============================================================================
// DESTRUCTOR - Libera toda la memoria al cerrar el juego
// ============================================================================
GameManager::~GameManager() {
    if (_golem != nullptr) {
        delete _golem; // Liberamos la memoria del golem
        _golem = nullptr; // Evitamos puntero colgante
    }

    if (_tienda != nullptr) {
        delete _tienda; // Liberamos la memoria de la tienda
        _tienda = nullptr; // Evitamos puntero colgante
    }
}