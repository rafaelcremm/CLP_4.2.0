#include "func.h"
#include "Historico_Ver4.h"

void interrupt INTERRUPT_InterruptManager (void){
    
    if(PIE4bits.TMR5IE == 1 && PIR4bits.TMR5IF == 1){
        t1ms_ISR++; 
        
        if(t1ms_ISR >= t1ms_OP){
            stop_timer();
        } 
        
        if(cfg.metal){
            if(flag_in.metal){
                t1msMM_ISR++;
                if(t1msMM_ISR >= tmp_metal){
                    statusSolda = FIXANDO;
                    stop_timer();
                }
            }
            else{
                t1msMM_ISR = 0;
            }
        }
        
        TMR5_Reload();
        PIR4bits.TMR5IF = 0;
    } 
    if(PIE4bits.TMR1IE == 1 && PIR4bits.TMR1IF == 1){   //deve ser de 10ms
        static char duty_mod_pot;
        t10ms_global++;
        t10ms++;
        
        bar.value = 0;
        if(flag.US){ 
            amostras++;
            pot_inst = pontencia_instantanea(get_duty());
//            if(pot_inst > potencia) {
//                flag.isr_pot = 1;
//                erro_sobrecarga++;
//            }
//            else{
//                flag.isr_pot = 0;
//                erro_sobrecarga = 0;
//            }
            if(pot_inst > potencia){
                erro_sobrecarga++;
                if(erro_sobrecarga > 3)
                    flag.isr_pot = 1;
            }
            else if(erro_sobrecarga != 0){
                erro_sobrecarga--;
            }
            
            duty_mod_pot = duty/mod_pot;
            for(char i=0; i<duty_mod_pot; i++){
                bar.value |= (1<<i);
            }
            energia_total += pot_inst;                      //fracao de tempo 10ms, energia = dP*dt = pot_inst*10ms
        }
//**************************** escreve bargraph ********************************
        load_bar();               
        TMR1_Reload();
        PIR4bits.TMR1IF = 0;
    }
    else{
        if(PIE3bits.TXIE == 1 && PIR3bits.TXIF == 1){
            EUSART_TxDefaultInterruptHandler();
        } 
        if(PIE3bits.RCIE == 1 && PIR3bits.RCIF == 1){
            EUSART_RxDefaultInterruptHandler();
        } 
    }
}

void PIN_MANAGER_Initialize(void)
{
    /**
    LATx registers
    */
    LATE = 0x00;
    LATD = 0x00;
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;

    /**
    TRISx registers
    */
    TRISE = 0x00;
    TRISA = 0xFF;
    TRISB = 0x00;
    TRISC = 0x84;
    TRISD = 0xB8;

    /**
    ANSELx registers
    */
    ANSELD = 0x00;
    ANSELC = 0x00;
    ANSELB = 0x00;
    ANSELE = 0x00;
    ANSELA = 0x00;

    /**
    WPUx registers
    */
    WPUD = 0x00;//0x38;
    WPUE = 0x00;
    WPUB = 0x00;
    WPUA = 0x00;
    WPUC = 0x00;

    /**
    ODx registers
    */
    ODCONE = 0x00;
    ODCONA = 0x00;
    ODCONB = 0x00;
    ODCONC = 0x00;
    ODCOND = 0x00;

    /**
    IOCx registers 
    */
    //interrupt on change for group IOCAF - flag
    IOCAFbits.IOCAF1 = 0;
    //interrupt on change for group IOCAF - flag
    IOCAFbits.IOCAF2 = 0;
    //interrupt on change for group IOCAF - flag
    IOCAFbits.IOCAF3 = 0;
    //interrupt on change for group IOCAF - flag
    IOCAFbits.IOCAF4 = 0;
    //interrupt on change for group IOCAF - flag
    IOCAFbits.IOCAF5 = 0;
    //interrupt on change for group IOCAN - negative
    IOCANbits.IOCAN1 = 0;//1;
    //interrupt on change for group IOCAN - negative
    IOCANbits.IOCAN2 = 0;//1;
    //interrupt on change for group IOCAN - negative
    IOCANbits.IOCAN3 = 0;//1;
    //interrupt on change for group IOCAN - negative
    IOCANbits.IOCAN4 = 0;//1;
    //interrupt on change for group IOCAN - negative
    IOCANbits.IOCAN5 = 0;//1;
    //interrupt on change for group IOCAP - positive
    IOCAPbits.IOCAP1 = 0;//1;
    //interrupt on change for group IOCAP - positive
    IOCAPbits.IOCAP2 = 0;//1;
    //interrupt on change for group IOCAP - positive
    IOCAPbits.IOCAP3 = 0;//1;
    //interrupt on change for group IOCAP - positive
    IOCAPbits.IOCAP4 = 0;//1;
    //interrupt on change for group IOCAP - positive
    IOCAPbits.IOCAP5 = 0;//1;
  
    // Enable IOCI interrupt 
    PIE0bits.IOCIE = 0;//1; 
	
    RXPPSbits.RXPPS = 0x17;   //RC7->EUSART:RX;    
    RC1PPS = 0x0A;   //RC1->CCP2:CCP2;    
    RC6PPS = 0x10;   //RC6->EUSART:TX; 
    CCP1PPSbits.CCP1PPS = 0x12;   //RC2->CCP1:CCP1; 
    
    SMT1SIGPPSbits.SMT1SIGPPS = 0x12;   //RC2->SMT1:SMT1SIG; 
    SMT1WINPPSbits.SMT1WINPPS = 0x12;   //RC2->SMT1:SMT1WIN;  
    
    SMT2SIGPPSbits.SMT2SIGPPS = 0x12;   //nao usa
    SMT2WINPPSbits.SMT2WINPPS = 0x12;   //nao usa
    
    CLCIN0PPSbits.CLCIN0PPS = 0x00;     // RA0 entrada do PWM de consumo
    
}

unsigned int get_freq(unsigned char acumulo){
    static unsigned long p,pp;
    static unsigned char i;
    static unsigned char i_arr;
    p = 0;
    for(i = 0; i<acumulo; i++){
        PIR8bits.SMT1PRAIF = 0;
        while(!PIR8bits.SMT1PRAIF);
        pp = _XTAL_FREQ/SMT1_GetCapturedPeriod();
        freq_atual_arr[i_arr%100] = pp;
        i_arr++;
        p += pp;
    }
    return (unsigned int)(p/acumulo);
    
//    while(periodo == 0);
//    return (unsigned int)((4*_XTAL_FREQ)/periodo);
}

void set_freq(unsigned int f){
//    return;
    char ok = 0;
    unsigned char i = 0;
    //unsigned long f_long = 0;
    unsigned int f_get = 0;
    PWM.byte0 = CCPR2L;
    PWM.byte1 = CCPR2H;
    led.HF = 1;
//    printf("\n\rf = %u | T = %u",f,temperatura);
//    printf("\n\rf0=%u|D=%u",get_freq(5),PWM.duty);
    
    if (f < limite_16 || f > limite_24){
        f = freq_gerador;
        freq_atual = freq_gerador;
    }
    
    do{
        CLRWDT();        
        f_get = get_freq(20);        
        if(f_get > f+20){
            PWM2_LoadDutyValue(++PWM.duty);
//            if(ok>0)
//                ok--;
        }
        else if(f_get < f-20){
            PWM2_LoadDutyValue(--PWM.duty); 
//            if(ok>0)
//                ok--;
        }
        else{
            ok++; 
//            printf("\n\rf_ok=%u|D=%u",f_get,PWM.duty);
        }
        __delay_ms(10);
    }while(ok <= 5); 
    led.HF = 0;
}

char ver_freq(unsigned int f){
    if     (f > limite_24)
        return 24;
    else if(f < limite_16)
        return 16;
    else if(f > limite_22)
        return 22;
    else if(f < limite_17)
        return 17;
    else
        return 0;
}

unsigned int pontencia_instantanea(unsigned char d){
    if(d>duty_ini)
        duty = 2*(d-duty_ini);//0 a 100%    0 a 3v entrada -> 4000 a 100%
    else 
        duty = 0;
    return (ret_pot*duty)/100;    //3500*d/100 = potencia a cada 10ms.
////    float num = (float)(d - duty_ini);//retorna 0 a 50%
////    num = 2*num/100;//x2 para a escala ficar de 0 a 100% .Existe o erro do duty_ini ser diferente de 50% | retorna 0 a 1;
////    if(num < 0) num = 0;
////    else if(num>1) num = 1;
////    return 3500*num; //leitura a cada 1ms. O normal seria 2800*num (carga maxima). É 3500 se a onda dente de serra for de +/-5v
////    // Pmax = 1000*Vp/1,4 (somente para 20KHz);
}

void ini_GERADOR(unsigned int f,unsigned int p,unsigned int c, unsigned int v){
//    printf("Entradas:\n\r");
    for(char i = 1;i<=ENTRADA_MAX;i++){
        //printf("ch = %i; in = %i; bytein = %i\n\r",i,in[i],byte_in[in[i]]);
//        printf("%s -> %s\n\r",entrada_str[i],opc_entrada_str[in[i]]);
    }
//    printf("Saidas:\n\r");
    for(char i = 1;i<=SAIDA_MAX;i++){
        //printf("ch = %i; out = %i; byteout = %i\n\r",i,out[i],byte_out[out[i]]);
//        printf("%s -> %s\n\r",saida_str[i],opc_saida_str[out[i]]);
    }
    for(char i = 0; i<10; i++){
        freq_atual_arr[i] = freq_gerador; //inicia o array da freq_ atual;
    }
    
    freq_gerador = f;
//    float temperatura = conv_temp(get_temp(k_temp)); 
//    printf("K = %.2f |T = %.2f\n\r",k_temp,temperatura);
//    if(temperatura < 10){
//        freq_gerador = f+100;
//    }
    freq_atual = freq_inicial;
//    printf("F_gerador = %u\n\r",freq_gerador);
    
    limite_24 = f + 600;//(unsigned int)round((f*1.040));             ---> 20,60 e 35,60
    limite_22 = f + 500;//(unsigned int)round((f*1.025));             ---> 20,50 e 35,50    
    limite_17 = f - 200;//(unsigned int)round((f*0.988));            ---> 19,80 e 34,80
    limite_16 = f - 250;//(unsigned int)round((f*0.980));            ---> 19,75 e 34,75
    
    potencia = (unsigned int)round((p*1.2)/100);       //potencia maxima
    float Xc1 = XC(f,c);
    float Xc2 = XC(f,22);
    float Xc1Xc2 = Xc1*Xc2;
    float ang = atan(560/(Xc1+Xc2));
//    float IT_3Vref = ((5.55*3)/Xc1)*sqrt(1+pow((560/Xc2),2));
    float IT_3Vref = (5.55*3)*sqrt(pow(560/Xc1Xc2,2)+ pow((Xc1+Xc2)/Xc1Xc2,2)); //Inserido 15/08/18 - Ver 4.1.0
    float Pot_3Vref = IT_3Vref*v;           // referencia de potencia a 3vref 
    float Pot_real = Pot_3Vref*cos(ang);
    mod_pot = (unsigned int)round(p*10/Pot_3Vref);               //para bargraf 7 para 2800, 8 para 1000 e 5 para 1200
    ret_pot = (unsigned int)round(Pot_3Vref/100);                //para convercao duty, pot
//    printf("Xc1=%.2f\n\r",Xc1);
//    printf("Xc2=%.2f\n\r",Xc2);
//    printf("I@3v=%.2f\n\r",IT_3Vref);
//    printf("P@3v=%.2f VA\n\r",Pot_3Vref);
//    printf("P_real@3v=%.2f\n\r",Pot_real);
//    printf("ang=%.2f\n\r",ang*180/3.1415);
//    printf("mod_pot=%u\n\r",mod_pot);
//    printf("ret_pot=%u\n\r",ret_pot);
    
//    printf("\n\r%.2f|%.2f|%.2f|%.2f\n\r",Xc1,Xc2,IT_3Vref,Pot_3Vref);
//    printf("%.2f|%.2f|%u|%u\n\r",ang*180/3.1415,Pot_real,mod_pot,ret_pot);//ok
//    pot_inst = pontencia_instantanea(23);
//    printf("%u | %u\n\r",pot_inst,potencia);
//    if(pot_inst > potencia) printf("Sobrecarga\n\r");
}

float XC(unsigned int f, unsigned int nF){
    float freq_l = f/1000;
    return(1000000/(6.28*freq_l*nF));
}

