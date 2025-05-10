//----------------------------------------------------- DATA GRAVACAO -> 08/04/2019
#define Nome_Cliente    "SONITRON"          //NAO ALTERAR MAIS ESTE NOME (max 16 caracteres) 
#define Num_Serie       "E25 3804"           //Numero de serie do Gerador
#define Tipo_Maquina    PISTOLA_PULSADO     //PISTOLA_PULSADO,MAQUINA_PULSADO,MAQUINA_MM,ESPECIAL (Especial deve ser configurado no programa, na funcao "config_maq_especial()")
#define Tipo_Gerador    SOW280              //SOW280,SOW160,SOW120,SOW100,SOWESP
#define IDIOMA          PORTUGUES              //PORTUGUES,INGLES
#define Versao_Soft     "Ver. 4.2.95"
#define N_PECAS         0                   // Quantidade de operacoes concluidas com sucesso
#define T_100ms         0                  // Tempo total de operacao. Onde 12345 =  1234,5s

// FUNCAO ESPECIAL
#define Temp_Atual      23              //Temperatura (ºC) ambiente do local onde estava armazenado o CI.
#define FUNCAO_TESTE_ENTER 0            // O gerador executa um teste de solda pressionando Botao no IHM
#define FREQ_INI        1               // É possivel mudar a freq inicial pressionando ENTER
#define HABILITA_SENHA  0               // Usar senha de entrada do MENU

#define RECONDICINADO   0               
#define STR_RECOND_1     "REPARADO POR" 
#define STR_RECOND_2     "SONITRON"
#define STR_RECOND_DATA  "EM 10/12/2024"

/******************************************************************************/
/******************************** HISTORICO ***********************************/
/******************************************************************************/
/*  Jan Fev Mar Abr Mai Jun Jul Ago Set Out Nov Dez */
/*  A   B   C   D   E   F   G   H   I   J   K   L   */

