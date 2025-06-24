#define PINEMG_SUPINADOR 13  //pino AD do sinal do musculo supinador
#define PINEMG_PRONADOR 14   //pino AD do sinal do musculo pronador bicepis braquial
#define PINEMG_EXTENSOR 27   //pino AD do sinal do musculo extensor de dedos
#define PINEMG_FLEXOR 26     //pino AD do sinal do musculo flexor dos dedos

#define TAMANHO 10
#define TAMANHO_RMS 1000

char msg[200];

double data_supinador[TAMANHO] ={0};
double data_pronador[TAMANHO] ={0};
double data_extensor[TAMANHO] ={0};
double data_flexor[TAMANHO] ={0};


double data_RMS_supinador[TAMANHO_RMS] = {0};
double data_RMS_pronador[TAMANHO_RMS] = {0};
double data_RMS_extensor[TAMANHO_RMS] = {0};
double data_RMS_flexor[TAMANHO_RMS] = {0};

int valEMG_supinador = 0;
int valEMG_pronador = 0;
int valEMG_extensor = 0;
int valEMG_flexor = 0;

double RMS_supinador = 0;
double RMS_pronador = 0;
double RMS_extensor = 0;
double RMS_flexor = 0;

double media_supinador=188;
double media_pronador=52;
double media_extensor=313;
double media_flexor=132;

double desvio_supinador=194;
double desvio_pronador=88;
double desvio_extensor=356;
double desvio_flexor=163;

double normalize_supinador=0;
double normalize_pronador=0;
double normalize_extensor=0;
double normalize_flexor=0;

int status_supinador = 0;
int status_pronador = 0;
int status_extensor = 0;
int status_flexor = 0;

double lda_w_supinador = 1.0477;
double lda_w_pronador = 0.6696;
double lda_w_extensor = 0.7162;
double lda_w_flexor = 0.3622;

double lda_threshold_supinador = 0.026373;
double lda_threshold_pronador = -0.0029882;
double lda_threshold_extensor = -0.029389;
double lda_threshold_flexor = -0.058031;

bool dados_completos = false;
bool dados_completos_RMS = false;
int q=0;
int a=0;
int ciclo=0;

// Função para deslocar os valores e adicionar o valor novo a direita da matriz
void adicionarValor(double vetor[], int novoValor, int tamanho) {
    // Desloca todos os elementos para a esquerda
    for (int i = 0; i < tamanho - 1; i++) {
        vetor[i] = vetor[i + 1];
    }
    // Adiciona o novo valor na última posição
    vetor[tamanho - 1] = novoValor;
}

// Função para calcular o RMS da matriz
void calculaRMS(double vetor[], double *RMS){
  double potencia=0;
  for (int i=0;i<TAMANHO;i++){
    potencia = potencia + (vetor[i]*vetor[i]);
  }
  *RMS = sqrt(potencia*0.1);
}

// Função para calcular a média
double calcular_media(double *data, int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += data[i];
    }
    return sum / size;
}

// Função para calcular o desvio padrão
double calcular_desvio(double *data, int size, double mean) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += pow(data[i] - mean, 2);
    }
    return sqrt(sum / size);
}

// Função para normalizar um valor
double normalizaSinal(double value, double mean, double stddev) {
  if(stddev>0){
    return (value - mean) / stddev;
    }
  else if(stddev==0){
    return (value-mean);
  }
    
}

