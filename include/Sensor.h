#ifndef SENSOR_H
#define SENSOR_H

#include "Dispositivo.h"

#include <string>

using namespace std;

/**
 * @file Sensor.h
 * @brief Classe base dos sensores da fabrica.
 *
 * Campos comuns: ID, MARCA, VALOR, PROBABILIDADE_AVARIA, X, Y.
 * Os tipos concretos (LUZ, HUMIDADE, FUMO, MISSEL, TEMPERATURA e
 * FOGO) encontram-se em TiposSensor.h; cada um define a sua regra
 * de aviso.
 */
class Sensor : public Dispositivo
{
    protected:
        double valor; ///< Leitura actual do sensor.
        bool avisoAnterior; ///< true se no passo anterior o sensor ja estava em aviso.

        /**
        * @brief Poe a leitura num valor que nao dispara aviso.
        *
        * Cada tipo define o seu valor seguro. Usado pela manutencao.
        */
        virtual void DefinirValorSeguro() = 0;

    public:
        /**
        * @brief Cria um sensor na posicao indicada.
        * @param id Identificador.
        * @param marca Fabricante.
        * @param valor Leitura inicial.
        * @param probabilidadeAvaria Probabilidade de avaria por hora.
        * @param posX Coordenada X, positiva.
        * @param posY Coordenada Y, positiva.
        */
        Sensor(int id, const string& marca, double valor, double probabilidadeAvaria, int posX, int posY);

        /** @brief Destrutor virtual. */
        virtual ~Sensor();

        /** @brief Devolve a leitura actual. */
        double GetValor() const;

        /**
        * @brief Substitui a leitura.
        * @param novoValor Nova leitura.
        */
        void SetValor(double novoValor);

        /** @brief Nome do tipo: LUZ, HUMIDADE, FUMO, MISSEL, TEMPERATURA ou FOGO. */
        virtual string GetTipo() const = 0;

        /**
        * @brief Indica se a leitura actual e um aviso.
        * @return true quando a regra do tipo e satisfeita.
        */
        virtual bool EmAviso() const = 0;

        /**
        * @brief Raio de vizinhanca, em celulas do campo.
        * @return Zero em todos os tipos excepto a humidade.
        */
        virtual int GetRaioVizinhanca() const;

        /**
        * @brief Detecta a passagem de normal para aviso.
        * @return true apenas no passo em que o aviso comeca.
        */
        bool Disparou();

        /**
        * @brief Actualiza a leitura com o horario e com o funcionamento dos motores.
        * @param motoresEmMarcha true se algum motor esta em RUN.
        * @param horaAtual Hora simulada, de 0 a 23.
        * @param horaAbertura Hora de abertura da fabrica.
        * @param horaEncerramento Hora de encerramento da fabrica.
        * @param fracaoHora Fracao de hora que este passo representa.
        */
        virtual void Actualizar(bool motoresEmMarcha, int horaAtual, int horaAbertura,
                                int horaEncerramento, double fracaoHora);

        /**
        * @brief Repoe o sensor: valor seguro e avaria limpa.
        */
        void Repor();
};

#endif
