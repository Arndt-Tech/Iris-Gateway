/* 
 *  GateIRIS 
 *  Autor: Daniel Gier Arndt
 *  
 *  
 * ---ENUMERAÇÕES DE ERROS---
 *    - '1' timeOUT de reconexão foi ativado, e reiniciou o módulo.
 *    - '2' 
 *
 *
 * 
 * Atualizações:
 * - Refatoração do código.
 * - Adição do display OLED.
 * - Adição da comunicação LoRa.
 * - LoRa está rodando no nucleo 0.
 * - Estação IRIS se comunicando via LoRa.
 * - Gateway está captando as informações da estação e enviando para o Firebase.
 * - Mudança na biblioteca bluetooth. Alterado para BLE.
 * - Bluetooth server criado para receber dados do clientAPP.
 * - Protocolos criados a partir do bluetooth para mais segurança na sincronização com o clietAPP.
 * - Função de reconectWifi criada e adicionada.
 * - Função de escrita de erros na EEPROM adicionada.
 * - 
 *
 *
 * \-----------------------------------------------------------------------------------------------------\
 * Afazeres:
 * 
 * GERAL:
 *    - Refatorar código da estação.------------------------------------------(FEITO)
 *    - Programar algoritimo para pegar o chipID de cada estação. 
 * 
 * FIREBASE:
 *    - Otimizar firebase e funções.
 *    - Escrever no firestore.
 *    - Criar um callback que retorna estado de conexão.
 *    - Na hora de adicionar os ID's no gateway, o módulo acessa o server e verifica a existência
 *    dos ID's.  
 *    
 * WIFI:
 *    - Criar Função de reconexão wifi.------------------------------------------(FEITO EM 20/07/2021)
 *    - Criar uma função de escanear o wifi. 
 *    Caso o ID wifi recebido pelo clientAPP não for encontrado no scan do gateway, ele retorna um erro.
 *    
 * 
 * REDE LORA:
 *    - Otimizar protocolo de envio para estação, via LoRa.
 *    - Melhorar sistema de recepção do gateway com a estação, via LoRa.
 *    
 * NÚCLEOS: 
 *    - Adicionar watchdogTimer em abos os nucleos de ambas estações.
 *    - Otimizar função LoRa no núcleo 0.
 *    - Realimentar watchdogTimer sem delay.
 *    - Resolver bugs.
 *    
 * EEPROM:
 *    - Otimizar função de verificação de dados na EEPROM.
 *    - Adicionar EEPROM externa.
 *    - Tornar endereços da EEPROM dinâmicos para escrita.
 *    - Adicionar endereços para guardar os ID's da última causa de reset.----(FEITO EM 20/07/2021)
 *    
 * BLUETOOTH:
 *    - Remodelar sistema bluetooth.------------------------------------------(FEITO EM 16/07/2021)
 *    - Estabilizar comunicação com clientAPP.--------------------------------(FEITO EM 16/07/2021)             
 * \-----------------------------------------------------------------------------------------------------\
 *
 *
 *
 * \-------------------------------------------------\
 * EEPROM:
 * - ADDR:   0 -  30 ---> Reservados para SSID WIFI.
 * - ADDR:  40 -  70 ---> Reservados para Senha WIFI.
 * - ADDR:  80 - 110 ---> Reservados para UserID.
 * - ADDR: 120 - 150 ---> Reservados para StationID.
 * - ADDR: 160 - 165 ---> Reservados para rstCause.
 * \-------------------------------------------------\
 *
 *
 * Anotações:
 *
 *
 * Notas sobre a LoRa:
 * \---------------------------------------\
 * Protocolo de envio LoRa
 * - Endereço do dispositivo local
 * - Linha de comunicação
 * - Tamanho dos dados
 * - Dados
 *\---------------------------------------\
 *
 *
 * ---TESTES REALIZADOS COM PROTOCOLO LORA---
 *  Pacotes enviados:357
 *  Erros:39 
 *  Tempo:30 min
 *  Taxa de erros: 10,92%
 *  Modificações: running firebase, com correção de tempo na IRIS.
 *
 *
 *  Pacotes enviados: 367
 *  Pacotes completos: 325
 *  Erros: 42
 *  Tempo:30 min
 *  Taxa de erros: 11,44%
 *  Modificações: running firebase, sem correção de tempo na IRIS.
 *
 *
 *  Pacotes enviados: 835
 *  Pacotes completos: 835
 *  Erros: 0
 *  Tempo:30 min
 *  Taxa de erros: 0%
 *  Modificações: Sem running firebase, sem correção de tempo na IRIS.(Rodando sozinho no nucleo 1)
 *  
 */
