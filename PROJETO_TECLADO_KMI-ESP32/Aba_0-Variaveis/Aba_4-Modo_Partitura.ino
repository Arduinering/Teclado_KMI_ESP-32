// Cabeçalhos das funções do modo paritura.
uint8_t CalcularEixoY(uint8_t botaoNumero, bool acidente);
void GravarEvento(uint16_t freq, uint8_t indiceBotao);

//Item que apaga a música criada no modo partitura.
void apagarMusica(){
  static bool piscada; //Variável que faz a tela piscar.

    //Printa os textos na tela que NÃO irão piscar.
    display.setTextSize(1);
    display.setCursor(0,0); //(X,Y)
    display.print("aperte a chave para:");
    display.setTextSize(2);

    //Printa os textos na tela que piscarão.
    if(piscada){
      display.setCursor(0,20); //(X,Y)
      display.print("!!APAGAR A");
      display.setCursor(25,40); //(X,Y)
      display.print("MUSICA!!");

      //Limpa a música ao apertar o botão
      if(!digitalRead(confirmar)){
        
        //For que limpa todos as posições do array e todas as variáveis da struct, além de limpar os dado relacionados a gravação das notas.
        for(uint16_t estadoApagado = 0; estadoApagado < totalEventos; estadoApagado++){
          partitura[estadoApagado].timestamp = 0;
          partitura[estadoApagado].frequencia = 0;
          partitura[estadoApagado].portaIndice = 0;
          partitura[estadoApagado].acidente = 0;
          notasSeguradas = 0;
          totalEventos = 0;
          notaAnterior = 0;
          gravando = 0;
        }
        //Som de confirmação.
        delay(1000);
        ledcWriteTone(buzzer, gerarNota(7,5));
        delay(100);
        ledcWriteTone(buzzer, gerarNota(0,5));
        delay(100);
        ledcWriteTone(buzzer, 0);
      }
    }
  delay(100);
  piscada = !piscada; //Faz a tela piscar.
}

//Grava os valores característicos da nota em sua struct, utiliza como parâmetro a frequência da nota e o índice do botão com base no array.
void GravarEvento(uint16_t frequencia, uint8_t indiceBotao) {

  //Analisa se a notas ainda não chegou ao limite e se o código está sendo gravado.
  if (totalEventos < maxEventos && gravando) {
    partitura[totalEventos].timestamp = millis() - inicioGravacao;
    partitura[totalEventos].frequencia = frequencia;

    // Determina se é Sustenido ou Bemol para o Eixo Y
      if(indiceBotao > notaAnterior){ //Se a nota pressionada agora for mais AGUDA que a anterior...
        partitura[totalEventos].acidente = 1; //Nota é Sustenida (a escala está subindo)
      }else 
      if(indiceBotao < notaAnterior){ //Se a nota pressionada agora for mais GRAVE que a anterior...
        partitura[totalEventos].acidente = 0; //Nota é Bemol (a escala está descendo)
      }else{
        if(indiceBotao > 0){ //Se a primeira nota tocada NÃO for Dó (índice = 0)
            partitura[totalEventos].acidente = partitura[totalEventos - 1].acidente; //Ver o valor de acidente da nota anterior
          }else{//Caso contrário...
            partitura[totalEventos].acidente = 1; //Deixar o valor sustenido para não haver erro de cálculo
          }
      }
      notaAnterior = indiceBotao; //define a nota que acabou de ser pressionada como anterior
    partitura[totalEventos].portaIndice = indiceBotao;
    totalEventos++;
  }
}


/*
Calcula a posição vertical das notas no Display com base no valor (índice) do botão dentro do array. 
Caso uma tecla preta seja pressionada, o Eixo Y deverá ser somado com o contrário do valor do acidente 
      EX: 
        Se a nota for Sutenida (1), o valor somado vai ser 0;
        Se a nota for Bemol (0), o valor somado vai ser 1;
*/
uint8_t CalcularEixoY(uint8_t botaoNumero, bool acidente) {
  uint8_t resultadoY;

  //Analisa se as teclas pretas NÃO estão pressionadas
  if (!(botaoNumero == 1 || botaoNumero == 3 || botaoNumero == 6 || botaoNumero == 8 || botaoNumero == 10)) {
    resultadoY = 42 - (conversor[botaoNumero] * 4); //Calculo que define a posição vertical da tecla branca
  } else {
    resultadoY = 42 - (conversor[(botaoNumero + (!acidente))] * 4); //Calculo que define a posição vertical da tecla preta
  }
  return resultadoY;
}

