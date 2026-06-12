#include "Estado.h"
#include "GameManager.h"
#include <iostream>
#include "Enemy.h"

// ============================================================================
// ESTADO: MENÚ
// ============================================================================
void EstadoMenu::procesarEventos(sf::Event& evento, GameManager& gm) {
    if (evento.type == sf::Event::KeyPressed) {
        if (evento.key.code == sf::Keyboard::Up)   gm._menu.moveUp();
        if (evento.key.code == sf::Keyboard::Down) gm._menu.moveDown();

        if (evento.key.code == sf::Keyboard::Enter) {
            int selected = gm._menu.getSelectedIndex();
            if (selected == 0) {
                gm.cambiarEstado(new EstadoJugando());
                gm.cambiarMusica(1);
                gm._reloj.restart();
                gm._mapa.generarClima(gm._VisualFX);
            }
            else if (selected == 1) {
                gm.cambiarEstado(new EstadoCreadorItems());
                gm._uiCreadorItems.actualizarSprite(gm._itemManager.getTexturaMaestra());
            }
            else if (selected == 2) {
                std::cout << "PANTALLA DE LOGROS EN CONSTRUCCION..." << std::endl;
            }
            else if (selected == 3) {
                gm.cambiarEstado(new EstadoCreditos());
            }
            else if (selected == 4) {
                gm._ventana.close();
            }
        }
    }
}

void EstadoMenu::actualizar(float dt, GameManager& gm) {}

void EstadoMenu::renderizar(GameManager& gm) {
    gm._ventana.setView(gm._ventana.getDefaultView());
    gm._menu.draw(gm._ventana);
}

// ============================================================================
// ESTADO: JUGANDO
// ============================================================================
EstadoJugando::~EstadoJugando() {
    for (int i = 0; i < (int)_enemigos.size(); i++) {
        delete _enemigos[i];
    }
    _enemigos.clear();
}

void EstadoJugando::procesarEventos(sf::Event& evento, GameManager& gm) {

    ///============================================================///
    ///     INVENTARIO - Si está abierto, WASD y flechas lo navegan
    ///============================================================///
    if (gm._hudInventario.isOpen() && evento.type == sf::Event::KeyPressed) {

        int indice_actual = gm._personaje.getInventario().getIndiceSeleccionado();
        int cantidad_de_slots = (int)gm._personaje.getInventario().getSlots().size();

        if (evento.key.code == sf::Keyboard::Right || evento.key.code == sf::Keyboard::D) {
            if (indice_actual < cantidad_de_slots - 1) {
                gm._personaje.getInventario().setIndiceSeleccionado(indice_actual + 1);
            }
        }
        if (evento.key.code == sf::Keyboard::Left || evento.key.code == sf::Keyboard::A) {
            if (indice_actual > 0) {
                gm._personaje.getInventario().setIndiceSeleccionado(indice_actual - 1);
            }
        }
        if (evento.key.code == sf::Keyboard::E) {
            gm._personaje.getInventario().usarItem(indice_actual, gm._personaje);
        }
        if (evento.key.code == sf::Keyboard::Escape) {
            gm._hudInventario.toggle(); // Cerrar inventario con Escape
        }
        return; // Mientras el inventario está abierto no procesamos otras teclas
    }

    ///============================================================///
    ///     TIENDA - Si está abierta, WASD y flechas navegan el menú
    ///============================================================///
    if (gm._tienda != nullptr && evento.type == sf::Event::KeyPressed) {

        if (gm._tienda->getLaTiendaEstaAbierta() == true) {
            if (evento.key.code == sf::Keyboard::Right || evento.key.code == sf::Keyboard::D)
                gm._tienda->seleccionar_item_siguiente();
            if (evento.key.code == sf::Keyboard::Left || evento.key.code == sf::Keyboard::A)
                gm._tienda->seleccionar_item_anterior();
            if (evento.key.code == sf::Keyboard::Up || evento.key.code == sf::Keyboard::W)
                gm._tienda->aumentar_cantidad_a_comprar();
            if (evento.key.code == sf::Keyboard::Down || evento.key.code == sf::Keyboard::S)
                gm._tienda->disminuir_cantidad_a_comprar();
            if (evento.key.code == sf::Keyboard::E)
                gm._tienda->intentar_comprar_item_seleccionado(gm._personaje);
            if (evento.key.code == sf::Keyboard::Escape)
                gm._tienda->cerrar_tienda();
            return;
        }

        if (gm._tienda->getJugadorEstaCercaDeLaTienda() == true && evento.key.code == sf::Keyboard::E) {
            gm._tienda->abrir_tienda();
            return;
        }
    }

    gm._input.procesarEvento(evento);
    gm._camara.procesarZoom(evento);

    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::F3) {
        gm._debug.toggleDebug();
    }
    gm._debug.procesarEventos(evento, gm._ventana, gm._hudInventario, gm._personaje, gm._golem);
    if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
        gm._debug.procesarClicMapa(sf::Mouse::getPosition(gm._ventana), gm._camara.getVista(), gm._ventana);
    }
}

