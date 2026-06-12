#include "GameManager.h"
#include "EstadoJugando.h"
#include "EstadoMenu.h"
#include "Estado.h"
#include <cmath>
#include <cstdlib>
#include <vector>


using namespace std;

// ============================================================================
// 1. INICIALIZACIÓN Y CONFIGURACIÓN
// ============================================================================
GameManager::GameManager()
    : _ventana(sf::VideoMode(1280, 720), "Daetherial - UTN"),
    _camara(1280.f, 720.f),
    _menu(1280.f, 720.f),
    _mapa(16, 1.0f),
    _estadoActual(new EstadoMenu())
{
    _camara.setLimitesMundo(sf::FloatRect(0, 0, 2000, 2000));
    _ventana.setFramerateLimit(60);

    _ventana.setMouseCursorVisible(false);

    _golem = new Boss(sf::Vector2f(1696.f, 640.f), &_mapa);
    _golem->setObjetivoJugador(&_personaje);

    if (!_mapa.cargarMapa("assets/collisions_mapa_v1_background.csv", "assets/mapa_v1_background.png")) {
        cout << "❌ Error crítico: No se pudo cargar el mapa." << endl;
        _ventana.close();
    }

    cambiarMusica(0);

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
    ///     TIENDA - Creamos la tienda cerca del inicio del jugador
    ///=====================================================================================///
    Item* item_para_vender = _itemManager.crearItemPorId(1);

    if (item_para_vender == nullptr) {
        std::cout << "ERROR: NO SE PUDO CREAR EL ITEM PARA LA TIENDA" << std::endl;
    }
    else {
        _tienda = new Tienda(sf::Vector2f(100, 260.f), item_para_vender, 10);
        _tienda->cargar_fuente_y_cartel();
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
// 3. CAMBIAR ESTADO
// ============================================================================
void GameManager::cambiarEstado(Estado* nuevoEstado) {
    if (_estadoActual != nullptr) {
        _estadoActual->salir(*this); // Por si algún estado quiere limpiar algo al salir
        delete _estadoActual;
    }
    _estadoActual = nuevoEstado;

    if (_estadoActual != nullptr) {
        _estadoActual->entrar(*this); // Llama al entrar del estado nuevo
    }
}

// ============================================================================
// 4. CONTROLADOR DE EVENTOS
// ============================================================================
void GameManager::procesarEventos() {
    sf::Event evento;
    while (_ventana.pollEvent(evento)) {
        if (evento.type == sf::Event::Closed) {
            _ventana.close();
        }
        if (_estadoActual != nullptr) {
            _estadoActual->procesarEventos(evento, *this);
        }
    }
}

// ============================================================================
// 5. ACTUALIZACIÓN LÓGICA
// ============================================================================
void GameManager::actualizar() {
    _cursor.actualizar(_ventana);

    float dt = _reloj.restart().asSeconds();

    if (_estadoActual != nullptr) {
        _estadoActual->actualizar(dt, *this);
    }
}

// ============================================================================
// 6. RENDERIZADO
// ============================================================================
void GameManager::renderizar() {
    _ventana.clear(sf::Color(30, 30, 30));

    if (_estadoActual != nullptr) {
        _estadoActual->renderizar(*this);
    }

    _cursor.dibujar(_ventana);
    _ventana.display();
}

// ============================================================================
// 7. COLISIONES ENTRE ENTIDADES
// ============================================================================
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

// ============================================================================
// 8. FUNCIONES AUXILIARES
// ============================================================================
void GameManager::spawnearDropSeguro(Item* item, float startX, float startY) {
    if (item == nullptr) return;

    sf::FloatRect hitbox = item->getBounds();
    hitbox.left = startX;
    hitbox.top = startY;

    // 1. Cambiamos '->' por '.' porque _mapa es un objeto, no un puntero
    // 2. Inicializamos la referencia inmediatamente (esto soluciona el C2530)
    const std::vector<BloqueMapa>& paredes = _mapa.getBloqueSolido();

    // 3. Definimos la lambda correctamente capturando 'paredes'
    auto estaBloqueado = [&](const sf::FloatRect& rect) -> bool {
        for (const auto& pared : paredes) {
            if (rect.intersects(pared.getColision())) return true;
        }
        return false;
        };

    int intentos = 0;
    const int MAX_INTENTOS = 100;

    // 4. Llamamos a la lambda pasando el hitbox
    while (estaBloqueado(hitbox) && intentos < MAX_INTENTOS) {
        startX += (rand() % 21 - 10);
        startY += (rand() % 21 - 10);
        hitbox.left = startX;
        hitbox.top = startY;
        intentos++;
    }

    _objectsManager.agregarItemAlMundo(item, startX, startY);
}

// ============================================================================
// 9. CAMBIO DE MÚSICA
// ============================================================================
void GameManager::cambiarMusica(int musicaID) {
    _musicaAmbiente.stop();

    if (musicaID == 0) {
        _musicaAmbiente.openFromFile("assets/menu_song.ogg");
    }
    else if (musicaID == 1) {
        _musicaAmbiente.openFromFile("assets/ambient.wav");
    }

    _musicaAmbiente.setLoop(true);
    _musicaAmbiente.play();
}

// ============================================================================
// DESTRUCTOR
// ============================================================================
GameManager::~GameManager() {
    if (_estadoActual != nullptr) {
        delete _estadoActual;
        _estadoActual = nullptr;
    } // Destructor del estado

    if (_golem != nullptr) {
        delete _golem;
        _golem = nullptr;
    } // Destructor Boss _golem (nivel 1)

    if (_tienda != nullptr) {
        delete _tienda;
        _tienda = nullptr;
    } // Destructor Tienda
}