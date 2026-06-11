#include "map.h"
#include "VisualFX.h"
#include <iostream>
#include <fstream> // para poder abrir y leer archivos CSV
#include <sstream> // para separar los numeros del CSV usando las comas

using namespace std;

Map::Map(int tamTile, float escala) {
	_tamTile = tamTile; // Guardamos el valor de tamaño del tile para usarlo al dibujar el mapa
	_escala = escala;   // Guardamos el valor de escala para usarlo al dibujar el mapa
	_filas = 0;         // arranca en 0 para luego asignar el valor correcto al cargar el mapa
	_columnas = 0;      // arranca en 0 para luego asignar el valor correcto al cargar el mapa

	// Le aplicamos la escala al sprite auxiliar para que se dibuje con el tamaño correcto
	_spriteTile.setScale(_escala, _escala);
}

bool Map::cargarMapa(const string& csvPath, const string& texturaPath) {

	// === EL "CLEAN" DE SEGURIDAD ===
	_filas = 0;
	_columnas = 0;
	mapa.clear();
	_bloqueSolido.clear(); // 🌟 Limpiamos también el vector de colisiones nuevas

	// Cargar la imagen de SFML
	if (!_texture.loadFromFile(texturaPath)) {
		cout << "❌ ERROR: No se pudo cargar la imagen del tileset en: " << texturaPath << endl;
		return false;
	}

	// Configuramos la textura nitida y se la pasamos al sprite
	_texture.setSmooth(false); // Pixel art nítido
	_spriteTile.setTexture(_texture);

	// Abrir el archivo CSV con fstream
	ifstream archivo(csvPath);
	if (!archivo.is_open()) {
		cout << "❌ ERROR: No se pudo abrir el archivo del mapa en: " << csvPath << endl;
		return false;
	}

	// =================================================================================================================================
	// LA HISTORIA DE LA FÁBRICA DE ALFAJORES: De texto chato CSV a matriz en memoria
	// =================================================================================================================================
	string linea;

	while (getline(archivo, linea)) {

		// Control de calidad: Limpiamos los saltos de línea invisibles de Windows (\r)
		if (!linea.empty() && linea.back() == '\r') {
			linea.pop_back();
		}

		stringstream ss(linea);
		string valor;
		vector<int> filaActual;

		while (getline(ss, valor, ',')) {
			if (!valor.empty()) {
				filaActual.push_back(stoi(valor));
			}
		}

		if (!filaActual.empty()) {
			mapa.push_back(filaActual);
		}
	}

	// Registramos las dimensiones reales que se auto-detectaron en los vectores
	_filas = mapa.size();
	if (_filas > 0) {
		_columnas = mapa[0].size();
	}
	else {
		_columnas = 0;
	}

	// =================================================================================================================================
	// 🌟 NUEVA SECCIÓN: LA ADUANA DE BALDOSAS SÓLIDAS OPTIMIZADA (FUSIÓN HORIZONTAL)
	// =================================================================================================================================
	// Recorremos la matriz recién fabricada fila por fila. Si encontramos bloques sólidos contiguos,
	// calculamos su ancho total y creamos un único 'BloqueMapa' alargado para aliviar al Profiler.
	float tamRealDelTile = _tamTile * _escala;

	for (int f = 0; f < _filas; f++) {
		int inicioC = -1;       // Guarda la columna donde empieza una pared larga
		int longitudPared = 0;  // Cuenta cuántas baldosas sólidas van en racha

		for (int c = 0; c < _columnas; c++) {
			int tileID = mapa[f][c];

			if (tileID != -1) {
				// 🧱 ¡Es sólido! 
				if (inicioC == -1) {
					inicioC = c; // Si no veníamos acumulando, acá empieza una pared nueva
				}
				longitudPared++; // Sumamos un casillero a la racha
			}
			else {
				// 💨 Es aire (Hole / Espacio vacío)
				// Si veníamos acumulando una pared y de repente hay aire, llegó el momento de fabricar el bloque físico
				if (inicioC != -1) {
					float posX = inicioC * tamRealDelTile;
					float posY = f * tamRealDelTile;
					float anchoPared = longitudPared * tamRealDelTile;

					// Creamos un solo bloque estirado: pasamos posición X, Y, ANCHO y ALTO
					_bloqueSolido.push_back(BloqueMapa(posX, posY, anchoPared, tamRealDelTile));

					// Reseteamos las variables de control para buscar la próxima pared en la misma fila
					inicioC = -1;
					longitudPared = 0;
				}
			}
		}

		// 🚨 CONTROL DE BORDE: Si terminamos de recorrer la fila completa y la pared llegaba 
		// justo hasta el último casillero de la derecha, la guardamos antes de pasar a la siguiente fila.
		if (inicioC != -1) {
			float posX = inicioC * tamRealDelTile;
			float posY = f * tamRealDelTile;
			float anchoPared = longitudPared * tamRealDelTile;

			_bloqueSolido.push_back(BloqueMapa(posX, posY, anchoPared, tamRealDelTile));
		}
	}

	return true;
}

// =================================================================================================================================
// FUNCIÓN PARA DIBUJAR EL MAPA COMPLETO EN LA PANTALLA
// =================================================================================================================================
void Map::dibujarMapa(sf::RenderWindow& ventana) const {
	// Dibuja el sprite de fondo gigante que ya configuramos en cargarMapa
	ventana.draw(_spriteTile);
}

// =================================================================================================================================
// FUNCIÓN PARA DIBUJAR FX AMBIENTAL
// =================================================================================================================================
void Map::generarClima(VisualFX& vfx) {
	sf::Vector2f tamanoMapa(2000.f, 2000.f);
	vfx.agregarParticulasAmbiente(tamanoMapa, 50, sf::Color(130, 200, 36));
}



// =================================================================================================================================
// FUNCION DEBUG REFACTORIZADA: Ahora dibuja rectángulos directamente desde nuestro vector de objetos físicos
// =================================================================================================================================
void Map::dibujarDebug(sf::RenderWindow& ventana) const {
	// Creamos un rectángulo auxiliar para dibujar las cajas de colisión
	sf::RectangleShape rectDebug;
	rectDebug.setFillColor(sf::Color(255, 0, 0, 100)); // Rojo semitransparente

	// Recorremos el vector unificado de colisiones. Si está acá, es sólido.
	for (const auto& bloque : _bloqueSolido) {
		sf::FloatRect limites = bloque.getBounds();

		// Seteamos el tamaño y la posición exacta que tiene el objeto físico en el mundo
		rectDebug.setSize(sf::Vector2f(limites.width, limites.height));
		rectDebug.setPosition(limites.left, limites.top);

		ventana.draw(rectDebug);
	}
}
//================================================================================================================================
// FUNCION PARA QUE EL PATHFINDER PUEDA CONSULTAR SI UNA BALDOSA ES SÓLIDA O NO SIN TENER QUE LIDIAR CON COORDENADAS DEL MUNDO
//================================================================================================================================

bool Map::esSolido(int f, int c) const {
	// 1. Protección de límites (si intentan mirar fuera del mapa, es pared)
	if (f < 0 || f >= _filas || c < 0 || c >= _columnas) return true;

	// 2. Si el valor es 0, es colision. 
	return mapa[f][c] != -1;
}