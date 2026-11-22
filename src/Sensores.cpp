/**
 * @file Sensores.cpp
 * @brief Implementacao dos metodos da classe base abstrata Sensor.
 */

#include "Sensores.h"

/**
 * @brief Construtor da classe Sensor.
 */
Sensor::Sensor(int idIn, const string& marcaIn, double valorIn, double probAvariaIn, int xIn, int yIn)
    : Dispositivo(idIn, marcaIn, probAvariaIn, xIn, yIn), valor(valorIn) {}

/**
 * @brief Destrutor virtual da classe Sensor.
 */
Sensor::~Sensor() {}

/**
 * @brief Obtem o valor atual registado pelo sensor.
 * @return Valor decimal da medicao atual.
 */
double Sensor::GetValor() const {
    return valor;
}

/**
 * @brief Atualiza o valor medido pelo sensor.
 */
void Sensor::SetValor(double v) {
    valor = v;
}

/**
 * @brief Efetua a reparacao do sensor.
 */
void Sensor::Reparar() {
    Dispositivo::Reparar();
    DefinirValorSeguro();
}
