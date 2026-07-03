#include "map.h"
#include <iostream>
#include <fstream>
#include <sstream>

///=============================================================///
///   #1 - CONSTRUCTOR
///=============================================================///
// #1
Map::Map(int tamano_de_cada_tile, float escala) {
    _tamano_de_cada_tile_en_pixeles = tamano_de_cada_tile; // Guarda el tamanio de cada celda
    _escala_del_mapa = escala; // Guarda el factor de escala
    _cantidad_de_filas_reales = 0; // Aun no hay mapa cargado
    _cantidad_de_columnas_reales = 0; // Aun no hay mapa cargado

    _sprite_del_fondo.setScale(_escala_del_mapa, _escala_del_mapa); // Aplica la escala a la imagen de fondo
}

///=============================================================///
///   #2 - CONVERTIR TEXTO A NUMERO
///=============================================================///
// #2
int Map::convertir_texto_a_numero(const std::string& texto) const {

    int numero_resultado = 0; // Acumula el resultado aqui
    int posicion_inicial = 0; // Desde donde empieza a leer los digitos
    bool el_numero_es_negativo = false; // Por defecto es positivo

    if (texto.length() > 0 && texto[0] == '-') {
        el_numero_es_negativo = true; // El primer caracter es un signo negativo
        posicion_inicial = 1; // Saltea el signo y empieza en el siguiente
    }

    int cantidad_de_caracteres = (int)texto.length(); // Cuantos caracteres tiene el texto
    for (int i = posicion_inicial; i < cantidad_de_caracteres; i++) {
        char caracter_actual = texto[i]; // Lee un digito del texto
        int valor_de_este_digito = caracter_actual - '0'; // Convierte el char a su valor numerico
        numero_resultado = numero_resultado * 10 + valor_de_este_digito; // Lo incorpora al resultado
    }

    if (el_numero_es_negativo == true) {
        numero_resultado = numero_resultado * -1; // Cambia el signo si corresponde
    }

    return numero_resultado; // Devuelve el numero convertido
}

///=============================================================///
///   #3 - CARGAR MAPA
///=============================================================///
// #3
bool Map::cargar_mapa(const std::string& ruta_del_csv, const std::string& ruta_de_la_textura) {

    _cantidad_de_filas_reales = 0; // Reinicia por si se carga un nuevo mapa
    _cantidad_de_columnas_reales = 0; // Reinicia por si se carga un nuevo mapa

    if (_textura_del_fondo.loadFromFile(ruta_de_la_textura) == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA IMAGEN DEL MAPA EN: " << ruta_de_la_textura << std::endl;
        return false; // No puede continuar sin la imagen
    }

    _textura_del_fondo.setSmooth(false); // Pixeles netos, sin suavizado
    _sprite_del_fondo.setTexture(_textura_del_fondo); // Asigna la imagen al sprite de fondo

    std::ifstream archivo_del_mapa(ruta_del_csv); // Abre el archivo CSV de colisiones
    if (archivo_del_mapa.is_open() == false) {
        std::cout << "ERROR: NO SE PUDO ABRIR EL ARCHIVO DEL MAPA EN: " << ruta_del_csv << std::endl;
        return false; // No puede continuar sin el archivo de colisiones
    }

    std::string linea_leida; // Guarda una linea completa del CSV
    int fila_actual = 0; // Contador de filas procesadas

    while (std::getline(archivo_del_mapa, linea_leida) && fila_actual < CANTIDAD_MAXIMA_DE_FILAS) {

        if (linea_leida.length() > 0 && linea_leida[linea_leida.length() - 1] == '\r') {
            linea_leida.pop_back(); // Elimina el retorno de carro de Windows si existe
        }

        std::stringstream linea_separada_por_comas(linea_leida); // Divide la linea por comas
        std::string numero_como_texto; // Guarda cada numero antes de convertirlo
        int columna_actual = 0; // Contador de columnas en esta fila

        while (std::getline(linea_separada_por_comas, numero_como_texto, ',') && columna_actual < CANTIDAD_MAXIMA_DE_COLUMNAS) {
            if (numero_como_texto.length() > 0) {
                _grilla_de_colisiones[fila_actual][columna_actual] = convertir_texto_a_numero(numero_como_texto); // Guarda el valor en la grilla
                columna_actual++; // Avanza a la siguiente columna
            }
        }

        if (columna_actual > 0) {
            _cantidad_de_columnas_reales = columna_actual; // Actualiza el ancho real del mapa
            fila_actual++; // Avanza a la siguiente fila
        }
    }

    _cantidad_de_filas_reales = fila_actual; // Guarda cuantas filas tiene el mapa

    return true; // El mapa cargo correctamente
}

