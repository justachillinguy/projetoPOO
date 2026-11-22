/**
 * @file Dispositivos.h
 * @brief Declaração da classe base abstrata Dispositivo.
 */

#ifndef DISPOSITIVOS_H
#define DISPOSITIVOS_H

#include <string>

using namespace std;

/**
 * @class Dispositivo
 * @brief Classe base abstrata para todos os equipamentos da fábrica (Motores e Sensores).
 */
class Dispositivo {
protected:
    int id;                     ///< Identificador único para motor ou sensor
    string marca;               ///< Marca do dispositivo
    double probabilidadeAvaria; ///< Probabilidade de avaria por hora (valor entre 0.0 e 1.0)
    int x;                      ///< Coordenada X na planta da fábrica (-1 quando é inválida)
    int y;                      ///< Coordenada Y na planta da fábrica (-1 quando é inválida)
    bool avariado;              ///< Indica se o dispositivo já avariou e ainda não foi reparado
    int totalAvarias;           ///< Contador acumulado do número total de avarias sofridas

public:
    /**
     * @brief Construtor da classe Dispositivo.
     * @param idIn Identificador único do dispositivo.
     * @param marcaIn Marca do fabricante do dispositivo.
     * @param probAvariaIn Probabilidade de avaria por hora (0.0 a 1.0).
     * @param xIn Coordenada X da localização na fábrica.
     * @param yIn Coordenada Y da localização na fábrica.
     */
    Dispositivo(int idIn, const string& marcaIn, double probAvariaIn, int xIn, int yIn);

    /**
     * @brief Destrutor virtual da classe Dispositivo.
     */
    virtual ~Dispositivo();

    /**
     * @brief Obtém o identificador único do dispositivo.
     * @return Número inteiro correspondente ao ID.
     */
    int GetId() const;

    /**
     * @brief Obtém a marca do dispositivo.
     * @return Texto com o nome da marca.
     */
    string GetMarca() const;

    /**
     * @brief Obtém a probabilidade de avaria por hora do dispositivo.
     * @return Valor decimal da probabilidade de avaria.
     */
    double GetProbabilidadeAvaria() const;

    /**
     * @brief Obtém a coordenada X do dispositivo na fábrica.
     * @return Posição no eixo X.
     */
    int GetX() const;

    /**
     * @brief Obtém a coordenada Y do dispositivo na fábrica.
     * @return Posição no eixo Y.
     */
    int GetY() const;

    /**
     * @brief Indica se o dispositivo já avariou e ainda não foi reparado.
     * @return true depois de RegistarAvaria, até ser reparado.
     */
    bool EstaAvariado() const;

    /**
     * @brief Obtém o número total de avarias registadas no dispositivo.
     * @return Contador total de avarias.
     */
    int GetTotalAvarias() const;

    /**
     * @brief Conta uma avaria. Só incrementa uma vez enquanto o dispositivo não for reparado.
     */
    void RegistarAvaria();

    /**
     * @brief Limpa o estado de avaria para o dispositivo voltar a operar e a contagem poder voltar a subir.
     */
    virtual void Reparar();

    /**
     * @brief Método virtual que devolve o tipo específico do dispositivo.
     * @return Texto identificador do tipo (ex.: "MCOMBUSTAO", "LUZ", "FUMO").
     */
    virtual string GetTipo() const = 0;
};

#endif
