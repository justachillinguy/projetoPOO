#ifndef TIPOS_SENSOR_H
#define TIPOS_SENSOR_H

#include "Sensor.h"

/**
 * @file TiposSensor.h
 * @brief Tipos concretos de sensor da fabrica.
 *
 * De acordo com o enunciado a fabrica tem sensores LUZ, HUMIDADE,
 * FUMO e MISSEL. Ha ainda TEMPERATURA ambiente e FOGO/chama.
 * Cada tipo decide quando esta em aviso:
 * - LUZ: o valor desce abaixo do limiar.
 * - HUMIDADE: o valor atinge o limiar. O raio e a vizinhanca X
 *   dos motores que devem ser desligados.
 * - FUMO: o valor atinge o limiar.
 * - MISSEL: o valor chega a 1.
 * - TEMPERATURA: o valor atinge o limiar.
 * - FOGO: o valor chega a 1.
 *
 * A classe base Sensor encontra-se em Sensor.h.
 */

/**
 * @brief Sensor de luminosidade.
 *
 * Dispara quando o valor e inferior ao limiar.
 * Na ultima hora antes do encerramento a luz desce.
 * Na hora de abertura a luz volta a um valor seguro.
 */
class SensorLuz : public Sensor
{
    private:
        double limiar; ///< Abaixo deste valor ha aviso.

    public:
        /**
        * @brief Cria um sensor de luz.
        * @param limiar Limite inferior de luminosidade.
        */
        SensorLuz(int id, const string& marca, double valor, double probabilidadeAvaria,
                  int posX, int posY, double limiar);
        virtual ~SensorLuz();

        /** @brief Devolve o limiar de luminosidade. */
        double GetLimiar() const;
        virtual string GetTipo() const;
        virtual bool EmAviso() const;
        virtual void Actualizar(bool motoresEmMarcha, int horaAtual, int horaAbertura,
                                int horaEncerramento, double fracaoHora);
        virtual void DefinirValorSeguro();
};

/**
 * @brief Sensor de humidade.
 *
 * Dispara quando o valor atinge o limiar.
 * Com motores em RUN a humidade sobe.
 * O raio define a vizinhanca X em que os motores sao desligados.
 */
class SensorHumidade : public Sensor
{
    private:
        double limiar; ///< A partir deste valor ha aviso.
        int raioVizinhanca; ///< Raio positivo da vizinhanca, em celulas.

    public:
        /**
        * @brief Cria um sensor de humidade.
        * @param limiar Valor a partir do qual ha aviso.
        * @param raioVizinhanca Raio da vizinhanca. So e guardado se for positivo.
        */
        SensorHumidade(int id, const string& marca, double valor, double probabilidadeAvaria,
                       int posX, int posY, double limiar, int raioVizinhanca);
        virtual ~SensorHumidade();

        /** @brief Devolve o limiar de humidade. */
        double GetLimiar() const;
        virtual int GetRaioVizinhanca() const;
        virtual string GetTipo() const;
        virtual bool EmAviso() const;
        virtual void Actualizar(bool motoresEmMarcha, int horaAtual, int horaAbertura,
                                int horaEncerramento, double fracaoHora);
        virtual void DefinirValorSeguro();
};

/**
 * @brief Sensor de fumo.
 *
 * Dispara quando o valor atinge o limiar.
 * Com motores em RUN ha, em cada passo, uma hipotese (proporcional a
 * probabilidade do evento e a fracao de hora) de a leitura atingir o limiar.
 */
class SensorFumo : public Sensor
{
    private:
        double limiar; ///< A partir deste valor ha aviso.
        double probEvento; ///< Probabilidade por hora de haver fumo, como fracao entre 0 e 1.

