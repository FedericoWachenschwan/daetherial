#include "habilidades.h"
#include <string>

// ============================================================================
// CONSTRUCTOR DE HABILIDADES (CLASE BASE)
// ============================================================================
// Inicializa todos los atributos de la habilidad con valores por defecto
// Los parámetros son recibidos y asignados a las variables miembro
// _cdListo = true: la habilidad comienza disponible para usar inmediatamente
Habilidades::Habilidades(std::string nombre, int danio, float rango, float cdDuracion)
	:_nombre(nombre), _nivel(1), _danio(danio), _rango(rango),
	_cdDuracion(cdDuracion), _cdActual(0.0f), _cdListo(true)
{
	// El constructor de inicialización ya ha asignado todos los valores
}

// ============================================================================
// MÉTODO: subirNivel()
// Incrementa el nivel de la habilidad
// Las clases hijas (como habilidad_1) sobrescriben esto para mejorar stats
// ============================================================================
void Habilidades::subirNivel()
{ 
	_nivel++;  // Incrementa el nivel en 1
}

// ============================================================================
// MÉTODO: cdActualizar(float deltaTime)
// GESTIÓN DEL COOLDOWN - EL CORAZÓN DEL SISTEMA DE ENFRIAMIENTO
// ============================================================================
// Este método controla cuánto tiempo debe pasar antes de poder usar la habilidad de nuevo
// Se llama CADA FRAME en actualizar() para mantener el cooldown sincronizado con el juego
//
// LÓGICA:
//   1. Si _cdListo = true: la habilidad está disponible, no hacer nada
//   2. Si _cdListo = false: la habilidad está en cooldown, restar tiempo
//   3. Cuando _cdActual <= 0: el cooldown terminó, marcar _cdListo = true
//
void Habilidades::cdActualizar(float deltaTime)
{
	// Si el cooldown NO está listo, decrementamos el tiempo restante
	if (!_cdListo) {
		// Decrementa el cooldown restante por el tiempo del frame actual
		// deltaTime es típicamente ~0.0166 segundos (a 60 FPS)
		_cdActual -= deltaTime;

		// Si el cooldown llegó a 0 o menos, marcamos la habilidad como lista
		if (_cdActual <= 0.0f) {
			_cdListo = true;  // ✅ Ahora el jugador puede usar la habilidad de nuevo
			_cdActual = 0.0f; // Aseguramos que no sea un número negativo
		}
	}
}   
