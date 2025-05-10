
/* 
 * Ver 4.2.96   -> Inserido controle de amplitude via PWM, pela porta serial configurada no pino RC6 - TX
 * Ver 4.2.95   -> HF alterado para Saida OUT3. OUT2 é Exclusiva para Pneumatico (¬¬)
 * Ver 4.2.94   -> Inserido Senha de Acesso ao Menu. (11/10/2023)
 *              -> Inserido funcao Solda continua por pulsos (01/11/2023)
 * Ver 4.2.93   -> Inserido controle de falha energy_low e time_OVL
 * Ver 4.2.92   -> Inserido controle Stop por Metal-Metal
 * Ver 4.2.91
 * Ver 4.2.9    -> Retirado os printf remanecentes no codigo
 * Ver 4.2.8    -> Corrigida falha que impede o inicio de solda ao pressionar EMERGENCIA
 * Ver 4.2.7    -> Contador tempo alterado para decimo de segundo (0,0s)
 * Ver 4.2.6    -> Correcao t1s_Solda_Continua
 * Ver 4.2.5    -> Inserido contador de tempo ativo por segundo 
 * Ver 4.2.4    -> Inserido possibilidade de escolha do modo de opercao via F1 ao ligar a maquina.
 * Ver 4.2.3    -> STATUS e ERRO insseridos em pistola e maquina (pinos 4 e 1 da tomada de 10 pinos)
 * Ver 4.2.2    -> Inserido protecao contra falha na correcao de freq. so corrigue com tempo de solda maior que 100ms
 * Ver 4.2.1    -> inserido status HF nas saidas dos reles
 * Ver 4.2.0    -> mostra total de ciclos historico na inicializacao
 * Ver 4.1.9    -> adaptacao para inversor novo. Reset antes de iniciar.
 * Ver 4.1.8    -> Adicionado lingua inglesa
 * Ver 4.1.8    -> Inserido ajuste de Frequencia manual para corrigir problemas com inicio de trabalho.
 * Ver 4.1.7    -> Melhoramentos na leitura da frequencia (28/02/2019).
 * Ver 4.1.6    -> Inserido modo de solda NORMAL, RAPIDO, CONTINUO e AUTOMATICO.
 * Ver 4.1.5    -> Corrigido frequencias limites: 16 (-350) 17(-200) 22(+400) 24(+500).
 * Ver 4.1.2    -> alteracao no teste pneumatico. Necessario bimanual para acionar. 04/09/2018.
 * ver 4.1.1    -> inserido escolha pistola / maquina via telas.
 *              -> Corrigido erro no calculo de temperarura. (estouro de int)
 */