void delay(unsigned int ms){
    unsigned int t = t10ms;
    while(ms > (t10ms - t)){
        CLRWDT();
    }
}

void salva_EEPROM(unsigned int valor, unsigned char address){
    ee.int0 = valor;
    eeprom_write(address,ee.byte0);
    __delay_ms(1);
    eeprom_write(++address,ee.byte1);
    __delay_ms(1);
}

unsigned int carrega_EEPROM(unsigned char address){
    ee.byte0 = eeprom_read(address);
    __delay_ms(1);
    ee.byte1 = eeprom_read(++address);
    __delay_ms(1);
    return ee.int0;
}

void salva_EEPROM32(unsigned long valor, unsigned char address){
    ee.value = valor;
    eeprom_write((address),ee.byte0);
    __delay_ms(1);
    eeprom_write((++address),ee.byte1);
    __delay_ms(1);
    eeprom_write((++address),ee.byte2);
    __delay_ms(1);
    eeprom_write((++address),ee.byte3);
    __delay_ms(1);
}

unsigned long carrega_EEPROM32(unsigned char address){
    ee.byte0 = eeprom_read(address);
    __delay_ms(1);
    ee.byte1 = eeprom_read(++address);
    __delay_ms(1);
    ee.byte2 = eeprom_read(++address);
    __delay_ms(1);
    ee.byte3 = eeprom_read(++address);
    __delay_ms(1);
    return ee.value;
}

void op_desce(unsigned int t){
    //printf("\n\r1 ");
    if(!flag.sussess) {
        flag.US = 0;
        led.HF = 0;
        start = 0;
        statusSolda = PARADO;
        return;     
    }
    else if (statusSolda == FIXANDO){
        return;
    }
    
    if(!ciclador){
        escreve_LCD(1,1,descendo_str[idioma]);
        if (modo == RAPIDO){
            escreve_LCD(2,1,n_pecas_str[idioma]);
            
            if(flag.regressivo)
                num = cont_pecas;
            else
                num = n_pecas;
            ltoa(str,num,10);
            Lcd_Write_String(str);
            Lcd_Write_Char(' ');
        }
        else if(modo == CONTINUO){
            escreve_LCD(2,1,solda_cont_str[idioma]);
        }
        else{
            escreve_LCD(2,1,aguarde_str[idioma]);
        }
    }
    statusSolda = DESCENDO;
    if(cfg.pne_desce){
        saida(PNE_DESCE,1);
        start_timer(t);   
        do{
            CLRWDT();
            ver_entrada();
            if (ciclador){
                flag.inicia = 1;
            }
            
            if(flag_in.emerg){
                erro = ERRO_EMERG;
                flag.inicia = 0;
                flag.sussess = 0;
                saida(PNE_DESCE,0);
                statusSolda = PARADO;
            }
            else if((cfg.aciona1 &&!flag_in.aciona1) || (cfg.aciona2 && !flag_in.aciona2)){
                flag.inicia = 0;
                flag.sussess = 0;
                saida(PNE_DESCE,0);
                statusSolda = PARADO;
            }
           
        }while(flag.inicia && status_timer);
        stop_timer();
    }
    //printf("\n\r2 ");
}

void op_solda(unsigned int t){
    if(!flag.sussess) {
        flag.US = 0;
        led.HF = 0;
        start = 0;
        statusSolda = PARADO;
        return;     
    }
    
    escreve_LCD(1,1,process_str[idioma]);
    char conta_erro = 0;
    char subida_acionamento = 0;
    amostras = 0;
    erro = 0;
    energia_total = 0;
    energia_total_kJ = 0;
    pot_inst = 0;
    //printf("\n\r3 ");
    
    led.HF = 1;
    flag.US = 1;
    flag.isr_pot = 0;
    status_gerador(STATUS_HF,1);
    start = 1;
    statusSolda = SOLDANDO;
    
    if(modo == CONTINUO){
        start_timer(1000); // 1 segundo para contar quanto tempo ficou acionada.
        cfg.stop_release = 1;
    }
    
    else if(modo == CONTINUO_PULSO){
        cfg.stop_release = 0;
        // Escreve na lina 2
        escreve_LCD(2,1,n_pecas_str[idioma]);
        ltoa(str,n_pecas,10);
        Lcd_Write_String(str);
        
        start_timer(t);
    }
    else{
        start_timer(t);
    }
    do{
        CLRWDT();
        ver_entrada();
        if(flag_in.emerg){ erro = ERRO_EMERG;}                                  //filtro
        else if(ovl){ erro = ERRO_INVERSOR;}                                    //filtro 
//        else if(cfg.metal && t10msMM >= tmp_metal){statusSolda = FIXANDO;}    // METAL-METAL   
        else if(flag.isr_pot){erro = ERRO_OVL; conta_erro = LIMITE_ERRO;}    // sobrecarga   
        else if(modo == NORMAL && flag.energia && energia_total >= ener_solda){statusSolda = FIXANDO;}                          // solda por energia.
        else{
            freq_atual = get_freq(4);            // ainda tem que testar isso
            erro = ver_freq(freq_atual);        // 
            if(ciclador)escreve_freq();
        }
        
        if(erro > 0){
            conta_erro++;
            if(conta_erro > LIMITE_ERRO) {                                           //filtro de erros
                flag.abortar = 1;
                flag.sussess = 0;
                flag.inicia = 0;
                start = 0;
                statusSolda = PARADO;
            }
        }
        else{                        
            if(cfg.stop_release){
                if ((cfg.aciona1 && !flag_in.aciona1) || (cfg.aciona2 && !flag_in.aciona2)){
                    statusSolda = FIXANDO;
                }
            }
            else if (modo == CONTINUO_PULSO){
              
                if ((cfg.aciona1 && flag_in.aciona1) || (cfg.aciona2 && flag_in.aciona2)){
                    //filtro de somente subida
                    if(subida_acionamento == 0){
                    
                        subida_acionamento = 1;
                        stop_timer(); // nao sei se precisa...
                        t1ms_solda_acum += (unsigned long)t1ms_ISR;
                        if(energia_total > 1000){
                            energia_total_kJ++;
                            energia_total -= 1000;
                        }
                        salva_EEPROM32(++n_pecas,ee_pecas);
                        salva_EEPROM32(++n_pecas_l,ee_total_p);

                        escreve_LCD(2,1,n_pecas_str[idioma]);
                        ltoa(str,n_pecas,10);
                        Lcd_Write_String(str);

                        start_timer(t); // reinicia o contador de tempo sempre que for precionado o botao.
                    }
                }
                else{
                    // Quando liberar acionamento abilita nova entrada 
                    subida_acionamento = 0;
                }
            }
            conta_erro = 0;
        }
        
        if(!status_timer){
            if(modo == CONTINUO){
                start_timer(1000); 
                t1ms_solda_acum += 1000UL;
                
                if(energia_total > 1000){
                    energia_total_kJ++;
                    energia_total -= 1000;
                }
            }
            else{
                statusSolda = FIXANDO;
            }            
        }

    //}while(statusSolda == SOLDANDO && !flag.abortar && (status_timer || modo == CONTINUO));
    }while(statusSolda == SOLDANDO && !flag.abortar);
    
    if(modo == NORMAL || modo == CONTINUO || flag.abortar) {
        status_gerador(STATUS_HF,0);
        flag.US = 0;
        led.HF = 0;
        start = 0;
        statusSolda = PARADO;
    }   
    
    stop_timer();
    t1ms_solda = t1ms_ISR;
    
    t1ms_solda_acum += ((unsigned long)t1ms_solda);
    if(t1ms_solda_acum > 100UL){
        t100ms_solda_cont = (t1ms_solda_acum/100UL);
        t100ms_solda += t100ms_solda_cont;
        t1ms_solda_acum %= 100UL;        
    }  
}

void op_fixar(unsigned int t){
    if(!flag.sussess) {
        if(cfg.pne_desce) 
            saida(PNE_DESCE,0);
        flag.US = 0;
        led.HF = 0;
        start = 0;
        statusSolda = PARADO;
        return;
    }
    
    escreve_LCD(1,1,fixando_str[idioma]);
    char conta_erro = 0,e = 0;
    statusSolda = FIXANDO;
    
    start_timer(t);
    
    if(modo == RAPIDO){
//        start_timer(1000);  // 1 seg de tempo
        
        if(cfg.pne_desce) 
            saida(PNE_DESCE,0);
        if(flag.sussess){
            if(flag.regressivo && cont_pecas >0){
                cont_pecas--;
                flag.zero_cont = 0;
            }
            else if (flag.regressivo){
                flag.zero_cont = 1;
            }
            salva_n_pecas();
        }
        
    }
    
//    else if (modo == NORMAL){
//        start_timer(t);
//    } 
    
    if(status_timer){
        do{
            CLRWDT();
            ver_entrada();
            if(flag_in.emerg){ e = ERRO_EMERG;}               //filtro
            
            else if(modo == RAPIDO && (cfg.aciona1 && flag_in.aciona1) && (cfg.aciona2 && flag_in.aciona2))
                return;//break;
            
            
            if(e > 0){
                conta_erro++;
                if(conta_erro > LIMITE_ERRO){                 //filtro de erros
                    flag.abortar = 1;
                    flag.sussess = 0;
                    statusSolda = PARADO;
                }
            }
            else{
                conta_erro = 0;
            }
        }while(!flag.abortar && status_timer);
    }
    
    if(!status_timer || flag.abortar){
        flag.US = 0;
        led.HF = 0;
        start = 0;
        statusSolda = PARADO;
    }
    stop_timer();
    
    if(modo == NORMAL || modo == CONTINUO){
        if(cfg.pne_desce){
            saida(PNE_DESCE,0);
        }
    }
    else if (modo == RAPIDO){
        
    }
    
    //printf("\n\r6 ");
}

//void op_after(unsigned int t){
//    if(!flag.sussess) return;
//    if(cfg.afterburst){
//        //printf("\n\r7 ");
//        char conta_erro = 0;
//        start_timer(t); 
//        start = 1;
//        do{
//            CLRWDT();
//            ver_entrada();
//            pot_inst = pontencia_instantanea(get_duty());
//            if((unsigned int)pot_inst > potencia){ erro = ERRO_OVL;}
//            else{ erro = ver_freq(get_freq(5));}
//            if(erro > 0){
//                conta_erro++;
//                if(conta_erro > LIMITE_ERRO){
//                    flag.abortar = 1;
//                    flag.sussess = 0;
//                }
//            }
//            else{
//                conta_erro = 0;
//            }
//        }while(!flag.abortar && !flag_in.emerg && status_timer);
//        stop_timer();
//        start = 0;
//        //printf("\n\r8 ");
//    }    
//}

void trata_dados(void){
    if(flag.sussess){
        if(modo != RAPIDO){
            if(flag.regressivo && cont_pecas >0){
                cont_pecas--;
            flag.zero_cont = 0;
            }
            else if (flag.regressivo){
                flag.zero_cont = 1;
            }
            salva_n_pecas();
        }
        if(0 != ver_freq(freq_atual)){
            freq_atual = freq_inicial;
        }
        else if(t1ms_solda > 100){                                              // VER 4.2.2
            uint32_t f = 0;
            for(char i = 0; i<100; i++){
                f+=freq_atual_arr[i];
            }
            freq_atual = (f/100) + 50;
        }
        
        if(modo == NORMAL && cfg.energ_erro && flag.energiaErro){   // trava gerador caso energia esteja abaixo do desejado
            if(flag.energia){   // da erro se houver estouro do tempo.
                if(t1ms_solda >= tmp_solda){
                    status_gerador(ERRO_ENE,1);
                    trava_erro(tempo_ovf_str[idioma]);
                }
            }
            else{               //da erro se a energia for menor que a setada.
                if(energia_total < ener_solda){
                    status_gerador(ERRO_ENE,1);
                    trava_erro(energia_low_str[idioma]);
                }
            }   
            status_gerador(ERRO_ENE,0);
        }
    }
    else if(ciclador){
        
        char bt;
        led.value = 0xFF;
        do{
            bt = teclado();
            led.value ^= 1;
        }while(bt != enter);
        freq_atual = freq_inicial;
        rst_inversor();
        led.value = 0;
    }
    else{
        status_gerador(ERRO,1);
        if(cfg.emergencia && erro == ERRO_EMERG){
            escreve_LCD(1,1,emergencia_str[idioma]);
        }
        else if(ovl){
            led.OVL = 1;
            escreve_LCD(1,1,erro_inv_str[idioma]);
            do{
                delay(50);
                rst_inversor(); 
                delay(50);
            }while(ovl);
            trava_erro("\0");
            freq_atual = freq_inicial;
        }
        else if(flag.abortar){ 
            led.ERR = 1;
            Lcd_Clear();
            if(erro == ERRO_OVL){
                escreve_LCD(1,1,erro_ovl_str[idioma]);
            }
            else{                
                escreve_LCD(1,1,erro_str[idioma]);
                ltoa(str,erro,10);
                Lcd_Write_String(str);
            }
            trava_erro("\0");
            freq_atual = freq_inicial;
            //aguarda enter para sair.
        }
        status_gerador(ERRO,0);
    }
    if(flag.regressivo && flag.zero_cont){ //sinaliza contador regressivo parou
        escreve_LCD(1,1,zero_cont_str[idioma]);
        escreve_LCD(2,1,Sai_enter_str[idioma]);
        do{
            CLRWDT();
            char bt = teclado();
            if(bt == enter){
                flag.zero_cont = 0;
                cont_pecas = carrega_EEPROM32(ee_cont);
            }                
        }while(flag.zero_cont); //aguarda confirmacao de visualizacao.
    }
    erro = 0;
    led.value = 0;
    flag.inicia = 0;
    flag.abortar = 0;
    flag.US = 0;
    set_freq(freq_atual);
    if(!ciclador){
        Inicia_LCD();
        tela_ultima_solda();
    }
    alarme = 1;
    delay(10);
    alarme = 0;
}

