


/**
 * @file Fabrica.cpp
 * @brief Implementação dos métodos da classe Fabrica, leitura/escrita XML e simulação.
 */

#include "Fabrica.h"

static void ExecutarVideo(const string& caminho) {
    if (caminho.empty()) return;
#ifdef _WIN32
    string cmd = "start "" "" + caminho + """;
#elif defined(__linux__) || defined(__APPLE__)
    string cmd = "xdg-open "" + caminho + "" 2>/dev/null || open "" + caminho + "" 2>/dev/null &";
#else
    string cmd = "";
#endif
    if (!cmd.empty()) {
        system(cmd.c_str());
    }
}

#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <vector>

/**
 * @brief Construtor da classe Fabrica.
 * @param ut Utilizador inicial associado à sessão.
 */
Fabrica::Fabrica(User *ut)
    : nome("Fabrica"), horaAbertura(8), horaFecho(16), vizinhancaAviso(10),
      dimensaoX(100), dimensaoY(100), horaSimulada(8), minutoSimulado(0),
      fabricaAtiva(true), userAtual(ut) {
    if (ut != NULL) {
        utilizadores.push_back(ut);
    }
}

/**
 * @brief Destrutor virtual da classe Fabrica.
 * Liberta toda a memória alocada dinamicamente para motores, sensores e utilizadores.
 */
Fabrica::~Fabrica() {
    for (list<Motor*>::iterator it = motores.begin(); it != motores.end(); ++it) {
        delete *it;
    }
    motores.clear();

    for (list<Sensor*>::iterator it = sensores.begin(); it != sensores.end(); ++it) {
        delete *it;
    }
    sensores.clear();

    for (list<User*>::iterator it = utilizadores.begin(); it != utilizadores.end(); ++it) {
        delete *it;
    }
    utilizadores.clear();
}

// ==================== AUXILIARES PRIVADOS ====================

/**
 * @brief Procura um motor na lista pelo seu identificador.
 * @param id Identificador do motor.
 * @return Ponteiro para o motor encontrado ou NULL se não existir.
 */
Motor* Fabrica::ProcurarMotor(int id) const {
    for (list<Motor*>::const_iterator it = motores.begin(); it != motores.end(); ++it) {
        if ((*it)->GetId() == id) {
            return *it;
        }
    }
    return NULL;
}

/**
 * @brief Procura um sensor na lista pelo seu identificador.
 * @param id Identificador do sensor.
 * @return Ponteiro para o sensor encontrado ou NULL se não existir.
 */
Sensor* Fabrica::ProcurarSensor(int id) const {
    for (list<Sensor*>::const_iterator it = sensores.begin(); it != sensores.end(); ++it) {
        if ((*it)->GetId() == id) {
            return *it;
        }
    }
    return NULL;
}

/**
 * @brief Calcula a distância euclidiana entre duas coordenadas da fábrica.
 * @param x1 Coordenada X do primeiro ponto.
 * @param y1 Coordenada Y do primeiro ponto.
 * @param x2 Coordenada X do segundo ponto.
 * @param y2 Coordenada Y do segundo ponto.
 * @return Distância entre os dois pontos.
 */
double Fabrica::CalcularDistancia(int x1, int y1, int x2, int y2) const {
    double dx = x1 - x2;
    double dy = y1 - y2;
    return sqrt(dx * dx + dy * dy);
}

/**
 * @brief Extrai o conteúdo existente entre uma tag de abertura e de fecho XML.
 * @param texto Texto XML onde procurar.
 * @param tag Nome da etiqueta XML (sem sinais < >).
 * @return Conteúdo interno da tag ou string vazia se não for encontrada.
 */
string Fabrica::ExtrairTagXML(const string& texto, const string& tag) const {
    string inicio = "<" + tag + ">";
    string fim = "</" + tag + ">";
    size_t pos1 = texto.find(inicio);
    if (pos1 == string::npos) return "";
    pos1 += inicio.length();
    size_t pos2 = texto.find(fim, pos1);
    if (pos2 == string::npos) return "";
    return texto.substr(pos1, pos2 - pos1);
}

/**
 * @brief Extrai dois valores reais separados por vírgula.
 * @param texto String contendo o par de valores.
 * @param primeiro Variável onde será guardado o primeiro valor.
 * @param segundo Variável onde será guardado o segundo valor.
 */
void Fabrica::ExtrairParDouble(const string& texto, double& primeiro, double& segundo) const {
    size_t virgula = texto.find(',');
    if (virgula != string::npos) {
        primeiro = atof(texto.substr(0, virgula).c_str());
        segundo = atof(texto.substr(virgula + 1).c_str());
    }
}

/**
 * @brief Extrai dois valores inteiros separados por vírgula.
 * @param texto String contendo o par de coordenadas.
 * @param primeiro Variável onde será guardado o primeiro valor.
 * @param segundo Variável onde será guardado o segundo valor.
 */
void Fabrica::ExtrairParInt(const string& texto, int& primeiro, int& segundo) const {
    size_t virgula = texto.find(',');
    if (virgula != string::npos) {
        primeiro = atoi(texto.substr(0, virgula).c_str());
        segundo = atoi(texto.substr(virgula + 1).c_str());
    }
}

/**
 * @brief Normaliza a probabilidade lida do XML para o intervalo [0.0, 1.0].
 * @param valor Valor lido do ficheiro XML.
 * @return Probabilidade normalizada.
 */
double Fabrica::NormalizarProbabilidade(double valor) const {
    if (valor >= 1.0) {
        return valor / 100.0;
    }
    return valor;
}