void modoPartitura() {

  //Gráficos base
  display.setTextSize(2);
  display.setCursor(10,0); //(X,Y)

  display.print(F("OITAVA: "));
  display.println(oitava);
  display.drawBitmap(0, 20, ImagemPartitura, 128, 40, WHITE);

  //Lógica de Debounce e para ver quantos botões estçao pressionados.

  oitava = map(AnalogReadFiltro(potenciometro), 0 , 4095, 3, 8); //Muda a oitava com base na leitura do potenciômetro.

  //Verifica se cada um dos botões com base no índice 'i'. Esse por sua vez vai sendo somado pelo comando for(){}.
  for (uint8_t i = 0; i < numBotoes; i++) {
    bool leituraAtual = !digitalRead(pinos[i]); //Lê o botão.

    //Se o botão alterar o seu estado, garvar a última mudança.
    if (leituraAtual != botoes[i].antEst) {
      botoes[i].tempoUltimaMudanca = millis();
    }

    //Se o tempo desde que o botão alterou o seu estado for maior do que o tempo de Debounce.
    if ((millis() - botoes[i].tempoUltimaMudanca) > tempoDebounce) {

      //Se a leitura atual (já estabilizado) do botão for diferente do estado anterior do botão.
      if (leituraAtual != botoes[i].estadoEstabilizado) {
        botoes[i].estadoEstabilizado = leituraAtual;

        //Verifica se o botão foi pressionado e se a gravação ainda não começou.
        if (!gravando && botoes[i].estadoEstabilizado == HIGH) {
          gravando = true;
          inicioGravacao = millis();
          Serial.println("Gravação Iniciada!");
        }

        //Verifica se o botão foi pressionado.
        if (botoes[i].estadoEstabilizado == HIGH) {
          notasSeguradas++;                       //Aumenta o número de teclas pressionadas ao mesmo tempo.
          uint16_t freq = gerarNota(i, oitava);   //Gera a frequência da nota em específico.
          ledcWriteTone(buzzer, freq);            //Reproduz o som pelo Alto falante.
          GravarEvento(freq, i);                  //Grava a frequência no array das "partitura".
        } 
        else {
          //Se a tecla foi solta.
          notasSeguradas--; //Diminui o número de teclas pressionadas ao mesmo tempo.
          
          if (notasSeguradas > 0) {

            //Verifica se as teclas ainda estão pressionadas.
            for (int j = numBotoes - 1; j >= 0; j--) {

              //Se o botão estiver sendo pressionado, guarda o evento e reproduz a nota.
              if (botoes[j].estadoEstabilizado == HIGH) {
                uint16_t f_restante = gerarNota(j, oitava);
                ledcWriteTone(buzzer, f_restante);
                GravarEvento(f_restante, j);
                break;
              }
            }
          } else {
            //Se nenhuma tecla estiver sendo pressionada.
            ledcWriteTone(buzzer, 0); //Faz o alto-falante ficar em silêncio.
            GravarEvento(0, i); //Grava o intervalo.
          }
        }
      }
    }
    botoes[i].antEst = leituraAtual; //Deixa o estado atual como anterior.
  }

  // --- LÓGICA DE PRINTAR NO DISPLAY OLED ---

  //Se o número de otas gravadas for maior do que zero.
  if(totalEventos > 0){
    uint16_t posicaoNotas_TempoReal = 0; //guarda as notas que já foram gravadas.

    ///Faz a leitura das notas que já foram gravadas (ignorando os intervalos).
    for(uint16_t p = 0; p < totalEventos; p++){
      if(partitura[p].frequencia != 0){

        uint8_t referencialInicialX; //Muda dependendo se estamos no ínicio da partitura ou não.

        uint8_t indiceVisual = posicaoNotas_TempoReal % 4; //O resto da divisão sempre vai dar 0, 1, 2 e 3... voltando ao zero sempre que o número for múltiplo de 4.

        // Se for a primeira nota de um novo grupo de 4, limpa a área da partitura.
        if(indiceVisual == 0) {
            // Desenha um retângulo preto para "apagar" as notas anteriores antes de desenhar as novas.
            display.fillRect(0, 18, 128, 80, BLACK); 
            
            // Se ainda estiver no início (4 primeiras notas), usa o gráfico "ImagemPartitura" (representanod o início da partitura).
            if(posicaoNotas_TempoReal < 4){
              display.drawBitmap(0, 20, ImagemPartitura, 128, 40, WHITE);
              referencialInicialX = 42;
            }else{
              display.drawBitmap(0, 20, Partitura_Meio, 128, 40, WHITE);
              referencialInicialX = 25;
            }
        }

        //Calcula os eixos da nota.
        uint8_t eixoY = CalcularEixoY(partitura[p].portaIndice, partitura[p].acidente);
        uint8_t eixoX = referencialInicialX + (20 * indiceVisual);

        //Printa a nota na tela.
        display.drawBitmap(eixoX, eixoY, Minima, 8, 25, WHITE);

        //Printa o acidente (bemol(b) ou sustenido(#)) na tela caso a tecla pressionada seja preta.
        if(partitura[p].portaIndice == 1 || partitura[p].portaIndice == 3 || partitura[p].portaIndice == 6 || partitura[p].portaIndice == 8 || partitura[p].portaIndice == 10){
          if(partitura[p].acidente == 1){ //Se o acidente for 1, a nota é Sustenida (#).
            display.drawBitmap(eixoX + 10, eixoY + 14, Sustenido, 6, 11, WHITE);
          }else
          if(partitura[p].acidente == 0){ //Se o acidente for 0, a nota é Bemol (b).
            display.drawBitmap(eixoX + 10, eixoY + 14, Bemol, 5, 12, WHITE);
          }
        }

        posicaoNotas_TempoReal++; //Passa para a próxima nota.
      }
    }
  }

  // --- LÓGICA DE REPRODUÇÃO (PLAYBACK) ---

  //Se o botão for pressionado e for registrado pelo menos um evento
  if (!digitalRead(confirmar) && totalEventos > 0) {

    display.clearDisplay();

    uint32_t tempoZero = millis();      //Tempo inicial referencial
    uint32_t posicaoAtual = 0;          //Posição das notas e intervalos reproduzidas no PlayBack.
    uint16_t posicaoNotas_PlayBack = 0; //Posição das notas reproduzidas no PlayBack.

    while (posicaoAtual < totalEventos) {
      uint32_t tempoAtual = millis() - tempoZero; //Tempo que irá correr e será analisado conforme o "timestamp" marcado na Struct de cada nota e intervalo.

      //Gráficos, printa PlayBack no Display.
      display.setTextSize(2);
      display.setCursor(0,0); //(X,Y)
      display.print(F("Play-bacK!"));

      //Analisa se o tempo atual, ou seja, o millis() é maior ou igual do que o tempo de acontecimentos marcado no array partitura, reproduzindo o som.
      if (tempoAtual >= partitura[posicaoAtual].timestamp) {
        ledcWriteTone(buzzer, partitura[posicaoAtual].frequencia); //Faz o buzzer reproduzir o som guardado no array "partitura".

         //Analisa se a frequência é diferente de zero, assim printando a nota no display.
        if(partitura[posicaoAtual].frequencia != 0){

          uint8_t referencialInicialX;  //Muda dependendo se estamos no ínicio da partitura ou não.

          uint8_t indiceVisual = posicaoNotas_PlayBack % 4; //O resto da divisão sempre vai dar 0, 1, 2 e 3... voltando ao zero sempre que o número for múltiplo de 4.

          // Se for a primeira nota de um novo grupo de 4, limpa a área da partitura
          if(indiceVisual == 0) {
              // Desenha um retângulo preto para "apagar" as notas anteriores antes de desenhar as novas
              display.fillRect(0, 18, 128, 80, BLACK); 
              
              // Se ainda estiver no início (4 primeiras notas), usa o gráfico "ImagemPartitura" (representanod o início da partitura).
              if(posicaoNotas_PlayBack < 4){
                display.drawBitmap(0, 20, ImagemPartitura, 128, 40, WHITE);
                referencialInicialX = 42;
              }else{
                display.drawBitmap(0, 20, Partitura_Meio, 128, 40, WHITE);
                referencialInicialX = 25;
              }
          }

          //Calcula os eixos da nota
          uint8_t eixoY = CalcularEixoY(partitura[posicaoAtual].portaIndice, partitura[posicaoAtual].acidente);
          uint8_t eixoX = referencialInicialX + (20 * indiceVisual);

          //Printa a nota na tela.
          display.drawBitmap(eixoX, eixoY, Minima, 8, 25, WHITE);

          //Printa o acidente (bemol(b) ou sustenido(#)) na tela caso a tecla pressionada seja preta.
          if(partitura[posicaoAtual].portaIndice == 1 || partitura[posicaoAtual].portaIndice == 3 || partitura[posicaoAtual].portaIndice == 6 || partitura[posicaoAtual].portaIndice == 8 || partitura[posicaoAtual].portaIndice == 10){
            if(partitura[posicaoAtual].acidente == 1){ //Se o acidente for 1, a nota é Sustenida (#).
              display.drawBitmap(eixoX + 10, eixoY + 14, Sustenido, 6, 11, WHITE);
            }else
            if(partitura[posicaoAtual].acidente == 0){ //Se o acidente for 0, a nota é Bemol (b).
              display.drawBitmap(eixoX + 10, eixoY + 14, Bemol, 5, 12, WHITE);
            }
          }
          display.display();

          posicaoNotas_PlayBack++; //Passa para a próxima nota.
        }

        posicaoAtual++; //Passa para a próxima nota ou intervalo.
      }
      
      /*
      Checagem de segurança para não travar o ESP32 (Watchdog), permite que o ESP32 procese códigos pesados e que ele reinicie a placa caso o código 
      exceda o limite do microcontrolador.
      */
      yield(); 
    }
    ledcWriteTone(buzzer, 0); //Deixa o Buzzer em silêncio após reproduzir toda a música.
  }
}