///=============================================================///
///   #4 - DIBUJAR MAPA
///=============================================================///
// #4
void Map::dibujar_mapa(sf::RenderWindow& ventana_del_juego) const {
    ventana_del_juego.draw(_sprite_del_fondo); // Dibuja la imagen de fondo del nivel
}

///=============================================================///
///   #5 - GENERAR CLIMA
///=============================================================///
// #5
void Map::generar_clima(VisualFX& efectos_visuales) {
    sf::Vector2f tamano_del_area_del_clima(2000.f, 2000.f); // El clima cubre toda la zona del mapa
    efectos_visuales.agregarParticulasAmbiente(tamano_del_area_del_clima, 50, sf::Color(130, 200, 36)); // Agrega 50 luciernagras verdes
}

///=============================================================///
///   #6 - GETTERS
///=============================================================///
// #6
bool Map::getEs_solido(int fila, int columna) const {
    if (fila < 0 || fila >= _cantidad_de_filas_reales || columna < 0 || columna >= _cantidad_de_columnas_reales) {
        return true; // Fuera del mapa se considera pared
    }
    return _grilla_de_colisiones[fila][columna] != -1; // -1 es libre, cualquier otro valor es solido
}

// #7
bool Map::getHay_colision(const sf::FloatRect& rectangulo) const {

    float tamano_real_de_cada_tile = _tamano_de_cada_tile_en_pixeles * _escala_del_mapa; // Tamanio del tile con escala aplicada

    int columna_inicial = (int)(rectangulo.left / tamano_real_de_cada_tile); // Primera columna que toca el rectangulo
    int columna_final = (int)((rectangulo.left + rectangulo.width) / tamano_real_de_cada_tile); // Ultima columna que toca el rectangulo
    int fila_inicial = (int)(rectangulo.top / tamano_real_de_cada_tile); // Primera fila que toca el rectangulo
    int fila_final = (int)((rectangulo.top + rectangulo.height) / tamano_real_de_cada_tile); // Ultima fila que toca el rectangulo

    for (int fila = fila_inicial; fila <= fila_final; fila++) {
        for (int columna = columna_inicial; columna <= columna_final; columna++) {
            if (getEs_solido(fila, columna) == true) {
                return true; // Encontro una pared, hay colision
            }
        }
    }

    return false; // Ninguna celda tocada es solida, no hay colision
}

///=============================================================///
///   #8 - DIBUJAR DEBUG
///=============================================================///
// #8
void Map::dibujar_debug(sf::RenderWindow& ventana_del_juego) const {

    float tamano_real_de_cada_tile = _tamano_de_cada_tile_en_pixeles * _escala_del_mapa; // Tamanio del tile en pantalla

    sf::RectangleShape rectangulo_de_debug;
    rectangulo_de_debug.setFillColor(sf::Color(255, 0, 0, 100)); // Relleno rojo semitransparente
    rectangulo_de_debug.setSize(sf::Vector2f(tamano_real_de_cada_tile, tamano_real_de_cada_tile)); // Del tamanio de un tile

    for (int fila = 0; fila < _cantidad_de_filas_reales; fila++) {
        for (int columna = 0; columna < _cantidad_de_columnas_reales; columna++) {
            if (getEs_solido(fila, columna) == true) {
                rectangulo_de_debug.setPosition(columna * tamano_real_de_cada_tile, fila * tamano_real_de_cada_tile); // Lo ubica sobre la celda
                ventana_del_juego.draw(rectangulo_de_debug); // Dibuja el rectangulo rojo sobre las paredes
            }
        }
    }
}