/**
 * @brief Verifica se a hora atual da simulação está dentro do horário de laboração.
 * @return true se a fábrica estiver aberta; false caso contrário.
 */
bool Fabrica::EstaAberta() const {
    return (horaSimulada >= horaAbertura && horaSimulada < horaFecho);
}

/**
 * @brief Atualiza os valores lidos pelos sensores durante a simulação.
 * Os sensores de temperatura refletem a maior temperatura dos motores na sua vizinhança.
 */
void Fabrica::AtualizarSensores() {
    for (list<Sensor*>::iterator it = sensores.begin(); it != sensores.end(); ++it) {
        Sensor* s = *it;
        if (s->EstaAvariado()) continue;

        if (s->GetTipo() == "TEMPERATURA") {
            double maxTemp = 20.0;
            for (list<Motor*>::iterator mIt = motores.begin(); mIt != motores.end(); ++mIt) {
                Motor* m = *mIt;
                if (CalcularDistancia(s->GetX(), s->GetY(), m->GetX(), m->GetY()) <= vizinhancaAviso) {
                    if (m->GetTemperaturaAtual() > maxTemp) {
                        maxTemp = m->GetTemperaturaAtual();
                    }
                }
            }
            s->SetValor(maxTemp);
        }
    }
}

/**
 * @brief Simula a ocorrência aleatória de avarias nos motores e sensores ativos.
 */
void Fabrica::SimularAvarias() {
    for (list<Motor*>::iterator it = motores.begin(); it != motores.end(); ++it) {
        Motor* m = *it;
        if (!m->EstaAvariado() && m->GetEstado() == ESTADO_MOTOR::RUN) {
            double r = (double)rand() / RAND_MAX;
            if (r < (m->GetProbabilidadeAvaria() / 6.0)) {
                m->RegistarAvaria();
                m->STOP();
                cout << "[ALERTA GESTOR] Motor ID " << m->GetId()
                     << " (" << m->GetMarca() << ") AVARIOU!\n";
            }
        }
    }

    for (list<Sensor*>::iterator it = sensores.begin(); it != sensores.end(); ++it) {
        Sensor* s = *it;
        if (!s->EstaAvariado()) {
            double r = (double)rand() / RAND_MAX;
            if (r < (s->GetProbabilidadeAvaria() / 6.0)) {
                s->RegistarAvaria();
                cout << "[ALERTA GESTOR] Sensor ID " << s->GetId()
                     << " (" << s->GetMarca() << ") AVARIOU!\n";
            }
        }
    }
}

/**
 * @brief Avança o relógio da simulação e reinicia o consumo diário na viragem do dia.
 * @param minutos Quantidade de minutos a somar.
 */
void Fabrica::AvancarRelogio(int minutos) {
    minutoSimulado += minutos;
    while (minutoSimulado >= 60) {
        minutoSimulado -= 60;
        horaSimulada++;
    }
    if (horaSimulada >= 24) {
        horaSimulada %= 24;
        for (list<Motor*>::iterator it = motores.begin(); it != motores.end(); ++it) {
            (*it)->ReiniciarConsumoDia();
        }
    }
}

// ==================== CARREGAMENTO XML ====================

/**
 * @brief Lê e carrega a configuração da fábrica, motores e sensores a partir de um ficheiro XML.
 * @param ficheiro Nome/caminho do ficheiro XML.
 * @return true se o ficheiro for lido com sucesso; false caso contrário.
 */
