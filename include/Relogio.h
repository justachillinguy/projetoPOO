/**
 * @file Relogio.h
 * @brief Declaração da classe Relogio para controlo do tempo e velocidade da simulação.
 */

#ifndef RELOGIO_H
#define RELOGIO_H

#include <ctime>

/**
 * @class Relogio
 * @brief Gere o tempo decorrido da simulação com fator de aceleração configurável.
 */
class Relogio {
private:
    time_t tInicio;             ///< Último instante real de referência
    long segundosAcumulados;    ///< Segundos simulados acumulados antes de alterações de velocidade
    int velocidade;             ///< Multiplicador de velocidade (segundos simulados por segundo real)

public:
    /**
     * @brief Construtor da classe Relogio.
     * @param _velocidade Fator de velocidade inicial (mínimo 1).
     */
    Relogio(int _velocidade = 600);

    /**
     * @brief Destrutor da classe Relogio.
     */
    ~Relogio();

    /**
     * @brief Calcula o timestamp atual da simulação.
     * @return Tempo simulado em segundos (time_t).
     */
    time_t GetTempo() const;

    /**
     * @brief Calcula quantos segundos simulados já decorreram desde o início.
     * @return Total de segundos simulados decorridos.
     */
    long GetSegundosSimulados() const;

    /**
     * @brief Altera o fator de velocidade da simulação preservando o tempo já decorrido.
     * @param vel Nova velocidade (mínimo 1).
     */
    void MudarVelocidade(int vel);

    /**
     * @brief Obtém a velocidade atual do relógio.
     * @return Valor atual do multiplicador de velocidade.
     */
    int GetVelocidade() const;
};

#endif
