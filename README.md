# IRIS Gateway 
IRIS Gateway é uma parte do projeto principal IRIS. Tem como principal função obter os dados enviados da estação via rede LoRa e envia-los ao servidor Firebase.

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
### [IRIS Gateway](https://github.com/SI-ART/GateIRIS).
### Desenvolvedor -> [Daniel Arndt](https://github.com/DanielArndt0).
-------------------------------------------------------------------------------
### [IRIS Station](https://github.com/I-grow/Station_IRIS).
### Desenvolvedor -> [Daniel Arndt](https://github.com/DanielArndt0).
-------------------------------------------------------------------------------


##  ATUALIZAÇÕES

- Adição do display OLED;

- Adição da comunicação LoRa;

- LoRa rodando no nucleo 0 do GateIRIS;

- Estação IRIS se comunicando via LoRa;

- Gateway está captando as informações da estação e enviando para o Firebase;

- Mudança na biblioteca bluetooth. Alterado para BLE;

- Bluetooth server criado para receber dados do clientAPP;

- Protocolos criados a partir do bluetooth para mais segurança na sincronização com o clientAPP;

- Função de reconexão wifi adicionada;

- Função de escrita de erros na EEPROM adicionada;

- Função de escrita de erros na EEPROM removida;

- Comunicação via LoRa estabelecida e estabilizada;

- Funções do firebase otimizadas e melhoradas;

- Protocolo de envio e recebimento LoRa refatorado;

- Núcleo 0 rodando tarefas LoRa;

- Núcleo 0 estabilizado;

- Interface de Display OLED desenvolvida;

- Timeout LoRa adicionado;

- Watchdog Timer rodando em ambos os núcleos;

- ID's de comunicação LoRa foram alterados para os chipID's;
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
## BLUETOOTH LOW ENERGY
A configuração inicial do *GateIRIS* é feita através do protocolo *Bluetooth Low Energy* (BLE), um protocolo de comunicação sem fio, caracterizado pelo seu baixo consumo de energia. Nesta etapa, o *GateIRIS* precisa das seguintes informações para ser conectado: 

- *SSID* da rede wifi;

- Senha da rede wifi;

- ID da conta do usuário;

- ID do *gateway* registrado na conta.

As informações acima são compartilhadas a partir da seguinte conexão: O *GateIRIS* cria um servidor bluetooth para possibilitar a comunicação com o cliente aplicativo *IRIS* (*clientAPP*).

-------------------------------------------------------------------------------
#### PROCEDIMENTOS DA COMUNICAÇÃO
Para uma comunicação estável, discreta e segura entre o *clientAPP* e o *GateIRIS*, foram desenvolvidos procedimentos que facilitam a instalação do *GateIRIS* para o usuário:

1. O processo inicia pela conexão entre o *clientAPP* e o servidor *BLE* do *GateIRIS*. Não havendo conexão, o *GateIRIS* permanece em espera.
- Função:
> waitingBT();            // Espera conexão com clientAPP

2. O *GateIRIS* recebe o *byte* de sincronização.
- Função:
> waitingSYNC();          // Espera sincronização do clientAPP

3. Uma requisição automática é enviada ao cliente para que este identifique se o dispositivo a ser conectado é ou não um *Gateway IRIS*. 
- Função:
> sendREQUEST();          // Envia requsição para o clientAPP

4. Quando o *clientAPP* receber a validação irá retornar uma requisição ao *GateIRIS*, para validar o cliente autorizado. Caso a validação do *GateIRIS* for inconsistente, o cliente será desconectado.
- Função:
> waitingREQUEST();       // Espera para receber o request do clientAPP

5. Com o sucesso das etapas, o *GateIRIS* irá esperar pelo envio das quatro crêdenciais necessárias.
- Função:
>  waitingSSID_WIFI();     // Espera clientAPP enviar SSID da rede
>  
>  waitingPASSWORD_WIFI(); // Espera clientAPP enviar PASSWORD da rede


6. O *GateIRIS* estará configurado para o funcionamento.

Segue um código de exemplo mostrando mais detalhes sobre os procedimentos de comunicação através do [Bluetooth Low Energy](https://github.com/SI-ART/GateIRIS/blob/main/Examples/BLE_GateIRIS/BLE_GateIRIS.ino).

-------------------------------------------------------------------------------
#### FLAGS DE COMUNICAÇÃO
Para que as funções apresentadas funcionem corretamente, foram definidas as seguintes *flags*:
|  FLAG | DESCRIÇÃO                                |
|:-----:|------------------------------------------|
|  *@*  | *Início da comunicação*                  |
|  */*  | *APP pode enviar o ID de usuário*        |
|  *~*  | *APP pode enviar o SSID da rede*         |
|  *^*  | *APP pode enviar a senha da rede*        |
|  *!*  | *Não foi possível conectar no WIFI*      |
|  *&*  | *IRIS foi conectada no WIFI com sucesso* |
> Tabela 1.

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
## EEPROM 

#### EEPROM ADDR's               	
Na tabela abaixo, é mostrado os endereços da memória EEPROM que foram particionados com a função de alocar os dados necessários para a conexão do *GateIRIS* com o servidor. Além disto, foram separados endereços para guardar a causa da última reinicialização.

|      | ADDR_MIN | ADDR_MAX | DESCRIÇÃO                   |
|:----:|----------|----------|-----------------------------|
| ADDR |   0      |  30      | Reservados para SSID WIFI.  |
| ADDR |  40      |  70      | Reservados para Senha WIFI. |
| ADDR |  80      | 110      | Reservados para User ID.    |
| ADDR | 120      | 150      | Reservados para Station ID. |
| ADDR | 160      | 165      | Reservados para reset Cause | 
> Tabela 2.


#### ENUMERAÇÃO DE ERROS
Abaixo, segue a enumeração das causas de reinicialização do GateIRIS:
| ERROS |                        DESCRIÇÃO                        |
|:-----:|:-------------------------------------------------------:|
|  '1'  | TimeOUT de reconexão foi ativado, e reiniciou o módulo. |
|  '2'  |                                                         |
> Tabela 3.

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
## REDE LORA

#### ESTRUTURA DE ENVIO LORA

|    |           DESCRIÇÃO           |
|:--:|:-----------------------------:|
| 1. | Endereço do dispositivo local.|
| 2. | Endereço do receptor.         |
| 3. | Tamanho dos dados.            |
| 4. | Dados.                        |
| 5. | Fim do pacote.                |
> Tabela 4.


#### TESTES COM REDE LORA

> Pacotes enviados: 357
> 
> Erros: 39
> 
> Tempo: 30 min
> 
> Taxa de erros: 10,92%
> 
> Modificações: running firebase, com correção de tempo na IRIS.

-------------------------------------------------------------------------------

> Pacotes enviados: 367
> 
> Pacotes completos: 325
> 
> Erros: 42
> 
> Tempo:30 min
> 
> Taxa de erros: 11,44%
> 
> Modificações: running firebase, sem correção de tempo na IRIS.

-------------------------------------------------------------------------------

> Pacotes enviados: 835
> 
> Pacotes completos: 835
> 
> Erros: 0
> 
> Tempo:30 min
> 
> Taxa de erros: 0%
> 
> Modificações: Sem running firebase, sem correção de tempo na IRIS.(Rodando sozinho no nucleo 1)

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