bool Fabrica::Load(const string &ficheiro) {
    ifstream fich(ficheiro.c_str());
    if (!fich.is_open()) {
        return false;
    }

    stringstream buffer;
    buffer << fich.rdbuf();
    string conteudo = buffer.str();
    fich.close();

    // 1. Ler seccao <DEFINICOES>
    string secDefinicoes = ExtrairTagXML(conteudo, "DEFINICOES");
    if (secDefinicoes.empty()) {
        cout << "XML invalido: falta seccao <DEFINICOES>.\n";
        return false;
    }

    string n = ExtrairTagXML(secDefinicoes, "NOME_EMPRESA");
    if (!n.empty()) nome = n;

    string hi = ExtrairTagXML(secDefinicoes, "HORA_INICIO");
    if (!hi.empty()) {
        horaAbertura = atoi(hi.c_str());
        horaSimulada = horaAbertura;
        minutoSimulado = 0;
    }

    string hf = ExtrairTagXML(secDefinicoes, "HORA_FECHO");
    if (!hf.empty()) horaFecho = atoi(hf.c_str());

    string viz = ExtrairTagXML(secDefinicoes, "VIZINHANCA_AVISO");
    if (!viz.empty()) vizinhancaAviso = atoi(viz.c_str());

    string dim = ExtrairTagXML(secDefinicoes, "DIMENSAO_FABRICA");
    if (!dim.empty()) ExtrairParInt(dim, dimensaoX, dimensaoY);

    string tiposMotor[3] = { "MELETRICO", "MCOMBUSTAO", "MINDUCAO" };
    map<string, double> avisoPorTipo;
    map<string, double> paragemPorTipo;
    map<string, double> probPorTipo;

    for (int i = 0; i < 3; i++) {
        string blocoTipo = ExtrairTagXML(secDefinicoes, tiposMotor[i]);
        string strVerde = ExtrairTagXML(blocoTipo, "VERDE");
        string strAmarelo = ExtrairTagXML(blocoTipo, "AMARELO");
        string strVermelho = ExtrairTagXML(blocoTipo, "VERMELHO");
        string strProb = ExtrairTagXML(blocoTipo, "PROB_AVARIA");

        defVerde[tiposMotor[i]] = strVerde;
        defAmarelo[tiposMotor[i]] = strAmarelo;
        defVermelho[tiposMotor[i]] = strVermelho;
        defProbAvaria[tiposMotor[i]] = strProb;

        double minAmarelo = 60.0, maxAmarelo = 80.0;
        double minVermelho = 81.0, maxVermelho = 200.0;
        ExtrairParDouble(strAmarelo, minAmarelo, maxAmarelo);
        ExtrairParDouble(strVermelho, minVermelho, maxVermelho);
        double prob = NormalizarProbabilidade(atof(strProb.c_str()));

        avisoPorTipo[tiposMotor[i]] = minAmarelo;
        paragemPorTipo[tiposMotor[i]] = minVermelho;
        probPorTipo[tiposMotor[i]] = prob;
    }

    // 2. Ler seccao <MOTORES>
    string secMotores = ExtrairTagXML(conteudo, "MOTORES");
    for (int t = 0; t < 3; t++) {
        string tagTipo = tiposMotor[t];
        string tagAbrir = "<" + tagTipo + ">";
        string tagFechar = "</" + tagTipo + ">";
        size_t pos = 0;

        while ((pos = secMotores.find(tagAbrir, pos)) != string::npos) {
            size_t fim = secMotores.find(tagFechar, pos);
            if (fim == string::npos) break;

            string bloco = secMotores.substr(pos + tagAbrir.length(), fim - (pos + tagAbrir.length()));

            int id = atoi(ExtrairTagXML(bloco, "ID").c_str());
            string marca = ExtrairTagXML(bloco, "MARCA");
            double tempAtual = atof(ExtrairTagXML(bloco, "VALOR_TEMP_ATUAL").c_str());
            string estadoStr = ExtrairTagXML(bloco, "ESTADO_ATUAL");
            double consumo = atof(ExtrairTagXML(bloco, "CONSUMO_HORA").c_str());

            int x = 0, y = 0;
            ExtrairParInt(ExtrairTagXML(bloco, "POSICAO"), x, y);

            if (x >= 0 && x <= dimensaoX && y >= 0 && y <= dimensaoY && ProcurarMotor(id) == NULL) {
                Motor* m = NULL;
                if (tagTipo == "MCOMBUSTAO") {
                    m = new MotorCombustao(id, marca, consumo, avisoPorTipo[tagTipo], paragemPorTipo[tagTipo], probPorTipo[tagTipo], x, y);
                } else if (tagTipo == "MINDUCAO") {
                    m = new MotorInducao(id, marca, consumo, avisoPorTipo[tagTipo], paragemPorTipo[tagTipo], probPorTipo[tagTipo], x, y);
                } else {
                    m = new MotorEletrico(id, marca, consumo, avisoPorTipo[tagTipo], paragemPorTipo[tagTipo], probPorTipo[tagTipo], x, y);
                }

                if (m != NULL) {
                    m->SetTemperaturaAtual(tempAtual);
                    if (estadoStr == "RUN") m->SetEstado(ESTADO_MOTOR::RUN);
                    else if (estadoStr == "START") m->SetEstado(ESTADO_MOTOR::START);
                    else if (estadoStr == "AVARIADO") m->SetEstado(ESTADO_MOTOR::AVARIADO);
                    else m->SetEstado(ESTADO_MOTOR::STOP);

                    motores.push_back(m);
                }
            }
            pos = fim + tagFechar.length();
        }
    }

    // 3. Ler seccao <SENSORES>
    string secSensores = ExtrairTagXML(conteudo, "SENSORES");
    string tagsSensores[6] = { "SHUMIDADE", "SLUZ", "SFUMO", "SMISSIL", "STEMPERATURA", "SFOGO" };

    for (int sIdx = 0; sIdx < 6; sIdx++) {
        string tagS = tagsSensores[sIdx];
        string tagAbrir = "<" + tagS + ">";
        string tagFechar = "</" + tagS + ">";
        size_t pos = 0;

        while ((pos = secSensores.find(tagAbrir, pos)) != string::npos) {
            size_t fim = secSensores.find(tagFechar, pos);
            if (fim == string::npos) break;

            string bloco = secSensores.substr(pos + tagAbrir.length(), fim - (pos + tagAbrir.length()));

            int id = atoi(ExtrairTagXML(bloco, "ID").c_str());
            string marca = ExtrairTagXML(bloco, "MARCA");
            double valorAviso = atof(ExtrairTagXML(bloco, "VALOR_AVISO").c_str());
            double prob = NormalizarProbabilidade(atof(ExtrairTagXML(bloco, "PROB_AVARIA").c_str()));
            double valorAtual = atof(ExtrairTagXML(bloco, "VALOR_ATUAL").c_str());

            int x = 0, y = 0;
            ExtrairParInt(ExtrairTagXML(bloco, "POSICAO"), x, y);

            if (x >= 0 && x <= dimensaoX && y >= 0 && y <= dimensaoY && ProcurarSensor(id) == NULL) {
                Sensor* s = NULL;
                if (tagS == "SHUMIDADE") {
                    s = new SensorHumidade(id, marca, valorAtual, prob, x, y, valorAviso, vizinhancaAviso);
                } else if (tagS == "SLUZ") {
                    s = new SensorLuz(id, marca, valorAtual, prob, x, y, valorAviso);
                } else if (tagS == "SFUMO") {
                    s = new SensorFumo(id, marca, valorAtual, prob, x, y, valorAviso);
                } else if (tagS == "SMISSIL") {
                    s = new SensorMissel(id, marca, valorAtual, prob, x, y);
                } else if (tagS == "SFOGO") {
                    s = new SensorFogo(id, marca, valorAtual, prob, x, y, valorAviso);
                } else {
                    s = new SensorTemperatura(id, marca, valorAtual, prob, x, y, valorAviso);
                }

                if (s != NULL) {
                    sensores.push_back(s);
                }
            }
            pos = fim + tagFechar.length();
        }
    }

    return true;
}

