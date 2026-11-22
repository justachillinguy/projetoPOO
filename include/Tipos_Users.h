#ifndef TIPOS_USERS_H
#define TIPOS_USERS_H

#include "Users.h"

/**
 * @brief Utilizador Administrador (tem acesso total a todas as operacoes da fabrica).
 */
class Admin : public User {
public:
    Admin(int id, const string& nome);
    virtual ~Admin();

    virtual bool PodeAdicionar() const;
    virtual bool PodeRemover() const;
    virtual bool PodeListar() const;
    virtual bool PodeRun() const;
    virtual bool PodeManutencao() const;
    virtual string GetTipo() const;
};

/**
 * @brief Utilizador Normal (pode operar, listar e fazer manutencao, mas nao pode remover/desligar criticamente).
 */
class Normal : public User {
public:
    Normal(int id, const string& nome);
    virtual ~Normal();

    virtual bool PodeAdicionar() const;
    virtual bool PodeRemover() const;
    virtual bool PodeListar() const;
    virtual bool PodeRun() const;
    virtual bool PodeManutencao() const;
    virtual string GetTipo() const;
};

/**
 * @brief Utilizador Visitante (apenas pode consultar/listar o estado da fabrica).
 */
class Visitante : public User {
public:
    Visitante(int id, const string& nome);
    virtual ~Visitante();

    virtual bool PodeAdicionar() const;
    virtual bool PodeRemover() const;
    virtual bool PodeListar() const;
    virtual bool PodeRun() const;
    virtual bool PodeManutencao() const;
    virtual string GetTipo() const;
};

#endif // TIPOS_USERS_H
