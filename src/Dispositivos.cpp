/**
 * @file Dispositivos.cpp
 * @brief Implementacao dos metodos da classe base abstrata Dispositivo.
 */

#include "Dispositivos.h"

/**
 * @brief Construtor da classe Dispositivo.
 */
Dispositivo::Dispositivo(int idIn, const string& marcaIn, double probAvariaIn, int xIn, int yIn)
    : id(idIn), marca(marcaIn), probabilidadeAvaria(probAvariaIn),
      x(xIn), y(yIn), avariado(false), totalAvarias(0) {}

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