void EstadoJugando::actualizar(float dt, GameManager& gm) {

    ///============================================================///
    ///     TIENDA - Siempre chequeamos si el jugador está cerca
    ///============================================================///
    if (gm._tienda != nullptr) {
        gm._tienda->actualizar_tienda(gm._personaje.getPosicion());
    }

    ///============================================================///
    ///     PAUSA - Si la tienda o el inventario están abiertos,
    ///     el juego se congela completamente
    ///============================================================///
    bool la_tienda_esta_abierta = gm._tienda != nullptr && gm._tienda->getLaTiendaEstaAbierta();
    bool el_inventario_esta_abierto = gm._hudInventario.isOpen();

    if (la_tienda_esta_abierta == true || el_inventario_esta_abierto == true) {
        return; // Salimos sin actualizar nada más
    }

    // --- A PARTIR DE ACÁ SOLO LLEGA SI TODO ESTÁ CERRADO ---

    gm._input.actualizarEstadoTiempoReal(gm._ventana);
    gm._camara.seguir(gm._personaje.getPosicion(), dt);
    gm._ventana.setView(gm._camara.getVista());

    if (gm._input.quiereAbrirInventario()) {
        gm._hudInventario.toggle();
        if (gm._hudInventario.isOpen() == true) {
            gm._personaje.getInventario().setIndiceSeleccionado(0); // Al abrir, seleccionamos el primer slot
        }
    }

    if (gm._input.quiereAtacar()) {
        gm._hudInventario.detectarClicCasillero(gm._input.getPosicionMouse(), gm._personaje.getInventario(), gm._ventana);
    }

    if (gm._input.quiereTirarItem()) {
        Item* itemATirar = gm._personaje.getInventario().extraerItemPorIndice();
        if (itemATirar != nullptr) {
            itemATirar->setPosicion(gm._personaje.getPosicion());
            gm._objectsManager.recibirItemSoltado(itemATirar);
        }
    }

    gm._personaje.manejarInput(gm._input, gm._mapa, gm._ventana, gm._hudInventario.isOpen(), dt);
    gm._personaje.actualizar(dt, gm._VisualFX);

    if (gm._golem != nullptr) {
        gm._golem->setPosicionObjetivo(gm._personaje.getCentroFisico());
        gm._golem->actualizar(dt);
        gm.colisionEntreEntidades(gm._personaje, *gm._golem);
    }

    actualizarHordaYSpawns(dt, gm);
    resolverCombateMagia(gm);

    gm._VisualFX.actualizar(dt);
    gm._mascota.setPosicionObjetivo(gm._personaje.getCentroFisico());
    gm._mascota.actualizar(dt);
    gm._niebla.actualizar(dt);
    gm._objectsManager.chequearInteracciones(gm._personaje, gm._input);
    gm._debug.actualizar(gm._hudInventario, gm._personaje, gm._golem);
}

