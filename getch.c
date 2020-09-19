#include <stdio.h>
#include <unistd.h>
#include <termios.h>

struct termios padrao_attr , nova_attr;

// Retorna as configurações padrão do terminal
void fechar_attr(void)
{ 
    tcsetattr(STDIN_FILENO,TCSANOW,&padrao_attr);
}

// Configura o terminal para execução do getch
void config_attr(void)
{
    // Pega as configurações atuais
    tcgetattr(0,&padrao_attr);
    nova_attr = padrao_attr;

    // Desativa o modo canonico
    nova_attr.c_lflag &=~ICANON ;

    // Configuração do tempo de espera ate o proximo byte
    nova_attr.c_cc[VTIME]=0 ;
    nova_attr.c_cc[VMIN]=1 ;
}

// getch
int getch_l(void)
{
    int ch;

    // Ativa as configurações para getch
    config_attr();

    // Desativa o echo do terminal
    nova_attr.c_lflag &= ~ECHO;

    // Faz a leitura do proximo byte sem o echo no terminal
    tcsetattr(STDIN_FILENO,TCSANOW,&nova_attr);
    ch = getchar() ;
    tcsetattr(STDIN_FILENO,TCSANOW,&padrao_attr);

    // Retorna as configurações
    fechar_attr();

    return ch;

}