/*** Historico de  Geradores:
 * i -> com controle de potencia
 * 
 * *** Dia 10/05/2025 / Modelo
 * E25 3805 SOW280 i
 * E25 3804 SOW280 i
 * 
 * *** Dia 10/04/2025 / Modelo
 * D25 3803 SOW100
 * D25 3802 SOW100
 * D25 3801 SOW100  
 * D25 3800 SOW100
 * D25 3799 SOW100
 *   
 * D25 3798 SOW280 i
 * D25 3797 SOW280 i
 * D25 3796 SOW280 i
 * D25 3795 SOW280 i
 * 
 * *** Dia 25/03/2025 / Modelo
 * C25 3794 SOW280 i
 * C25 3793 SOW280 i
 * C25 3792 SOW280 i
 * C25 3791 SOW280 i
 * C25 3790 SOW280 i
 * C25 3789 SOW100 
 * C25 3788 SOW100 
 * C25 3787 SOW100 
 * 
 * *** Dia 10/03/2025 / Modelo
 * C25 3786 SOW280 i
 * C25 3785 SOW280 i
 * 
 * *** Dia 06/12/2024 / Modelo
 * L24 3784 SOW100 
 * L24 3783 SOW100
 * L24 3782 SOW100
 * L24 3781 SOW280
 * L24 3780 SOW280
 * L24 3779 SOW280
 * L24 3778 SOW280
 * L24 3777 SOW280
 * 
 * *** Dia 11/11/2024 / Modelo
 * K24 3776 SOW280
 * K24 3775 SOW280
 * K24 3774 SOW280
 * K24 3773 SOW280
 * K24 3772 SOW280
 * 
 * *** Dia 27/09/2024 / Modelo
 * J24 3771 SOW100
 * J24 3770 SOW100
 * J24 3769 SOW100
 * J24 3768 SOW100
 * 
 * J24 3767 SOW280
 * J24 3766 SOW280
 * J24 3765 SOW280
 * J24 3764 SOW280
 * J24 3763 SOW280
 * 
 * *** Dia 12/08/2024 / Modelo
 * I24 3762 SOW280
 * I24 3761 SOW280
 * I24 3760 SOW280
 * 
 * *** Dia 12/08/2024 / Modelo
 * H24 3759 SOW280i
 * H24 3758 SOW280i
 * H24 3757 SOW280i
 * 
 * *** Dia 16/05/2024 / Modelo
 * E24 3756 SOW280
 * E24 3755 SOW280
 * E24 3754 SOW280
 * 
 * *** Dia 16/04/2024 / Modelo
 * D24 3753 SOW100
 * D24 3752 SOW100
 * D24 3751 SOW100
 * D24 3750 SOW100
 * D24 3749 SOW100
 * 
 * D24 3748 SOW280
 * D24 3747 SOW280
 * 
 * 
 * *** Dia 16/03/2024 / Modelo
 * C24 3746 SOW280
 * C24 3745 SOW280
 * C24 3744 SOW100
 * C24 3743 SOW100
 * 
 * *** Dia 16/01/2024 / Modelo
 * A24 3742 SOW280
 * A24 3741 SOW280
 * A24 3740 SOW280
 * A24 3739 SOW280
 * A24 3738 SOW280
 * A24 3737 SOW280
 * A24 3736 SOW280
 * A24 3735 SOW280
 * 
 * *** Dia 11/01/2024 / Modelo
 * A24 3734 SOW280
 * A24 3733 SOW280
 * A24 3732 SOW280
 * A24 3731 SOW280
 * 
 * *** Dia 10/12/2023 / Modelo
 * L23 3730 SOW100
 * L23 3729 SOW100
 * L23 3728 SOW100
 * L23 3727 SOW100
 * L23 3726 SOW100(i)
 * L23 3725 SOW100
 * 
 * *** Dia 30/05/2023 / Modelo
 * E23 3724 SOW280
 * E23 3723 SOW280
 * E23 3722 SOW280
 * E23 3721 SOW280
 * E23 3720 SOW280
 * 
 * E23 3719 SOW100
 * E23 3718 SOW100
 * E23 3717 SOW100 - Entregue ao estoque em 12/01/2024
 * E23 3716 SOW100 - Agrovicola Filippsen
 * E23 3715 SOW100
 * 
 * *** Dia 20/04/2023 / Modelo
 * D23 3714 SOW100
 * D23 3713 SOW100
 * D23 3712 SOW100
 * D23 3711 SOW100
 * D23 3710 SOW100
 * D23 3709 SOW100
 * D23 3708 SOW100
 * D23 3707 SOW100
 * 
 * *** Dia 27/02/2023 / Modelo
 * B23 3706 SOW280
 * B23 3705 SOW280
 * B23 3704 SOW280
 * B23 3703 SOW280
 * B23 3702 SOW280
 * B23 3701 SOW280
 * B23 3700 SOW280
 * B23 3699 SOW280
 * B23 3698 SOW280
 * B23 3671 SOW280
 * 
 * B23 3697 SOW100
 * B23 3696 SOW100
 * B23 3695 SOW100
 * B23 3694 SOW100
 * B23 3693 SOW100
 * 
 * *** Dia 15/12/2022 / Modelo
 * L22 3692 SOW280
 * L22 3691 SOW280
 * L22 3690 SOW280
 * L22 3689 SOW280
 * L22 3688 SOW280
 * 
 * *** Dia 05/12/2022 / Modelo
 * L22 3669 SOW280
 * L22 3668 SOW280
 * L22 3667 SOW280
 * L22 3666 SOW280
 * L22 3665 SOW280
 * 
 * *** Dia 04/07/2022 / Modelo
 * H22 3664 SOW100
 * H22 3663 SOW100
 * H22 3662 SOW100
 * H22 3661 SOW100
 * H22 3660 SOW100
 * H22 3659 SOW100
 * H22 3658 SOW100
 * H22 3657 SOW100
 * H22 3656 SOW100
 * H22 3655 SOW100 
 * 
 * *** Dia 04/07/2022 / Modelo
 * G22 3654 SOW280
 * G22 3653 SOW280
 * G22 3652 SOW280
 * G22 3651 SOW280
 * G22 3650 SOW280
 * G22 3649 SOW280
 * G22 3648 SOW280
 * G22 3647 SOW280
 * G22 3646 SOW280
 * G22 3645 SOW280
 * 
 * *** Dia 31/05/2022 / Modelo
 * E22 3644 SOW100
 * E22 3643 SOW100
 * E22 3642 SOW100
 * E22 3641 SOW100
 * E22 3640 SOW100 ****************************** 3640 - 3669 depois recomeça em 3688 
 * 
 * *** Dia 24/05/2022 / Modelo
 * E22 3687 SOW280
 * E22 3686 SOW280
 * E22 3685 SOW280
 * E22 3684 SOW280
 * 
 * *** Dia 22/04/2022 / Modelo
 * D22 3683 SOW100
 * D22 3682 SOW100
 * D22 3681 SOW100
 * D22 3680 SOW100
 * 
 * *** Dia 22/03/2022 / Modelo
 * C22 3679 SOW280
 * C22 3678 SOW280
 * C22 3677 SOW280
 * C22 3676 SOW280
 * C22 3675 SOW280
 * C22 3674 SOW280
 * C22 3673 SOW280
 * C22 3672 SOW280
 * C22 3671 SOW280 <<< Parece q nao foi encontrado em estoque. Regravado como B23 3671 dia 27/2/23
 * C22 3670 SOW280 *******************************
 * 
 * *** Dia 02/03/2022 / Modelo
 * C22 3639 SOW280 ********************************
 * C22 3638 SOW280
 * C22 3637 SOW280
 * C22 3636 SOW280
 * C22 3635 SOW280

 * *** Dia 17/02/2022 / Modelo
 * B22 3634 SOW280
 * B22 3633 SOW280
 * B22 3632 SOW280
 * B22 3631 SOW280
 * B22 3630 SOW280
 * 
 * *** Dia 16/02/2022 / Modelo
 * B22 3629 SOW100
 * B22 3628 SOW100
 * B22 3627 SOW100
 * B22 3626 SOW100
 * B22 3625 SOW100
 * 
 * *** Dia 25/01/2022 / Modelo
 * A22 3624 SOW280i
 * A22 3623 SOW280i
 * A22 3622 SOW280i
 * A22 3621 SOW280
 * 
 * *** Dia 20/09/2021 / Modelo
 * K21 3620 SOW280**********
 * K21 3619 SOW280
 * K21 3618 SOW280
 * K21 3617 SOW280
 * 
 * *** Dia 20/09/2021 / Modelo
 * J21 3616 SOW100
 * J21 3615 SOW100
 * J21 3614 SOW100
 * 
 * *** Dia 20/09/2021 / Modelo
 * I21 3613 SOW280
 * I21 3612 SOW280 -> SOW100
 * I21 3611 SOW280
 * I21 3610 SOW280
 * I21 3609 SOW280
 * 
 * *** Dia 20/09/2021 / Modelo
 * I21 3608 SOW280
 * I21 3606 SOW280
 * I21 3605 SOW280
 * I21 3604 SOW280
 * 
 * *** Dia 20/09/2021 / Modelo
 * I21 3603 SOW100
 * I21 3602 SOW100
 * I21 3601 SOW100
 * I21 3600 SOW100
 * 
 * *** Dia 14/06/2021 / Modelo
 * G21 3599 SOW100
 * G21 3598 SOW100
 * G21 3597 SOW280
 * G21 3596 SOW280
 * G21 3595 SOW280
 * G21 3594 SOW280
 * G21 3593 SOW280
 * G21 3592 SOW280
 * G21 3591 SOW280
 * G21 3590 SOW280
 * 
 * *** Dia 14/06/2021 / Modelo
 * F21 3589 SOW280
 * F21 3588 SOW280
 * F21 3587 SOW280
 * F21 3586 SOW280
 * F21 3585 SOW280
 * F21 3584 SOW280
 * F21 3583 SOW100
 * F21 3582 SOW100
 * F21 3581 SOW100
 * 
 * *** Dia 14/06/2021 / Modelo
 * F21 3580 SOW100 
 * 
 * *** Dia 10/06/2021 / Modelo
 * F21 3579 SOW280 
 * F21 3578 SOW280 
 * F21 3577 SOW280 
 * F21 3576 SOW280 
 * F21 3575 SOW280 
 * F21 3574 SOW280 
 * 
 * *** Dia 16/05/2021 / Modelo
 * E21 3570 SOW280 
 * E21 3571 SOW280 
 * E21 3572 SOW280 
 * E21 3573 SOW280 
 * 
 * *** Dia 16/05/2021 / Modelo
 * E21 3569 SOW280 
 * 
 * *** Dia 06/05/2021 / Modelo
 * E21 3568 SOWESP 15 KHz
 * 
 * *** Dia 03/05/2021 / Modelo
 * E21 3565 SOW100
 * E21 3566 SOW100
 * E21 3567 SOW100
 * 
 * *** Dia 29/04/2021 / Modelo
 * D21 3561 SOW280
 * D21 3562 SOW280
 * D21 3563 SOW280
 * D21 3564 SOW280
 * 
 * *** Dia 27/04/2021 / Modelo
 * D21 3558 SOW100
 * D21 3559 SOW280
 * D21 3560 SOW280
 * 
 * *** Dia 05/04/2021 / Modelo
 * D21 3554 SOW280
 * D21 3555 SOW280
 * D21 3556 SOW280
 * D21 3557 SOW280
 * 
 * *** Dia 05/04/2021 / Modelo
 * D21 3553 SOW280
 * D21 3552 SOW280
 * 
 * *** Dia 05/04/2021 / Modelo
 * D21 3551 SOW280
 * 
 * *** Dia 05/04/2021 / Modelo
 * D21 3546 SOW280
 * D21 3548 SOW280
 * D21 3549 SOW280
 * D21 3550 SOW100
 * 
 * *** Dia 05/04/2021 / Modelo
 * D21 3544 SOW280
 * 
 * *** Dia 19/03/2021 / Modelo
 * C21 3543 SOW280
 * C21 3542 SOW280
 * 
 * *** Dia 18/03/2021 / Modelo
 * C21 3547 SOW280
 * 
 * *** Dia 17/03/2021 / Modelo
 * C21 3546 SOW280
 * 
 * *** Dia 16/03/2021 / Modelo
 * C21 3545 SOW100
 * C21 3544 SOW280
 * 
 * *** Dia 15/03/2021 / Modelo
 * A21 3541 SOW100
 * 
 * *** Dia 26/01/2021 / Modelo
 * A21 3539 SOW100
 * A21 3540 SOW100
 * 
 * *** Dia 09/12/2020 / Modelo
 * K20 3525 SOW280
 * K20 3529 SOW280
 * K20 3530 SOW280
 * K20 3531 SOW280
 * K20 3532 SOW280
 * K20 3533 SOW280
 * K20 3534 SOW280
 * K20 3535 SOW280
 * K20 3536 SOW280
 * K20 3537 SOW280
 * K20 3538 SOW280
 * 
 * 
 * *** Atenção entre K20 3521 e K20 3528
 * K20 3526 SOW100
 * K20 3527 SOW100
 * 
 * *** Dia 18/11/2020 / Modelo
 * K20 3519 SOW280
 * K20 3520 SOW280
 * K20 3521 SOW280
 * 
 * *** Dia 11/11/2020 / Modelo
 * K20 3512 SOW280
 * K20 3513 SOW280
 * K20 3515 SOW280
 * 
 * *** Dia 09/11/2020 / Modelo
 * K20 3510 (LILO) CONFIG Especial SOW280 -> StopOnRelease = 1.
 * K20 3511 SOW280
 * 
 *** Dia 22/10/2020 / Modelo
 * E12 2794 SOW280 - LILO (2282)
 *** Dia 10/10/2020 / Modelo
 * J20 3502 SOW280
 * J20 3503 SOW280
 * J20 3504 SOW280
 * J20 3505 SOW280
 * J20 3506 SOW280
 * J20 3507 SOW280 ***
 * J20 3508 SOW280
 * J20 3509 SOW280 
 * 
 *** Dia 29/09/2020 / Modelo
 * E20 3367 SOW280 - OS2355 OP115 T7684,9
 * 
 *** Dia 28/09/2020 / Modelo
 * I20 3498 SOW280
 * I20 3499 SOW280
 * I20 3500 SOW280
 * I20 3501 SOW280
 * I20 3502 SOW280
 * 
 *** Dia 25/09/2020 / Modelo
 * G20 3447G SOW280 - Substituido em garantia
 * 
 *** Dia 18/09/2020 / Modelo
 * D19 3261 SOW100 - OS2356 - Atualizacao
 * 
 *** Dia 17/09/2020 / Modelo
 * I20 3494 SOW280
 * I20 3495 SOW280
 * I20 3496 SOW280
 * I20 3497 SOW280
 * I20 3460 SOW100
 * I20 3479 SOW100
 * 
 *** Dia 16/09/2020 / Modelo
 * I20 3492 SOW280
 * I20 3493 SOW280
 * 
 *** Dia 09/09/2020 / Modelo
 * I20 3491 SOW280
 * 
 *** Dia 04/09/2020 / Modelo
 * Automatisa (Atualizacao) -   E20 3388/E20 3389/E20 3390/E20 3405/E20 3406/E20 3407/E20 3408/E20 3409/E20 3410
 *                              F20 3422/F20 3423/F20 3424/G20 3439/G20 3442/G20 3443
 * 
 *** Dia 27/08/2020 / Modelo
 * B20 3299 OS2344 - JL CAP
 * 
 *** Dia 27/08/2020 / Modelo
 * H20 3480 SOW280
 * H20 3481 SOW280
 * H20 3482 SOW280
 * H20 3483 SOW280
 * H20 3484 SOW280
 * H20 3485 SOW280
 * H20 3486 SOW280
 * H20 3487 SOW280
 * H20 3488 SOW280
 * H20 3489 SOW280
 * H20 3490 SOW280
 * 
 *** Dia 20/08/2020 / Modelo
 * H20 3458 SOW100
 * H20 3459 SOW100
 * H20 3460 SOW100
 * H20 3479 SOW280
 * D20 3315 SOW280 - GVS (OBS: Divergencia entre numero do serie: PIC -> D20 3313 | Gerador-> D20 3315)
 * 
 *** Dia 20/08/2020 / Modelo
 * H20 3474 SOW280
 * H20 3475 SOW280
 * H20 3476 SOW280
 * H20 3477 SOW280
 * H20 3478 SOW280
 * 
 *** Dia 18/08/2020 / Modelo
 * I15 3070 SOW280 - PV15248 (Harpla)
 * 
 * Dia 14/08/2020 / Modelo
 * D19 3266 SOW100 - OS2328
 * H20 3466 SOW280
 * H20 3467 SOW280
 * H20 3468 SOW280
 * H20 3469 SOW280 -> Pulou ***************
 * H20 3470 SOW280
 * H20 3471 SOW280
 * H20 3472 SOW280
 * H20 3473 SOW280
 * 
 *** Dia 13/08/2020 / Modelo
 * H20 3463 SOW100
 * H20 3464 SOW280
 * H20 3465 SOW280
 * 
 *** Dia 12/08/2020 / Modelo
 * H20 3453 SOW280
 * H20 3461 SOW280
 * H20 3462 SOW280
 * 
 *** Dia 11/08/2020 / Modelo
 * H20 3456 SOW280
 * H20 3457 SOW280
 * H20 3458 SOW280 -> pulou ***************
 * H20 3459 SOW280 -> pulou ***************
 * H20 3460 SOW280 -> pulou ***************
 */

//(copiar a funcao config_maq_especial para saber como foi configurada)
//void config_maq_especial(void){           //-->> Tem que deixar a funcao comentada, pois é so uma copia. Caso contrario a compilacao ira dar erro.
//    
//    config_entrada(EMERGENCIA,IN5);  
//    config_entrada(ACIONAMENTO1,IN4);           
//    config_entrada(SENSOR_PECA,IN3);      
//    config_entrada(SENSOR_HOLD,IN2);              
//    config_entrada(RESET,IN1);             
//
//    config_saidas(RST_INV,OUT1);        
//    config_saidas(PNE_DESCE,OUT2);      
//    config_saidas(ERRO,OUT3);              
//    config_saidas(STATUS_GERADOR,OUT4);     
//    cfg.stop_release = 1;               
//}                                         //-->> Fim do comentario da funcao config_maq_especial.