void EstadoJugando::renderizar(GameManager& gm) {

    // --- CAPA 1: MUNDO (con vista de cámara) ---
    gm._ventana.setView(gm._camara.getVista());
    gm._mapa.dibujarMapa(gm._ventana);
    gm._VisualFX.dibujar(gm._ventana);
    gm._objectsManager.dibujarItems(gm._ventana);

    if (gm._tienda != nullptr) {
        gm._tienda->dibujar_sprite_en_el_mapa(gm._ventana, gm._debug.estaActivo());
    }

    for (int i = 0; i < (int)_enemigos.size(); i++) {
        _enemigos[i]->dibujar(gm._ventana);
    }
    gm._personaje.dibujar(gm._ventana);
    gm._mascota.dibujar(gm._ventana);
    if (gm._golem != nullptr) gm._golem->dibujar(gm._ventana);
    gm._niebla.dibujar(gm._ventana, gm._camara.getVista());

    // --- CAPA 2: DEBUG ---
    if (gm._debug.estaActivo()) {
        gm._mapa.dibujarDebug(gm._ventana);
        gm._debug.dibujarCajaColision(gm._ventana, gm._personaje, sf::Color::Green);
        gm._debug.dibujarGrillaMapa(gm._ventana);
        for (int i = 0; i < (int)_enemigos.size(); i++) {
            gm._debug.dibujarCajaColision(gm._ventana, *_enemigos[i], sf::Color::Red);
        }
        if (gm._golem != nullptr) {
            gm._golem->dibujarPathFinder(gm._ventana);
            gm._debug.dibujarCajaColision(gm._ventana, *gm._golem, sf::Color::Magenta);
        }
    }

    // --- CAPA 3: HUD (vista de pantalla) ---
    gm._ventana.setView(gm._ventana.getDefaultView());
    gm._hudInventario.dibujar(gm._ventana, gm._personaje.getInventario());

    ///============================================================///
    ///     HUD - ORO, VIDA Y MANÁ arriba a la derecha
    ///============================================================///
    sf::Text texto_del_oro;
    texto_del_oro.setFont(gm._fontCreditos);
    texto_del_oro.setCharacterSize(16);
    texto_del_oro.setFillColor(sf::Color::Yellow);
    texto_del_oro.setString("Oro:  " + std::to_string(gm._personaje.getOro()));
    texto_del_oro.setPosition(1100.f, 10.f);
    gm._ventana.draw(texto_del_oro);

    sf::Text texto_de_la_vida;
    texto_de_la_vida.setFont(gm._fontCreditos);
    texto_de_la_vida.setCharacterSize(16);
    texto_de_la_vida.setFillColor(sf::Color::Red);
    texto_de_la_vida.setString("Vida: " + std::to_string(gm._personaje.getVida()) + "/" + std::to_string(gm._personaje.getVidaMaxima()));
    texto_de_la_vida.setPosition(1100.f, 30.f);
    gm._ventana.draw(texto_de_la_vida);

    sf::Text texto_del_mana;
    texto_del_mana.setFont(gm._fontCreditos);
    texto_del_mana.setCharacterSize(16);
    texto_del_mana.setFillColor(sf::Color::Cyan);
    texto_del_mana.setString("Mana: " + std::to_string(gm._personaje.getMana()) + "/" + std::to_string(gm._personaje.getManaMaXima()));
    texto_del_mana.setPosition(1100.f, 50.f);
    gm._ventana.draw(texto_del_mana);

    // Interfaz de la tienda
    if (gm._tienda != nullptr) {
        gm._tienda->dibujar_interfaz_de_compra(gm._ventana, gm._fontCreditos);
    }

    if (gm._debug.estaActivo() && gm._debug.getObjetivoActual() == ObjetivoDebug::EXTRACTOR) {
        gm._debug.dibujarExtractor(gm._ventana, gm._itemManager.getTexturaMaestra());
    }
}