void liberar(void){
    do{
        CLRWDT();
        ver_entrada();
    }while(flag_in.aciona1 || flag_in.aciona2);//aguarda liberar botoes de acionamento.
    flag_in.liberado = 1;
    flag.abortar = 0;
    flag.inicia = 0;
}

void trava_erro(const char s[]){//aguarda enter;
    char bt;
    if(s[0] != '\0')
        escreve_LCD(1,1,s);
    escreve_LCD(2,1,Sai_enter_str[idioma]);
    unsigned int t = t10ms;
    do{
        CLRWDT();
       bt = teclado();
       if(t10ms - t > 100){
           alarme = ~alarme;
           t = t10ms;
       }
    }while(bt != enter && !entrada(RESET));
    alarme = 0;
    if(!flag.teste)
        tela_inicial();
    do{
        CLRWDT();
        bt = teclado();
    }while(bt == enter || entrada(RESET));
}

void emergencia(void){
    uint8_t bt = 0;
    if(cfg.emergencia && flag_in.emerg){
        status_gerador(ERRO,1);
        EMERG_1:
        escreve_LCD(1,1,emergencia_str[idioma]);
        escreve_LCD(2,1,emerg2_str[idioma]);
//        printf("\n\rEmergencia Pressionado");

        alarme = 1;
        do{  //Botao de emergencia precionado.
            ver_entrada();
            bt = teclado();
            if(bt == menu){
                alarme = 0;
                rotina_tela();
                bt = 0;
                goto EMERG_1;
            }
        }while(flag_in.emerg);
        alarme = 0;
//        printf("\n\rLiberado!");
//        trava_erro(emergencia_str[idioma]);
        status_gerador(ERRO,0);
        if(!flag.teste)
            tela_inicial();
    }
    alarme = 0;
}

void config_IO_start(void){
    cfg.value = 0;
    flag.value = 0;
    flag_in.value = 0;
    led.value = 0;
    ee.value = 0;
    flag_in.value = 0;
    btt.byte = 0;
}

void ISR_entrada(entradas ch){   
    unsigned char x = in[ch];
    unsigned char p = PORTA;
    unsigned b = p & byte_in[x];
    switch(x){
        case EMERGENCIA:
            if(cfg.emergencia){
                if(!b)       flag_in.emerg = 1;
                else        flag_in.emerg = 0;
            }
            else flag_in.emerg = 0;
            break;
        case ACIONAMENTO1:            
            if(cfg.aciona1){
                if(b)      flag_in.aciona1 = 1;
                else        flag_in.aciona1 = 0;
            }
            else flag_in.aciona1 = 0;
            break;
        case ACIONAMENTO2:
            if(cfg.aciona2){
                if(b)      flag_in.aciona2 = 1;
                else        flag_in.aciona2 = 0;
            }
            else flag_in.aciona2 = 0;
            break;
        case RESET:
            if(cfg.reset){
                if(b)      flag_in.liberar = 1;
                else        flag_in.liberar = 0;
            }
            else flag_in.liberar = 0;
            break;
        case METAL:
            if(cfg.metal){
                if(b)      flag_in.metal = 1;
                else        flag_in.metal = 0;
            }
            break;
        case SENSOR_PECA:
            break;
        case SENSOR_PNE_AV:
            break;
        case SENSOR_PNE_RC:
            break;
        case SENSOR_HOLD:
            break;
    }
}

unsigned entrada(opc_entradas x){
    char p = PORTA;
    return (p & byte_in[x]);
}

void ver_entrada(void){
    static char temp = 0;
    ISR_entrada(IN1);
    ISR_entrada(IN2);
    ISR_entrada(IN3);
    ISR_entrada(IN4);
    ISR_entrada(IN5);
    
    if (cfg.emergencia && flag_in.emerg){
        flag.inicia = 0;
        flag.abortar = 1; // Testar!!!
    }
    else if(cfg.aciona1 && cfg.aciona2){        
        if(temp == 0 && (flag_in.aciona1 || flag_in.aciona2)){
            t10ms = 0;
            if(flag_in.aciona1) temp = 1;
            if(flag_in.aciona2) temp = 2;
        }
        else if(temp > 0 && flag_in.aciona1 && flag_in.aciona2 && t10ms < 50){
            flag.inicia = 1;
        }
        else if (temp > 0 && !flag_in.aciona1 && !flag_in.aciona2 && t10ms > 50){
            flag.inicia = 0;
            temp = 0;
        }
    }
    else if(cfg.aciona1 || cfg.aciona2){
        if(flag_in.aciona1 || flag_in.aciona2){
            flag.inicia = 1;
        }
        else{
            flag.inicia = 0;
        }
    }
}

void config_entrada(opc_entradas x, entradas ch){
    switch (ch) {
        case IN1:
            in[ch] = x;
            byte_in[x] = (1<<ch);
            break;
            
        case IN2:
            in[ch] = x;
            byte_in[x] = (1<<ch);
            break;
            
        case IN3:
            in[ch] = x;
            byte_in[x] = (1<<ch);
            break;
            
        case IN4:
            in[ch] = x;
            byte_in[x] = (1<<ch);
            break;
            
        case IN5:
            in[ch] = x;
            byte_in[x] = (1<<ch);
            break;
            
        case IN_NONE:
            in[ch] = 0;
            byte_in[x] = (1<<ch);
            break;
    }
    config_flag_entrada(x,ch);
}

void config_saidas(opc_saidas x, saidas ch){
    clear_saida(ch);
    
    switch (ch){
        case OUT1:
            out[ch] = x;                //aponta a saida ch para a opcao x
            byte_out[x] = (0b00000001);      //gera comparador "and" para saida. 
            break;            
        case OUT2:
            out[ch] = x;
            byte_out[x] = (0b00100000);
            break;            
        case OUT3:
            out[ch] = x;
            byte_out[x] = (0b00010000);
            break;            
        case OUT4:
            out[ch] = x;
            byte_out[x] = (0b00001000);
            break;            
    }
    config_flag_saida(x,ch);
}

void clear_entrada (opc_entradas x) {
    unsigned char ch;
    for (ch = 0; in[ch] != x; ch++){
        if(ch > 4) break;
    }
    if(ch < 4){
        in[ch] = 0;
        byte_in[x] = 0;
        config_flag_entrada(x,0);   //zera flags da entrada x
    }
}

void clear_saida (saidas ch){
    config_flag_saida(out[ch],0);
    
//    unsigned char ch;
//    for (ch = 0; out[ch] != x; ch++){
//        if(ch > 4) break;
//    }
//    if(ch < 4){
//        out[ch] = 0;
//        byte_out[x] = 0;
//        config_flag_saida(x,0);   //zera flags da saida x
//    }
}

void config_maq_especial(void){
    for(char i = 0;i<=ENTRADA_MAX;i++){
        byte_in[in[i]] = 0;
        in[i] = 0; 
    }
    for(char i = 0;i<=SAIDA_MAX;i++){
        byte_out[out[i]] = 0;
        out[i] = 0; 
    }
    cfg.value = 0;
    flag_in.value = 0;
    
    config_entrada(EMERGENCIA,IN5);         // EMERGENCIA
    config_entrada(ACIONAMENTO1,IN4);              // ACIONAMENTO1
    config_entrada(ACIONAMENTO2,IN3);              // ACIONAMENTO2
    config_entrada(RESET,IN2);              // RESET    // Inserido 30/11/2018 - Ver. 4.1.3
    config_entrada(0,IN1);                  // 

    config_saidas(RST_INV,OUT1);        // RST_INV
    config_saidas(PNE_DESCE,OUT2);      // PNE_DESCE
    if(flag.energiaErro)
        config_saidas(ERRO_ENE,OUT3);
    else
        config_saidas(STATUS_GERADOR,OUT3);              // ERRO ou STATUS_GERADOR //Inserido 08/2018 - Ver 4.0.2
    config_saidas(ERRO,OUT4);              // ERRO ou STATUS_GERADOR ou ESTOURO TEMPO/ENERGIA N OK
    cfg.stop_release = 1;
}

void config_maq (opc_maquinas x){
    for(char i = 0;i<=ENTRADA_MAX;i++){
        byte_in[in[i]] = 0;
        in[i] = 0; 
    }
    for(char i = 0;i<=SAIDA_MAX;i++){
        byte_out[out[i]] = 0;
        out[i] = 0; 
    }
    cfg.value = 0;
    flag_in.value = 0;
    switch(x){
        case PISTOLA_PULSADO:
            config_entrada(0,IN5);     // EMERGENCIA
            config_entrada(ACIONAMENTO1,IN4);              // ACIONAMENTO1
            config_entrada(0,IN3);              // Metal
            config_entrada(RESET,IN2);              // RESET    // Inserido 30/11/2018 - Ver. 4.1.3
            config_entrada(0,IN1);              // 

            config_saidas(RST_INV,OUT1);        // RST_INV
//            config_saidas(STATUS_HF,OUT2);      // PNE_DESCE,STATUS_HF
            if(flag.energiaErro)
                config_saidas(ERRO_ENE,OUT3);
            else
                config_saidas(STATUS_HF,OUT3);   // ERRO ou STATUS_HF //Inserido 05/2024 - Ver 4.0.2
            config_saidas(ERRO,OUT4);              // ERRO ou STATUS_GERADOR ou ESTOURO TEMPO/ENERGIA N OK
            cfg.stop_release = 1;
            break;
        case MAQUINA_PULSADO:
            config_entrada(EMERGENCIA,IN5);     // EMERGENCIA
            config_entrada(ACIONAMENTO1,IN4);              // ACIONAMENTO1
            config_entrada(ACIONAMENTO2,IN3);              // ACIONAMENTO2
            config_entrada(RESET,IN2);              // RESET    // Inserido 30/11/2018 - Ver. 4.1.3
            config_entrada(0,IN1);              // 

            config_saidas(RST_INV,OUT1);        // RST_INV
            config_saidas(PNE_DESCE,OUT2);      // PNE_DESCE
            if(flag.energiaErro)
                config_saidas(ERRO_ENE,OUT3);
            else
                config_saidas(STATUS_GERADOR,OUT3);              // ERRO ou STATUS_GERADOR //Inserido 08/2018 - Ver 4.0.2
            config_saidas(ERRO,OUT4);              // ERRO ou STATUS_GERADOR ou ESTOURO TEMPO/ENERGIA N OK
                        
            if(modo == CONTINUO)    cfg.stop_release = 1;
            else                    cfg.stop_release = 0;
            break;
        case MAQUINA_MM:
            config_entrada(EMERGENCIA,IN5);     // EMERGENCIA
            config_entrada(ACIONAMENTO1,IN4);              // ACIONAMENTO1
            config_entrada(ACIONAMENTO2,IN3);              // ACIONAMENTO2
            config_entrada(METAL,IN2);              // Dector metal metal (4,2,92 10/05/2021)
            config_entrada(0,IN1);              // 

            config_saidas(RST_INV,OUT1);        // RST_INV
            config_saidas(PNE_DESCE,OUT2);      // PNE_DESCE
            if(flag.energiaErro)
                config_saidas(ERRO_ENE,OUT3);
            else
                config_saidas(STATUS_GERADOR,OUT3);              // ERRO ou STATUS_GERADOR //Inserido 08/2018 - Ver 4.0.2
            config_saidas(ERRO,OUT4);              // ERRO ou STATUS_GERADOR ou ESTOURO TEMPO/ENERGIA N OK    
            if(modo == CONTINUO)    cfg.stop_release = 1;
            else                    cfg.stop_release = 0;
            break;
    }
//    if(!cfg.pne_desce){
//        pagina = TELA_TMP_SOLDA;
//    }
}