// ==================== UTILIZADORES E ADICAO ====================

/**
 * @brief Adiciona um novo utilizador à lista da fábrica.
 * @param ut Ponteiro para o utilizador a adicionar.
 * @return true se adicionado com sucesso; false caso contrário.
 */
bool Fabrica::Add(User *ut) {
    if (ut == NULL) return false;
    if (userAtual != NULL && !userAtual->PodeAdicionar()) {
        cout << "Permissao negada: utilizador atual nao pode adicionar utilizadores.\n";
        return false;
    }
    for (list<User*>::iterator it = utilizadores.begin(); it != utilizadores.end(); ++it) {
        if ((*it)->GetId() == ut->GetId()) return false;
    }
    utilizadores.push_back(ut);
    return true;
}

/**
 * @brief Adiciona um novo motor à fábrica se o ID for único e a posição válida.
 * @param m Ponteiro para o motor a adicionar.
 * @return true se adicionado com sucesso; false caso contrário.
 */
bool Fabrica::Add(Motor *m) {
    if (m == NULL) return false;
    if (userAtual != NULL && !userAtual->PodeAdicionar()) {
        cout << "Permissao negada: utilizador atual nao pode adicionar motores.\n";
        return false;
    }
    if (m->GetX() < 0 || m->GetX() > dimensaoX || m->GetY() < 0 || m->GetY() > dimensaoY) return false;
    if (ProcurarMotor(m->GetId()) != NULL) return false;
    motores.push_back(m);
    return true;
}

/**
 * @brief Adiciona um novo sensor à fábrica se o ID for único e a posição válida.
 * @param s Ponteiro para o sensor a adicionar.
 * @return true se adicionado com sucesso; false caso contrário.
 */
bool Fabrica::Add(Sensor *s) {
    if (s == NULL) return false;
    if (userAtual != NULL && !userAtual->PodeAdicionar()) {
        cout << "Permissao negada: utilizador atual nao pode adicionar sensores.\n";
        return false;
    }
    if (s->GetX() < 0 || s->GetX() > dimensaoX || s->GetY() < 0 || s->GetY() > dimensaoY) return false;
    if (ProcurarSensor(s->GetId()) != NULL) return false;
    sensores.push_back(s);
    return true;
}

/**
 * @brief Remove um motor ou sensor da fábrica pelo seu ID.
 * @param id Identificador do equipamento a remover.
 * @return true se o equipamento foi encontrado e removido; false caso contrário.
 */
bool Fabrica::Remover(int id) {
    if (userAtual != NULL && !userAtual->PodeRemover()) {
        cout << "Permissao negada: apenas ADMIN pode remover dispositivos.\n";
        return false;
    }
    for (list<Motor*>::iterator it = motores.begin(); it != motores.end(); ++it) {
        if ((*it)->GetId() == id) {
            delete *it;
            motores.erase(it);
            return true;
        }
    }
    for (list<Sensor*>::iterator it = sensores.begin(); it != sensores.end(); ++it) {
        if ((*it)->GetId() == id) {
            delete *it;
            sensores.erase(it);
            return true;
        }
    }
    return false;
}

/**
 * @brief Define o utilizador atual da sessão.
 * @param ut Ponteiro para o utilizador.
 */
void Fabrica::SetUserAtual(User *ut) {
    userAtual = ut;
}

/**
 * @brief Obtém o utilizador atual da sessão.
 * @return Ponteiro para o utilizador ativo.
 */
User* Fabrica::GetUserAtual() const {
    return userAtual;
}

// ==================== LISTAGEM E CONTROLO ====================

/**
 * @brief Escreve o estado detalhado da fábrica para uma stream de saída.
 * @param f Stream de saída (ex.: cout ou ficheiro).
 */
void Fabrica::Listar(ostream &f) {
    if (userAtual != NULL && !userAtual->PodeListar()) {
        f << "Permissao negada para listar.\n";
        return;
    }
    f << "\n=== ESTADO DA FABRICA: " << nome << " (" << horaAbertura << "h - " << horaFecho
      << "h | Hora Atual: " << horaSimulada << ":";
    if (minutoSimulado < 10) {
        f << "0";
    }
    f << minutoSimulado << " | Area: " << dimensaoX << "x" << dimensaoY << ") ===\n";

    for (list<Motor*>::iterator it = motores.begin(); it != motores.end(); ++it) {
        Motor* m = *it;
        string est = "STOP";
        if (m->GetEstado() == ESTADO_MOTOR::START) est = "START";
        else if (m->GetEstado() == ESTADO_MOTOR::RUN) est = "RUN";
        else if (m->GetEstado() == ESTADO_MOTOR::AVARIADO) est = "AVARIADO";

        f << "[MOTOR] ID: " << m->GetId() << " | Tipo: " << m->GetTipo()
          << " | Marca: " << m->GetMarca() << " | Pos: (" << m->GetX() << "," << m->GetY() << ")"
          << " | Estado: " << est << " | Temp: " << m->GetTemperaturaAtual()
          << " | ConsumoDia: " << m->GetConsumoDia() << "\n";
    }

    for (list<Sensor*>::iterator it = sensores.begin(); it != sensores.end(); ++it) {
        Sensor* s = *it;
        string textoAviso = "NAO";
        if (s->EmAviso()) {
            textoAviso = "SIM";
        }

        string textoAvariado = "NAO";
        if (s->EstaAvariado()) {
            textoAvariado = "SIM";
        }

        f << "[SENSOR] ID: " << s->GetId() << " | Tipo: " << s->GetTipo()
          << " | Marca: " << s->GetMarca() << " | Pos: (" << s->GetX() << "," << s->GetY() << ")"
          << " | Valor: " << s->GetValor() << " | Aviso: " << textoAviso
          << " | Avariado: " << textoAvariado << "\n";
    }
}

