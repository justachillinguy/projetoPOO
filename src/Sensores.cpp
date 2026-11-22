/**
 * @file Sensores.cpp
 * @brief Implementação dos métodos da classe base abstrata Sensor.
 */

#include "Sensores.h"

/**
 * @brief Construtor da classe Sensor.
 * Inicializa os atributos herdados de Dispositivo e define o valor inicial da medição.
 * @param id- Identificador único do sensor.
 * @param marca- Marca do fabricante do sensor.
 * @param valor- Valor inicial lido pelo sensor.
 * @param probAvaria- Probabilidade de avaria por hora (0.0 a 1.0).
 * @param x- Coordenada X da localização na fábrica.
 * @param y- Coordenada Y da localização na fábrica.
 */
Sensor::Sensor(int id, const string& marca, double valor, double probAvaria, int x, int y)
    : Dispositivo(id, marca, probAvaria, x, y), valor(valor) {}

/**
 * @brief Destrutor virtual da classe Sensor.
 */
Sensor::~Sensor() {}

/**
 * @brief Obtém o valor atual registado pelo sensor.
 * @return Valor decimal da medição atual.
 */
double Sensor::GetValor() const {
    return valor;
}

/**
 * @brief Atualiza o valor medido pelo sensor.
 * @param v- Novo valor da leitura do sensor.
 */
void Sensor::SetValor(double v) {
    valor = v;
}

/**
 * @brief Efetua a reparação do sensor.
 * Invoca Dispositivo::Reparar() para limpar o estado de avaria e chama DefinirValorSeguro()
 * para restaurar uma leitura normal após a manutenção.
 */
void Sensor::Reparar() {
    Dispositivo::Reparar(); // Repoe avariado = false na classe Dispositivo
    DefinirValorSeguro();   // Repoe um valor seguro especifico do tipo de sensor
}
