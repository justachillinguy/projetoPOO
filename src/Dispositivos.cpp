/**
 * @file Dispositivos.cpp
 * @brief Implementação dos métodos da classe base abstrata Dispositivo.
 */

#include "Dispositivos.h"

/**
 * @brief Construtor da classe Dispositivo.
 * Inicializa os atributos do dispositivo e define o estado inicial como operacional (sem avarias).
 * @param id- Identificador único do dispositivo.
 * @param marca- Marca do fabricante do dispositivo.
 * @param probAvaria- Probabilidade de avaria por hora (0.0 a 1.0).
 * @param x- Coordenada X da localização na fábrica.
 * @param y- Coordenada Y da localização na fábrica.
 */
Dispositivo::Dispositivo(int id, const string& marca, double probAvaria, int x, int y)
    : id(id), marca(marca), probabilidadeAvaria(probAvaria),
      x(x), y(y), avariado(false), totalAvarias(0) {}

/**
 * @brief Destrutor virtual da classe Dispositivo.
 */
Dispositivo::~Dispositivo() {}

/**
 * @brief Obtém o identificador único do dispositivo.
 * @return Número inteiro correspondente ao ID.
 */
int Dispositivo::GetId() const {
    return id;
}

/**
 * @brief Obtém a marca do dispositivo.
 * @return Texto com o nome da marca.
 */
string Dispositivo::GetMarca() const {
    return marca;
}

/**
 * @brief Obtém a probabilidade de avaria por hora????????? do dispositivo.
 * @return Valor decimal da probabilidade de avaria.
 */
double Dispositivo::GetProbabilidadeAvaria() const {
    return probabilidadeAvaria;
}

/**
 * @brief Obtém a coordenada X do dispositivo na fábrica.
 * @return Posição no eixo X.
 */
int Dispositivo::GetX() const {
    return x;
}

/**
 * @brief Obtém a coordenada Y do dispositivo na fábrica.
 * @return Posição no eixo Y.
 */
int Dispositivo::GetY() const {
    return y;
}

/**
 * @brief Indica se o dispositivo já avariou e ainda não foi reparado.
 * @return true depois de RegistarAvaria, até ser reparado.
 */
bool Dispositivo::EstaAvariado() const {
    return avariado;
}

/**
 * @brief Obtém o número total de avarias registadas no dispositivo.
 * @return Contador total de avarias.
 */
int Dispositivo::GetTotalAvarias() const {
    return totalAvarias;
}

/**
 * @brief Conta uma avaria.
 * Só altera o estado e incrementa o contador se o dispositivo ainda não estiver avariado.
 */
void Dispositivo::RegistarAvaria() {
    if (!avariado) {
        avariado = true;
        totalAvarias++;
    }
}

/**
 * @brief Limpa o estado de avaria para o dispositivo voltar a operar e a contagem poder voltar a subir.
 */
void Dispositivo::Reparar() {
    avariado = false;
}
