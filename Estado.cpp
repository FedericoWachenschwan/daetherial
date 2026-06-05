#include "Estado.h"
#include "GameManager.h"
#include <iostream>

// ============================================================================
// ESTADO: MENÚ
// ============================================================================
void EstadoMenu::procesarEventos(sf::Event& evento, GameManager& GameManager) {
    if (evento.type == sf::Event::KeyPressed) {
        if (evento.key.code == sf::Keyboard::Up) GameManager._menu.moveUp();
        if (evento.key.code == sf::Keyboard::Down) GameManager._menu.moveDown();

        if (evento.key.code == sf::Keyboard::Enter) {
            int selected = GameManager._menu.getSelectedIndex();

            if (selected == 0) {
                // Pasamos al juego
                GameManager.cambiarEstado(new EstadoJugando());
                GameManager.cambiarMusica(1); // 1 = JUGANDO
                GameManager._reloj.restart(); // Reiniciamos el reloj para evitar un delta time gigante
            }
            else if (selected == 1) {
                GameManager.cambiarEstado(new EstadoCreadorItems());
                GameManager._uiCreadorItems.actualizarSprite(GameManager._itemManager.getTexturaMaestra());
            }
            else if (selected == 2) {
                std::cout << "🏆 Pantalla de Logros en construccion..." << std::endl;
            }
            else if (selected == 3) {
                GameManager.cambiarEstado(new EstadoCreditos());
            }
            else if (selected == 4) {
                GameManager._ventana.close();
            }
        }
    }
}

void EstadoMenu::actualizar(float dt, GameManager& GameManager) {
    // El menú es estático, no necesita actualizar físicas por ahora.
}

void EstadoMenu::renderizar(GameManager& GameManager) {
    GameManager._ventana.setView(GameManager._ventana.getDefaultView());
    GameManager._menu.draw(GameManager._ventana);
}

// ============================================================================
// ESTADO: JUGANDO
// ============================================================================
void EstadoJugando::procesarEventos(sf::Event& evento, GameManager& GameManager) {
    GameManager._input.procesarEvento(evento);
    GameManager._camara.procesarZoom(evento);

    // Toggle Debug
    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::F3) {
        GameManager._debug.toggleDebug();
    }
    GameManager._debug.procesarEventos(evento, GameManager._ventana, GameManager._hudInventario, GameManager._personaje, GameManager._golem);
}

void EstadoJugando::actualizar(float dt, GameManager& GameManager) {
    // 1. SISTEMAS CORE
    GameManager._input.actualizarEstadoTiempoReal(GameManager._ventana);
    GameManager._camara.seguir(GameManager._personaje.getPosicion(), dt);
    GameManager._ventana.setView(GameManager._camara.getVista());

    // 2. SISTEMAS DE INTERFAZ E INVENTARIO
    if (GameManager._input.quiereAbrirInventario()) GameManager._hudInventario.toggle();

    if (GameManager._input.quiereAtacar()) {
        GameManager._hudInventario.detectarClicCasillero(GameManager._input.getPosicionMouse(), GameManager._personaje.getInventario(), GameManager._ventana);
    }

    if (GameManager._input.quiereTirarItem()) {
        Item* itemATirar = GameManager._personaje.getInventario().extraerItemPorIndice();
        if (itemATirar != nullptr) {
            itemATirar->setPosicion(GameManager._personaje.getPosicion());
            GameManager._objectsManager.recibirItemSoltado(itemATirar);
        }
    }

    // 3. ACTUALIZACIÓN DE ENTIDADES
    GameManager._personaje.manejarInput(GameManager._input, GameManager._mapa, GameManager._ventana, GameManager._hudInventario.isOpen(), dt);
    GameManager._personaje.actualizar(dt);

    sf::Vector2f centroJugador = GameManager._personaje.getCentroFisico();

    if (GameManager._golem != nullptr) {
        GameManager._golem->setPosicionObjetivo(centroJugador);
        GameManager._golem->actualizar(dt);
        GameManager.colisionEntreEntidades(GameManager._personaje, *GameManager._golem);
    }

    GameManager._mascota.setPosicionObjetivo(centroJugador);
    GameManager._mascota.actualizar(dt);
    GameManager._niebla.actualizar(dt);

    // 4. INTERACCIONES FÍSICAS MUNDO-PERSONAJE
    GameManager._objectsManager.chequearInteracciones(GameManager._personaje, GameManager._input);
    GameManager._debug.actualizar(GameManager._hudInventario, GameManager._personaje, GameManager._golem);

    // 5. COMBATE: MAGIA VS ENEMIGOS
    BolaDeFuego& magia = GameManager._personaje.getBolaDeFuego();
    if (magia.estaActiva() && GameManager._golem != nullptr) {
        if (magia.getBounds().intersects(GameManager._golem->getBounds())) {

            GameManager._golem->recibirDanio(GameManager._personaje.getDanio());
            magia.desactivar();
            std::cout << "🔥 ¡IMPACTO! El Gólem recibió " << GameManager._personaje.getDanio() << " de daño." << std::endl;

            if (GameManager._golem->estaMuerto()) {
                std::cout << "💀 ¡EL GÓLEM HA SIDO DERROTADO! Liberando memoria..." << std::endl;
                delete GameManager._golem;
                GameManager._golem = nullptr; // Clave para no crashear
            }
        }
    }
}

