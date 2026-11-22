/**
 * @file Sensores.h
 * @brief Declaração da classe base abstrata Sensor, derivada de Dispositivo.
 */

#ifndef SENSORES_H
#define SENSORES_H

#include <string>
#include "Dispositivos.h"

/**
 * @class Sensor
 * @brief Classe base abstrata para todos os sensores da fábrica.
 */
class Sensor : public Dispositivo {
protected:
    double valor; ///< Valor atual medido pelo sensor

public:
    /**
     * @brief Construtor da classe Sensor.
     * @param id- Identificador único do sensor.
     * @param marca- Marca do fabricante do sensor.
     * @param valor- Valor inicial lido pelo sensor.
     * @param probAvaria- Probabilidade de avaria por hora (0.0 a 1.0).
     * @param x- Coordenada X da localização na fábrica.
     * @param y- Coordenada Y da localização na fábrica.
     */
    Sensor(int id, const string& marca, double valor, double probAvaria, int x, int y);

    /**
     * @brief Destrutor virtual da classe Sensor.
     */
    virtual ~Sensor();

    /**
     * @brief Obtém o valor atual registado pelo sensor.
     * @return Valor decimal da medição atual.
     */
    double GetValor() const;

    /**
     * @brief Atualiza o valor medido pelo sensor.
     * @param v Novo valor da leitura do sensor.
     */
    void SetValor(double v);

    /**
     * @brief Efetua a reparação do sensor.
     * Limpa o estado de avaria na classe base e repõe um valor seguro específico do tipo de sensor.
     */
    virtual void Reparar();

    /**
     * @brief Verifica se a leitura atual do sensor ultrapassou o limite de segurança.
     * @return true se o sensor estiver em estado de aviso/alerta; false caso contrário.
     */
    virtual bool EmAviso() const = 0;

    /**
     * @brief Repõe o valor medido pelo sensor para um nível considerado seguro.
     */
    virtual void DefinirValorSeguro() = 0;

    /**
     * @brief Obtém o valor de limiar de aviso configurado no sensor.
     * @return Limiar de aviso (0.0 por defeito para sensores sem limiar configurável).
     */
    virtual double GetLimiar() const { return 0.0; }

    /**
     * @brief Obtém o raio de vizinhança em que o sensor atua sobre os motores próximos.
     * @return Raio de alcance (0 por defeito para sensores sem alcance espacial específico).
     */
    virtual int GetRaioVizinhanca() const { return 0; }
};

#endif // SENSORES_H