void config_flag_entrada(opc_entradas x, entradas ch){
    switch(x){
        case IN_NC:
            break;
        case EMERGENCIA:
            if(ch != IN_NONE)  cfg.emergencia = 1;
            else            cfg.emergencia = 0;
            break;
        case ACIONAMENTO1:
            if(ch != IN_NONE)  cfg.aciona1 = 1;
            else            cfg.aciona1 = 0;
            break;
        case ACIONAMENTO2:
            if(ch != IN_NONE)  cfg.aciona2 = 1;
            else            cfg.aciona2 = 0;
            break;
        case RESET:
            if(ch != IN_NONE)  cfg.reset = 1;
            else            cfg.reset = 0;
            break;
        case METAL:
            if(ch != IN_NONE)  cfg.metal = 1;
            else            cfg.metal = 0;
            break;
        case SENSOR_PECA:
            if(ch != IN_NONE)  cfg.sensor_peca = 1;
            else            cfg.sensor_peca = 0;
            break;
        case SENSOR_PNE_AV:
            if(ch != IN_NONE)  cfg.sensor_pne_av = 1;
            else            cfg.sensor_pne_av = 0;
            break;
        case SENSOR_PNE_RC:
            if(ch != IN_NONE)  cfg.sensor_pne_rc = 1;
            else            cfg.sensor_pne_rc = 0;
            break;
        case SENSOR_HOLD:
            if(ch != IN_NONE)  cfg.sensor_hold= 1;
            else            cfg.sensor_hold = 0;
            break;
    }
}
 
void config_flag_saida(opc_saidas x, saidas ch){
    switch (x){
        case OUT_NC:
            break;
        case PNE_DESCE:
            if(ch != OUT_NONE) cfg.pne_desce = 1;
            else cfg.pne_desce = 0;
            break;
        case PNE_SOBE:
            if(ch != OUT_NONE) cfg.pne_sobe = 1;
            else cfg.pne_sobe = 0;
            break;
        case ERRO:
            if(ch != OUT_NONE) cfg.trava_erro = 1;
            else cfg.trava_erro = 0;
            break;
        case ERRO_ENE:
            if(ch != OUT_NONE) cfg.energ_erro = 1;
            else cfg.energ_erro = 0;
            break;
        case STATUS_GERADOR:
            if(ch != OUT_NONE) cfg.status_out = 1;
            else cfg.status_out = 0;
            break; 
        case STATUS_HF:
            if(ch != OUT_NONE) cfg.status_hf = 1;
            else cfg.status_hf = 0;
            break;
        case RST_INV:
            if(ch != OUT_NONE) cfg.rst_inv = 1;
            else cfg.rst_inv = 0;
            break;
    }
}

void saida(opc_saidas x, unsigned b){
    char port = LATC;
    if(!b){
        LATC = (unsigned char)(port & ~byte_out[x]);  //zera bit da saida correspondente
    }
    if(b){
        LATC |= byte_out[x];    //seta o bit.
    }
}

void clr(void){
    clr_led = 0;
    NOP();NOP();NOP();NOP();
    clr_led = 1;
    NOP();
}

void data(unsigned b){
    data_led = b;
    NOP();NOP();
    clk_led = 1;
    NOP();NOP();
    clk_led = 0;
    NOP();NOP();
    data_led = 0;
}

void load_bar(void){
    clr();        
    data(bar.POT100);
    data(bar.POT90);
    data(bar.POT80);
    data(bar.POT70);
    data(bar.POT60);
    data(bar.POT50);
    data(bar.POT40);
    data(bar.POT30);
    data(bar.POT20);
    data(bar.POT10);
    data(led.ERR);
    data(led.OVL);
    data(led.HF);
}

void teste_leds(void){
    char temp = INTCONbits.GIE;
    INTCONbits.GIE = 0;
    clr();
    data(1);
    for(char i = 0;i<14;i++){
        CLRWDT();
        __delay_ms(200);
        data(0);
    }
    INTCONbits.GIE = (unsigned)temp;
}

void tela_ultima_solda(void){
    Lcd_Clear();
    
    if(modo == CONTINUO || modo == CONTINUO_PULSO){
        //########################### LINHA 1
        escreve_LCD(1,1,energia_str[idioma]);
        ltoa(str,(energia_total_kJ),10);
        Lcd_Write_String(str);
        Lcd_Write_String(" kJ"); 
        //########################### LINHA 2
        escreve_LCD(2,1,tempo_str[idioma]);
        ltoa(str,(t100ms_solda_cont/10),10);
        Lcd_Write_String(str);
        Lcd_Write_String(".");
        ltoa(str,(t100ms_solda_cont%10),10);
        Lcd_Write_String(str);
        Lcd_Write_String("s  ");
    }
    else{
        //########################### LINHA 1
        escreve_LCD(1,1,energia_str[idioma]);
        ltoa(str,(energia_total),10);
        Lcd_Write_String(str);
        Lcd_Write_String(" J"); 
        //########################### LINHA 2
        escreve_LCD(2,1,tempo_str[idioma]);
        ltoa(str,(t1ms_solda),10);
        Lcd_Write_String(str);
        Lcd_Write_String(" ms");
    }
}

void tela_inicial(void){
    CLRWDT();
    //Inicia_LCD();
    Lcd_Clear();
//    ########################### LINHA 1
    escreve_LCD(1,1,Sonitron_str);
    Lcd_Write_Char('0'+ modo);
//    ########################### LINHA 2
    escreve_LCD(2,1,n_pecas_str[idioma]);
    if(flag.regressivo)
        num = cont_pecas;
    else
        num = n_pecas;
    ltoa(str,num,10);
    Lcd_Write_String(str);
    //printf("\n\rFatual=%u Hz",freq_atual);
}

void totalCiclos(void){
      CLRWDT();
    //Inicia_LCD();
    Lcd_Clear();
//    ########################### LINHA 1
    escreve_LCD(1,1,"OP:");
    ltoa(str,n_pecas_l,10);
    Lcd_Write_String(str);
//    ########################### LINHA 2
    escreve_LCD(2,1,"T:");
    ltoa(str,t100ms_solda/10,10);
    Lcd_Write_String(str);
    Lcd_Write_String(".");
    ltoa(str,t100ms_solda%10,10);
    Lcd_Write_String(str);
    Lcd_Write_String("s  ");
}

void rst_inversor(void){
    if(cfg.rst_inv){
        saida(RST_INV,1);
        __delay_ms(200);
        saida(RST_INV,0);
    }
}

void salva_n_pecas(void){
    if(!ciclador){
        salva_EEPROM32(++n_pecas,ee_pecas);
        salva_EEPROM32(++n_pecas_l,ee_total_p);
        salva_EEPROM32(t100ms_solda,ee_t1s);
    }
}

unsigned char teclado(void){
    static unsigned char b;
//    if(b>8) {printf("b=%u || id =%u \n\r",b,id_btt);}
    if(btt.press){        
        if(b == id_btt){
//            printf("H\n\r");
            btt.hold = 1;
            return b;
        }
        else{
//            printf("R\n\r");
            btt.press = 0;
            btt.hold = 0;
            return 0;
        }
    }
    else {
        b = 0;
        PORT_BTT = 0b00000001;//0b00111110;
        start_timer(MATRIZ_TEMPO);
        while(status_timer && !btt.press){
            b = id_btt;
            if(b > btt_min){
                btt.press = 1;
                bip_tecla();
                stop_timer();
                return b;
            }
        }
        
        PORT_BTT = 0b00000010;//0b00111101;
        start_timer(MATRIZ_TEMPO);
        while(status_timer && !btt.press){
            b = id_btt;
            if(b > btt_min){
                btt.press = 1;
                bip_tecla();
                stop_timer();
                return b;
            }
        }
        
        PORT_BTT = 0b00000100;//0b00111011;
        start_timer(MATRIZ_TEMPO);
        while(status_timer && !btt.press){
            b = id_btt;
            if(b > btt_min){
                btt.press = 1;
                bip_tecla();
                stop_timer();
                return b;
            }
        }        
    }
    return b;
}

void bip_tecla(void){
    if(!flag_in.emerg){
        alarme = 1;
        __delay_ms(100);
        alarme = 0;
    }
    else
        __delay_ms(100);
}

unsigned liberar_tecla(unsigned char b, unsigned int t){
    char tc;
    unsigned int temp = t10ms, tempo = t/10;

    do{
        CLRWDT();
        tc = teclado();
        if(t && (t10ms - temp >= tempo))
            return 0;
    }while(b == tc);

    return 1; //se segurado por +500ms -> returna falso.

    
//    
//    char tc,fl = 1;
//    unsigned int temp = t10ms, tempo = t/10;
//    do{
//        CLRWDT();
//        tc = teclado();
//        
//    }while(t10ms - temp < tempo && b == tc);
//    if(t10ms - temp >= tempo)
//        fl = 0;
//    return fl; //se segurado por +500ms -> returna falso.
}

