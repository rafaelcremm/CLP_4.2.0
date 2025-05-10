#include "func.h"
#include "Historico_Ver4.h"

const char senha_padrao[5] = {'0','2','3','5',0}; //oct Ø
char senha_gerador[5];
char senha_user[5];

void LoadSenhaUser(void){
    
    senha_user[0] = eeprom_read(ee_senha0);
    senha_user[1] = eeprom_read(ee_senha1);
    senha_user[2] = eeprom_read(ee_senha2);
    senha_user[3] = eeprom_read(ee_senha3);    
    senha_user[4] = 0;
}

void LoadSenhaGerador(void){
    
    // Algoritimo para senha individual de cada gerador é baseado no numero de Serie:
    //  * Dig1 = (Dig2*7 + Dig3*11 + Dig4*13)%10
    //  * Dig2 = (Dig1*7 + Dig3*11 + Dig4*13)%10
    //  * Dig3 = (Dig2*7 + Dig1*11 + Dig4*13)%10
    //  * Dig4 = (Dig2*7 + Dig3*11 + Dig1*13)%10
    
    senha_gerador[0] = eeprom_read(ee_senha4);
    senha_gerador[1] = eeprom_read(ee_senha5);
    senha_gerador[2] = eeprom_read(ee_senha6);
    senha_gerador[3] = eeprom_read(ee_senha7);    
    senha_gerador[4] = 0;
}

void printTelaSenha(char *s, char col){
    Lcd_Clear();
    escreve_LCD(1,1,menu_senha_str[idioma]);
    sprintf(str, "-%i:",col+1);
    Lcd_Write_String(str);
    escreve_LCD(2,1,s);
//    Lcd_Write_String(senha_user);
//    Lcd_Write_String(senha_padrao);
}

void printTelaEditarSenha(char *s, char col){
    Lcd_Clear();
    escreve_LCD(1,1,edit_senha_str[idioma]);
    sprintf(str, "-%i:",col+1);
    Lcd_Write_String(str);
    escreve_LCD(2,1,s);
}

void EditSenhaUser(void){
    char senha[5];
    char coluna = 0;
    char bt_a;
    
    LoadSenhaUser();
    sprintf(senha,senha_user); // copia user em senha
    printTelaEditarSenha(senha,coluna);
    
    do{
        bt_a = teclado();
        switch(bt_a){
            case menu:                
                liberar_tecla(bt_a,0);
                return;
                
            case enter:
                liberar_tecla(bt_a,0);
                sprintf(senha_user,senha);
                eeprom_write(ee_senha0,senha[0]);
                eeprom_write(ee_senha1,senha[1]);
                eeprom_write(ee_senha2,senha[2]);
                eeprom_write(ee_senha3,senha[3]);
                senha[4] = 0;
                return;
                
            case up:
                if(senha[coluna] < '9'){
                    senha[coluna]++;
                }
                else {
                    senha[coluna] = '0';
                }
                printTelaEditarSenha(senha,coluna);
                liberar_tecla(bt_a,0);
                break;
                
            case down:
                if(senha[coluna] > '0'){
                    senha[coluna]--;
                }
                else {
                    senha[coluna] = '0';
                }
                printTelaEditarSenha(senha,coluna);
                liberar_tecla(bt_a,0);
                break;
                
            case rigth:
                if(coluna < 3){
                    coluna++;
                }
                else {
                    coluna = 0;
                }
                printTelaEditarSenha(senha,coluna);
                liberar_tecla(bt_a,0);
                break;
                
            case left:
                if(coluna > 0){
                    coluna--;
                }
                else {
                    coluna = 3;
                }
                printTelaEditarSenha(senha,coluna);
                liberar_tecla(bt_a,0);
                break;
        }
    }while(1);
}

char verifSenhas(char *sh){
    if((strcmp(sh,senha_padrao) == 0) || (strcmp(sh,senha_gerador) == 0)|| (strcmp(sh,senha_user) == 0))
        return 1;
    return 0;
}

char Autenticar(void){
    
    if(HABILITA_SENHA == 0) return 1;
    
    char senha[5] = {'0','0','0','0',0};
    char coluna = 0;
    char bt_a;   
    
    LoadSenhaGerador();
    LoadSenhaUser();
    
    printTelaSenha(senha,coluna);
    __delay_ms(500);
        
    do{
        bt_a = teclado();
        switch(bt_a){
            case menu:
                liberar_tecla(bt_a,0);
                return -1;
                
            case F1:
                liberar_tecla(bt_a,0);
                if(verifSenhas(senha)){
                    EditSenhaUser();
                    liberar_tecla(bt_a,0);
                    return 1;
                }                
                return 0;                
                
            case enter:
                liberar_tecla(bt_a,0);
                if(verifSenhas(senha)){
                    return 1;
                }
                else{
                    return 0;
                }
                
            case up:
                if(senha[coluna] < '9'){
                    senha[coluna]++;
                }
                else {
                    senha[coluna] = '0';
                }
                printTelaSenha(senha,coluna);
                liberar_tecla(bt_a,0);
                break;
                
            case down:
                if(senha[coluna] > '0'){
                    senha[coluna]--;
                }
                else {
                    senha[coluna] = '9';
                }
                printTelaSenha(senha,coluna);
                liberar_tecla(bt_a,0);
                break;
                
            case rigth:
                if(coluna < 3){
                    coluna++;
                }
                else {
                    coluna = 0;
                }
                printTelaSenha(senha,coluna);
                liberar_tecla(bt_a,0);
                break;
                
            case left:
                if(coluna > 0){
                    coluna--;
                }
                else {
                    coluna = 3;
                }
                printTelaSenha(senha,coluna);
                liberar_tecla(bt_a,0);
                break;
        }
    }while(1);
    
    return 0;
}