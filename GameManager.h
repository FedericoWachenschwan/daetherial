#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Colisionable.h"
#include "map.h"
#include "menu.h"
#include "Personaje.h"
#include "Mascota.h"
#include "Camara.h"
#include "Niebla.h"
#include "Enemy.h"
#include "ItemManager.h"    // 🌟 Nuevo: La fábrica de ítems
#include "ObjectsManager.h" // 🌟 Nuevo: Para manejar el mapa
#include "UI_Inventario.h"  // 🌟 Nuevo: La interfaz del inventario
#include "DebugManager.h"   // 🌟 Nuevo: Para activar el modo debug y mover cosas en caliente
#include "BolaDeFuego.h"    // 🌟 Nuevo: Una habilidad de ejemplo para el personaje
#include "InputManager.h" // 🌟 Nuevo: Para manejar el input de forma centralizada

// Estados del juego
enum GameState {
    MENU,
    JUGANDO,
    CREDITOS
};

class GameManager {
private:
    
    // Manager de input para manejar las entradas del jugador de forma centralizada
	InputManager _input; 

	// Habilidad de ejemplo para el personaje (puedes expandir esto con más habilidades y un sistema de gestión de habilidades)
    // La habilidad BolaDeFuego ahora vive en Personaje; se eliminó la instancia duplicada aquí
    
    // Debug Manager para controlar el modo debug y mover cosas en caliente
    DebugManager _debug;
    
    // Ventana principal
    sf::RenderWindow _ventana;

    // Cámara
    Camara _camara;

    // Estado actual del juego
    GameState _estado;
	Niebla _niebla; // Capa de niebla para el efecto visual
	sf::Clock _reloj; // Reloj para medir el tiempo entre frames

    // Objetos del juego
    UI_Inventario _hudInventario;
    Menu _menu;
    Map _mapa;
    Personaje _personaje;
    Mascota _mascota;
	Enemy* _golem; // Puntero al Gólem para decir que sabemos manejar su memoria dinámicamente (si decides crear más enemigos, podrías usar un vector de punteros a enemigos)

    // 🌟 NUEVOS ATRIBUTOS: Tus dos nuevos motores de objetos
    ItemManager _itemManager;
    ObjectsManager _objectsManager;

    // 🔹 Elementos para pantalla de créditos
    sf::Font _fontCreditos;   // Fuente del texto
    sf::Text _textoCreditos;  // Texto a mostrar

    // Métodos del Game Loop
	void procesarEventos(); // Método dedicado al procesamiento de eventos para mantener el código organizado
	void actualizar(); // Método dedicado a la actualización de la lógica del juego para mantener el código organizado
	void renderizar(); // Método dedicado al renderizado para mantener el código organizado
	void spawnearDropSeguro(Item* item, float startX, float startY); //spawnear un drop sin que quede bloqueado en paredes
	void cambiarMusica(GameState nuevoEstado); // Cambia la música de fondo según el estado del juego (ej: música de menú, música de juego, música de créditos)
    sf::Music _musicaAmbiente;
    void colisionEntreEntidades(EntidadViva& jugador, EntidadViva& enemigo); // Maneja la colisión entre el jugador y un enemigo, aplicando daño o efectos según corresponda



public:
    ~GameManager();
    GameManager();
    void run();
};