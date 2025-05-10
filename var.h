#define _XTAL_FREQ  20000000
//#define DEBUG

#define INPUT       1
#define OUTPUT      0
#define HIGH        1
#define LOW         0
#define ANALOG      1
#define DIGITAL     0

#define id_btt      (PORTD&0b00111111)
#define PORT_BTT    LATD
#define btt_min     8
#define F2          36 
#define rigth       34 
#define F1          33 
#define down        20  
#define enter       18  
#define up          17  
#define rst         12  
#define left        10  
#define menu        9 

#define i_us        PORTAbits.RA0
#define in5         PORTAbits.RA1
#define in4         PORTAbits.RA2
#define in3         PORTAbits.RA3
#define in2         PORTAbits.RA4 
#define in1         PORTAbits.RA5

#define alarme      LATBbits.LATB7

#define out1        LATCbits.LATC0
#define out2        LATCbits.LATC5
#define out3        LATCbits.LATC4
#define out4        LATCbits.LATC3

#define start       LATDbits.LATD6
#define is_start    PORTDbits.RD6
#define ovl         PORTDbits.RD7

#define clk_led     LATEbits.LATE0
#define data_led    LATEbits.LATE1
#define clr_led     LATEbits.LATE2
/********************* EEPROM ADDRESS **********************/
#define ee_ini      0
#define ee_total_p  1       //long 1 2 3 4;
#define ee_pecas    5       //long 5 6 7 8      /////???? EEERRROOO o proximo é 9!!!

#define ee_receita_1 9      //+16
#define ee_receita_2 25     //
#define ee_receita_3 41     //
#define ee_receita_4 57     //
#define ee_receita_5 73     //
#define ee_receita_6 89     //
#define ee_receita_7 105     //
#define ee_receita_8 121     //
#define ee_receita_9 137     //
#define ee_receita_10 153    //9 - 168
// vazios 169-195 = 26
#define ee_modo     196
#define ee_maquina  198
#define ee_desce    200     
#define ee_solda    202     
#define ee_joule    204     
#define ee_fixar    206     
#define ee_metal    208
#define ee_flag     210     //32bits
#define ee_cont     214     //32bits
#define ee_freq_ini 218     //16bits
#define ee_t1s      220     //32bits
//vazios 228-239 = 20

#define ee_senha0   230
#define ee_senha1   231
#define ee_senha2   232
#define ee_senha3   233

#define ee_senha4   234
#define ee_senha5   235
#define ee_senha6   236
#define ee_senha7   237

#define ee_temp     240
#define ee_idioma   250

/********************* EEPROM ADDRESS END!!! **********************/

#define ERRO_EMERG      250
#define ERRO_INVERSOR   200
#define ERRO_OVL        100

#define LIMITE_ERRO     5

#define duty_OVL        90

#define status_timer    T5CONbits.TMR5ON
#define MATRIZ_TEMPO    50

typedef enum {
    PORTUGUES,
    INGLES,
    TAMANHO_IDIOMA
}opc_idiomas;

typedef enum {
    ESPECIAL,
    PISTOLA_PULSADO,
    MAQUINA_PULSADO,
    MAQUINA_MM,
    TAMANHO_MAQUINAS
}opc_maquinas;

typedef enum {
    SOW280,
    SOW160,
    SOW120,
    SOW100,
    SOWESP,
    TAMANHO_GERADOR
}opc_gerador;

typedef enum {
    NORMAL,
    RAPIDO,
    CONTINUO,
    CONTINUO_PULSO,
    TAMANHO_MODO = CONTINUO_PULSO
}modo_maquina;

typedef enum {
    PARADO,
    DESCENDO,
    SOLDANDO,
    FIXANDO,
    ERRO_SOLDA,
}status_solda;

typedef enum {
    TELA_NC,
    TELA_TMP_DESCE,
    TELA_TMP_SOLDA,
    TELA_TMP_JOULE,
    TELA_TIPO_SOLDA,
    TELA_TMP_FIXAR,
    TELA_METAL,
    TELA_TMP_CONT,
    TELA_TIPO_CONT,
    TELA_FREQ_INI,
    TELA_TIPO_MAQU,
//    TELA_MODO_OP,
    TELA_TMP_LOAD,
    TELA_TMP_SAVE,
    TELA_OP_TMP,
    TELA_IDIOMA,    
    TAMANHO_MENU = TELA_IDIOMA
}opc_telas;