/**
 * @brief Verifica se existe um motor registado com o identificador indicado.
 * @param id Identificador do motor.
 * @return true se existir; false caso contrário.
 */
bool Fabrica::ExisteMotor(int id) const {
    return ProcurarMotor(id) != NULL;
}

/**
 * @brief Liga um motor específico (coloca em START) se o utilizador tiver permissão, a fábrica estiver aberta e o ID existir.
 * @param id_motor Identificador do motor a ligar.
 * @return true se o comando foi aceite; false caso contrário.
 */
bool Fabrica::Ligar(int id_motor) {
    if (userAtual != NULL && !userAtual->PodeRun()) {
        cout << "Permissao negada para ligar motores.\n";
        return false;
    }
    if (!fabricaAtiva || !EstaAberta()) {
        cout << "Nao e possivel ligar o motor: fabrica fechada ou inativa.\n";
        return false;
    }
    Motor* m = ProcurarMotor(id_motor);
    if (m == NULL) {
        cout << "Erro: Motor com ID " << id_motor << " nao encontrado.\n";
        return false;
    }

    m->START();
    cout << "Comando START enviado ao motor " << id_motor << ".\n";
    return true;
}

/**
 * @brief Pára um motor específico pelo seu identificador, validando a sua existência.
 * @param id_motor Identificador do motor a desligar.
 */
void Fabrica::Desligar(int id_motor) {
    if (userAtual != NULL && !userAtual->PodeRun()) {
        cout << "Permissao negada para desligar motores.\n";
        return;
    }
    Motor* m = ProcurarMotor(id_motor);
    if (m == NULL) {
        cout << "Erro: Motor com ID " << id_motor << " nao encontrado.\n";
        return;
    }

    m->STOP();
    cout << "Comando STOP enviado ao motor " << id_motor << ".\n";
}

/**
 * @brief Obtém o estado operacional atual de um motor.
 * @param id_motor Identificador do motor.
 * @return Estado atual do motor (STOP, START, RUN ou AVARIADO).
 */
ESTADO_MOTOR Fabrica::Get_Estado(int id_motor) {
    Motor* m = ProcurarMotor(id_motor);
    if (m == NULL) {
        return ESTADO_MOTOR::STOP;
    }
    if (m->EstaAvariado()) return ESTADO_MOTOR::AVARIADO;
    return m->GetEstado();
}

/**
 * @brief Lista os motores de um determinado tipo e devolve-os numa lista ligada.
 * @param Tipo Designação do tipo de motor (ex.: "MCOMBUSTAO").
 * @param f Stream de saída para listagem.
 * @return Ponteiro para lista com os motores encontrados.
 */
list<Motor*> *Fabrica::Listar_Tipo(string Tipo, ostream &f) {
    list<Motor*> *resultado = new list<Motor*>();
    if (userAtual != NULL && !userAtual->PodeListar()) {
        return resultado;
    }
    f << "\n--- MOTORES DO TIPO: " << Tipo << " ---\n";
    for (list<Motor*>::iterator it = motores.begin(); it != motores.end(); ++it) {
        Motor* m = *it;
        if (m->GetTipo() == Tipo) {
            resultado->push_back(m);
            f << "ID: " << m->GetId() << " | Marca: " << m->GetMarca()
              << " | Temp: " << m->GetTemperaturaAtual() << "\n";
        }
    }
    return resultado;
}

// ==================== SIMULACAO E MANUTENCAO ====================

/**
 * @brief Executa um passo completo de 10 minutos na simulação da fábrica.
 */
void Fabrica::StepSimulacao() {
    if (!fabricaAtiva) {
        cout << "[SISTEMA] Fabrica encerrada por alerta de emergencia.\n";
        return;
    }

    bool aberta = EstaAberta();

    for (list<Motor*>::iterator it = motores.begin(); it != motores.end(); ++it) {
        Motor* m = *it;

        if (!aberta) {
            if (m->GetEstado() == ESTADO_MOTOR::START || m->GetEstado() == ESTADO_MOTOR::RUN) {
                m->STOP();
            }
            m->Arrefecer(2.0);
            continue;
        }

        if (m->GetEstado() == ESTADO_MOTOR::STOP) {
            m->Arrefecer(1.0);
        } else if (m->GetEstado() == ESTADO_MOTOR::START || m->GetEstado() == ESTADO_MOTOR::RUN) {
            CLASSE_TEMPERATURA antes = m->ESTOU_QUENTE();
            m->RUN();
            CLASSE_TEMPERATURA depois = m->ESTOU_QUENTE();

            if (depois == AMARELO && antes != AMARELO) {
                cout << "[AVISO GESTOR] Motor ID " << m->GetId() << " entrou na zona AMARELA!\n";
            } else if (depois == VERMELHO) {
                cout << "[ALERTA GESTOR] Motor ID " << m->GetId() << " atingiu zona VERMELHA -> STOP!\n";
            }
        }
    }

    AtualizarSensores();
    SimularAvarias();

    list<Motor*> desligados;
    Aviso_Humidade(desligados);
    Aviso_Fumo(desligados, "videos/aviso_fumo.mp4");
    Aviso_Luz("videos/aviso_luz.mp4");
    Aviso_Missel("videos/aviso_missel.mp4", "Estado.txt");

    AvancarRelogio(10);
}

