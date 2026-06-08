#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Colisionable.h"
#include "map.h"
#include "menu.h"
#include "Personaje.h"
#include "Mascota.h"
#include "Camara.h"
#include "Niebla.h"
#include "Boss.h"
#include "EntidadViva.h"
#include "ItemManager.h"
#include "ObjectsManager.h"
#include "UI_Inventario.h"
#include "DebugManager.h"
#include "BolaDeFuego.h"
#include "InputManager.h"
#include "Cursor_Visual.h"
#include "UI_CreadorItems.h"
#include "VisualFX.h"
#include "Tienda.h"

// Le avisamos al compilador que Estado existe sin incluir todo su archivo
// Esto evita la dependencia circular entre GameManager y Estado
class Estado;
class EstadoMenu;
class EstadoJugando;
class EstadoCreadorItems;
class EstadoCreditos;

class GameManager {
    friend class EstadoMenu;
    friend class EstadoJugando;
    friend class EstadoCreadorItems;
    friend class EstadoCreditos;

private:
    UI_CreadorItems _uiCreadorItems;
    InputManager _input;
    DebugManager _debug;
    sf::RenderWindow _ventana;
    Camara _camara;
    Estado* _estadoActual;
    Niebla _niebla;
    sf::Clock _reloj;
    UI_Inventario _hudInventario;
    Menu _menu;
    Map _mapa;
    Personaje _personaje;
    Mascota _mascota;
    Boss* _golem;
    Cursor_Visual _cursor;

    ///=========================
    ///          TIENDA
    ///=========================
    Tienda* _tienda = nullptr;

    ItemManager _itemManager;
    ObjectsManager _objectsManager;
    VisualFX _VisualFX;
    sf::Font _fontCreditos;
    sf::Text _textoCreditos;

    void procesarEventos();
    void actualizar();
    void renderizar();
    void spawnearDropSeguro(Item* item, float startX, float startY);
    void cambiarMusica(int musicaID);
    sf::Music _musicaAmbiente;
    void colisionEntreEntidades(EntidadViva& jugador, EntidadViva& enemigo);

public:
    ~GameManager();
    GameManager();
    void run();
    void cambiarEstado(Estado* nuevoEstado);
};