void EstadoJugando::renderizar(GameManager& GameManager) {
    // --- CAPA 1: MUNDO Y ENTIDADES ---
    GameManager._ventana.setView(GameManager._camara.getVista());
    GameManager._mapa.dibujarMapa(GameManager._ventana);
    GameManager._objectsManager.dibujarItems(GameManager._ventana);
    GameManager._personaje.dibujar(GameManager._ventana);
    GameManager._mascota.dibujar(GameManager._ventana);

    if (GameManager._golem != nullptr) {
        GameManager._golem->dibujar(GameManager._ventana);
    }
    GameManager._niebla.dibujar(GameManager._ventana, GameManager._camara.getVista());

    // --- CAPA 2: MODO DEBUG POLIMÓRFICO ---
    if (GameManager._debug.estaActivo()) {
        GameManager._mapa.dibujarDebug(GameManager._ventana);
        GameManager._debug.dibujarCajaColision(GameManager._ventana, GameManager._personaje, sf::Color::Green);
        if (GameManager._golem != nullptr) {
            GameManager._golem->dibujarPathFinder(GameManager._ventana);
            GameManager._debug.dibujarCajaColision(GameManager._ventana, *GameManager._golem, sf::Color::Magenta);
        }
    }

    // --- CAPA 3: INTERFAZ Y HUD ---
    GameManager._ventana.setView(GameManager._ventana.getDefaultView());
    GameManager._hudInventario.dibujar(GameManager._ventana, GameManager._personaje.getInventario());

    // --- CAPA 4: EXTRACTOR DEBUG ---
    if (GameManager._debug.estaActivo() && GameManager._debug.getObjetivoActual() == ObjetivoDebug::EXTRACTOR) {
        GameManager._debug.dibujarExtractor(GameManager._ventana, GameManager._itemManager.getTexturaMaestra());
    }
}

// ============================================================================
// ESTADO: CREADOR DE ÍTEMS
// ============================================================================
void EstadoCreadorItems::procesarEventos(sf::Event& evento, GameManager& GameManager) {
    GameManager._uiCreadorItems.procesarEventos(evento, GameManager._itemManager);

    if (evento.type == sf::Event::KeyPressed &&
        (evento.key.code == sf::Keyboard::Left || evento.key.code == sf::Keyboard::Right)) {
        GameManager._uiCreadorItems.actualizarSprite(GameManager._itemManager.getTexturaMaestra());
    }

    if (GameManager._uiCreadorItems.quiereGuardar()) {
        ItemReg nuevoItem = GameManager._uiCreadorItems.generarRegistro();

        if (GameManager._itemManager.guardarRegistro(nuevoItem)) {
            std::cout << "✅ ÍTEM GUARDADO: " << nuevoItem.nombre << std::endl;
        }
        else {
            std::cout << "❌ Error al guardar el ítem." << std::endl;
        }
        GameManager._uiCreadorItems.confirmarGuardado();
    }

    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
        GameManager.cambiarEstado(new EstadoMenu());
    }
}

void EstadoCreadorItems::actualizar(float dt, GameManager& GameManager) {
    // La UI se actualiza por eventos principalmente
}

void EstadoCreadorItems::renderizar(GameManager& GameManager) {
    GameManager._ventana.setView(GameManager._ventana.getDefaultView());
    GameManager._uiCreadorItems.dibujar(GameManager._ventana);
}

// ============================================================================
// ESTADO: CRÉDITOS
// ============================================================================
void EstadoCreditos::procesarEventos(sf::Event& evento, GameManager& GameManager) {
    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
        GameManager.cambiarEstado(new EstadoMenu());
    }
}

void EstadoCreditos::actualizar(float dt, GameManager& GameManager) {
    // Créditos estáticos
}

void EstadoCreditos::renderizar(GameManager& GameManager) {
    GameManager._ventana.setView(GameManager._ventana.getDefaultView());
    GameManager._ventana.draw(GameManager._textoCreditos);
}