/**
 * @brief Realiza manutenção preventiva e corretiva em todos os equipamentos.
 * @return true se a manutenção foi realizada; false por falta de permissão.
 */
bool Fabrica::Manutencao() {
    if (userAtual != NULL && !userAtual->PodeManutencao()) {
        cout << "Permissao negada para manutencao.\n";
        return false;
    }
    for (list<Motor*>::iterator it = motores.begin(); it != motores.end(); ++it) {
        Motor* m = *it;
        m->Reparar();
        m->STOP();
        string strV = defVerde[m->GetTipo()];
        double minV = 0.0, maxV = m->GetTemperaturaAviso() - 1.0;
        if (!strV.empty()) {
            ExtrairParDouble(strV, minV, maxV);
        }
        double amp = maxV - minV;
        if (amp <= 0) amp = 10.0;
        double novaTemp = minV + ((rand() % 10001) / 10000.0) * amp;
        m->SetTemperaturaAtual(novaTemp);
    }
    for (list<Sensor*>::iterator it = sensores.begin(); it != sensores.end(); ++it) {
        (*it)->Reparar();
    }
    fabricaAtiva = true;
    cout << "[MANUTENCAO] Todos os motores e sensores foram reparados e a fabrica esta novamente ativa.\n";
    return true;
}

// ==================== RANKINGS ====================

/**
 * @brief Gera o ranking das marcas ordenadas pelo maior número total de avarias.
 * @return Ponteiro para a lista ordenada de nomes de marcas.
 */
list<string> *Fabrica::Ranking_Dos_Fracos() {
    if (userAtual != NULL && !userAtual->PodeListar()) {
        cout << "Permissao negada: utilizador atual nao pode consultar rankings.\n";
        return NULL;
    }
    map<string, int> avariasPorMarca;
    for (list<Motor*>::iterator it = motores.begin(); it != motores.end(); ++it) {
        avariasPorMarca[(*it)->GetMarca()] += (*it)->GetTotalAvarias();
    }
    for (list<Sensor*>::iterator it = sensores.begin(); it != sensores.end(); ++it) {
        avariasPorMarca[(*it)->GetMarca()] += (*it)->GetTotalAvarias();
    }

    vector<pair<string, int> > vec(avariasPorMarca.begin(), avariasPorMarca.end());
    for (size_t i = 0; i < vec.size(); i++) {
        for (size_t j = i + 1; j < vec.size(); j++) {
            if (vec[j].second > vec[i].second) {
                pair<string, int> tmp = vec[i];
                vec[i] = vec[j];
                vec[j] = tmp;
            }
        }
    }

    list<string> *ranking = new list<string>();
    for (size_t i = 0; i < vec.size(); i++) {
        ranking->push_back(vec[i].first);
    }
    return ranking;
}

/**
 * @brief Gera o ranking dos motores com maior tempo de trabalho acumulado.
 * @return Ponteiro para a lista ordenada de ponteiros de motores.
 */
list<Motor*> *Fabrica::Ranking_Dos_Mais_Trabalhadores() {
    if (userAtual != NULL && !userAtual->PodeListar()) {
        cout << "Permissao negada: utilizador atual nao pode consultar rankings.\n";
        return NULL;
    }
    list<Motor*> *ranking = new list<Motor*>();

    for (list<Motor*>::iterator it = motores.begin(); it != motores.end(); ++it) {
        Motor* atual = *it;
        list<Motor*>::iterator pos = ranking->begin();
        while (pos != ranking->end() && (*pos)->GetTempoTrabalho() >= atual->GetTempoTrabalho()) {
            ++pos;
        }
        ranking->insert(pos, atual);
    }
    return ranking;
}

// ==================== RELATORIO XML E HTML ====================

/**
 * @brief Exporta o estado atual da fábrica para um ficheiro XML no formato oficial.
 * @param fich_xml Caminho/nome do ficheiro XML a criar.
 * @return true se gravou com sucesso; false caso contrário.
 */