void telas(unsigned char b){
    static char ee_addr;
    static long temp;
#ifdef DEBUG debug(__LINE__); 
#endif;
    switch(b){
        case rigth:
            if(pagina < TAMANHO_MENU)
                pagina++;
            else
                pagina = TELA_TMP_DESCE;
            // Verifica se pagina pode ser exibida:
            pagina = verifPagina(pagina,b);
            break;
            
        case left:
            if(pagina > TELA_TMP_DESCE)
                pagina--;
            else
                pagina = TAMANHO_MENU;  
            // Verifica se pagina pode ser exibida:
            pagina = verifPagina(pagina,b);
            break;
            
        case up:
            switch(pagina){
                case TELA_TMP_LOAD:
                case TELA_TMP_SAVE:
                    if(ee_addr < 9)
                        ee_addr++;
                    flag.muda_valor = 1;
                    break;
                case TELA_TMP_DESCE:
                    if(temp < 3000){
                        temp = temp + tmp_soma;
                        if(temp > 3000) temp = 3000;
                    }
                    flag.muda_valor = 1;
                    break;
                case TELA_METAL:
                    if(temp < 500){
                        temp = temp + tmp_soma;
                        if(temp > 500) temp = 500;
                    }
                    flag.muda_valor = 1;
                    break;
                case TELA_TMP_CONT:
                    if(temp < 100000){
                        temp = temp + tmp_soma;
                        if(temp > 10000) temp = 10000;
                    }
                    flag.muda_valor = 1;
                    break;
                case TELA_FREQ_INI:
                    if(temp < limite_22 - 50)
                        temp = temp + tmp_soma;
                    else temp = limite_22;
                    flag.muda_valor = 1;
                    break;
                case TELA_TIPO_SOLDA:
                case TELA_TIPO_CONT:
                    flag.muda_valor = 0;
                    flag.salvar = 1;
                    break;
                case TELA_OP_TMP:
                    flag.muda_valor = 0;
                    flag.salvar = 0;
                default:
                    if(temp < 10000)
                        temp = temp + tmp_soma;
                    flag.muda_valor = 1;
                    break;
            }
            
            break;
            
        case down:            
            switch(pagina){
                case TELA_TMP_LOAD:
                case TELA_TMP_SAVE:
                    if(ee_addr > 0)
                        ee_addr--;
                    flag.muda_valor = 1;
                    break;
                case TELA_FREQ_INI:
                    if(temp > limite_17 + 50)
                        temp = temp - tmp_soma;
                    else temp = limite_17;
                    flag.muda_valor = 1;
                    break;
                case TELA_TIPO_SOLDA:
                case TELA_TIPO_CONT:
                    flag.muda_valor = 0;
                    flag.salvar = 1;
                    break;
                case TELA_OP_TMP:
                    flag.muda_valor = 0;
                    flag.salvar = 0;
                    break;
                default:
                    temp = temp - tmp_soma;
                    if(temp<0)
                        temp = 0;
                    flag.muda_valor = 1;
                    break; 
            }
            break;
            
        case enter:
            switch (pagina){
                case TELA_TMP_LOAD:
                    tmp_desce = carrega_EEPROM(addr_receitas(ee_addr,0));
                    tmp_solda = carrega_EEPROM(addr_receitas(ee_addr,2));
                    ener_solda = carrega_EEPROM(addr_receitas(ee_addr,4));
                    tmp_fixar = carrega_EEPROM(addr_receitas(ee_addr,6));
                    tmp_metal = carrega_EEPROM(addr_receitas(ee_addr,8)); 
                    flag.value |= (7&carrega_EEPROM(addr_receitas(ee_addr,10)));    //carrega b0 e b1 de flag.
                    cont_pecas= carrega_EEPROM32(addr_receitas(ee_addr,12)); 
                    flag.salvar = 1;
                    flag.muda_valor = 0;
                    break;
                case TELA_TMP_SAVE:
                    salva_EEPROM(tmp_desce,addr_receitas(ee_addr,0));
                    salva_EEPROM(tmp_solda,addr_receitas(ee_addr,2));
                    salva_EEPROM(ener_solda,addr_receitas(ee_addr,4));
                    salva_EEPROM(tmp_fixar,addr_receitas(ee_addr,6));
                    salva_EEPROM(tmp_metal,addr_receitas(ee_addr,8));
                    salva_EEPROM((7&flag.value),addr_receitas(ee_addr,10));         //salva b0 e b1 de flag.
                    salva_EEPROM32(cont_pecas,addr_receitas(ee_addr,12));
                    flag.salvar = 1;
                    flag.muda_valor = 0;
                    break;
                case TELA_TIPO_SOLDA:
                    // Configura saida de erro de tempo ou energia
                    flag.energiaErro = ~flag.energiaErro;
                    if(flag.energiaErro)
                        config_saidas(ERRO_ENE,OUT3);
                    else
                        config_saidas(STATUS_GERADOR,OUT3);
                    flag.muda_valor = 1;
                    break;
                case TELA_TIPO_CONT:
                    flag.muda_valor = 0;
                    flag.salvar = 1;
                    break;
                case TELA_TIPO_MAQU:
                    flag.muda_valor = 1;
                    break;
                case TELA_OP_TMP:
                    flag.muda_valor = 0;
                    flag.salvar = 0;
                    break;
                case TELA_IDIOMA:
                    flag.muda_valor = 1;
                    break;
                default:
                    flag.muda_tela = 1;
                    if(flag.tela && tmp_soma == 1)
                        tmp_soma = 10;
                    else if(flag.tela && tmp_soma == 10)
                        tmp_soma = 100;
                    else if(flag.tela)
                        tmp_soma = 1;
                    break;
            }
            break;
    }
    
    // Verifica se pagina pode ser exibida:
    //pagina = verifPagina(pagina,b);
#ifdef DEBUG debug(__LINE__); 
#endif;

    switch (pagina){        
        case TELA_TMP_DESCE:
//            printf("DS\n\r");
            if(flag.muda_valor){
                tmp_desce = temp;
                flag.muda_valor = 0;
            }
            escreve_tempos(descida_str[idioma],tmp_desce,pagina);
            temp = tmp_desce;
            break;
            
        case TELA_TMP_SOLDA:
//            printf("SD\n\r");
            if(flag.muda_valor){
                tmp_solda = temp;
                flag.muda_valor = 0;
            }
            escreve_tempos(solda_str[idioma],tmp_solda,pagina);
            temp = tmp_solda;
            break;
            
        case TELA_TMP_JOULE:
            if(flag.muda_valor){
                ener_solda = temp;
                flag.muda_valor = 0;
            }
            escreve_tempos(joule_str[idioma],ener_solda,pagina);
            temp = ener_solda;
            break;
            
        case TELA_TMP_FIXAR:
//            printf("FX\n\r");
            if(flag.muda_valor){
                tmp_fixar = temp;
                flag.muda_valor = 0;
            }
            escreve_tempos(fixacao_str[idioma],tmp_fixar,pagina);
            temp = tmp_fixar;
            break;
        case TELA_METAL:
            if(flag.muda_valor){
                tmp_metal = temp;
                flag.muda_valor = 0;
            }
            escreve_tempos(metal_str[idioma],tmp_metal,pagina);
            temp = tmp_metal;
            break;
            
        case TELA_TIPO_MAQU:
            if(flag.muda_valor){
                tipo_maquina++;
                if(tipo_maquina >= TAMANHO_MAQUINAS) tipo_maquina = PISTOLA_PULSADO;
                config_maq(tipo_maquina);
                flag.muda_valor = 0;
            }
            escreve_tempos(tipo_maquina_str[idioma],tipo_maquina,pagina);
            break;  
            
//        case TELA_MODO_OP:
//            
//            break;
                
        case TELA_TMP_CONT:
            if(flag.muda_valor){
                cont_pecas = temp;
                flag.muda_valor = 0;
            }
//            printf("AF:%u\n\r",cont_pecas);
            escreve_tempos(cont_pecas_str[idioma],cont_pecas,pagina);
            temp = cont_pecas;
            break;            
            
        case TELA_TIPO_SOLDA:       //solda por tempo ou energia.
//            printf("DS\n\r");
            if(flag.salvar){
                flag.energia = ~flag.energia;
                flag.salvar = 0;
            }
            
            escreve_tempos(ctrl_solda_str[idioma],(char)flag.energia,pagina);
            break;
            
        case TELA_TIPO_CONT:        //contador progressivo ou regressivo.
//            printf("DS\n\r");
            if(flag.salvar){
                flag.regressivo = ~flag.regressivo;
                flag.salvar = 0;
            }
            escreve_tempos(ctrl_pecas_str[idioma],(char)flag.regressivo,pagina);
            break;
            
        case TELA_FREQ_INI:
            if(flag.muda_valor){
                freq_inicial = temp;
                freq_atual = temp;
                flag.muda_valor = 0;
            }
            escreve_tempos(freq_ini_str,freq_inicial,pagina);
            temp = freq_inicial;
            break;
        case TELA_TMP_LOAD:
//            printf("LO\n\r");
            if(flag.salvar){
                strcpy(str1,receita_carreg_str[idioma]);
                flag.salvar = 0;
            }
            else
                escreve_tempos(carrega_str[idioma],(ee_addr+1),pagina);   //mostra de 1 a 10, mas na realidade é de 0 a 9.   
            break;
            
        case TELA_TMP_SAVE:
//            printf("SA\n\r");
            if(flag.salvar){
                strcpy(str1,receita_salva_str[idioma]);
                flag.salvar = 0;
            }
            else
                escreve_tempos(salva_str[idioma],(ee_addr+1),pagina);
            break;
        
        case TELA_OP_TMP:
            flag.muda_tela = 0;
            totalCiclos();
            break;
        case TELA_IDIOMA:
            if(flag.muda_valor){
                idioma++;
                if(idioma >= TAMANHO_IDIOMA) idioma = PORTUGUES;
                flag.muda_valor = 0;
            }
            escreve_tempos(idioma_str[idioma],idioma,pagina);
            break;
    }
#ifdef DEBUG debug(__LINE__); 
#endif;
    if(flag.muda_tela){
        Lcd_Clear();
        escreve_LCD(1,1,str1);
        escreve_LCD(2,1,str2);
//        printf("%s\n\r%s\n\r",str1,str2);
        flag.muda_tela = 0;
    }
#ifdef DEBUG debug(__LINE__); 
#endif;
}

void escreve_tempos(const char s1[], unsigned int t, char pag){
    strcpy(str1,s1);
    switch(pag){
        case TELA_TMP_LOAD:
        case TELA_TMP_SAVE:
            strcpy(str2,valor_a_str[idioma]);
            ltoa(str,t,10);
            strcat(str2,str);
            break;
        case TELA_TIPO_SOLDA:
            if(!flag.energia)
                strcpy(str2,tipo_tempo_str[idioma]);
            else
                strcpy(str2,tipo_energia_str[idioma]);
            if(flag.energiaErro)
                strcat(str2,erro_energia_str[idioma]);
            break;
        case TELA_TIPO_CONT:
            if(flag.regressivo)
                strcpy(str2,tipo_regress_str[idioma]);
            else
                strcpy(str2,tipo_progress_str[idioma]);
            break;
        case TELA_TIPO_MAQU:
            if(t == PISTOLA_PULSADO)
                strcpy(str2,tipo_maquina1_str[idioma]);
            else if(t == MAQUINA_PULSADO)
                strcpy(str2,tipo_maquina2_str[idioma]); 
            else if(t == MAQUINA_MM)
                strcpy(str2,tipo_maquina3_str[idioma]);
            break;
         case TELA_IDIOMA:
                strcpy(str2,tipo_idioma_str[idioma]);
            break;
//        case TELA_METAL:
//            if(t) strcpy(str2,sim_str[idioma]);
//            else strcpy(str2,nao_str[idioma]);
//            break;
        default:
            ltoa(str,tmp_soma,10);   
            strcat(str1,str);
            strcat(str1,")");
            strcpy(str2,valor_a_str[idioma]);
            ltoa(str,t,10);
            strcat(str2,str);
            if(pag == TELA_TMP_JOULE)
                strcat(str2,"J");
            else if(pag == TELA_FREQ_INI)
                strcat(str2,"Hz");
            else if (pag != TELA_TMP_CONT)
                strcat(str2,"ms");
            break;
    }
}

unsigned char addr_receitas(unsigned char addr, unsigned char tempo){
    return (ee_receita_1 + tempo + (addr*16));
}

void func_enter(void){

    if(!liberar_tecla(enter,2000)){
#if (FUNCAO_TESTE_ENTER) 
        char stop_release = cfg.stop_release;
        cfg.stop_release = 0;
        alarme = 1;
        __delay_ms(2000);    //alarme por 2 segundos, processo cancelado por botao de emergencia
        alarme = 0;
        
        liberar();
        ver_entrada();
        if(!flag.abortar && flag_in.liberado){
            status_gerador(STATUS_GERADOR,1);
            t10ms = 0;  
            escreve_LCD(1,1,process_str[idioma]); 
            escreve_LCD(2,1,aguarde_str[idioma]); 
            flag.sussess = 1;
            do{
                ciclador = 1;
                op_desce(tmp_desce);
                ciclador = 0;
                op_solda(tmp_solda);
                op_fixar(tmp_fixar);
            }while(modo != NORMAL && statusSolda != PARADO);
            flag_in.liberado = 0;
            flag.sussess = 0; //(Para nao utilizar dados e acusar erro quando houver.)
            trata_dados();
            t_tela = 0;
            flag.inatividade = 0;
            status_gerador(STATUS_GERADOR,0);
            cfg.stop_release = stop_release;
        }
#endif
    }
    else{
//            Mostra Frequencia Inicial.
#if(FREQ_INI)
        char bt = 0;
        do{
            alarme = 1;
        }while(teclado() == enter);
        alarme = 0;

        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String(freq_ini_str);
        Lcd_Write_String("x10)");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(freq_str);
        Lcd_Set_Cursor(2,8);
        utoa(str,freq_inicial,10);
        Lcd_Write_String(str);
        Lcd_Write_String("Hz   ");
        do{
            bt = teclado();
            if(bt == up || bt == down){

                if(bt == up){
                    freq_inicial += 10;
                }
                else{
                    freq_inicial -= 10;
                }

                Lcd_Set_Cursor(2,8);
                utoa(str,freq_inicial,10);
                Lcd_Write_String(str);
                Lcd_Write_String("Hz   ");
                freq_atual = freq_inicial;
                do{
                    alarme = 1;
//                        __delay_ms(10);
                    bt = teclado();
                }while(bt == up || bt == down);
                alarme = 0;
            }
        }while(bt != enter);
        set_freq(freq_atual);
#endif
    }
}

void func_rst(void){
    if(liberar_tecla(rst,1000)){
        rst_inversor();        
    }
    else{
        freq_atual = freq_inicial;
        set_freq(freq_atual);
        n_pecas = 0;
        salva_EEPROM32(n_pecas,ee_pecas);
        while(id_btt == rst){
            CLRWDT();
        }
        //Lcd_Init();
    }
}

