/**
 * @file Motor.cpp
 * @brief Implementacao dos metodos da classe base Motor.
 */

#include "Motor.h"
#include <cstdlib>

/**
 * @brief Construtor da classe Motor.
 */
Motor::Motor(int idIn, const string& tipoIn, const string& marcaIn, double consumoHoraIn, double tempAvisoIn, double tempParagemIn,
             double probAvariaIn, int xIn, int yIn)
    : Dispositivo(idIn, marcaIn, probAvariaIn, xIn, yIn), tipo(tipoIn), consumoHora(consumoHoraIn),
      consumoDia(0.0), temperaturaAtual(25.0), temperaturaAviso(tempAvisoIn),
      temperaturaParagem(tempParagemIn), tempoTrabalho(0), estado(ESTADO_MOTOR::STOP) {}

/**
 * @brief Destrutor virtual da classe Motor.
 */
Motor::~Motor() {}

/**
 * @brief Obtém o tipo específico do motor.
 * @return Texto com o tipo de motor.
 */
string Motor::GetTipo() const {
    return tipo;
}

/**
 * @brief Obtém o estado operacional atual do motor.
 * @return Estado atual do motor (STOP, START, RUN ou AVARIADO).
 */
ESTADO_MOTOR Motor::GetEstado() const {
    return estado;
}

/**
 * @brief Define o estado operacional atual do motor.
 * @param est Novo estado operacional.
 */
void Motor::SetEstado(ESTADO_MOTOR est) {
    estado = est;
    if (est == ESTADO_MOTOR::AVARIADO) {
        avariado = true;
    }
}

/**
 * @brief Obtém o consumo energético horário do motor.
 * @return Consumo por hora.
 */
double Motor::GetConsumoHora() const {
    return consumoHora;
}

/**
 * @brief Obtém o consumo energético acumulado no dia atual.
 * @return Consumo diário.
 */
double Motor::GetConsumoDia() const {
    return consumoDia;
}

/**
 * @brief Obtém a temperatura atual do motor.
 * @return Temperatura em graus Celsius.
 */

double Motor::GetTemperaturaAviso() const {
    return temperaturaAviso;
}

double Motor::GetTemperaturaParagem() const {
    return temperaturaParagem;
}

double Motor::GetTemperaturaAtual() const {
    return temperaturaAtual;
}

/**
 * @brief Define a temperatura atual do motor.
 * @param temp Nova temperatura em graus Celsius.
 */
void Motor::SetTemperaturaAtual(double temp) {
    temperaturaAtual = temp;
}

/**
 * @brief Obtém o tempo total de trabalho acumulado pelo motor.
 * @return Tempo total em minutos.
 */
int Motor::GetTempoTrabalho() const {
    return tempoTrabalho;
}

/**
 * @brief Reinicia o contador de consumo energético diário para zero.
 */
void Motor::ReiniciarConsumoDia() {
    consumoDia = 0.0;
}

/**
 * @brief Inicia o motor.
 * Verifica primeiro se a temperatura se encontra na zona VERMELHA.
 * Se não estiver avariado nem sobreaquecido e estiver parado (STOP), passa para START.
 */
void Motor::START() {
    if (ESTOU_QUENTE() == VERMELHO) {
        STOP();
        return;
    }

    if (!avariado && estado == ESTADO_MOTOR::STOP) {
        estado = ESTADO_MOTOR::START;
    }
}

/**
 * @brief Executa um ciclo de funcionamento de 10 minutos do motor.
 */
void Motor::RUN() {
    if (ESTOU_QUENTE() == VERMELHO) {
        STOP();
        return;
    }

    if (!avariado && (estado == ESTADO_MOTOR::START || estado == ESTADO_MOTOR::RUN)) {
        estado = ESTADO_MOTOR::RUN;
        tempoTrabalho += 10;
        consumoDia += consumoHora * (10.0 / 60.0);
        temperaturaAtual += 2.0;

        if (ESTOU_QUENTE() == VERMELHO) {
            STOP();
        }
    }
}

/**
 * @brief Reinicia o motor, efetuando paragem seguida de arranque.
 */
void Motor::RESTART() {
    if (!avariado) {
        STOP();
        START();
    }
}

/**
 * @brief Pára o motor, colocando-o em STOP caso não esteja avariado.
 */
void Motor::STOP() {
    if (estado != ESTADO_MOTOR::AVARIADO) {
        estado = ESTADO_MOTOR::STOP;
    }
}

/**
 * @brief Reduz gradualmente a temperatura do motor quando está parado.
 * @param graus Quantidade de graus Celsius a reduzir.
 */
void Motor::Arrefecer(double graus) {
    temperaturaAtual -= graus;
    if (temperaturaAtual < 20.0) {
        temperaturaAtual = 20.0;
    }
}

/**
 * @brief Avalia a classe térmica atual do motor.
 * @return VERMELHO, AMARELO ou VERDE consoante os limiares configurados.
 */
CLASSE_TEMPERATURA Motor::ESTOU_QUENTE() {
    if (temperaturaAtual >= temperaturaParagem) {
        return VERMELHO;
    }
    if (temperaturaAtual >= temperaturaAviso) {
        return AMARELO;
    }
    return VERDE;
}

/**
 * @brief Efetua a manutenção do motor, reparando avarias e repondo temperatura na zona VERDE.
 */
void Motor::Manutencao() {
    Reparar();
    estado = ESTADO_MOTOR::STOP;

    int maxVerde = (int)temperaturaAviso - 1;
    if (maxVerde < 20) {
        maxVerde = 20;
    }
    temperaturaAtual = 20 + (rand() % (maxVerde - 19));
}