void Fabrica::Relatorio(string fich_xml) {
    ofstream f(fich_xml.c_str());
    if (!f.is_open()) {
        cout << "Erro: Nao foi possivel escrever em " << fich_xml << " (fecha o ficheiro se estiver aberto!).\n";
        return;
    }

    f << "<DADOS>\n";
    f << "<DEFINICOES>\n";
    f << "<NOME_EMPRESA>" << nome << "</NOME_EMPRESA>\n";
    f << "<HORA_INICIO>" << horaAbertura << "</HORA_INICIO>\n";
    f << "<HORA_FECHO>" << horaFecho << "</HORA_FECHO>\n";
    f << "<VIZINHANCA_AVISO>" << vizinhancaAviso << "</VIZINHANCA_AVISO>\n";
    f << "<DIMENSAO_FABRICA>" << dimensaoX << "," << dimensaoY << "</DIMENSAO_FABRICA>\n";

    string tiposMotor[3] = { "MELETRICO", "MCOMBUSTAO", "MINDUCAO" };
    for (int i = 0; i < 3; i++) {
        string t = tiposMotor[i];
        if (!defVerde[t].empty()) {
            f << "<" << t << ">\n";
            f << "<VERDE>" << defVerde[t] << "</VERDE>\n";
            f << "<AMARELO>" << defAmarelo[t] << "</AMARELO>\n";
            f << "<VERMELHO>" << defVermelho[t] << "</VERMELHO>\n";
            f << "<PROB_AVARIA>" << defProbAvaria[t] << "</PROB_AVARIA>\n";
            f << "</" << t << ">\n";
        }
    }
    f << "</DEFINICOES>\n";

    f << "<MOTORES>\n";
    for (list<Motor*>::iterator it = motores.begin(); it != motores.end(); ++it) {
        Motor* m = *it;
        string est = "STOP";
        if (m->GetEstado() == ESTADO_MOTOR::START) est = "START";
        else if (m->GetEstado() == ESTADO_MOTOR::RUN) est = "RUN";
        else if (m->GetEstado() == ESTADO_MOTOR::AVARIADO) est = "AVARIADO";

        string cor = "VERDE";
        CLASSE_TEMPERATURA cl = m->ESTOU_QUENTE();
        if (cl == AMARELO) cor = "AMARELO";
        else if (cl == VERMELHO) cor = "VERMELHO";

        f << "<" << m->GetTipo() << ">\n";
        f << "<ID>" << m->GetId() << "</ID>\n";
        f << "<MARCA>" << m->GetMarca() << "</MARCA>\n";
        f << "<VALOR_TEMP_ATUAL>" << m->GetTemperaturaAtual() << "</VALOR_TEMP_ATUAL>\n";
        f << "<ESTADO_ATUAL>" << est << "</ESTADO_ATUAL>\n";
        f << "<ESTADO_COR>" << cor << "</ESTADO_COR>\n";
        f << "<CONSUMO_HORA>" << m->GetConsumoHora() << "</CONSUMO_HORA>\n";
        f << "<CONSUMO_DIA>" << m->GetConsumoDia() << "</CONSUMO_DIA>\n";
        f << "<TEMPO_TRABALHO>" << m->GetTempoTrabalho() << "</TEMPO_TRABALHO>\n";
        f << "<POSICAO>" << m->GetX() << "," << m->GetY() << "</POSICAO>\n";
        f << "</" << m->GetTipo() << ">\n";
    }
    f << "</MOTORES>\n";

    f << "<SENSORES>\n";
    for (list<Sensor*>::iterator it = sensores.begin(); it != sensores.end(); ++it) {
        Sensor* s = *it;
        string tagS = "S" + s->GetTipo();
        double probXml = s->GetProbabilidadeAvaria() * 100.0;

        f << "<" << tagS << ">\n";
        f << "<ID>" << s->GetId() << "</ID>\n";
        f << "<MARCA>" << s->GetMarca() << "</MARCA>\n";
        f << "<VALOR_AVISO>" << s->GetLimiar() << "</VALOR_AVISO>\n";
        f << "<POSICAO>" << s->GetX() << "," << s->GetY() << "</POSICAO>\n";
        f << "<PROB_AVARIA>" << probXml << "</PROB_AVARIA>\n";
        f << "<VALOR_ATUAL>" << s->GetValor();
        if (tagS == "SHUMIDADE") {
            f << "%";
        }
        f << "</VALOR_ATUAL>\n";
        f << "</" << tagS << ">\n";
    }
    f << "</SENSORES>\n";
    f << "</DADOS>\n";
    f.close();
}

/**
 * @brief Gera uma página HTML com o resumo do estado atual da fábrica.
 * @param fich_html Caminho/nome do ficheiro HTML a criar.
 */
void Fabrica::Listagem_HTML(string fich_html) {
    ofstream f(fich_html.c_str());
    if (!f.is_open()) return;

    f << "<!DOCTYPE html>\n<html><head><meta charset=\"UTF-8\"><title>" << nome << "</title></head><body>\n";
    f << "<h1>Estado Atual da Fabrica: " << nome << "</h1>\n";
    f << "<table border=\"1\" cellpadding=\"6\">\n";
    f << "<tr><th>Categoria</th><th>ID</th><th>Tipo</th><th>Marca</th><th>Posicao</th><th>Detalhe</th></tr>\n";

    for (list<Motor*>::iterator it = motores.begin(); it != motores.end(); ++it) {
        Motor* m = *it;
        f << "<tr><td>Motor</td><td>" << m->GetId() << "</td><td>" << m->GetTipo()
          << "</td><td>" << m->GetMarca() << "</td><td>(" << m->GetX() << "," << m->GetY() << ")</td>"
          << "<td>Temp: " << m->GetTemperaturaAtual() << "C | Consumo: " << m->GetConsumoDia() << "</td></tr>\n";
    }
    for (list<Sensor*>::iterator it = sensores.begin(); it != sensores.end(); ++it) {
        Sensor* s = *it;
        string textoAviso = "NAO";
        if (s->EmAviso()) {
            textoAviso = "SIM";
        }

        f << "<tr><td>Sensor</td><td>" << s->GetId() << "</td><td>" << s->GetTipo()
          << "</td><td>" << s->GetMarca() << "</td><td>(" << s->GetX() << "," << s->GetY() << ")</td>"
          << "<td>Valor: " << s->GetValor() << " | Aviso: " << textoAviso << "</td></tr>\n";
    }
    f << "</table>\n</body></html>\n";
    f.close();
}