// Classificar uma nova amostra
int classeLDA(double normalized_value, double lda_w , double lda_threshold) {
    double projection = normalized_value * lda_w;
    return (projection > lda_threshold) ? 1 : 0; // 1: ativação, 0: repouso
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  //ler os canais analaogicos
  valEMG_supinador = analogRead(PINEMG_SUPINADOR);
  valEMG_pronador = analogRead(PINEMG_PRONADOR);
  valEMG_extensor = analogRead(PINEMG_EXTENSOR);
  valEMG_flexor = analogRead(PINEMG_FLEXOR);
  
  //preenche a matriz de dados
  adicionarValor(data_supinador,valEMG_supinador,TAMANHO);
  adicionarValor(data_pronador,valEMG_pronador,TAMANHO);
  adicionarValor(data_extensor,valEMG_extensor,TAMANHO);
  adicionarValor(data_flexor,valEMG_flexor,TAMANHO);
  
  //Ler e completar os dados na base de dados
  if(dados_completos == false){
    q=q+1;
    Serial.println("lendo os dados EMG...");
    if(q>=10){
      dados_completos = true;
      Serial.println("dados completos EMG");
    }
    return;
  }

  //calcula o RMS da matriz
  calculaRMS(data_supinador,&RMS_supinador);
  calculaRMS(data_pronador,&RMS_pronador);
  calculaRMS(data_extensor,&RMS_extensor);
  calculaRMS(data_flexor,&RMS_flexor);
  
  //adiciona os valores de RMS para a matirz RMS
  adicionarValor(data_RMS_supinador,RMS_supinador,TAMANHO_RMS);
  adicionarValor(data_RMS_pronador,RMS_pronador,TAMANHO_RMS);
  adicionarValor(data_RMS_extensor,RMS_extensor,TAMANHO_RMS);
  adicionarValor(data_RMS_flexor,RMS_flexor,TAMANHO_RMS);

  //carregar todos os dados do RMS
  if(dados_completos_RMS == false){
    a=a+1;
    Serial.println("lendo os dados EMG RMS ...");
    if(a >= TAMANHO_RMS){
      dados_completos_RMS = true;
      Serial.println("dados completos EMG RMS");

      media_supinador = calcular_media(data_RMS_supinador,TAMANHO_RMS);
      media_pronador = calcular_media(data_RMS_pronador,TAMANHO_RMS);
      media_extensor = calcular_media(data_RMS_extensor,TAMANHO_RMS);
      media_flexor = calcular_media(data_RMS_flexor,TAMANHO_RMS);

      desvio_supinador = calcular_desvio(data_RMS_supinador,TAMANHO_RMS,media_supinador);
      desvio_pronador = calcular_desvio(data_RMS_pronador,TAMANHO_RMS,media_pronador);
      desvio_extensor = calcular_desvio(data_RMS_extensor,TAMANHO_RMS,media_extensor);
      desvio_flexor = calcular_desvio(data_RMS_flexor,TAMANHO_RMS,media_flexor);
    }
    return;
  }
  
  normalize_supinador = normalizaSinal(RMS_supinador,media_supinador,desvio_supinador);
  normalize_pronador = normalizaSinal(RMS_pronador,media_pronador,desvio_pronador);
  normalize_extensor = normalizaSinal(RMS_extensor,media_extensor,desvio_extensor);
  normalize_flexor = normalizaSinal(RMS_flexor,media_flexor,desvio_flexor);

  status_supinador = classeLDA(normalize_supinador,lda_w_supinador,lda_threshold_supinador);
  status_pronador = classeLDA(normalize_pronador,lda_w_pronador,lda_threshold_pronador);
  status_extensor = classeLDA(normalize_extensor,lda_w_extensor,lda_threshold_extensor);
  status_flexor = classeLDA(normalize_flexor,lda_w_flexor,lda_threshold_flexor);

  sprintf(msg,"s:%f; p:%f; e:%f; f:%f; ds:%f; dp:%f; de:%f; df:%f; supinador:%d; pronador:%d; extensor:%d; flexor:%d;",normalize_supinador,normalize_pronador,normalize_extensor,normalize_flexor,desvio_supinador,desvio_pronador,desvio_extensor,desvio_flexor,status_supinador,status_pronador,status_extensor,status_flexor);
  //sprintf(msg,"s:%d; p:%d; e:%d; f:%d;",status_supinador,status_pronador,status_extensor,status_flexor);

  Serial.println(msg);
  
  if(ciclo >= TAMANHO_RMS){
      ciclo=0;

      media_supinador = calcular_media(data_RMS_supinador,TAMANHO_RMS);
      media_pronador = calcular_media(data_RMS_pronador,TAMANHO_RMS);
      media_extensor = calcular_media(data_RMS_extensor,TAMANHO_RMS);
      media_flexor = calcular_media(data_RMS_flexor,TAMANHO_RMS);

      desvio_supinador = calcular_desvio(data_RMS_supinador,TAMANHO_RMS,media_supinador);
      desvio_pronador = calcular_desvio(data_RMS_pronador,TAMANHO_RMS,media_pronador);
      desvio_extensor = calcular_desvio(data_RMS_extensor,TAMANHO_RMS,media_extensor);
      desvio_flexor = calcular_desvio(data_RMS_flexor,TAMANHO_RMS,media_flexor);      
  }
  
  ciclo = ciclo+1;
  delay(100);
}