// ============================================================================
// SUB-FUNCIONES DE LÓGICA
// ============================================================================
void EstadoJugando::actualizarHordaYSpawns(float dt, GameManager& gm) {
    sf::Vector2f centroJugador = gm._personaje.getCentroFisico();

    if (gm._golem != nullptr && gm._golem->estaVivo()) {
        _relojSpawn += dt;
        if (_relojSpawn >= _intervaloSpawn) {
            sf::Vector2f posVFX(1344.f, 1408.f);
            gm._VisualFX.agregarPortal(posVFX);
            EntidadViva* marcianitos = new Enemy(posVFX, &gm._mapa, "assets/marciano.png");
            _enemigos.push_back(marcianitos);
            _relojSpawn = 0.f;
        }
    }

    for (int i = 0; i < (int)_enemigos.size(); i++) {
        _enemigos[i]->setPosicionObjetivo(centroJugador);
        _enemigos[i]->actualizar(dt);

        if (_enemigos[i]->getBounds().intersects(gm._personaje.getBounds())) {
            if (_enemigos[i]->puedeAtacar()) {
                gm._personaje.recibirDanio(_enemigos[i]->getDanio());
                sf::Vector2f dirEmpuje = gm._personaje.getPosicion() - _enemigos[i]->getPosicion();
                float len = std::hypot(dirEmpuje.x, dirEmpuje.y);
                if (len != 0) dirEmpuje /= len;
                gm._personaje.aplicarMovimientoConColisiones(dirEmpuje * 25.f, gm._mapa);
                std::cout << "GOLPE Y EMPUJE!" << std::endl;
            }
        }
    }
}

void EstadoJugando::resolverCombateMagia(GameManager& gm) {
    BolaDeFuego& magia = gm._personaje.getBolaDeFuego();
    if (!magia.estaActiva()) return;

    bool impacto = false;

    for (int i = 0; i < (int)_enemigos.size(); i++) {
        if (magia.getBounds().intersects(_enemigos[i]->getBounds())) {
            _enemigos[i]->recibirDanio(gm._personaje.getDanio());
            impacto = true;
            if (_enemigos[i]->estaMuerto()) {
                delete _enemigos[i];
                _enemigos.erase(_enemigos.begin() + i);
                i--;
            }
            break;
        }
    }

    if (!impacto && gm._golem != nullptr) {
        if (magia.getBounds().intersects(gm._golem->getBounds())) {
            gm._golem->recibirDanio(gm._personaje.getDanio());
            impacto = true;
            if (gm._golem->estaMuerto()) {
                std::cout << "EL GOLEM HA SIDO DERROTADO!" << std::endl;
                delete gm._golem;
                gm._golem = nullptr;
            }
        }
    }

    if (impacto) magia.desactivar();
}

// ============================================================================
// ESTADO: CREADOR DE ÍTEMS
// ============================================================================
void EstadoCreadorItems::procesarEventos(sf::Event& evento, GameManager& gm) {
    gm._uiCreadorItems.procesarEventos(evento, gm._itemManager);

    if (evento.type == sf::Event::KeyPressed &&
        (evento.key.code == sf::Keyboard::Left || evento.key.code == sf::Keyboard::Right)) {
        gm._uiCreadorItems.actualizarSprite(gm._itemManager.getTexturaMaestra());
    }

    if (gm._uiCreadorItems.quiereGuardar()) {
        ItemReg nuevoItem = gm._uiCreadorItems.generarRegistro();
        if (gm._itemManager.guardarRegistro(nuevoItem)) {
            std::cout << "ITEM GUARDADO: " << nuevoItem.nombre << std::endl;
        }
        else {
            std::cout << "ERROR AL GUARDAR EL ITEM." << std::endl;
        }
        gm._uiCreadorItems.confirmarGuardado();
    }

    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
        gm.cambiarEstado(new EstadoMenu());
    }
}

void EstadoCreadorItems::actualizar(float dt, GameManager& gm) {}

void EstadoCreadorItems::renderizar(GameManager& gm) {
    gm._ventana.setView(gm._ventana.getDefaultView());
    gm._uiCreadorItems.dibujar(gm._ventana);
}

// ============================================================================
// ESTADO: CRÉDITOS
// ============================================================================
void EstadoCreditos::procesarEventos(sf::Event& evento, GameManager& gm) {
    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
        gm.cambiarEstado(new EstadoMenu());
    }
}

void EstadoCreditos::actualizar(float dt, GameManager& gm) {}

void EstadoCreditos::renderizar(GameManager& gm) {
    gm._ventana.setView(gm._ventana.getDefaultView());
    gm._ventana.draw(gm._textoCreditos);
}