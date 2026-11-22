/**
 * @file Motor.h
 * @brief Declaração da classe base Motor, derivada de Dispositivo.
 */

#ifndef MOTOR_H
#define MOTOR_H

#include <string>
#include "Dispositivos.h"
#include "Tipos.h"

/**
 * @class Motor
 * @brief Classe base para todos os motores da fábrica.
 */
class Motor : public Dispositivo {
protected:
    string tipo;                ///< Tipo específico do motor (ex.: "MCOMBUSTAO", "MELETRICO", "MINDUCAO")
    double consumoHora;         ///< Consumo energético do motor por hora (em kWh)
    double consumoDia;          ///< Consumo energético acumulado durante o dia atual
    double temperaturaAtual;    ///< Temperatura atual de funcionamento do motor (em graus Celsius)
    double temperaturaAviso;    ///< Limiar de temperatura para entrada na zona AMARELA (aviso ao gestor)
    double temperaturaParagem;  ///< Limiar de temperatura para entrada na zona VERMELHA (paragem automática)
    int tempoTrabalho;          ///< Tempo total acumulado de trabalho do motor (em minutos)
    ESTADO_MOTOR estado;        ///< Estado operacional atual do motor (STOP, START, RUN ou AVARIADO)

public:
    /**
     * @brief Construtor da classe Motor.
     * @param id Identificador único do motor.
     * @param tipo Designação do tipo de motor.
     * @param marca Marca do fabricante do motor.
     * @param consumoHora Consumo energético por hora de funcionamento.
     * @param tempAviso Temperatura a partir da qual é emitido aviso (zona AMARELA).
     * @param tempParagem Temperatura a partir da qual o motor pára por segurança (zona VERMELHA).
     * @param probAvaria Probabilidade de avaria por hora (0.0 a 1.0).
     * @param x Coordenada X da localização na fábrica.
     * @param y Coordenada Y da localização na fábrica.
     */
    Motor(int id, const string& tipo, const string& marca,
          double consumoHora, double tempAviso, double tempParagem,
          double probAvaria, int x, int y);

    /**
     * @brief Destrutor virtual da classe Motor.
     */
    virtual ~Motor();

    /** @brief Obtém o tipo específico do motor. */
    virtual string GetTipo() const;

    /** @brief Obtém o estado operacional atual do motor. */
    ESTADO_MOTOR GetEstado() const;

    /** @brief Define o estado operacional do motor. */
    void SetEstado(ESTADO_MOTOR est);

    /** @brief Obtém o consumo energético horário do motor. */
    double GetConsumoHora() const;

    /** @brief Obtém o consumo energético acumulado no dia atual. */
    double GetConsumoDia() const;

    /** @brief Obtém a temperatura atual do motor. */
    double GetTemperaturaAtual() const;

    /** @brief Obtém o limiar de aviso de temperatura. */
    double GetTemperaturaAviso() const;

    /** @brief Obtém o limiar de paragem de temperatura. */
    double GetTemperaturaParagem() const;


    /** @brief Define a temperatura atual do motor. */
    void SetTemperaturaAtual(double temp);

    /** @brief Obtém o tempo total de trabalho acumulado pelo motor (minutos). */
    int GetTempoTrabalho() const;

    /** @brief Reinicia o contador de consumo energético diário para zero. */
    void ReiniciarConsumoDia();

    /** @brief Inicia o motor (STOP -> START) se não estiver avariado nem sobreaquecido. */
    virtual void START();

    /** @brief Executa um ciclo de funcionamento de 10 minutos do motor. */
    virtual void RUN();

    /** @brief Reinicia o motor (STOP seguido de START). */
    virtual void RESTART();

    /** @brief Pára o motor (coloca em STOP se não estiver avariado). */
    virtual void STOP();

    /** @brief Reduz a temperatura do motor quando parado, até ao mínimo ambiente (20 ºC). */
    virtual void Arrefecer(double graus);

    /** @brief Avalia a classe térmica atual do motor (VERDE, AMARELO ou VERMELHO). */
    virtual CLASSE_TEMPERATURA ESTOU_QUENTE();

    /** @brief Efetua a manutenção do motor (repara avarias e repõe temperatura na zona VERDE). */
    virtual void Manutencao();
};

#endif // MOTOR_H
