#ifndef USERS_H
#define USERS_H

#include <string>

using namespace std;

/**
 * @brief Classe base abstrata para todos os utilizadores da fabrica.
 */
class User {
protected:
    int id;
    string nome;

public:
    User(int idIn, const string& nomeIn);
    virtual ~User();

    int GetId() const;
    string GetNome() const;

    virtual bool PodeAdicionar() const = 0;
    virtual bool PodeRemover() const = 0;
    virtual bool PodeListar() const = 0;
    virtual bool PodeRun() const = 0;
    virtual bool PodeManutencao() const = 0;
    virtual string GetTipo() const = 0;
};

#endif
