#include "GameManager.h"
#include <iostream>
#include "cmath" // Para usar cos() y sin() en spawnearDropSeguro
#include <cstdlib>

using namespace std;

// ==========================================
// 1. INICIALIZACIÓN (El constructor)
// ==========================================
GameManager::GameManager()
    : _ventana(sf::VideoMode(1280, 720), "Daetherial - UTN"),
    _camara(1280.f, 720.f),
    _estado(MENU),
    _menu(1280.f, 720.f),
    _mapa(16, 1.0f),
    _golem(sf::Vector2f(500.f, 400.f)) // Posición inicial del Gólem
{
    _ventana.setFramerateLimit(60);

    // ================= MAPA =================
    if (!_mapa.cargarMapa("assets/collisions_mapa_v1_background.csv", "assets/mapa_v1_background.png")) {
        cout << "❌ No se pudo cargar el mapa. Cerrando el juego." << endl;
        _ventana.close();
    }

    // ================= SONIDO AMBIENTE (CORREGIDO) =================
    // 🎵 Arrancamos la música apenas se crea el GameManager usando tu nueva función
    cambiarMusica(_estado);

    // ================= ITEMS Y OBJETOS (NUEVO) =================
    // 🧪 Spawneamos ítems de prueba para testear tu nuevo sistema
    Item* pocionDePrueba = _itemManager.crearPocionVida();
    spawnearDropSeguro(pocionDePrueba, _itemManager.getTexturaPocionVida(), 400.f, 300.f);

    Item* hornoDePrueba = _itemManager.crearHorno();
    _objectsManager.agregarItemAlMundo(hornoDePrueba, _itemManager.getTexturaHorno(), 550.f, 350.f);

    // ================= CREDITOS =================
    if (!_fontCreditos.loadFromFile("assets/NorthEternal-yYl4V.otf")) {
        cout << "❌ Error cargando fuente de creditos" << endl;
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
        "Presiona ESC para volver\n\n"
        "Para volver preciona ESC"
    );
    _textoCreditos.setPosition(120, 100);
}

// ==========================================
// 2. BUCLE PRINCIPAL (El run)
// ==========================================
void GameManager::run() {
    while (_ventana.isOpen()) {
        procesarEventos();
        actualizar();
        renderizar();
    }
}

// ==========================================
// 3. PROCESO DE EVENTOS (El procesarEventos)
// ==========================================
void GameManager::procesarEventos() {
    sf::Event evento;

    while (_ventana.pollEvent(evento)) {

        if (evento.type == sf::Event::Closed) {
            _ventana.close();
        }

        // Zoom solo en juego
        if (_estado == JUGANDO) {
            _camara.procesarZoom(evento);

            // 🌟 PRENDER/APAGAR DEBUG CON F3
            if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::F3) {
                _debug.toggleDebug();
            }

            _debug.procesarEventos(evento, _hudInventario, _personaje);
        }

        if (_estado == MENU) {
            if (evento.type == sf::Event::KeyPressed) {

                if (evento.key.code == sf::Keyboard::Up) _menu.moveUp();
                if (evento.key.code == sf::Keyboard::Down) _menu.moveDown();

                if (evento.key.code == sf::Keyboard::Enter) {
                    int selected = _menu.getSelectedIndex();

                    if (selected == 0) {
                        _estado = JUGANDO;
                        // 🎵 CORREGIDO: Cambiamos la música al entrar a jugar
                        cambiarMusica(_estado);
                        _reloj.restart();
                    }
                    else if (selected == 2) {
                        _estado = CREDITOS;
                    }
                    else if (selected == 3) {
                        _ventana.close();
                    }
                }
            }
        }

        else if (_estado == CREDITOS) {
            if (evento.type == sf::Event::KeyPressed) {
                if (evento.key.code == sf::Keyboard::Escape) {
                    _estado = MENU;
                    // Opcional: si querés que la música del menú vuelva al salir de créditos
                    // cambiarMusica(_estado); 
                }
            }
        }
    }
}

// ==========================================
// 4. ACTUALIZACIÓN DE LÓGICA (El actualizar)
// ==========================================
void GameManager::actualizar() {
    if (_estado == JUGANDO) {
        float dt = _reloj.restart().asSeconds();

        _camara.seguir(_personaje.getPosicion());
        _personaje.manejarInput(_mapa, _ventana);
        // Actualizamos las habilidades del personaje (mueve proyectiles y actualiza cooldowns)
        _personaje.actualizarHabilidades(dt);

        _mascota.seguir(_personaje.getPosicion());
        _golem.actualizar(_personaje.getPosicion(), dt);
        _niebla.actualizar(dt);
        _objectsManager.chequearInteracciones(_personaje);
    }
}

// ==========================================
// 5. RENDERIZACIÓN (El renderizar)
// ==========================================
void GameManager::renderizar() {
    _ventana.clear(sf::Color(30, 30, 30));

    if (_estado == MENU) {
        _ventana.setView(_ventana.getDefaultView());
        _menu.draw(_ventana);
    }
    else if (_estado == JUGANDO) {
        _ventana.setView(_camara.getVista());
        _mapa.dibujarMapa(_ventana);
        _objectsManager.dibujarItems(_ventana);
        _personaje.dibujar(_ventana);
        _mascota.dibujar(_ventana);
        _golem.dibujar(_ventana);
        _niebla.dibujar(_ventana, _camara.getVista());
        _hudInventario.dibujar(_ventana, _personaje.getInventario());
        _debug.dibujarHitboxes(_ventana, _personaje, _mapa);
    }
    else if (_estado == CREDITOS) {
        _ventana.setView(_ventana.getDefaultView());
        _ventana.draw(_textoCreditos);
    }

    _ventana.display();
}

// ==========================================
// 6. FUNCIONES AUXILIARES 
// ==========================================
void GameManager::spawnearDropSeguro(Item* item, const sf::Texture& textura, float startX, float startY) {
    sf::FloatRect hitbox = item->getBounds();
    hitbox.left = startX;
    hitbox.top = startY;

    int intentos = 0;
    const int MAX_INTENTOS = 100;

    std::cout << "🔍 Debug Hitbox - Ancho: " << hitbox.width << " Alto: " << hitbox.height << std::endl;

    while (_mapa.hayColision(hitbox) && intentos < MAX_INTENTOS) {
        startX += (rand() % 21 - 10);
        startY += (rand() % 21 - 10);
        hitbox.left = startX;
        hitbox.top = startY;
        intentos++;
    }

    if (intentos >= MAX_INTENTOS) {
        std::cout << "⚠️ Advertencia: El drop de " << item->getNombre()
            << " no encontró lugar seguro y quedó en la pared." << std::endl;
    }

    _objectsManager.agregarItemAlMundo(item, const_cast<sf::Texture&>(textura), startX, startY);
}

// ==========================================
// 7. ORQUESTADOR DE AUDIO
// ==========================================
void GameManager::cambiarMusica(GameState nuevoEstado) {
    _musicaAmbiente.stop(); // Paramos lo que esté sonando

    if (nuevoEstado == MENU) {
        _musicaAmbiente.openFromFile("assets/menu_song.ogg");
    }
    else if (nuevoEstado == JUGANDO) {
        _musicaAmbiente.openFromFile("assets/ambient.wav");
    }

    _musicaAmbiente.setLoop(true);
    _musicaAmbiente.play();
}