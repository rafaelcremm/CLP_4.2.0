#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "var.h"
#include "lcd.h"
#include "eusart.h"

void interrupt INTERRUPT_InterruptManager(void);
void PIN_MANAGER_Initialize (void);
void ini_GERADOR(unsigned int f,unsigned int p,unsigned int c, unsigned int v);
float XC(unsigned int f, unsigned int nF);
unsigned int get_freq(unsigned char acumulo);
void set_freq(unsigned int f);
char ver_freq(unsigned int f);
unsigned int pontencia_instantanea(unsigned char duty);
void delay(unsigned int ms);
void salva_EEPROM(unsigned int valor, unsigned char address);
unsigned int carrega_EEPROM(unsigned char address);
void salva_EEPROM32(unsigned long valor, unsigned char address);
unsigned long carrega_EEPROM32(unsigned char address);
void op_desce(unsigned int t);
void op_solda(unsigned int t);
void op_fixar(unsigned int t);
void trata_dados(void);
void liberar(void);
void trava_erro(const char s[]);
void emergencia(void);
void config_IO_start(void);
void ISR_entrada(entradas ch);
unsigned entrada(opc_entradas x);
void ver_entrada(void);
void config_entrada(opc_entradas x, entradas ch);
void config_saidas(opc_saidas x, saidas ch);
void config_flag_entrada(opc_entradas x, entradas ch);
void config_flag_saida(opc_saidas x, saidas ch);
void clear_entrada (opc_entradas x);
void clear_saida (saidas ch);
void config_maq_especial(void);
void config_maq (opc_maquinas x);
void saida(opc_saidas x, unsigned b);
void clr(void);
void data(unsigned b);
void load_bar(void);
void teste_leds(void);
void tela_ultima_solda(void);
void tela_inicial(void);
void totalCiclos(void);
void rst_inversor(void);
void salva_n_pecas(void);
unsigned char teclado(void);
void bip_tecla(void);
unsigned liberar_tecla(unsigned char b, unsigned int t);
void telas(unsigned char b);
void escreve_tempos(const char s1[], unsigned int t, char pag);
unsigned char addr_receitas(unsigned char addr, unsigned char tempo);
void func_enter(void);
void func_rst(void);
void teste_pneumatico(char b);
void teste_US(char b);
void status_gerador(opc_saidas x, unsigned b);

/******************************************************************************/
void TMR5_Initialize(void);
void TMR5_WriteTimer(unsigned int timerVal);
void TMR5_Reload(void);
void start_timer(unsigned int t);
void stop_timer(void);

void TMR6_Initialize(void);

void SMT1_Initialize(void);
uint32_t SMT1_GetPeriod(void);
uint32_t SMT1_GetCapturedPulseWidth(void);
uint32_t SMT1_GetCapturedPeriod(void);
uint32_t SMT1_GetTimerValue(void);

void SMT2_Initialize(void);
uint32_t SMT2_GetPeriod(void);
uint32_t SMT2_GetCapturedPulseWidth(void);
uint32_t SMT2_GetCapturedPeriod(void);
uint32_t SMT2_GetTimerValue(void);
unsigned char get_duty(void);

void TMR1_Initialize(void);
void TMR1_StartTimer(void);
uint16_t TMR1_ReadTimer(void);
void TMR1_WriteTimer(uint16_t timerVal);
void TMR1_Reload(void);

void PWM2_Initialize(void);
void PWM2_LoadDutyValue(uint16_t dutyValue);

void SYSTEM_Initialize(void);
void OSCILLATOR_Initialize(void);
void PMD_Initialize(void);
void CLC1_Initialize(void);
void ADCC_Initialize(void);
float get_temp(float K);
char conv_temp(float adc);
void ini_ee_tempos(float t,opc_maquinas tipo);
void rotina_tela(void);
void escreve_freq(void);
char verifPagina(char pag , char bt_local);

void ciclar(void);
void ciclarPneumatico(void);

char Autenticar(void);

#ifdef DEBUG
void debug(int n);
#endif