typedef enum {
    IN_NC,
    EMERGENCIA,
    ACIONAMENTO1,
    ACIONAMENTO2,
    RESET,
    METAL,
    SENSOR_PECA,
    SENSOR_PNE_AV,
    SENSOR_PNE_RC,
    SENSOR_HOLD,
    OPC_ENTRADA_MAX = SENSOR_HOLD
}opc_entradas;

typedef enum {
    IN_NONE,
    IN1,
    IN2,
    IN3,
    IN4,
    IN5,
    ENTRADA_MAX = IN5
}entradas;

typedef enum {
    OUT_NC,
    PNE_DESCE,
    PNE_SOBE,
    ERRO,
    ERRO_ENE,
    STATUS_GERADOR,
    STATUS_HF,
    RST_INV,
    OPC_SAIDA_MAX = RST_INV
}opc_saidas;

typedef enum {
    OUT_NONE,
    OUT1,
    OUT2,
    OUT3,
    OUT4,
    SAIDA_MAX = OUT4
}saidas;

typedef union{
    char byte;
    struct{
        unsigned b0:1,b1:1,b2:1,b3:1,b4:1,b5:1,b6:1,b7:1;
    };
}Bit_in_Byte;

/******************************************************************************/
/*********************************** Variaveis ********************************/
/******************************************************************************/
union{
    unsigned long value;
    struct {
        unsigned emergencia     :1; //emergencia
        unsigned aciona1        :1; //acionamento
        unsigned aciona2        :1; //acionamento
        unsigned pne_sobe       :1; //pneumatico
        unsigned pne_desce      :1; //pneumatico
        unsigned sensor_hold    :1; //tem sensor de trava.
        unsigned sensor_pne_av  :1; //tem sensor de cilindro avancado
        unsigned sensor_pne_rc  :1; //tem sensor de cilindro recuado
        unsigned sensor_peca    :1; //tem sensor de peca
        unsigned solda_cont     :1; //solda tipo continua
        unsigned trava_erro     :1; //sinal externo informa erro (saida)
        unsigned reset          :1; //sinal externo reconhece erro (entrada)
        unsigned metal          :1; //sinal externo parada Metal-Metal
        unsigned status_out     :1; //sinal externo busy (saida)
        unsigned status_hf      :1; //sinal externo US (saida)
        unsigned rst_inv        :1; //saida rele reseta inversor
        unsigned stop_release   :1; //Para solda quando botao é liberado
        unsigned energ_erro     :1; // habilita saida para erro de estouro de tempo ou energia
    };
} cfg;

union{
    volatile unsigned long value;
    struct { 
        volatile unsigned energia    :1;    //tipo de solda
        volatile unsigned energiaErro:1;    //
        volatile unsigned regressivo :1;    //tipo de contagem
        volatile unsigned sussess    :1;
        volatile unsigned inicia     :1;
        volatile unsigned US         :1;
        volatile unsigned abortar    :1;
        volatile unsigned isr_pot    :1;
        volatile unsigned tela       :1;
        volatile unsigned muda_tela  :1;
        volatile unsigned muda_valor :1;
        volatile unsigned salvar     :1;
        volatile unsigned rapido     :1;                
        volatile unsigned zero_cont  :1;
        volatile unsigned teste      :1;
        volatile unsigned inatividade:1;
        volatile unsigned metal      :1;
    };
} flag;

union{
    volatile unsigned int value;
    struct {
        volatile unsigned emerg:1;
        volatile unsigned aciona1:1;
        volatile unsigned aciona2:1;
        volatile unsigned sobrecarga:1;
        volatile unsigned liberar:1;
        volatile unsigned sensor_presenca:1;
        volatile unsigned sensor_pne_av:1;
        volatile unsigned sensor_pne_rc:1;
        volatile unsigned sensor_hold:1;
        volatile unsigned liberado :1;
        volatile unsigned metal:1;
    };
} flag_in;

union{
    volatile unsigned char value;
    struct{
        volatile unsigned HF    :1;
        volatile unsigned OVL   :1;
        volatile unsigned ERR   :1;
    };
    
}led;