// ==================== AVISOS DOS SENSORES ====================

/**
 * @brief Desliga os motores situados dentro do raio de vizinhança de sensores de humidade em aviso.
 * @param lm Lista onde serão guardados os motores desligados.
 * @return Número total de motores desligados.
 */
int Fabrica::Aviso_Humidade(list<Motor*> &lm) {
    lm.clear();
    int desligados = 0;

    for (list<Sensor*>::iterator it = sensores.begin(); it != sensores.end(); ++it) {
        Sensor* s = *it;
        if (s->GetTipo() == "HUMIDADE" && s->EmAviso()) {
            int raio = s->GetRaioVizinhanca();
            if (raio <= 0) raio = vizinhancaAviso;

            for (list<Motor*>::iterator mIt = motores.begin(); mIt != motores.end(); ++mIt) {
                Motor* m = *mIt;
                if (CalcularDistancia(s->GetX(), s->GetY(), m->GetX(), m->GetY()) <= raio) {
                    if (m->GetEstado() != ESTADO_MOTOR::STOP) {
                        m->STOP();
                        lm.push_back(m);
                        desligados++;
                    }
                }
            }
        }
    }
    if (desligados > 0) {
        cout << "[AVISO GESTOR] Humidade elevada! Motores desligados na vizinhanca: " << desligados << "\n";
    }
    return desligados;
}

/**
 * @brief Em caso de aviso de fumo/fogo, desliga todos os motores e gera o ficheiro de emergência.
 * @param lm Lista onde serão guardados os motores desligados.
 * @param fich_video Nome do ficheiro de vídeo/aviso a gerar.
 * @return Número total de motores desligados.
 */
int Fabrica::Aviso_Fumo(list<Motor*> &lm, string fich_video) {
    lm.clear();
    int desligados = 0;
    bool haFumo = false;

    for (list<Sensor*>::iterator it = sensores.begin(); it != sensores.end(); ++it) {
        if (((*it)->GetTipo() == "FUMO" || (*it)->GetTipo() == "FOGO") && (*it)->EmAviso()) {
            haFumo = true;
            break;
        }
    }

    if (haFumo) {
        for (list<Motor*>::iterator mIt = motores.begin(); mIt != motores.end(); ++mIt) {
            Motor* m = *mIt;
            if (m->GetEstado() != ESTADO_MOTOR::STOP) {
                m->STOP();
                lm.push_back(m);
                desligados++;
            }
        }
        ofstream f(fich_video.c_str());
        if (f.is_open()) {
            f << "ALERTA DE FUMO/FOGO - DIRIJA-SE AS SAIDAS DE EMERGENCIA!\n";
            f.close();
        }
        ExecutarVideo("videos/aviso_fumo.mp4");
        cout << "[AVISO GESTOR] Fumo/Fogo detetado! Motores desligados e video invocado (" << fich_video << ").\n";
    }
    return desligados;
}

/**
 * @brief Em caso de aviso de baixa luminosidade, gera o ficheiro de aviso de regresso a casa.
 * @param fich_video Nome do ficheiro de vídeo/aviso a gerar.
 * @return Número de sensores de luz em estado de aviso.
 */
int Fabrica::Aviso_Luz(string fich_video) {
    int avisos = 0;
    for (list<Sensor*>::iterator it = sensores.begin(); it != sensores.end(); ++it) {
        if ((*it)->GetTipo() == "LUZ" && (*it)->EmAviso()) {
            avisos++;
        }
    }
    if (avisos > 0) {
        ofstream f(fich_video.c_str());
        if (f.is_open()) {
            f << "BAIXA LUMINOSIDADE - BOM REGRESSO A CASA!\n";
            f.close();
        }
        ExecutarVideo("videos/aviso_luz.mp4");
        cout << "[AVISO GESTOR] Luz baixa! Video invocado (" << fich_video << ").\n";
    }
    return avisos;
}

/**
 * @brief Em caso de alerta de míssil, guarda o estado da fábrica, pára todos os motores e gera o aviso de abrigo.
 * @param fvideo Nome do ficheiro de vídeo/aviso de abrigos.
 * @param festado Nome do ficheiro de texto onde é guardado o estado atual da fábrica.
 */
void Fabrica::Aviso_Missel(string fvideo, string festado) {
    bool alertaMissel = false;
    for (list<Sensor*>::iterator it = sensores.begin(); it != sensores.end(); ++it) {
        if ((*it)->GetTipo() == "MISSEL" && (*it)->EmAviso()) {
            alertaMissel = true;
            break;
        }
    }

    if (alertaMissel) {
        ofstream fEst(festado.c_str());
        if (fEst.is_open()) {
            Listar(fEst);
            fEst.close();
        }
        for (list<Motor*>::iterator mIt = motores.begin(); mIt != motores.end(); ++mIt) {
            (*mIt)->STOP();
        }
        fabricaAtiva = false;
        ofstream fVid(fvideo.c_str());
        if (fVid.is_open()) {
            fVid << "ALERTA DE MISSEL - DIRIJA-SE IMEDIATAMENTE AOS ABRIGOS!\n";
            fVid.close();
        }
        ExecutarVideo("videos/aviso_missel.mp4");
        cout << "[ALERTA CRITICO] Missel detetado! Fabrica encerrada. Video invocado e estado guardado em: " << festado << "\n";
    }
}
