#ifndef USER_H
#define USER_H

#include <string>

using namespace std;

/**
 * @file User.h
 * @brief Classe base dos utilizadores da fabrica.
 *
 * USER (ID, NOME). Os tipos concretos (Admin, Normal e Visitante)
 * encontram-se em TiposUser.h, com os seguintes niveis de acesso:
 * - ADMIN: ADD, REMOVER, LISTAR, RUN, MANUTENCAO
 * - NORMAL: ADD, LISTAR, RUN, MANUTENCAO
 * - VISITANTE: LISTAR
 *
 * A operacao REMOVER fica reservada ao perfil Admin. O metodo da fabrica
 * que a executa entra com o menu, na fase do simulador.
 */
class User
{
    protected:
        int id; ///< Identificador do utilizador.
        string nome; ///< Nome do utilizador.

    public:
        /**
        * @brief Cria um utilizador.
        * @param id Identificador.
        * @param nome Nome do utilizador.
        */
        User(int id, const string& nome);

        /** @brief Destrutor virtual: a fabrica guarda User*, pelo que a
        *         libertacao correta passa por este destrutor. */
        virtual ~User();

        /** @brief Devolve o identificador. */
        int GetId() const;

        /** @brief Devolve o nome. */
        string GetNome() const;

        /** @brief Nome do tipo: ADMIN, NORMAL ou VISITANTE. */
        virtual string GetTipo() const = 0;

        /** @brief Pode executar ADD? */
        virtual bool PodeAdicionar() const = 0;

        /** @brief Pode executar REMOVER? */
        virtual bool PodeRemover() const = 0;

        /** @brief Pode executar LISTAR? */
        virtual bool PodeListar() const = 0;

        /** @brief Pode executar RUN? */
        virtual bool PodeRun() const = 0;

        /** @brief Pode executar MANUTENCAO? */
        virtual bool PodeManutencao() const = 0;
};

#endif
