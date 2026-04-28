void setup() {
  Serial.begin(9600); // Começa o monitor Serial.

  //Detecta se o display não está conectado adequadamente, caso isso ocorra, o Pojeto não iniciará.
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 falhou ao iniciar");
    while(true);
  }
  
  //Defina as portas das teclas definida no array "pinos[]" como saída.
  for(byte y = 0; y < numBotoes; y++){
    pinMode(pinos[y], INPUT_PULLUP);

    //Define os valores (struct) de todos os botões como 0 (já que o código está sendo inicializado).
    botoes[y].estadoEstabilizado = 0;
    botoes[y].antEst = 0;
    botoes[y].tempoUltimaMudanca = 0;
  }

  pinMode(retroceder, INPUT_PULLUP);
  pinMode(confirmar, INPUT_PULLUP);
  pinMode(avancar, INPUT_PULLUP);

  //Define o Buzzer como uma porta de saída para áudio usando PWM INTERNO do ESP32.
  ledcAttach(buzzer, 2000, 8);

  display.clearDisplay();// Limpa o display antes de começar a rodar o programa.
  display.setTextColor(SSD1306_WHITE); //Define a cor do texto como branco.

  /*
  roda a animação de introdução dependendo da variável booleana "rodarIntro". Você pode pular essa introdução colocando o valor da variável como'0'. 
  Recomendado para testes de Debug.
  */
  if(rodarIntro){
    intro();
  }
}
void loop() {

  //O while() cria uma repetição enquanto o código roda
  while(aba == 0){
    //Animação da tela de título
    display.clearDisplay();
    display.drawBitmap(0, 0, title_1, 128, 64, WHITE);
    display.display();
    
    //Muda a aba ao apertar o botão do Encoder
    if(!digitalRead(confirmar)){
      itens = 0;
      aba++;
      ledcWriteTone(buzzer, gerarNota(7,5));
      delay(100);
      ledcWriteTone(buzzer, gerarNota(0,5));
      delay(100);
      ledcWriteTone(buzzer, 0);
    }
  }

    //Aba do Menu, contém os itens principais do menu e, ao clicar no botão, leva para as outras abas.
  while(aba == 1){
    notaAnterior = 0;
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0); //(X,Y)
    display.print("-MENU:");

    display.setTextSize(1);
    display.setCursor(0,16); //(X,Y)
    if(itens == 0){//leva para a aba 2
      display.print("-> teclado");
    }else{
      display.print("teclado");
    }
    display.setCursor(0,26); //(X,Y)
    if(itens == 1){//leva para a aba 3
      display.print("-> partitura");
    }else{
      display.print("partitura");
    }
    display.setCursor(0,36); //(X,Y)
    if(itens == 2){//leva para a aba 3
      display.print("-> links");
    }else{
      display.print("links");
    }
    display.setCursor(0,46); //(X,Y)
    if(itens == 3){//leva para a aba 3
      display.print("-> sistema de notas");
    }else{
      display.print("sistema de notas");
    }
    display.setCursor(0,56); //(X,Y)
    if(itens == itensMax){//volta para a aba 0
      display.print("-> voltar");
    }else{
      display.print("voltar");
    }
    display.display();
    
    //Muda a aba ao apertar o botão do Encoder
    if(!digitalRead(confirmar)){
      if(itens != itensMax){
        aba = itens + 2;
      }else
      if(itens == itensMax){
        aba = 0;
      }

      itens = 0;
      ledcWriteTone(buzzer, gerarNota(7,5));
      delay(100);
      ledcWriteTone(buzzer, gerarNota(0,5));
      delay(100);
      ledcWriteTone(buzzer, 0);
    }
    itens = selecionar(itens, 0, itensMax);
  }

  // Gera os gráficos e lê as teclas do piano no modo teclado
  while(aba == 2){
    display.clearDisplay();
    if(itens == 0){
      modoTeclado();
    }else
    if(itens == 1){
      displayVoltar(aba - 2);
    }
    itens = selecionar(itens, 0, 1);
    display.display();
  }

  // Gera os gráficos e lê as teclas do piano no modo partitura
  while(aba == 3){
    display.clearDisplay();
    if(itens == 0){
      modoPartitura();
    }else
    if(itens == 1){
      apagarMusica();
    }else
    if(itens == 2){
      displayVoltar(aba - 2);
    }
    itens = selecionar(itens, 0, 2);
    display.display();
  }

  // itens com os respectivos links
  while(aba == 4){
    display.clearDisplay();
    if(itens == 0){
      //LINK DAS REVISTAS: https://www.newtoncbraga.com.br/mecatronicajovem
      display.setTextSize(2);
      display.setCursor(0,0); //(X,Y)
      display.print("REVISTAS:");
      display.setTextSize(1);
      display.setCursor(0,16); //(X,Y)
      display.print("https://www.");
      display.setCursor(0,26); //(X,Y)
      display.print("newtoncbraga.com.br");
      display.setCursor(0,36); //(X,Y)
      display.print("/mecatronicajovem");
    }else
    if(itens == 1){
      //LINK DO INSTAGRAM: https://www.instagram.com/mecjovem_oficial/
      display.setTextSize(2);
      display.setCursor(0,0); //(X,Y)
      display.print("INSTAGRAM:");
      display.setTextSize(1);
      display.setCursor(0,16); //(X,Y)
      display.print("https://www.");
      display.setCursor(0,26); //(X,Y)
      display.print("instagram.com/");
      display.setCursor(0,36); //(X,Y)
      display.print("mecjovem_oficial/");
    }else
    if(itens == 2){
      //LINK DA TWITCH: https://www.twitch.tv/mecatronica_jovem
      display.setTextSize(2);
      display.setCursor(0,0); //(X,Y)
      display.print("TWITCH:");
      display.setTextSize(1);
      display.setCursor(0,16); //(X,Y)
      display.print("https://www.");
      display.setCursor(0,26); //(X,Y)
      display.print("twitch.tv");
      display.setCursor(0,36); //(X,Y)
      display.print("/mecatronica_jovem");
    }else
    if(itens == 3){
      //LINK DO GRUPO DO DISCORD: https://discord.com/invite/sHmBawH6dT
      display.setTextSize(2);
      display.setCursor(0,0); //(X,Y)
      display.print("DISCORD:");
      display.setTextSize(1);
      display.setCursor(0,20); //(X,Y)
      display.print("https://discord.com");
      display.setCursor(0,30); //(X,Y)
      display.print("/invite/sHmBawH6dT");
    }else
    if(itens == 4){
      //LINK DO CANAL DO YOUTUBE DA INCB: https://www.youtube.com/@InstitutoNCBNewtonCBraga
      display.setTextSize(2);
      display.setCursor(0,0); //(X,Y)
      display.print("YOUTUBE:");
      display.setTextSize(1);
      display.setCursor(0,16); //(X,Y)
      display.print("https://www.youtube");
      display.setCursor(0,26); //(X,Y)
      display.print(".com/@InstitutoNCB");
      display.setCursor(0,36); //(X,Y)
      display.print("NewtonCBraga");
    }else
    if(itens == 5){
      //LINK DO CANAL DO YOUTUBE DA INCB: https://kitmodulariot.com.br
      display.setTextSize(2);
      display.setCursor(0,0); //(X,Y)
      display.print("Site KMI:");
      display.setTextSize(1);
      display.setCursor(0,30); //(X,Y)
      display.print("https://kitmodulariot");
      display.setCursor(0,40); //(X,Y)
      display.print(".com.br");
    }else
    if(itens == 6){
      displayVoltar(aba - 2);
    }
  display.display();
  itens = selecionar(itens, 0, 6);
  }

  // Alterna o sistema de notas (nomeclatura) entre Latino e Anglo-Saxônico
  while(aba == 5){
    display.clearDisplay();
    if(itens == 0){
      display.setTextSize(2);
      display.setCursor(20,0); //(X,Y)
      display.print("SISTEMA:");
      display.setTextSize(1);
      if(nomeNota == 0){
        display.setCursor(20,16); //(X,Y)
        display.print("Anglo-saxonico");
        display.setCursor(20,40); //(X,Y)
        display.print("C-D-E-F-G-A-B");
      }else
      if(nomeNota == 1){
        display.setCursor(45,16); //(X,Y)
        display.print("Latino");
        display.setCursor(0,40); //(X,Y)
        display.print("Do-Re-Mi-Fa-Sol-La-Si");
      }

      if(!digitalRead(confirmar)){
        nomeNota = !nomeNota;
        ledcWriteTone(buzzer, gerarNota(7,5));
        delay(100);
        ledcWriteTone(buzzer, gerarNota(0,5));
        delay(100);
        ledcWriteTone(buzzer, 0);
      }
    }else
    if(itens == 1){
      displayVoltar(aba - 2);
    }
    display.display();
    itens = selecionar(itens, 0, 1);
  }
}