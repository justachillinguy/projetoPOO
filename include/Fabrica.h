/**
 * @file Fabrica.h
 * @brief Declaração da classe gestora Fabrica.
 */

#ifndef FABRICA_H
#define FABRICA_H

#include <iostream>
#include <string>
#include <list>
#include <map>
#include "Tipos.h"
#include "Dispositivos.h"
#include "Motor.h"
#include "Tipos_Motor.h"
#include "Sensores.h"
#include "Tipos_Sensores.h"
#include "Users.h"
#include "Tipos_Users.h"

using namespace std;

/**
 * @class Fabrica
 * @brief Classe principal que gere os motores, sensores, utilizadores e a simulação da fábrica.
 */
class Fabrica {
private:
    string nome;                        ///< Nome da empresa/fábrica
    int horaAbertura;                   ///< Hora de abertura da fábrica (0-23)
    int horaFecho;                      ///< Hora de encerramento da fábrica (0-23)
    int vizinhancaAviso;                ///< Raio de vizinhança global para avisos
    int dimensaoX;                      ///< Largura máxima da planta da fábrica
    int dimensaoY;                      ///< Altura máxima da planta da fábrica

    int horaSimulada;                   ///< Hora atual do relógio da simulação (0-23)
    int minutoSimulado;                 ///< Minuto atual do relógio da simulação (0-59)
    bool fabricaAtiva;                  ///< Indica se a fábrica está operacional (false em alerta de míssil)

    list<Motor*> motores;               ///< Lista de ponteiros para os motores da fábrica
    list<Sensor*> sensores;             ///< Lista de ponteiros para os sensores da fábrica
    list<User*> utilizadores;           ///< Lista de ponteiros para os utilizadores registados
    User* userAtual;                    ///< Ponteiro para o utilizador atualmente autenticado

    map<string, string> defVerde;       ///< Intervalo térmico VERDE lido do XML por tipo de motor
    map<string, string> defAmarelo;     ///< Intervalo térmico AMARELO lido do XML por tipo de motor
    map<string, string> defVermelho;    ///< Intervalo térmico VERMELHO lido do XML por tipo de motor
    map<string, string> defProbAvaria;  ///< Probabilidade de avaria lida do XML por tipo de motor

    /** @brief Procura um motor pelo seu identificador único. */
    Motor* ProcurarMotor(int id) const;

    /** @brief Procura um sensor pelo seu identificador único. */
    Sensor* ProcurarSensor(int id) const;

    /** @brief Calcula a distância euclidiana entre dois pontos da planta. */
    double CalcularDistancia(int x1, int y1, int x2, int y2) const;

    /** @brief Extrai o conteúdo textual entre uma tag XML de abertura e fecho. */
    string ExtrairTagXML(const string& texto, const string& tag) const;

    /** @brief Separa dois números decimais separados por vírgula. */
    void ExtrairParDouble(const string& texto, double& primeiro, double& segundo) const;

    /** @brief Separa dois números inteiros separados por vírgula. */
    void ExtrairParInt(const string& texto, int& primeiro, int& segundo) const;

    /** @brief Converte percentagens do XML para frações entre 0.0 e 1.0. */
    double NormalizarProbabilidade(double valor) const;

    /** @brief Verifica se o relógio simulado está dentro do horário de funcionamento. */
    bool EstaAberta() const;

    /** @brief Atualiza as leituras dos sensores durante um passo de simulação. */
    void AtualizarSensores();

    /** @brief Avalia probabilidades de avaria aleatória para todos os motores e sensores. */
    void SimularAvarias();

    /** @brief Avança o relógio da simulação em 10 minutos e gere a mudança de dia. */
    void AvancarRelogio(int minutos);

public:
    /**
     * @brief Construtor da classe Fabrica.
     * @param ut Utilizador inicial associado à sessão.
     */
    Fabrica(User *ut);


    bool ExisteMotor(int id) const;

    /**
     * @brief Destrutor virtual da classe Fabrica. Liberta toda a memória alocada.
     */
    virtual ~Fabrica();

    /** @brief Carrega as definições, motores e sensores a partir de um ficheiro XML. */
    bool Load(const string &ficheiro);

    /** @brief Adiciona um novo utilizador à fábrica. */
    bool Add(User *ut);

    /** @brief Adiciona um novo motor à fábrica, validando ID e posição. */
    bool Add(Motor *m);

    /** @brief Adiciona um novo sensor à fábrica, validando ID e posição. */
    bool Add(Sensor *s);

    /** @brief Remove um motor ou sensor pelo seu ID (requer permissão de ADMIN). */
    bool Remover(int id);

    /** @brief Define o utilizador atualmente ativo no sistema. */
    void SetUserAtual(User *ut);

    /** @brief Obtém o utilizador atualmente ativo no sistema. */
    User* GetUserAtual() const;

    /** @brief Lista o estado atual de todos os motores e sensores da fábrica. */
    void Listar(ostream &f = std::cout);

    /** @brief Liga (coloca em START) um motor específico pelo seu ID. */
    bool Ligar(int id_motor);

    /** @brief Desliga (coloca em STOP) um motor específico pelo seu ID. */
    void Desligar(int id_motor);

    /** @brief Obtém o estado operacional atual de um motor. */
    ESTADO_MOTOR Get_Estado(int id_motor);

    /** @brief Lista e devolve todos os motores de um determinado tipo. */
    list<Motor*> *Listar_Tipo(string Tipo, ostream &f = std::cout);

    /** @brief Avança um passo (10 minutos) na simulação completa da fábrica. */
    void StepSimulacao();

    /** @brief Executa a manutenção geral de todos os motores e sensores. */
    bool Manutencao();

    /** @brief Devolve as marcas ordenadas da que regista mais avarias para a mais fiável. */
    list<string> *Ranking_Dos_Fracos();

    /** @brief Devolve os motores ordenados por maior tempo de trabalho acumulado. */
    list<Motor*> *Ranking_Dos_Mais_Trabalhadores();

    /** @brief Exporta o estado atual da fábrica para um ficheiro XML. */
    void Relatorio(string fich_xml);

    /** @brief Exporta uma listagem formatada em tabela HTML do estado atual da fábrica. */
    void Listagem_HTML(string fich_html);

    /** @brief Verifica sensores de humidade em aviso e desliga motores na vizinhança. */
    int Aviso_Humidade(list<Motor*> &lm);

    /** @brief Verifica sensores de fumo/fogo em aviso, desliga motores e gera vídeo de saída. */
    int Aviso_Fumo(list<Motor*> &lm, string fich_video);

    /** @brief Verifica sensores de luz em aviso e gera vídeo de regresso a casa. */
    int Aviso_Luz(string fich_video);

    /** @brief Verifica alerta de míssil, guarda estado, desliga motores e gera vídeo de abrigo. */
    void Aviso_Missel(string fvideo, string festado = "Estado.txt");

    /** @brief Obtém a hora atual da simulação. */
    int GetHoraSimulada() const { return horaSimulada; }

    /** @brief Obtém o minuto atual da simulação. */
    int GetMinutoSimulado() const { return minutoSimulado; }

    /** @brief Indica se a fábrica está ativa ou foi encerrada por emergência. */
    bool EstaAtiva() const { return fabricaAtiva; }
};

#endif // FABRICA_H
