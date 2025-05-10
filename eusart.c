#include "eusart.h"
#define EUSART_TX_BUFFER_SIZE 32
#define EUSART_RX_BUFFER_SIZE 32

volatile char eusartTxHead = 0;
volatile char eusartTxTail = 0;
volatile char eusartTxBuffer[EUSART_TX_BUFFER_SIZE];
volatile char eusartTxBufferRemaining;

volatile char eusartRxHead = 0;
volatile char eusartRxTail = 0;
volatile char eusartRxBuffer[EUSART_RX_BUFFER_SIZE];
volatile char eusartRxCount;

/**
  Section: EUSART APIs
*/
void EUSART_Initialize(void)
{
    // disable interrupts before changing states
    PIE3bits.RCIE = 0;
    EUSART_SetRxInterruptHandler(EUSART_Receive_ISR);
    PIE3bits.TXIE = 0;
    EUSART_SetTxInterruptHandler(EUSART_Transmit_ISR);
    // Set the EUSART module to the options selected in the user interface.

    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD1CON = 0x08;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RC1STA = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TX1STA = 0x24;

    // SP1BRGL 64; 
    SP1BRGL = 0x08;

    // SP1BRGH 3; 
    SP1BRGH = 0x02;


    // initializing the driver state
    eusartTxHead = 0;
    eusartTxTail = 0;
    eusartTxBufferRemaining = sizeof(eusartTxBuffer);

    eusartRxHead = 0;
    eusartRxTail = 0;
    eusartRxCount = 0;

    // enable receive interrupt
    PIE3bits.RCIE = 1;
}

char EUSART_is_tx_ready(void)
{
    return eusartTxBufferRemaining;
}

char EUSART_is_rx_ready(void)
{
    return eusartRxCount;
}

bool EUSART_is_tx_done(void)
{
    return TX1STAbits.TRMT;
}

char EUSART_Read(void)
{
    char readValue  = 0;
    
    while(0 == eusartRxCount)
    {
        CLRWDT();
    }

    readValue = eusartRxBuffer[eusartRxTail++];
    if(sizeof(eusartRxBuffer) <= eusartRxTail)
    {
        eusartRxTail = 0;
    }
    PIE3bits.RCIE = 0;
    eusartRxCount--;
    PIE3bits.RCIE = 1;

    return readValue;
}

void EUSART_Write(char txData)
{
    while(0 == eusartTxBufferRemaining)
    {
        CLRWDT();
    }

    if(0 == PIE3bits.TXIE)
    {
        TX1REG = txData;
    }
    else
    {
        PIE3bits.TXIE = 0;
        eusartTxBuffer[eusartTxHead++] = txData;
        if(sizeof(eusartTxBuffer) <= eusartTxHead)
        {
            eusartTxHead = 0;
        }
        eusartTxBufferRemaining--;
    }
    PIE3bits.TXIE = 1;
}

char getch(void)
{
    return EUSART_Read();
}

void putch(char txData)
{
    EUSART_Write(txData);
}

void EUSART_Transmit_ISR(void)
{

    // add your EUSART interrupt custom code
    if(sizeof(eusartTxBuffer) > eusartTxBufferRemaining)
    {
        TX1REG = eusartTxBuffer[eusartTxTail++];
        if(sizeof(eusartTxBuffer) <= eusartTxTail)
        {
            eusartTxTail = 0;
        }
        eusartTxBufferRemaining++;
    }
    else
    {
        PIE3bits.TXIE = 0;
    }
}

void EUSART_Receive_ISR(void)
{
    
    if(1 == RC1STAbits.OERR)
    {
        // EUSART error - restart

        RC1STAbits.CREN = 0;
        RC1STAbits.CREN = 1;
    }

    // buffer overruns are ignored
    eusartRxBuffer[eusartRxHead++] = RC1REG;
    if(sizeof(eusartRxBuffer) <= eusartRxHead)
    {
        eusartRxHead = 0;
    }
    eusartRxCount++;
}

void EUSART_SetTxInterruptHandler(void (* interruptHandler)(void)){
    EUSART_TxDefaultInterruptHandler = interruptHandler;
}

void EUSART_SetRxInterruptHandler(void (* interruptHandler)(void)){
    EUSART_RxDefaultInterruptHandler = interruptHandler;
}
/**
  End of File
*/
