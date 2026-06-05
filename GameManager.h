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
#include "ItemManager.h"    
#include "ObjectsManager.h" 
#include "UI_Inventario.h"  
#include "DebugManager.h"   
#include "BolaDeFuego.h"    
#include "InputManager.h" 
#include "Cursor_Visual.h"      
#include "UI_CreadorItems.h" 
#include "Estado.h"

class GameManager {
	// Para que los estados puedan acceder a los métodos privados del GameManager sin hacerlos públicos para todo el mundo
    friend class EstadoMenu;
    friend class EstadoJugando;
    friend class EstadoCreadorItems;
    friend class EstadoCreditos;

private:
	UI_CreadorItems _uiCreadorItems; // Interfaz del creador de ítems
	InputManager _input; // Manejo de entradas del jugador
	DebugManager _debug; // Modo debug para visualizar hitboxes y el extractor de texturas
	sf::RenderWindow _ventana; // Ventana principal del juego
	Camara _camara; // Cámara que sigue al jugador por el mundo

    // Estado actual del juego
	Estado* _estadoActual; // Puntero al estado actual (puede ser menú, juego, creador de ítems, créditos, etc.)
	Niebla _niebla; // Efecto de niebla para darle ambiente al juego
	sf::Clock _reloj; // Reloj para calcular el delta time entre frames y lograr una velocidad de juego consistente

    // Objetos del juego
	UI_Inventario _hudInventario; // Interfaz de inventario para mostrar la mochila del jugador
	Menu _menu; // Pantalla de menú principal
	Map _mapa; // Mapa del mundo con colisiones y dibujo
	Personaje _personaje; // El jugador controlable
	Mascota _mascota; // Compañero que sigue al jugador
	Enemy* _golem; // Enemigo principal del juego (inicializado dinámicamente para poder matarlo y liberar su memoria)

	Cursor_Visual _cursor; // Cursor personalizado que sigue al mouse

    // Managers
	ItemManager _itemManager; // Manejo de creación, almacenamiento y consulta de ítems (base de datos)
	ObjectsManager _objectsManager; // Manejo de objetos que existen en el mundo (items tirados en el piso, muebles interactuables, etc.)

    // Elementos para pantalla de créditos
    sf::Font _fontCreditos;
    sf::Text _textoCreditos;

    // Métodos del Game Loop
	void procesarEventos(); // Procesamiento de eventos de SFML (teclado, mouse, etc.)
	void actualizar(); // Actualización de la lógica del juego (movimiento, IA, interacciones, etc.)
	void renderizar(); // Dibujado de todo en pantalla
	void spawnearDropSeguro(Item* item, float startX, float startY); // Función para tirar un item al piso sin que quede atrapado en colisiones (lo sube 16 pixeles y lo deja caer)
	void cambiarMusica(int musicaID); // Cambia la música de fondo según el estado del juego (0 = menú, 1 = jugando, 2 = creador de ítems, 3 = créditos)
	sf::Music _musicaAmbiente; // Música de fondo que cambia según el estado del juego
	void colisionEntreEntidades(EntidadViva& jugador, EntidadViva& enemigo); // Función para manejar la colisión entre el jugador y un enemigo, aplicando daño si es necesario

public:
	~GameManager(); // Destructor para limpiar recursos dinámicos
	GameManager(); // Constructor para inicializar todo el juego (ventana, personajes, mapa, etc.)
	void run(); // Método principal para ejecutar el juego, contiene el bucle principal que llama a procesarEventos(), actualizar() y renderizar() en cada iteración
	void cambiarEstado(Estado* nuevoEstado); // Método para cambiar el estado actual del juego (menú, juego, creador de ítems, créditos, etc.) y manejar la transición entre ellos
};