union{
    volatile unsigned int value;
    struct{        
        volatile unsigned POT10 :1;
        volatile unsigned POT20 :1;
        volatile unsigned POT30 :1;
        volatile unsigned POT40 :1;
        volatile unsigned POT50 :1;
        volatile unsigned POT60 :1;
        volatile unsigned POT70 :1;
        volatile unsigned POT80 :1;
        volatile unsigned POT90 :1;
        volatile unsigned POT100:1;
    };
}bar;

union{
    volatile unsigned long value;
    struct{
        volatile unsigned int int0;
        volatile unsigned int int1;
    };
    struct{
        volatile unsigned char byte0;
        volatile unsigned char byte1;
        volatile unsigned char byte2;
        volatile unsigned char byte3;
    };
}ee;

union{
    volatile unsigned int duty;
    struct{
        volatile unsigned char
        byte0 : 8,
        byte1 : 8;
    };
}PWM;

union{
    volatile unsigned char byte;
    struct{
        volatile unsigned press      : 1;
        volatile unsigned release    : 1;
        volatile unsigned hold       : 1;
        volatile unsigned exe        : 1;
        volatile unsigned b4         : 1;
        volatile unsigned b5         : 1;
        volatile unsigned bi_ok      : 1;
        volatile unsigned timer      : 1;
    };
}btt;
union{
    volatile unsigned int data;
    struct
    {
        volatile unsigned char ccpr1l;
        volatile unsigned char ccpr1h;
    };
}periodo1;
union{
    volatile unsigned int data;
    struct{      
        volatile unsigned char ccpr1l;
        volatile unsigned char ccpr1h;
    };
}periodo2;

const char Sonitron_str[] =     "  SONITRON M:";// "SONITRON M:0"
const char freq_str[] =         "FREQ = ";
const char freq_ini_str[] =     "Freq Ini: (x";
const char branco_str[] =       "                ";


