/**
 * @file Relogio.cpp
 * @brief Implementação dos métodos da classe Relogio.
 */

#include "Relogio.h"

/**
 * @brief Construtor da classe Relogio.
 * @param _velocidade Velocidade inicial do relógio.
 */
Relogio::Relogio(int _velocidade) {
    tInicio = time(NULL);
    segundosAcumulados = 0;
    if (_velocidade >= 1) {
        velocidade = _velocidade;
    } else {
        velocidade = 1;
    }
}

/**
 * @brief Destrutor da classe Relogio.
 */
Relogio::~Relogio() {}

/**
 * @brief Obtém o total de segundos simulados decorridos desde o arranque do relógio.
 * @return Segundos simulados decorridos.
 */
long Relogio::GetSegundosSimulados() const {
    time_t agora = time(NULL);
    return segundosAcumulados + (long)(velocidade * (agora - tInicio));
}

/**
 * @brief Calcula o tempo atual da simulação somando ao início o tempo simulado decorrido.
 * @return Timestamp simulado atual.
 */
time_t Relogio::GetTempo() const {
    return tInicio + (time_t)GetSegundosSimulados();
}

/**
 * @brief Altera a velocidade da simulação acumulando o tempo simulado anterior.
 * @param vel Novo valor de velocidade (deve ser >= 1).
 */
void Relogio::MudarVelocidade(int vel) {
    if (vel >= 1) {
        segundosAcumulados = GetSegundosSimulados();
        tInicio = time(NULL);
        velocidade = vel;
    }
}

/**
 * @brief Obtém a velocidade atual configurada no relógio.
 * @return Fator de velocidade atual.
 */
int Relogio::GetVelocidade() const {
    return velocidade;
}
