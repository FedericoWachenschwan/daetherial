#include "map.h"
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
	_bloquesSolidos.clear(); // 🌟 Limpiamos también el vector de colisiones nuevas

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
	// 🌟 NUEVA SECCIÓN: LA ADUANA DE BALDOSAS SOLIDAS
	// =================================================================================================================================
	// Recorremos la matriz recién fabricada. Si encontramos una pared (0 o más), calculamos su posición real 
	// en la pantalla usando _tamTile y _escala, y creamos un objeto físico 'BloqueMapa'.
	float tamRealDelTile = _tamTile * _escala;

	for (int f = 0; f < _filas; f++) {
		for (int c = 0; c < _columnas; c++) {
			int tileID = mapa[f][c];

			if (tileID != -1) { // Si NO es -1, significa que Tiled pintó un obstáculo sólido (0, 1, 2...)
				float posX = c * tamRealDelTile;
				float posY = f * tamRealDelTile;

				// Guardamos el bloque sólido con su posición y tamaño reales en el mundo
				_bloquesSolidos.push_back(BloqueMapa(posX, posY, tamRealDelTile));
			}
		}
	}

	return true;
}

// =================================================================================================================================
// FUNCIÓN PARA DIBUJAR EL MAPA COMPLETO EN LA PANTALLA
// =================================================================================================================================
void Map::dibujarMapa(sf::RenderWindow& ventana) {
	// Dibuja el sprite de fondo gigante que ya configuramos en cargarMapa
	ventana.draw(_spriteTile);
}

// =================================================================================================================================
// EL SEGURIDAD JUBILADO (Mantenemos la función por compatibilidad, pero con lógica de rectángulos)
// =================================================================================================================================
bool Map::hayColision(float x, float y) {
	// Nota: Esta función ya no es necesaria si el GameManager usa el nuevo vector polimórfico,
	// pero la dejamos acá para que no te tire error de compilación si la llamabas desde otro lado.
	sf::FloatRect puntoJugador(x, y, 1.f, 1.f);

	for (const auto& bloque : _bloquesSolidos) {
		if (bloque.getBounds().intersects(puntoJugador)) {
			return true;
		}
	}
	return false;
}

// Nueva implementación: comprobar colisión contra un rectángulo AABB completo
bool Map::hayColision(const sf::FloatRect& rect) const {
	for (const auto& bloque : _bloquesSolidos) {
		if (bloque.getBounds().intersects(rect)) {
			return true;
		}
	}
	return false;
}

// =================================================================================================================================
// FUNCION DEBUG REFACTORIZADA: Ahora dibuja rectángulos directamente desde nuestro vector de objetos físicos
// =================================================================================================================================
void Map::dibujarDebug(sf::RenderWindow& ventana) {
	// Creamos un rectángulo auxiliar para dibujar las cajas de colisión
	sf::RectangleShape rectDebug;
	rectDebug.setFillColor(sf::Color(255, 0, 0, 100)); // Rojo semitransparente

	// Recorremos el vector unificado de colisiones. Si está acá, es sólido.
	for (const auto& bloque : _bloquesSolidos) {
		sf::FloatRect limites = bloque.getBounds();

		// Seteamos el tamaño y la posición exacta que tiene el objeto físico en el mundo
		rectDebug.setSize(sf::Vector2f(limites.width, limites.height));
		rectDebug.setPosition(limites.left, limites.top);

		ventana.draw(rectDebug);
	}
}