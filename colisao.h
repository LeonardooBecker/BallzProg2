
int numAleatFloat(int inicio, int fim);

int paredeEsquerda(b_uni *apontaBola);

int paredeDireita(b_uni *apontaBola);

int paredeCima(b_uni *apontaBola);

int paredeBaixo(b_uni *apontaBola);

int colisaoBaixo(int j, int k, int linha[QDSALT][QDSLARG], b_uni *apontaBola);

int colisaoDireita(int j, int k, int linha[QDSALT][QDSLARG], b_uni *apontaBola);

int colisaoCima(int j, int k, int linha[QDSALT][QDSLARG], b_uni *apontaBola);

int colisaoEsquerda(int j, int k, int linha[QDSALT][QDSLARG], b_uni *apontaBola);

int verticeInferiorEsquerdo(int j, int k, int linha[QDSALT][QDSLARG], b_uni *apontaBola);

int verticeInferiorDireito(int j, int k, int linha[QDSALT][QDSLARG], b_uni *apontaBola);

int verticeSuperiorDireito(int j, int k, int linha[QDSALT][QDSLARG], b_uni *apontaBola);

int verticeSuperiorEsquerdo(int j, int k, int linha[QDSALT][QDSLARG], b_uni *apontaBola);

int distribuiColisao(ball *bola, int linha[QDSALT][QDSLARG], inicio *rodadaAtual, bool *done, int *adicionaBola);