const char n_pecas_str          [TAMANHO_IDIOMA][17] =  {"Operacoes: "          ,"Cycles: "      };
const char n_pecas_total_str    [TAMANHO_IDIOMA][17] =  {"Total Operacoes:"     ,"Total Cycles: "};
const char t_solda_total_str    [TAMANHO_IDIOMA][17] =  {"Total Tempo:"         ,"Total Time: "};
const char descida_str          [TAMANHO_IDIOMA][17] =  {"T.Descida (x"         ,"Down Time (x"  };
const char solda_str            [TAMANHO_IDIOMA][17] =  {"T.Operacao(x"         ,"Weld Time (x"  };
const char joule_str            [TAMANHO_IDIOMA][17] =  {"Energia Op(x"         ,"Energy Op.(x"    };
const char ctrl_solda_str       [TAMANHO_IDIOMA][17] =  {"Ctrl. Solda "         ,"Weld Control "   };
const char ctrl_pecas_str       [TAMANHO_IDIOMA][17] =  {"Ctrl. Pecas "         ,"Parts Qty Ctrl"  };
const char fixacao_str          [TAMANHO_IDIOMA][17] =  {"T.Espera  (x"         ,"Hold Time (x"    };
const char metal_str            [TAMANHO_IDIOMA][17] =  {"T.M-Metal (x"         ,"M-M Time  (x" };    
const char tipo_maquina_str     [TAMANHO_IDIOMA][17] =  {"Tipo Maquina:"        ,"Operation Mode:" }; 
const char modo_op_str          [TAMANHO_IDIOMA][17] =  {"Selecione Modo:"      ,"Select Mode:"    }; 
const char modo_normal_str      [TAMANHO_IDIOMA][17] =  {"Normal         "      ,"Default"         }; 
const char modo_rapido_str      [TAMANHO_IDIOMA][17] =  {"Rapido         "      ,"Fast"            }; 
const char modo_contnuo_str     [TAMANHO_IDIOMA][17] =  {"Continuo       "      ,"Continuous"      }; 
const char modo_contByPulse_str [TAMANHO_IDIOMA][17] =  {"Cont. por pulso "     ,"Cont. by pulse"  }; 
const char cont_pecas_str       [TAMANHO_IDIOMA][17] =  {"Qt.Pecas  (x"         ,"Parts Qty (x"    }; 
const char salva_str            [TAMANHO_IDIOMA][17] =  {"Salvar Receita "      ,"Save Paramet Set"};
const char receita_salva_str    [TAMANHO_IDIOMA][17] =  {" Receita Salva "      ,"Param. Set Seved"};
const char receita_carreg_str   [TAMANHO_IDIOMA][17] =  {" Receita Carr. "      ,"Loaded Param Set"};
const char carrega_str          [TAMANHO_IDIOMA][17] =  {"Carregar Receita"     ,"Load Paramet Set"};
const char serial_str           [TAMANHO_IDIOMA][17] =  {"Config. Senha:  "     ,"Password:"       };
const char posicao_str          [TAMANHO_IDIOMA][17] =  {"Posicao: "            ,"Memory: "        };
const char valor_a_str          [TAMANHO_IDIOMA][17] =  {"Valor: "              ,"Value: "         };
const char descendo_str         [TAMANHO_IDIOMA][17] =  {"   Descendo     "     ," US Head Moving "};
const char process_str          [TAMANHO_IDIOMA][17] =  {"   Processando  "     ,"     Welding    "};
const char fixando_str          [TAMANHO_IDIOMA][17] =  {"   Fixando      "     ,"     Holding    "};
const char aguarde_str          [TAMANHO_IDIOMA][17] =  {"   Aguarde...   "     ,"     Wait...    "};
const char solda_cont_str       [TAMANHO_IDIOMA][17] =  {" Solda Continua "     ,"   Continuous   "};
const char inicia_str           [TAMANHO_IDIOMA][17] =  {"INICIALIZANDO..."     ,"INITIALIZING... "};
const char atencao_str          [TAMANHO_IDIOMA][17] =  {"   Atencao!!!   "     ,"  Attention!!!  "};
const char falha_str            [TAMANHO_IDIOMA][17] =  {"Falha Pneumatico"     ,"Pneumatic Fail"  };
const char emergencia_str       [TAMANHO_IDIOMA][17] =  {" ##Emergencia## "     ," ##EMERGENCY ## "};
const char emerg2_str           [TAMANHO_IDIOMA][17] =  {"##Pressionado!##"     ," ##PRESSED!!!## "};
const char teste_US1_str        [TAMANHO_IDIOMA][17] =  {"Teste Ultrassom "     ,"Ultrasonics Test"};
const char teste_PN1_str        [TAMANHO_IDIOMA][17] =  {"Teste Pneumatico"     ,"Pneumatic Test"  };
const char Sai_RST_str          [TAMANHO_IDIOMA][17] =  {"  RST p/ sair..."     ," RST to leave!"  };
const char Sai_enter_str        [TAMANHO_IDIOMA][17] =  {"ENTER p/ sair..."     ,"ENTER to leave!" };
const char erro_str             [TAMANHO_IDIOMA][17] =  {"ERRO "                ,"ERROR "          };
const char erro_ovl_str         [TAMANHO_IDIOMA][17] =  {"SOBRECARGA!!!   "     ,"OVERLOAD!!!"     };
const char erro_inv_str         [TAMANHO_IDIOMA][17] =  {"ERRO INVERSOR!!!"     ,"INVERTER ERROR!" };
const char zero_cont_str        [TAMANHO_IDIOMA][17] =  {"FIM CONTAGEM!!! "     ,"COUTING ENDED! " };
const char idioma_str           [TAMANHO_IDIOMA][17] =  {"Idioma:"              ,"Language:" };
const char tipo_idioma_str      [TAMANHO_IDIOMA][17] =  {"PORTUGUES"            ,"ENGLISH"};
const char tipo_energia_str     [TAMANHO_IDIOMA][17] =  {"Energia"              ,"Energy"};
const char tipo_tempo_str       [TAMANHO_IDIOMA][17] =  {"Tempo"                ,"Time"};
const char tipo_regress_str     [TAMANHO_IDIOMA][17] =  {"Regressivo"           ,"Decremental"};
const char tipo_progress_str    [TAMANHO_IDIOMA][17] =  {"Progressivo"          ,"Incremental"};
const char tipo_maquina1_str    [TAMANHO_IDIOMA][17] =  {"Pistola/Pulsado"      ,"Hand Gun"};
const char tipo_maquina2_str    [TAMANHO_IDIOMA][17] =  {"Maquina/Pulsado"      ,"Pneumat. Machine"};
const char tipo_maquina3_str    [TAMANHO_IDIOMA][17] =  {"Maquina c/ MM"        ,"MM Pne. Machine"};
const char energia_str          [TAMANHO_IDIOMA][17] =  {"Energia: "            ,"Energy: "};
const char tempo_str            [TAMANHO_IDIOMA][17] =  {"Tempo: "              ,"Time: "};
const char sim_str              [TAMANHO_IDIOMA][17] =  {"Sim"                  ,"Yes"};
const char nao_str              [TAMANHO_IDIOMA][17] =  {"Nao"                  ,"No"};
const char erro_energia_str     [TAMANHO_IDIOMA][17] =  {" + Erro"              ," + Error"};
const char energia_low_str      [TAMANHO_IDIOMA][17] =  {"Energia Baixa!"       ,"Low Energy!"};   
const char tempo_ovf_str        [TAMANHO_IDIOMA][17] =  {"Est. Tempo!"          ,"Time Over!"};    

