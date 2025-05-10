#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#define EUSART_DataReady  (EUSART_is_rx_ready())

extern volatile char eusartTxBufferRemaining;
extern volatile char eusartRxCount;

void (*EUSART_TxDefaultInterruptHandler)(void);
void (*EUSART_RxDefaultInterruptHandler)(void);

void EUSART_Initialize(void);

char EUSART_is_tx_ready(void);

char EUSART_is_rx_ready(void);

bool EUSART_is_tx_done(void);

char EUSART_Read(void);

void EUSART_Write(char txData);

void EUSART_Transmit_ISR(void);

void EUSART_Receive_ISR(void);

void EUSART_SetTxInterruptHandler(void (* interruptHandler)(void));

void EUSART_SetRxInterruptHandler(void (* interruptHandler)(void));