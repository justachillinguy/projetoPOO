#ifndef TIPOS_USER_H
#define TIPOS_USER_H

#include "User.h"

/**
 * @file TiposUser.h
 * @brief Tipos concretos de utilizador da fabrica: Admin, Normal e Visitante.
 *
 * Cada tipo define o seu nivel de acesso, de acordo com o enunciado:
 * - ADMIN: ADD, REMOVER, LISTAR, RUN, MANUTENCAO
 * - NORMAL: ADD, LISTAR, RUN, MANUTENCAO
 * - VISITANTE: LISTAR
 *
 * A classe base User encontra-se em User.h.
 */

/**
 * @brief Utilizador ADMIN: tem todos os direitos, incluindo REMOVER.
 */
class Admin : public User
{
    public:
        /**
        * @brief Cria um administrador.
        * @param id Identificador.
        * @param nome Nome do utilizador.
        */
        Admin(int id, const string& nome);
        virtual ~Admin();

        /** @brief Devolve "ADMIN". */
        virtual string GetTipo() const;
        virtual bool PodeAdicionar() const;
        virtual bool PodeRemover() const;
        virtual bool PodeListar() const;
        virtual bool PodeRun() const;
        virtual bool PodeManutencao() const;
};

/**
 * @brief Utilizador NORMAL: pode ADD, LISTAR, RUN e MANUTENCAO.
 */
class Normal : public User
{
    public:
        /**
        * @brief Cria um utilizador normal.
        * @param id Identificador.
        * @param nome Nome do utilizador.
        */
        Normal(int id, const string& nome);
        virtual ~Normal();

        /** @brief Devolve "NORMAL". */
        virtual string GetTipo() const;
        virtual bool PodeAdicionar() const;
        virtual bool PodeRemover() const;
        virtual bool PodeListar() const;
        virtual bool PodeRun() const;
        virtual bool PodeManutencao() const;
};

/**
 * @brief Utilizador VISITANTE: so pode LISTAR.
 */
class Visitante : public User
{
    public:
        /**
        * @brief Cria um visitante.
        * @param id Identificador.
        * @param nome Nome do utilizador.
        */
        Visitante(int id, const string& nome);
        virtual ~Visitante();

        /** @brief Devolve "VISITANTE". */
        virtual string GetTipo() const;
        virtual bool PodeAdicionar() const;
        virtual bool PodeRemover() const;
        virtual bool PodeListar() const;
        virtual bool PodeRun() const;
        virtual bool PodeManutencao() const;
};

#endif
