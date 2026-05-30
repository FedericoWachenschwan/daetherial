#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>

// ============================================================================
// CLASE BASE: Habilidades
// Define la estructura y mecánica común para todas las habilidades del juego
// ============================================================================
class Habilidades
{
protected:
	std::string _nombre;        // Nombre de la habilidad (ej: "Bola de fuego")
	int _nivel;                 // Nivel actual de la habilidad (comienza en 1)
	int _danio;                 // Daño que inflige la habilidad
	float _rango;               // Distancia máxima que puede recorrer el proyectil
	float _cdDuracion;          // Duración total del cooldown en segundos
	float _cdActual;            // Tiempo restante del cooldown actual
	bool _cdListo;              // Flag: true = habilidad lista para usar, false = en cooldown

public:
	// Constructor: Inicializa los parámetros básicos de la habilidad
	// Parámetros:
	//   - nombre: Nombre descriptivo de la habilidad
	//   - danio: Daño base que causará
	//   - rango: Distancia máxima de alcance
	//   - cdDuracion: Segundos que debe esperar antes de usarla de nuevo
	Habilidades(std::string nombre, int danio, float rango, float cdDuracion); 
	virtual ~Habilidades(){} // Destructor virtual para permitir herencia

	// ========== ACTIVACIÓN DE LA HABILIDAD ==========
	// Método virtual puro: cada habilidad implementa su propia lógica de activación
	// Se ejecuta cuando el jugador presiona la tecla y el cooldown está disponible
	// Parámetros:
	//   - inicio: Posición del personaje (punto de origen del proyectil)
	//   - objetivo: Posición donde apunta el cursor (destino)
	virtual void activar(sf::Vector2f inicio, sf::Vector2f objetivo) = 0; 

	// ========== AUMENTO DE NIVEL ==========
	// Incrementa el nivel de la habilidad y sus estadísticas
	// Se llama cuando el personaje sube de nivel
	virtual void subirNivel();

	// ========== GESTIÓN DE COOLDOWN ==========
	// Actualiza el estado del cooldown usando deltaTime (tiempo transcurrido del frame)
	// Decrementa _cdActual hasta llegar a 0, luego marca _cdListo = true
	// NOTA: Esta función se llama cada frame en actualizar() para mantener el cooldown sincronizado
	void cdActualizar(float deltaTime); 
};