    public:
        /**
        * @brief Cria um sensor de fumo.
        * @param limiar Valor a partir do qual ha aviso.
        * @param probEvento Probabilidade por hora de haver fumo, como fracao entre 0 e 1 (por omissao 0.01 = 1%).
        */
        SensorFumo(int id, const string& marca, double valor, double probabilidadeAvaria,
                   int posX, int posY, double limiar, double probEvento = 0.01);
        virtual ~SensorFumo();

        /** @brief Devolve o limiar de fumo. */
        double GetLimiar() const;
        /** @brief Devolve a probabilidade por hora de haver fumo (fracao entre 0 e 1). */
        double GetProbabilidadeEvento() const;
        virtual string GetTipo() const;
        virtual bool EmAviso() const;
        virtual void Actualizar(bool motoresEmMarcha, int horaAtual, int horaAbertura,
                                int horaEncerramento, double fracaoHora);
        virtual void DefinirValorSeguro();
};

/**
 * @brief Sensor de missel.
 *
 * Dispara quando o valor e pelo menos 1.
 * Em cada passo ha uma hipotese, proporcional a probabilidade do evento e a fracao
 * de hora, de a leitura passar a 1. A probabilidade do evento e distinta da
 * probabilidade de avaria do sensor.
 */
class SensorMissel : public Sensor
{
    private:
        double probEvento; ///< Probabilidade por hora de aparecer um missel, como fracao entre 0 e 1.

    public:
        /**
        * @brief Cria um sensor de missel.
        * @param probEvento Probabilidade por hora de aparecer um missel, como fracao entre 0 e 1 (por omissao 0.01).
        */
        SensorMissel(int id, const string& marca, double valor, double probabilidadeAvaria,
                     int posX, int posY, double probEvento = 0.01);
        virtual ~SensorMissel();

        /** @brief Devolve a probabilidade por hora de aparecer um missel (fracao entre 0 e 1). */
        double GetProbabilidadeEvento() const;
        virtual string GetTipo() const;
        virtual bool EmAviso() const;
        virtual void Actualizar(bool motoresEmMarcha, int horaAtual, int horaAbertura,
                                int horaEncerramento, double fracaoHora);
        virtual void DefinirValorSeguro();
};

/**
 * @brief Sensor de temperatura ambiente.
 *
 * Dispara quando o valor atinge o limiar.
 * Sobe gradualmente enquanto houver motores em RUN.
 */
class SensorTemperatura : public Sensor
{
    private:
        double limiar;

    public:
        SensorTemperatura(int id, const string& marca, double valor, double probabilidadeAvaria,
                          int posX, int posY, double limiar);
        virtual ~SensorTemperatura();

        double GetLimiar() const;
        virtual string GetTipo() const;
        virtual bool EmAviso() const;
        virtual void Actualizar(bool motoresEmMarcha, int horaAtual, int horaAbertura,
                                int horaEncerramento, double fracaoHora);
        virtual void DefinirValorSeguro();
};

/**
 * @brief Sensor de fogo/chama.
 *
 * Dispara quando o valor e pelo menos 1.
 * Comportamento semelhante ao sensor de fumo mas associado a chama directa.
 */
class SensorFogo : public Sensor
{
    private:
        double limiar;
        double probEvento; ///< Probabilidade por hora de haver fogo, como fracao entre 0 e 1.

    public:
        /** @param probEvento Probabilidade por hora de haver fogo, como fracao entre 0 e 1 (por omissao 0.01). */
        SensorFogo(int id, const string& marca, double valor, double probabilidadeAvaria,
                   int posX, int posY, double limiar, double probEvento = 0.01);
        virtual ~SensorFogo();

        double GetLimiar() const;
        /** @brief Devolve a probabilidade por hora de haver fogo (fracao entre 0 e 1). */
        double GetProbabilidadeEvento() const;
        virtual string GetTipo() const;
        virtual bool EmAviso() const;
        virtual void Actualizar(bool motoresEmMarcha, int horaAtual, int horaAbertura,
                                int horaEncerramento, double fracaoHora);
        virtual void DefinirValorSeguro();
};

#endif
