/**
 * @file Tipos.h
 * @brief Definição dos tipos de dados globais(estados do motor e estados da temperatura) utilizados no sistema da fábrica.
 */

#ifndef TIPOS_H
#define TIPOS_H

/**
 * @enum ESTADO_MOTOR
 * @brief Representa os possíveis estados operacionais de um motor.
 */
enum ESTADO_MOTOR {
    STOP,       ///< Motor parado (desligado ou em espera)
    START,      ///< Motor pronto a arrancar (em fase de ignição/arranque)
    RUN,        ///< Motor em funcionamento ativo
    AVARIADO    ///< Motor inoperacional devido a avaria (requer manutenção)
};

/**
 * @enum CLASSE_TEMPERATURA
 * @brief Classifica a zona térmica atual de um motor face aos seus limiares de segurança.
 */
enum CLASSE_TEMPERATURA {
    VERDE,      ///< Temperatura normal de funcionamento (abaixo do limiar de aviso)
    AMARELO,    ///< Zona de aviso (entre o limiar de aviso e o limiar de paragem)
    VERMELHO    ///< Zona crítica/perigo (atingiu ou ultrapassou o limiar de paragem)
};

#endif