void teste_pneumatico(char b){
    if(!cfg.pne_desce) 
        return;    
    else if(liberar_tecla(b,1000)){
        flag.teste = 1;
        char bt;
//        printf("Teste Pneumatico\n\r");
        Lcd_Clear();
        escreve_LCD(1,1,teste_PN1_str[idioma]);
        escreve_LCD(2,1,Sai_enter_str[idioma]);
        __delay_ms(1000);
        flag.tela = 1;
        do{
            CLRWDT();
            ver_entrada();
//            if(!flag_in.emerg && (flag_in.aciona1 || flag_in.aciona2)) // Teste pressioando 2 botoes.
            if(flag.inicia)
                saida(PNE_DESCE,1);
            else
                saida(PNE_DESCE,0);
            bt = teclado();
            if(flag_in.emerg){
                emergencia();
                liberar();
                Lcd_Clear();
                escreve_LCD(1,1,teste_PN1_str[idioma]);
                escreve_LCD(2,1,Sai_enter_str[idioma]);
            }
            if(bt == enter){
                flag.tela = 0;
                liberar_tecla(enter,1000);
            }
        }while(flag.tela || flag.inicia);
//        printf("F1 pressed\n\r");
        saida(PNE_DESCE,0);
    }
    else{
//        printf("F1 hold\n\r");
    }
    flag.teste = 0;
}

void teste_US(char b){
    if(liberar_tecla(b,1000)){
        flag.teste = 1;
        char bt;
//        printf("Teste US\n\r");
        Lcd_Clear();
        strcpy(str1,teste_US1_str[idioma]);
        ltoa(str,freq_inicial,10);
        strcat(str1,str);
        strcat(str1," Hz");
        escreve_LCD(1,1,str1);
        escreve_LCD(2,1,Sai_enter_str[idioma]);
        set_freq(freq_inicial);
        flag.tela = 1;
        char conta_erro = 0;
        do{
            CLRWDT();
            bt = teclado();
            ver_entrada();
            if(flag_in.emerg){
                emergencia();//trava gerador
                Lcd_Clear();
                strcpy(str1,teste_US1_str[idioma]);
                ltoa(str,freq_inicial,10);
                strcat(str1,str);
                strcat(str1," Hz");
                escreve_LCD(1,1,str1);
                escreve_LCD(2,1,Sai_enter_str[idioma]);
                set_freq(freq_inicial);
            }
//            else if(bt == up){
//                if(freq_inicial < limite_22)
//                    freq_inicial +=10;
//                strcpy(str1,teste_US1_str[idioma]);
//                ltoa(str,freq_inicial,10);
//                strcat(str1,str);
//                strcat(str1," Hz");
//                escreve_LCD(1,1,str1);
//                set_freq(freq_inicial);
//            }
//            else if (bt == down){
//                if(freq_inicial > limite_17)
//                    freq_inicial -=10;
//                strcpy(str1,teste_US1_str[idioma]);
//                ltoa(str,freq_inicial,10);
//                strcat(str1,str);
//                strcat(str1," Hz");
//                escreve_LCD(1,1,str1);
//                set_freq(freq_inicial);
//            }
            else if(flag.inicia){
                start = 1;
                flag.US = 1;
                flag.isr_pot = 0;
                do{
                    ver_entrada();
                    if(flag_in.emerg){ erro = ERRO_EMERG;}   //filtro
                    else if(ovl){ erro = ERRO_INVERSOR;}                                                //filtro 
                    else if(flag.isr_pot){erro = ERRO_OVL;}    // sobrecarga                     
                    else {erro = ver_freq(get_freq(5));}
                    if(erro > 0){
                        conta_erro++;
                        if(conta_erro > LIMITE_ERRO) {                                           //filtro de erros
                            flag.abortar = 1;
                        }
                    }
                    else{
                        conta_erro = 0;
                    }

                }while(!flag.abortar && (flag_in.aciona1 || flag_in.aciona2));
                flag_in.liberado = 0;
                start = 0;
                flag.US = 0;
                if(flag.abortar){ 
                    start = 0;
                    led.ERR = 1;
                    Lcd_Clear();
                    if(erro == ERRO_EMERG){
                        emergencia();
                        escreve_LCD(1,1,emergencia_str[idioma]);
                    }
                    else if(ovl){
                        led.OVL = 1;
                        escreve_LCD(1,1,erro_inv_str[idioma]);
                        //escreve_LCD(2,1,"Ver Freq Inicial");
                        do{
                            __delay_ms(500);
                            rst_inversor(); 
                        }while(ovl);
                        led.OVL = 0;
                    }
                    else if(erro == ERRO_OVL){
                        escreve_LCD(1,1,erro_ovl_str[idioma]);
                    }
                    else if (erro < 100){                
                        escreve_LCD(1,1,erro_str[idioma]);
                        ltoa(str,erro,10);
                        Lcd_Write_String(str);
                    }
                    trava_erro("\0");
                    led.ERR = 0;
                    flag.abortar = 0;
                    Lcd_Clear();
                    strcpy(str1,teste_US1_str[idioma]);
                    ltoa(str,freq_inicial,10);
                    strcat(str1,str);
                    strcat(str1," Hz");
                    escreve_LCD(1,1,str1);
                    escreve_LCD(2,1,Sai_enter_str[idioma]);
                    set_freq(freq_inicial);
                }
                else{
                    salva_EEPROM(freq_inicial,ee_freq_ini);
                    freq_atual = freq_inicial;
                    set_freq(freq_atual);
                }
                liberar();                              
            }
            else{
                start = 0;
                bt = teclado();
                if(bt == enter){
                    flag.tela = 0;
                    liberar_tecla(enter,1000);
                }
            }
        }while(flag.tela);
//        printf("F1 pressed\n\r");
        flag.abortar = 0;
        flag.inicia = 0;
    }
    else{
//        printf("F1 hold\n\r");
    }
    flag.teste = 0;
}

void status_gerador(opc_saidas x, unsigned b){
    switch (x){
        case OUT_NC:
            break;
        case PNE_DESCE:
            if(cfg.pne_desce)
                saida(x,b);
            break;
        case PNE_SOBE:
            if(cfg.pne_sobe)
                saida(x,b); 
            break;
        case ERRO:
            if(cfg.trava_erro)
                saida(x,b); 
            break;
        case ERRO_ENE:
            if(cfg.energ_erro)
                saida(x,b); 
            break;
        case STATUS_GERADOR:
            if(cfg.status_out)
                saida(x,b); 
            break;
        case STATUS_HF:
            if(cfg.status_hf)
                saida(x,b); 
            break;
        case RST_INV:
            if(cfg.rst_inv)
                saida(x,b); 
            break;
    }
}

void PWM2_Initialize(void)
{
    // Set the PWM2 to the options selected in the User Interface
	
	// MODE PWM; EN enabled; CCP2FMT right_aligned; 
	CCP2CON = 0x8F;    
	
    PWM2_LoadDutyValue(511);
//    
//	// RH 1; 
//	CCPR2H = 0x01;    
//	
//	// RL 255; 
//	CCPR2L = 0xFF;    

	// Selecting Timer 6
	CCPTMRS0bits.C2TSEL = 0x3;
    
}

void PWM2_LoadDutyValue(uint16_t dutyValue)
{
    dutyValue &= 0x03FF;
    
    // Load duty cycle value
    if(CCP2CONbits.CCP2FMT)
    {
        dutyValue <<= 6;
        CCPR2H = dutyValue >> 8;
        CCPR2L = dutyValue;
    }
    else
    {
        CCPR2H = dutyValue >> 8;
        CCPR2L = dutyValue;
    }
}

void SMT1_Initialize(void)
{
    // WPOL high/rising edge enabled; SMT1STP rolls over to 24'h000000; SMT1SPOL high/rising edge enabled; SMT1EN enabled; SMT1PS 1:1 Prescaler; SMT1CPOL rising edge; 
    SMT1CON0 = 0x80;

    // SMT1REPEAT Repeat Data Acquisition; SMT1MODE Period and Duty-Cycle Acquisition; SMT1GO disabled; 
    SMT1CON1 = 0x42;

    // SMT1CPWUP SMT1CPW1 update complete; SMT1CPRUP SMT1PR1 update complete; SMT1RST SMT1TMR1 update complete; 
    SMT1STAT = 0x00;

    // SMT1CSEL FOSC; 
    SMT1CLK = 0x01;

    // SMT1WSEL SMT1WINPPS; 
    SMT1WIN = 0x00;

    // SMT1SSEL SMT1SIGPPS; 
    SMT1SIG = 0x00;

    // SMT1PR 0; 
    SMT1PRU = 0x00;

    // SMT1PR 0; 
    SMT1PRH = 0x00;

    // SMT1PR 0; 
    SMT1PRL = 0x00;

    // Start the SMT module by writing to SMTxGO bit
    SMT1CON1bits.SMT1GO = 1;
    
}

uint32_t SMT1_GetPeriod()
{
    return (SMT1PR);
}

uint32_t SMT1_GetCapturedPulseWidth()
{
    return (SMT1CPW);
}

uint32_t SMT1_GetCapturedPeriod()
{
    return (SMT1CPR);
}

void SMT2_Initialize(void)
{
    // WPOL high/rising edge enabled; SMT1STP rolls over to 24'h000000; SMT1SPOL high/rising edge enabled; SMT1EN enabled; SMT1PS 1:1 Prescaler; SMT1CPOL rising edge; 
    SMT2CON0 = 0x80;

    // SMT1REPEAT Repeat Data Acquisition; SMT1MODE Period and Duty-Cycle Acquisition; SMT1GO disabled; 
    SMT2CON1 = 0x42;

    // SMT1CPWUP SMT1CPW1 update complete; SMT1CPRUP SMT1PR1 update complete; SMT1RST SMT1TMR1 update complete; 
    SMT2STAT = 0x00;

    // SMT1CSEL FOSC; 
    SMT2CLK = 0x01;

    // SMT1WSEL SMT1WINPPS; 
    SMT2WIN = 0x00;

    // SMT1SSEL CLC1OUT; 
    SMT2SIG = 0x15;

    // SMT1PR 0; 
    SMT2PRU = 0x00;

    // SMT1PR 0; 
    SMT2PRH = 0x00;

    // SMT1PR 0; 
    SMT2PRL = 0x00;

    // Start the SMT module by writing to SMTxGO bit
    SMT2CON1bits.SMT2GO = 1;
    
}

uint32_t SMT2_GetPeriod()
{
    return (SMT2PR);
}

uint32_t SMT2_GetCapturedPulseWidth()
{
    return (SMT2CPW);
}

uint32_t SMT2_GetCapturedPeriod()
{
    return (SMT2CPR);
}

unsigned char get_duty(void){
   return (unsigned char)((SMT2CPW*100)/SMT2CPR);// retorna de 50 a 100%
}

void TMR1_Initialize(void)
{
    //Set the Timer to the options selected in the GUI

    //T1GE disabled; T1GTM disabled; T1GPOL low; T1GGO done; T1GSPM disabled; 
    T1GCON = 0x00;

    //GSS T1G_pin; 
    T1GATE = 0x00;

    //CS 500khz; 
    T1CLK = 0x05;

    //TMR1H 0; 
    TMR1H = 0xEC; //0x38; //0xFE;

    //TMR1L 0; 
    TMR1L = 0x77; //0xFF; //0x0C;

    // Load the TMR value to reload variable
    timer1ReloadVal=(uint16_t)((TMR1H << 8) | TMR1L);

    // Clearing IF flag.
    PIR4bits.TMR1IF = 0;
    
    PIE4bits.TMR1IE = 1;

    // CKPS 1:1; nT1SYNC do_not_synchronize; TMR1ON enabled; T1RD16 enabled; 
    T1CON = 0x07;
    
//    //************************** TIMER 3 (Capture) ***************************//
//    T3GCON = 0x00;
//    T3GATE = 0x00;
//    //CS FOSC; 
//    T3CLK = 0x02;
//    //TMR1H 0; 
//    TMR3H = 0x00; //0x38; //0xFE;
//    //TMR1L 0; 
//    TMR3L = 0x00; //0xFF; //0x0C;
//    // Clearing IF flag.
//    PIR4bits.TMR3IF = 0;
//    PIE4bits.TMR3IE = 0;
//    // CKPS 1:1; nT1SYNC do_not_synchronize; TMR1ON enabled; T1RD16 enabled; 
//    T3CON = 0x07;    
}

