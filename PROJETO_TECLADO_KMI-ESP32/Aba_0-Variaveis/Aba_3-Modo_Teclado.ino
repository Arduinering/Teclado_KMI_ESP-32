//Aba que contém as funções relacionadas ao modo Teclado.

// Faz a leitura das teclas.
void modoTeclado(){
  //Gráficos base
  display.setTextSize(2);
  display.setCursor(10,0); //(X,Y)

  //Printa a oitava e o piano no Display
  display.print(F("OITAVA: "));
  display.println(oitava);
  display.drawBitmap(0, 30, Piano, 73, 33, WHITE);

  //Altera a oitava com base no valor do potenciômetro
  oitava = map(AnalogReadFiltro(potenciometro), 0 , 4095, 3, 8);

  //Muda a tecla para ter ou não modulação usando o botão de confirmação.
  if(!digitalRead(confirmar)){
    modulation = !modulation;
    delay(250);
  }

  display.setCursor(80,35); //(X,Y)

  /*
  Muda o tamanho do nome da nota, a fonte fica maior se ela estiver no Sistema Anglo-Saxônico (composto pelas letras do alfabeto) e 
  fica menor se estiver no Sistema Latino (composto por neumas).
  */
  if(nomeNota == 0){
    display.setTextSize(3);
  }else
  if(nomeNota == 1){
    display.setTextSize(2);
  }

  /*
  Faz a leitura individual das 12 Teclas. Ele define a última nota pressionada e, com base na análise da nota anterior, se a progressão da música é 
  crescente (da nota mais grave a mais aguda) ou decrescente (da nota mais aguda a mais grave), além de printar no display um círculo na tecla que esta 
  sendo pressionada.
  */
  if(!digitalRead(pinos[0])){
    if(notaAnterior > 0){
      ordem = 0;
    }
    display.fillCircle(6, 55, 2, SSD1306_BLACK);

    //Verifica se a nota deverá ter ou não vibração
    if(modulation){
      toneModulation(buzzer, gerarNota(0, (oitava - 1)), gerarNota(0, oitava));
    }else{
      ledcWriteTone(buzzer, gerarNota(0, oitava));
    }
    notaAnterior = 0;
  }
  if(!digitalRead(pinos[1])){
    if(notaAnterior < 1){
        ordem = 1;
    }else
    if(notaAnterior > 1){
      ordem = 0;
    }

    display.fillCircle(11, 42, 2, SSD1306_WHITE);

    //Verifica se a nota deverá ter ou não vibração
    if(modulation){
      toneModulation(buzzer, gerarNota(1, (oitava - 1)), gerarNota(1, oitava));
    }else{
      ledcWriteTone(buzzer, gerarNota(1, oitava));
    }
    notaAnterior = 1;
  }
  if(!digitalRead(pinos[2])){

      display.fillCircle(16, 55, 2, SSD1306_BLACK);

    //Verifica se a nota deverá ter ou não vibração
    if(modulation){
      toneModulation(buzzer, gerarNota(2, (oitava - 1)), gerarNota(2, oitava));
    }else{
      ledcWriteTone(buzzer, gerarNota(2, oitava));
    }
    notaAnterior = 2;
  }
  if(!digitalRead(pinos[3])){
    if(notaAnterior < 3){
        ordem = 1;
      }else
      if(notaAnterior > 3){
        ordem = 0;
      }
      display.fillCircle(21, 42, 2, SSD1306_WHITE);

    //Verifica se a nota deverá ter ou não vibração
    if(modulation){
      toneModulation(buzzer, gerarNota(3, (oitava - 1)), gerarNota(3, oitava));
    }else{
      ledcWriteTone(buzzer, gerarNota(3, oitava));
    }
    notaAnterior = 3;
  }
  if(!digitalRead(pinos[4])){

    display.fillCircle(26, 55, 2, SSD1306_BLACK);
  
    //Verifica se a nota deverá ter ou não vibração
    if(modulation){
      toneModulation(buzzer, gerarNota(4, (oitava - 1)), gerarNota(4, oitava));
    }else{
      ledcWriteTone(buzzer, gerarNota(4, oitava));
    }
    notaAnterior = 4;
  }
  if(!digitalRead(pinos[5])){
    //Printa a nota 'F' ("Fá") no modo piano
      display.fillCircle(36, 55, 2, SSD1306_BLACK);

    //Verifica se a nota deverá ter ou não vibração
    if(modulation){
      toneModulation(buzzer, gerarNota(5, (oitava - 1)), gerarNota(5, oitava));
    }else{
      ledcWriteTone(buzzer, gerarNota(5, oitava));
    }
    notaAnterior = 5;
  }
  if(!digitalRead(pinos[6])){
    if(notaAnterior < 6){
        ordem = 1;
      }else
      if(notaAnterior > 6){
        ordem = 0;
      }
      display.fillCircle(41, 42, 2, SSD1306_WHITE);

    //Verifica se a nota deverá ter ou não vibração
    if(modulation){
      toneModulation(buzzer, gerarNota(6, (oitava - 1)), gerarNota(6, oitava));
    }else{
      ledcWriteTone(buzzer, gerarNota(6, oitava));
    }
    notaAnterior = 6;
  }
  if(!digitalRead(pinos[7])){

      display.fillCircle(46, 55, 2, SSD1306_BLACK);

    //Verifica se a nota deverá ter ou não vibração
    if(modulation){
      toneModulation(buzzer, gerarNota(7, (oitava - 1)), gerarNota(7, oitava));
    }else{
      ledcWriteTone(buzzer, gerarNota(7, oitava));
    }
    notaAnterior = 7;
  }
  if(!digitalRead(pinos[8])){
    if(notaAnterior < 8){
        ordem = 1;
      }else
      if(notaAnterior > 8){
        ordem = 0;
      }
      display.fillCircle(51, 42, 2, SSD1306_WHITE);

    //Verifica se a nota deverá ter ou não vibração
    if(modulation){
      toneModulation(buzzer, gerarNota(8, (oitava - 1)), gerarNota(8, oitava));
    }else{
      ledcWriteTone(buzzer, gerarNota(8, oitava));
    }
    notaAnterior = 8;
  }
  if(!digitalRead(pinos[9])){
    //Printa a nota 'A' ("Lá") no modo piano
      display.fillCircle(56, 55, 2, SSD1306_BLACK);

    //Verifica se a nota deverá ter ou não vibração
    if(modulation){
      toneModulation(buzzer, gerarNota(9, (oitava - 1)), gerarNota(9, oitava));
    }else{
      ledcWriteTone(buzzer, gerarNota(9, oitava));
    }
    notaAnterior = 9;
  }
  if(!digitalRead(pinos[10])){
    if(notaAnterior < 10){
        ordem = 1;
      }else
      if(notaAnterior > 10){
        ordem = 0;
      }
      display.fillCircle(61, 42, 2, SSD1306_WHITE);

    //Verifica se a nota deverá ter ou não vibração
    if(modulation){
      toneModulation(buzzer, gerarNota(10, oitava -1), gerarNota(10, oitava));
    }else{
      ledcWriteTone(buzzer, gerarNota(10, oitava));
    }
    notaAnterior = 10;
  }
  if(!digitalRead(pinos[11])){

      display.fillCircle(66, 55, 2, SSD1306_BLACK);

    //Verifica se a nota deverá ter ou não vibração
    if(modulation){
      toneModulation(buzzer, gerarNota(11, oitava - 1), gerarNota(11, oitava));
    }else{
      ledcWriteTone(buzzer, gerarNota(11, oitava));
    }
    notaAnterior = 11;
  }
  // Analisa se todas as teclas não estão pressionadas e desliga o buzzer
  if(digitalRead(pinos[0]) && digitalRead(pinos[1]) && digitalRead(pinos[2]) && digitalRead(pinos[3]) && digitalRead(pinos[4]) && digitalRead(pinos[5]) && digitalRead(pinos[6]) && digitalRead(pinos[7]) && digitalRead(pinos[8]) && digitalRead(pinos[9]) && digitalRead(pinos[10]) && digitalRead(pinos[11])){
    ledcWriteTone(buzzer, 0);
  }else{
    //Analisa se as teclas pretas NÃO estão pressionadas para printar o nome das teclas brancas. Dependendo da variável nomeNota, o código pode usar a nomeclatura Latina ou Anglo-Saxônica
    if (!(notaAnterior == 1 || notaAnterior == 3 || notaAnterior == 6 || notaAnterior == 8 || notaAnterior == 10)) {
      if(nomeNota){
        display.print(notasLatin[conversor[notaAnterior]]);
      }else{
        display.print(notasSaxao[conversor[notaAnterior]]);
      }
    }else{ //Caso contrário, ele printa a tecla preta e analisa sua posição e tipo de acidente (Bemol ou Sustenido) com base na variável ordem.
      if(nomeNota){
        display.print(notasLatin[conversor[notaAnterior + !ordem]]);
      }else{
        display.print(notasSaxao[conversor[notaAnterior + !ordem]]);
      }
      if(ordem){
        display.println("#");
      }else{
        display.println("b");
      }
    }
  }
}