#include "Historico_Ver4.h"
#include "config.h"
#include "func.h"
void main(void)
{
    const float t_amb = Temp_Atual;
    opc_maquinas tipo = Tipo_Maquina;
    opc_gerador gerador = Tipo_Gerador;
    idioma = IDIOMA;
/************************* Configuracao do Gerador ****************************/
    SYSTEM_Initialize();
    config_IO_start(); 
    ini_ee_tempos(t_amb,tipo);
    if(tipo == ESPECIAL){config_maq_especial();}
    else{config_maq(tipo_maquina);}
/******************************************************************************/
//    (freq(Hz) , potencia(W) , cap_referencia(nF) , tensao de pico na saida US)
    if(gerador == SOW280)
        ini_GERADOR(20000,2800,1000,1000);                        //Gerador 20KHz 2800W
    else if (gerador == SOW160)
        ini_GERADOR(20000,1600,1000,1000);                        //Gerador 20KHz 1600W
    else if(gerador == SOW100)
        ini_GERADOR(35000,1000,220,500);                          //Gerador 35KHz 1000W
    else if (gerador == SOW120)
        ini_GERADOR(35000,1200,220,1000);                         //Gerador 35KHz 1200W
    else if (gerador == SOWESP)
        ini_GERADOR(30000,1000,220,500);                         //Somente para testes
    else{
        while(1);
    }
/******************************************************************************/     
    cfg.solda_cont =    0;//nao usar
/******************************************************************************/
    status_gerador(STATUS_GERADOR,1);
    delay(100);
    totalCiclos();
    teste_leds();
    
#if RECONDICINADO
    Lcd_Clear();
    Lcd_White_Center_String(1,STR_RECOND_1);
    Lcd_White_Center_String(2,STR_RECOND_2);
    delay(200);
    Lcd_Clear();
    Lcd_White_Center_String(1,STR_RECOND_2);
    Lcd_White_Center_String(2,STR_RECOND_DATA);
    delay(200);
#endif
    
    Lcd_Clear();
    Lcd_White_Center_String(1,Nome_Cliente);
    Lcd_White_Center_String(2,Num_Serie);
    delay(100);
    
    duty_ini = get_duty();
        
    do{
        rst_inversor(); 
        delay(20);
    }while(ovl);
    
    if(ciclador){
        ver_entrada();
        if(flag_in.aciona1 || flag_in.aciona2){
            switch(ciclador){
                case 1:
                    ciclar();
                    break;
                case 2:
                    ciclarPneumatico();
                    break;
            }
        }
    } 
    ciclador = 0;
    set_freq(freq_inicial);
    tela_inicial();
    status_gerador(STATUS_GERADOR,0);
    
    liberar();
    while (1){
        ver_entrada();
        if(ovl){
            do{
                rst_inversor(); 
                delay(20);
            }while(ovl);
            trava_erro(erro_inv_str[idioma]);
        }
        else if(flag_in.emerg){
            status_gerador(STATUS_GERADOR,1);
            status_gerador(ERRO,1);
            emergencia();//trava gerador
            t_tela = 0;
            flag.inatividade = 0;
            liberar();
            status_gerador(STATUS_GERADOR,0);
            status_gerador(ERRO,0);
        }
        else if(flag.inicia && !flag.abortar && flag_in.liberado){
            status_gerador(STATUS_GERADOR,1);
            t10ms = 0;
//            printf("OP inicio t10ms=%u, Di=%u\n\r",t10ms,duty_ini);   
//            escreve_LCD(1,1,process_str[idioma]); 
//            escreve_LCD(2,1,aguarde_str[idioma]); 
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            flag.sussess = 1;
            do{
                op_desce(tmp_desce);
                op_solda(tmp_solda);
                op_fixar(tmp_fixar);
            }while(modo != NORMAL && statusSolda != PARADO);
            flag_in.liberado = 0;
            trata_dados();
            t_tela = 0;
            flag.inatividade = 0;
            liberar();
            status_gerador(STATUS_GERADOR,0);
//            printf("OP Fim t10ms=%u\n\r",t10ms);
        }
        else if(flag_in.liberado){
            char bt = teclado();
            char senha_OK = 0;
            if(bt == menu){
                status_gerador(STATUS_GERADOR,1);
                liberar_tecla(bt,0);
                senha_OK = Autenticar();
                if(senha_OK == 1)
                    rotina_tela();
                else if(senha_OK == 0){
                    //Erro de senha: Apenas mostra os valores! Nao edita
                    trava_erro(erro_senha_str[idioma]);
                }
                liberar();
                tela_inicial(); 
                status_gerador(STATUS_GERADOR,0);
            }
            else if(bt == enter){
                status_gerador(STATUS_GERADOR,1);
                func_enter();
                liberar();
                tela_inicial();
                status_gerador(STATUS_GERADOR,0);
                flag.inatividade = 0; 
                t_tela = 0;
            }
            else if(bt == rst){
                status_gerador(STATUS_GERADOR,1);
                func_rst();
                Inicia_LCD();
                liberar();
                tela_inicial();
                flag.inatividade = 0; 
                t_tela = 0;
                status_gerador(STATUS_GERADOR,0);
            }
            else if(bt == F1){
                status_gerador(STATUS_GERADOR,1);
                teste_pneumatico(bt);
                liberar();
                tela_inicial();
                flag.inatividade = 0; 
                t_tela = 0;
                status_gerador(STATUS_GERADOR,0);
            }
            else if(bt == F2){
                status_gerador(STATUS_GERADOR,1);
                teste_US(bt);
                liberar();
                tela_inicial();
                flag.inatividade = 0; 
                status_gerador(STATUS_GERADOR,0);
            }
            else if(!flag.inatividade){
                flag.inatividade = 1;
                tInatividade = t10ms_global;
                t5min = 0;
                for(char i = 0; i<4;i++){
                    t1s[i] = 0;
                }
            }
            else if (flag.inatividade){
                if(t10ms_global - tInatividade > 100){
                    tInatividade = t10ms_global;
                    t5min++;
                    for(char ii = 0; ii<4;ii++){
                        t1s[ii] += 1;
                    }
                    
                    if(t1s[0] >= 20){ //a cada 20seg
                        t1s[0] = 0;
                        led.HF = 1;
                        //alarme = 1;
                        if(ver_freq(get_freq(10)) > 0){
                            freq_atual = freq_inicial;
                        }
                        set_freq(freq_atual);
                        led.HF = 0;
                        //alarme = 0;
                    }
                    if(t1s[1] >= 2){ // a cada 2 segundos
                        t1s[1] = 0;
                        if (!troca_tela){
                            troca_tela = 1;
                            tela_inicial();
                        }
                        else{
                            troca_tela = 0;
                            tela_ultima_solda(); 
                        }
                    }
                    if(t5min >= 60*10){ //depois de 10 minutos reseta freqeuncia.
                        t5min = 0;
                        led.value = 0xFF;
                        alarme = 1;
                        freq_atual = freq_inicial;
                        set_freq(freq_atual);
                        delay(10);
                        led.value = 0;
                        alarme = 0;
                    }
                }
            }
        }
    }
}