uint16_t TMR1_ReadTimer(void)
{
    uint16_t readVal;
    uint8_t readValHigh;
    uint8_t readValLow;
    
    T1CONbits.T1RD16 = 1;
	
    readValLow = TMR1L;
    readValHigh = TMR1H;
    
    readVal = ((uint16_t)readValHigh << 8) | readValLow;

    return readVal;
}

void TMR1_WriteTimer(uint16_t timerVal)
{
    if (T1CONbits.nT1SYNC == 1)
    {
        // Stop the Timer by writing to TMRxON bit
        T1CONbits.TMR1ON = 0;

        // Write to the Timer1 register
        TMR1H = (timerVal >> 8);
        TMR1L = timerVal;

        // Start the Timer after writing to the register
        T1CONbits.TMR1ON =1;
    }
    else
    {
        // Write to the Timer1 register
        TMR1H = (timerVal >> 8);
        TMR1L = timerVal;
    }
}

void TMR1_Reload(void)
{
    TMR1_WriteTimer(timer1ReloadVal);
}

void TMR5_Initialize(void)
{ 
    T5GCON = 0x00;
    //GSS T5G_pin; 
    T5GATE = 0x00;
    //CS MFINTOSC_500kHz; 
    T5CLK = 0x05;
    //TMR5H 254; 
    TMR5H = 0xFE;
    //TMR5L 12; 
    TMR5L = 0x0C;
    // Load the TMR value to reload variable
    timer5ReloadVal=(uint16_t)((TMR5H << 8) | TMR5L);
    // CKPS 1:1; nT5SYNC synchronize; TMR5ON enabled; T5RD16 enabled; 
    T5CON = 0x03;
    // Clearing IF flag before enabling the interrupt.
    PIR4bits.TMR5IF = 0;
    // Enabling TMR5 interrupt.
    PIE4bits.TMR5IE = 1;
    
    T5CONbits.TMR5ON = 0;
}
void TMR5_WriteTimer(uint16_t timerVal)
{
        TMR5H = (timerVal >> 8);
        TMR5L = timerVal;
}
void TMR5_Reload(void)
{
    TMR5_WriteTimer(timer5ReloadVal);
}
void start_timer(unsigned int t){
    t1ms_OP = t;
    t1ms_ISR = 0;
    t1msMM_ISR = 0;
    T5CONbits.TMR5ON = 1;
}
void stop_timer(void){
    T5CONbits.TMR5ON = 0;
}

void TMR6_Initialize(void)
{
    // Set TMR6 to the options selected in the User Interface

    // T6CS FOSC/4; 
    T6CLKCON = 0x01;

    // T6PSYNC Not Synchronized; T6MODE Software control; T6CKPOL Rising Edge; T6CKSYNC Not Synchronized; 
    T6HLT = 0x00;

    // T6RSEL T6CKIPPS pin; 
    T6RST = 0x00;

    // PR6 255; 
    T6PR = 0xFF;

    // TMR6 0; 
    T6TMR = 0x00;

    // Clearing IF flag.
    PIR4bits.TMR6IF = 0;
    PIE4bits.TMR6IE = 0;

    // T6CKPS 1:1; T6OUTPS 1:1; TMR6ON on; 
    T6CON = 0x00;
    T6CONbits.CKPS = 1;
    T6CONbits.ON = 1;
    
//    //************************************\timer 3 para capture
//    T3GCON = 0x00;
//    //GSS T5G_pin; 
//    T3GATE = 0x00;
//    //CS FOSC; 
//    T3CLK = 0x02;
//    TMR3H = 0x00; 
//    TMR3L = 0x00;
//    // CKPS 1:1; nT3SYNC synchronize; TMR3ON enabled; T3RD16 enabled; 
//    T3CON = 0x03;
//    // Clearing IF flag before enabling the interrupt.
//    PIR4bits.TMR3IF = 0;
//    // Enabling TMR5 interrupt.
//    PIE4bits.TMR3IE = 0;
}

void SYSTEM_Initialize(void)
{
    PMD_Initialize();
    PIN_MANAGER_Initialize();
    OSCILLATOR_Initialize();
    Inicia_LCD();
    TMR6_Initialize();
    PWM2_Initialize();
    CLC1_Initialize();
    SMT1_Initialize();
    SMT2_Initialize();
    TMR5_Initialize();
    TMR1_Initialize();
    ADCC_Initialize();
    EUSART_Initialize();
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
}

void OSCILLATOR_Initialize(void)
{
    // NOSC EXTER; NDIV 1; 
    OSCCON1 = 0x70;
    // CSWHOLD may proceed; SOSCPWR Low power; 
    OSCCON3 = 0x00;
    // MFOEN disabled; LFOEN disabled; ADOEN disabled; SOSCEN disabled; EXTOEN disabled; HFOEN disabled; 
    OSCEN = 0x00;
    // HFFRQ 32_MHz; 
    OSCFRQ = 0x06;
    // HFTUN 0; 
    OSCTUNE = 0x00;
}

void PMD_Initialize(void)
{
    // CLKRMD CLKR enabled; SYSCMD SYSCLK enabled; SCANMD SCANNER enabled; FVRMD FVR enabled; IOCMD IOC enabled; CRCMD CRC enabled; NVMMD NVM enabled; 
    PMD0 = 0x00;
    // TMR0MD TMR0 enabled; TMR1MD TMR1 enabled; TMR4MD TMR4 enabled; TMR5MD TMR5 enabled; TMR2MD TMR2 enabled; TMR3MD TMR3 enabled; NCOMD DDS(NCO) enabled; TMR6MD TMR6 enabled; 
    PMD1 = 0x00;
    // ZCDMD ZCD enabled; DACMD DAC enabled; CMP1MD CMP1 enabled; ADCMD ADC enabled; CMP2MD CMP2 enabled; 
    PMD2 = 0x00;
    // CCP2MD CCP2 enabled; CCP1MD CCP1 enabled; CCP4MD CCP4 enabled; CCP3MD CCP3 enabled; CCP5MD CCP5 enabled; PWM6MD PWM6 enabled; PWM7MD PWM7 enabled; 
    PMD3 = 0x00;
    // CWG3MD CWG3 enabled; CWG2MD CWG2 enabled; CWG1MD CWG1 enabled; MSSP1MD MSSP1 enabled; UART1MD EUSART enabled; MSSP2MD MSSP2 enabled; 
    PMD4 = 0x00;
    // DSMMD DSM enabled; CLC3MD CLC3 enabled; CLC4MD CLC4 enabled; SMT1MD SMT1 enabled; SMT2MD SMT2 enabled; CLC1MD CLC1 enabled; CLC2MD CLC2 enabled; 
    PMD5 = 0x00;
}

void CLC1_Initialize(void){
    CLC1POL  = 0x00;
    CLC1SEL0 = 0x00;
    CLC1SEL1 = 0x00;
    CLC1SEL2 = 0x00;
    CLC1SEL3 = 0x00;
    CLC1GLS0 = 0xAA;
    CLC1GLS1 = 0xAA;
    CLC1GLS2 = 0xAA;
    CLC1GLS3 = 0xAA;
    CLC1CON  = 0x80;
}

void ADCC_Initialize(void)
{
    // set the ADCC to the options selected in the User Interface
    // ADLTHL 0; 
    ADLTHL = 0x00;
    // ADLTHH 0; 
    ADLTHH = 0x00;
    // ADUTHL 0; 
    ADUTHL = 0x00;
    // ADUTHH 0; 
    ADUTHH = 0x00;
    // ADSTPTL 0; 
    ADSTPTL = 0x00;
    // ADSTPTH 0; 
    ADSTPTH = 0x00;
    // ADRPT 0; 
    ADRPT = 0x00;
    // ADPCH ANA0; 
    ADPCH = 0x00;
    // ADCAP Additional uC disabled; 
    ADCAP = 0x00;
    // ADDSEN disabled; ADGPOL digital_low; ADIPEN disabled; ADPPOL VSS; 
    ADCON1 = 0x00;
    // ADCRS 0; ADMD Basic_mode; ADACLR disabled; ADPSIS ADRES; 
    ADCON2 = 0x00;
    // ADCALC First derivative of Single measurement; ADTMD disabled; ADSOI ADGO not cleared; 
    ADCON3 = 0x00;
    // ADAOV ACC or ADERR not Overflowed; 
    ADSTAT = 0x00;
    // ADNREF VSS; ADPREF VDD; 
    ADREF = 0x00;
    // ADACT disabled; 
    ADACT = 0x00;
    // ADCCS FOSC/2; 
    ADCLK = 0x00;
    // ADGO stop; ADFM right; ADON enabled; ADCONT disabled; ADCS FRC; 
    ADCON0 = 0x94;
    
    FVRCONbits.TSEN = 1;
    FVRCONbits.TSRNG = 1;
    
    

}
float get_temp(float K){
    float adc = 0;
    float adc_c = 0; 
    ADCON0bits.ADON = 1;
    ADCON0bits.ADCONT = 0; 
    for(char i = 0;i<10;i++){
        ADPCH = 0x3C;
        __delay_us(500);
        ADPCH = 0x3D;
        __delay_us(500);
        ADCON0bits.ADGO = 1;
        while (ADCON0bits.ADGO){CLRWDT();}
        adc += (float)((ADRESH << 8) + ADRESL);
    }
    adc/=10;
    adc_c = adc + K;
    
//    float temp = ((0.659 -((5/4)*(1-(adc/1023))))/0.00132)-40;
        
//    printf("adc = %.2f | adc_c = %.2f\n\r",adc,adc_c);
    return (adc_c);
}
char conv_temp(float adc){
    float temp = adc;
    temp = temp/1023;       //printf("%f\n\r",temp);
    temp = 1-temp;          //printf("%f\n\r",temp);
    temp = temp*5/4;        //printf("%f\n\r",temp);
    temp = 0.659-temp;      //printf("%f\n\r",temp);
    temp = temp/0.00132;    //printf("%f\n\r",temp);
    temp = temp - 40;       //printf("T = %.3f\n\r",temp);
    temp = round(temp);     //printf("T = %.3f\n\r",temp);
    return (char)temp;
}

void selectModo(void){  //local!
    char loop = 1;
    char b;
    char * s[4];
        
    s[0] = modo_normal_str[idioma];
    s[1] = modo_rapido_str[idioma];
    s[2] = modo_contnuo_str[idioma];
    s[3] = modo_contByPulse_str[idioma];
    
    Lcd_Clear();
    escreve_LCD(1,1,modo_op_str[idioma]);
    escreve_LCD(2,1,s[modo]);
    
    while(loop){
        b = teclado();
        switch(b){
            case up:
                if(modo == NORMAL)
                    modo = TAMANHO_MODO;
                else modo--;
                escreve_LCD(2,1,s[modo]);
                break;
            case down:
                if(modo == TAMANHO_MODO)
                    modo = NORMAL;
                else modo++;
                escreve_LCD(2,1,s[modo]);
                break;
            case enter:
                loop = 0;
                break;
            default:
                break;
        }
        if(b > btt_min)
            liberar_tecla(b,100);
    }
    eeprom_write(ee_modo,modo);
    alarme = 1;
    __delay_ms(500);
    alarme = 0;   
    
}