const char menu_senha_str       [TAMANHO_IDIOMA][17] =  {"Senha "     ,"Password "}; 
const char edit_senha_str       [TAMANHO_IDIOMA][17] =  {"Editar Senha "     ,"Password Edit "};      
const char erro_senha_str       [TAMANHO_IDIOMA][17] =  {"Senha Incorreta!"     ,"Password Error! "};      

char str[20],str1[20],str2[20];

unsigned int t1ms_ISR;
unsigned int t1msMM_ISR;
unsigned int t1ms_OP;
unsigned int t1ms_solda;

unsigned int t10ms;
unsigned int t10ms_global;

unsigned int amostras;
unsigned int tInatividade;
unsigned int t5min;
unsigned char t1s[4];
unsigned char t_tela;
unsigned char troca_tela = 0;
unsigned char ciclador = 0;
unsigned char tipo_maquina;

unsigned char in[8],out[8];
unsigned char byte_in[8],byte_out[8];
unsigned char erro, erro_ovl;

unsigned int tmp_desce;
unsigned int tmp_solda;
unsigned int ener_solda;
unsigned int tmp_fixar;
unsigned int tmp_metal;


unsigned int tmp_soma = 100;

unsigned long cont_pecas;
unsigned long n_pecas;
unsigned long n_pecas_l;
unsigned long t100ms_solda;
unsigned long t100ms_solda_cont = 0;
unsigned long t1ms_solda_acum = 0;

unsigned char duty; 
unsigned char duty_ini = 0;


unsigned int pot_inst;
unsigned int energia_total;
unsigned int energia_total_kJ;

unsigned int potencia;
unsigned int ret_pot;
unsigned int mod_pot;

unsigned int freq_gerador;
unsigned int freq_inicial;
unsigned int freq_atual;
unsigned int freq_atual_arr[100];

unsigned int limite_24;
unsigned int limite_16;
unsigned int limite_22;
unsigned int limite_17;

unsigned int periodo;

volatile unsigned int timer1ReloadVal;
volatile unsigned int timer5ReloadVal;
volatile unsigned char erro_sobrecarga = 0;


float k_temp;

const char opc_entrada_str[10][16]={
    "IN_NC",
    "EMERGENCIA",
    "ACIONAMENTO1",
    "ACIONAMENTO2",
    "RESET",
    "METAL",
    "SENSOR_PECA",
    "SENSOR_PNE_AV",
    "SENSOR_PNE_RC",
    "SENSOR_HOLD",
};
const char opc_saida_str[7][16] = {
    "OUT_NC",
    "PNE_DESCE",
    "PNE_SOBE",
    "ERRO",
    "ERRO_ENER",
    "STT_BUSY",
    "RST_INV",
};
const char entrada_str[6][8] = {
    "INN",
    "IN1",
    "IN2",
    "IN3",
    "IN4",
    "IN5",
};
const char saida_str[5][8] = {
    "OUTN",
    "OUT1",
    "OUT2",
    "OUT3",
    "OUT4",
};

const char modelo_str[5][8] = {
    "SOW280",
    "SOW160",
    "SOW120",
    "SOW100",
    "SOWESP",
};

modo_maquina modo;
status_solda statusSolda;
status_solda nextSolda;
opc_idiomas idioma;
volatile char pagina = TELA_TMP_DESCE;

unsigned long num;