void ini_ee_tempos(float t,opc_maquinas tipo){
    char b1 = teclado();
    char b2 = teclado();
    if(b1 != b2){
        b1 = 0;
        b2 = 0;
    }
//    __delay_ms(1000);
    
    if(eeprom_read(ee_ini)){
//        printf("limpa EEPROM\n\r");
        __delay_ms(1000);
        alarme = 1;
        CLRWDT();
        for(unsigned char i = 0; i<0xFF;i++){
            CLRWDT();
            eeprom_write(i,0);
        }
        switch (Tipo_Gerador){
            case SOW280:
            case SOW160:
                freq_inicial = 20100;
                break;
            case SOW120:
            case SOW100:
                freq_inicial = 35100;
                break;
            case SOWESP:
                freq_inicial = 15100;
                break; 
        }
//        printf("ee freqIni = %i\n\r",freq_inicial);
        salva_EEPROM(200,ee_desce);
        salva_EEPROM(1000,ee_solda);
        salva_EEPROM(1000,ee_joule);
        salva_EEPROM(200,ee_fixar);
        salva_EEPROM(10,ee_metal);
        salva_EEPROM(freq_inicial,ee_freq_ini);
        salva_EEPROM32(0,ee_flag);        
        salva_EEPROM32(200,ee_cont);    //contador regressivo de quantas pecas precisam ser produzidas.
        
        eeprom_write(ee_modo,modo);
        eeprom_write(ee_maquina,tipo);   //qual a maquina esta configurada
        eeprom_write(ee_idioma,idioma);  //Salva Idioma
        
        salva_EEPROM32(N_PECAS,ee_total_p);
        salva_EEPROM32(T_100ms,ee_t1s);
        
        eeprom_write(ee_senha0,'0');
        eeprom_write(ee_senha1,'0');
        eeprom_write(ee_senha2,'0');
        eeprom_write(ee_senha3,'0');
        
        // Algoritimo para senha individual de cada gerador é baseado no numero de Serie:
        //  * Dig1 = (Dig2*7 + Dig3*11 + Dig4*13)%10
        //  * Dig2 = (Dig1*7 + Dig3*11 + Dig4*13)%10
        //  * Dig3 = (Dig2*7 + Dig1*11 + Dig4*13)%10
        //  * Dig4 = (Dig2*7 + Dig3*11 + Dig1*13)%10
        
        int _num[4];
        _num[0] = Num_Serie[4] - '0';
        _num[1] = Num_Serie[5] - '0';
        _num[2] = Num_Serie[6] - '0';
        _num[3] = Num_Serie[7] - '0';

        _num[0] = ((_num[1]*7 + _num[2]*11 + _num[3]*13)%10) + '0';
        _num[1] = ((_num[0]*7 + _num[2]*11 + _num[3]*13)%10) + '0';
        _num[2] = ((_num[1]*7 + _num[0]*11 + _num[3]*13)%10) + '0';
        _num[3] = ((_num[1]*7 + _num[2]*11 + _num[0]*13)%10) + '0';
        
        eeprom_write(ee_senha4,(char)_num[0]);
        eeprom_write(ee_senha5,(char)_num[1]);
        eeprom_write(ee_senha6,(char)_num[2]);
        eeprom_write(ee_senha7,(char)_num[3]);        
        
        float K = 5-(4*(0.659-((t+40)*0.00132)));
        K = (1023*K)/5;
//        printf("adc@%.2f*C = %.2f\n\r",t,K);
//        printf("adc@atual = %.2f\n\r",get_temp(0));
        K = K - get_temp(0);
        int k_int = (int)(K*100);
        salva_EEPROM(k_int,ee_temp);
//        printf("K = %i | eeK = %i\n\r",k_int,(int)carrega_EEPROM(ee_temp));
        alarme = 0;
    }
    __delay_ms(500);
    Lcd_Clear();
    Lcd_White_Center_String(1,modelo_str[Tipo_Gerador]);
    Lcd_White_Center_String(2,Versao_Soft);
    
    idioma = eeprom_read(ee_idioma); 
    tipo_maquina = eeprom_read(ee_maquina);
    modo = eeprom_read(ee_modo);
    
    tmp_desce  = carrega_EEPROM(ee_desce);
    tmp_solda  = carrega_EEPROM(ee_solda);
    ener_solda = carrega_EEPROM(ee_joule);
    tmp_fixar  = carrega_EEPROM(ee_fixar);
    tmp_metal  = carrega_EEPROM(ee_metal);
    freq_inicial = carrega_EEPROM(ee_freq_ini);
//    printf("freqIni = %i\n\r",freq_inicial);
    flag.value  |= (7&carrega_EEPROM(ee_flag));                                 //carrega solda por energia e contador.
    
    cont_pecas = carrega_EEPROM32(ee_cont);
    n_pecas    = carrega_EEPROM32(ee_pecas);
    n_pecas_l  = carrega_EEPROM32(ee_total_p);
    t100ms_solda  = carrega_EEPROM32(ee_t1s);

    k_temp = (float)(((int)carrega_EEPROM(ee_temp)))/100;
    
    switch(b1){
        case enter: // teste ultrassom
            ciclador = 1;
            bip_tecla();
            bip_tecla();
            bip_tecla();
            break;
        case F1:
            selectModo();
            break;
        case F2:    //Teste Pneumatico
            ciclador = 2;
            bip_tecla();
            bip_tecla();
            bip_tecla();
            break;
    }
    
}

void ciclar(void){    
    unsigned char etapa = 0;
    unsigned char bt;
    unsigned int ciclos = 0;    
    unsigned char temp = (unsigned char)cfg.stop_release;
    unsigned long temp_n_pecas = n_pecas;
    
    Lcd_Clear();
    trava_erro("MODO CICLADOR");
    
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String(freq_str);
    ltoa(str,freq_inicial,10);
    Lcd_Write_String(str);
    Lcd_Write_String("   ");   
    escreve_freq();
    cfg.stop_release = 0;
    status_gerador(STATUS_GERADOR,1);
    set_freq(freq_inicial);
    
    while (etapa != 100){
        ///liberar();
        bt = teclado();
        switch (bt){
            default:
                if(etapa == 10){     // etapa = 10, ciclando
                    flag.sussess = 1;
                    op_desce(0);
                    op_solda(tmp_solda);
                    op_fixar(0);
                    flag_in.liberado = 0;
                    trata_dados();
                    set_freq(freq_atual);
                    if(flag.abortar){
                        etapa = 0;
                        trava_erro("Erro ao Ciclar");
                    }
                    else if(ciclos >= (unsigned int)cont_pecas){
                        etapa = 0;  //parar ciclador.
                        trava_erro("Fim da contagem");
                    }
                    else {
                        ciclos++;
                        Lcd_Set_Cursor(1,1);
                        Lcd_Write_String("CICLOS:");
                        ltoa(str,ciclos,10);
                        Lcd_Write_String(str);
                        Lcd_Write_String("    ");
                        escreve_freq();
                        delay(tmp_desce/10);
                    }
                }
                break;
            
            case menu:
            case rst:
                if(etapa == 10){
                    ciclos = 0;
                    etapa = 0;
                    trava_erro("Interrompido");
                }
                else{  
                    etapa = 100; //Sai do ciclador1
                }
                break;
                
            case enter:
                if(etapa != 10){
                    etapa = 10;  //inicia ciclos.
                    ciclos = 0;
                }
                break;
            case up:
                if(etapa == 10){
                    trava_erro("Interrompido");
                    ciclos = 0;
                    etapa = 0;
                }                
                
                freq_inicial += 10;
                set_freq(freq_inicial);
                
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String(freq_str);
                ltoa(str,freq_inicial,10);
                Lcd_Write_String(str);
                Lcd_Write_String("   ");
                break;
                
            case down:
                if(etapa == 10){
                    trava_erro("Interrompido");
                    ciclos = 0;
                    etapa = 0;
                }                
                
                freq_inicial -= 10;
                set_freq(freq_inicial);
                
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String(freq_str);
                ltoa(str,freq_inicial,10);
                Lcd_Write_String(str);
                Lcd_Write_String("   ");
                break;
                
            case rigth:
            case left:
            case F1:
            case F2:
                if(etapa == 10){
                    ciclos = 0;
                    etapa = 0;
                    trava_erro("Interrompido");
                }
                break;
        }
    }
    n_pecas = temp_n_pecas;
    cfg.stop_release = temp;

}

void ciclarPneumatico(void){
    uint16_t contarDescidas = 0;
    uint16_t contarTempo = 0;
    Lcd_Clear();
    trava_erro("Teste Pneum!");
    
    while(teclado() != rst){
        
        contarDescidas += 1;
        
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("Ciclos:");
        ltoa(str,contarDescidas,10);
        Lcd_Write_String(str);
        Lcd_Write_String("   ");
        
        saida(PNE_DESCE,1);
        contarTempo = t10ms;
        while((t10ms - contarTempo) < (tmp_solda/10)){
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("time:");
            ltoa(str,t10ms - contarTempo,10);
            Lcd_Write_String(str);
            Lcd_Write_String("   ");
            if(teclado() == enter){
                saida(PNE_DESCE,0);
                return;
            }
        }
        
        saida(PNE_DESCE,0);
        contarTempo = t10ms;
        while((t10ms - contarTempo) < (tmp_fixar/10)){
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("time:");
            ltoa(str,t10ms - contarTempo,10);
            Lcd_Write_String(str);
            Lcd_Write_String("   ");
            if(teclado() == enter){
                saida(PNE_DESCE,0);
                return;
            }
        }  
    }
}

void rotina_tela(void){
    static uint8_t bt;
#ifdef DEBUG debug(__LINE__); 
#endif;
//    while(0 == liberar_tecla(menu,50));
    //Senha entra aqui.
    //Retorna se der erro na senha...
#ifdef DEBUG debug(__LINE__); 
#endif;
    delay(10);
    flag.tela = 1;
    flag.muda_tela = 1;
    pagina = verifPagina(pagina,rigth);
    do{
        CLRWDT();
#ifdef DEBUG debug(__LINE__); 
#endif;
        bt = teclado();
#ifdef DEBUG debug(__LINE__); 
#endif;
        if(bt > btt_min){
            flag.muda_tela = 1;
            if(!flag.rapido){
                if(0 == liberar_tecla(bt,500) && (bt==up || bt==down)){
                    flag.rapido = 1;
                }
            }
            else if(bt == up || bt == down){
                if(1 == liberar_tecla(bt,50)){
                        flag.rapido = 0;
                        delay(20);
                }
            }
            else{
                flag.rapido = 0;
            }
        }
        if(flag.muda_tela){
            telas(bt);
            flag.muda_tela = 0;
        }
        if(bt == menu){
            flag.tela = 0;
        }
    }while(flag.tela);
    bt = 0;
//                printf(">>Sai menu...\n\r");
    
    eeprom_write(ee_maquina,tipo_maquina);
    eeprom_write(ee_idioma,idioma);
    salva_EEPROM(tmp_desce,ee_desce);
    salva_EEPROM(tmp_solda,ee_solda);
    salva_EEPROM(ener_solda,ee_joule);
    salva_EEPROM(tmp_fixar,ee_fixar);
    salva_EEPROM(tmp_metal,ee_metal);
    salva_EEPROM(freq_inicial,ee_freq_ini);
    salva_EEPROM32((7&flag.value),ee_flag);
    salva_EEPROM32(cont_pecas,ee_cont);
    
    set_freq(freq_atual);
    flag.inatividade = 0;
    t_tela = 0;
//                printf("Sai menu\n\r");
}

void escreve_freq(void){
    Lcd_Set_Cursor(2,1);
    ltoa(str,freq_atual,10);
    Lcd_Write_String(str);
    Lcd_Write_String(" Hz   ");
}

char verifPagina(char pag , char bt_local){ //pag -> 1 a TAMANHO_MENU
    char ok = 0;
#ifdef DEBUG debug(__LINE__); 
#endif;
    do{
        ok = 0;
        if(bt_local == rigth){          
            if(!cfg.pne_desce && pag == TELA_TMP_DESCE){
                 pag++;
                 ok++;
            }
            else if (pag == TELA_FREQ_INI){                                              //pula escolha freq inicial
                pag++;
                ok++;
            }
            else if (pag == TELA_TIPO_MAQU && tipo_maquina == ESPECIAL) {                //pula tipo de maquina se maq especial
                pag++;
                ok++;
            }
            else if(!cfg.metal && pag == TELA_METAL){
                pag++;
                ok++;
            }
            
            if(ok && pag > TAMANHO_MENU){ 
                pag = TELA_TMP_DESCE; 
            }
        }
        else if(bt_local == left){
            if(!cfg.pne_desce && pag == TELA_TMP_DESCE){
                 pag = TAMANHO_MENU; 
                 ok++;
            }
            else if (pag == TELA_TIPO_MAQU && tipo_maquina == ESPECIAL){                 //pula tipo de maquina se maq especial
                pag--; 
                ok++;
            }
            else if (pag == TELA_FREQ_INI) {                                          //pula escolha freq inicial
                pag--;  
                ok++;
            }
            else if(!cfg.metal && pag == TELA_METAL){
                pag--;
                ok++;
            }
            
            if(ok && pag < TELA_TMP_DESCE){
                pag = TAMANHO_MENU;
            }
        }
    }while(ok);
#ifdef DEBUG debug(__LINE__); 
#endif    
    return pag;
}

#ifdef DEBUG
void debug(int n){
    static char s[5];
    sprintf(s,"%i",n);
    escreve_LCD(2,12,s);
}
#endif