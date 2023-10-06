#include <iostream>
#include <Windows.h>
#include <stdio.h>

/*
        Função que move o ponteiro dentro do console
*/
void moverxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/*
        Função que gera o menu fixo
*/
void menu_principal(HANDLE* console)
{
    SetConsoleTextAttribute(*console, 3);
    printf("AutoClicker\n");

    SetConsoleTextAttribute(*console, 7);
    printf("Criado por @CauanGS\n");
    printf("\nLigar/Desligar: F8\n"); 
    printf("Aumentar/Diminuir: SHIFT + UP/DOWN\n");

    SetConsoleTextAttribute(*console, 14);
    printf("\nStatus: ");

    SetConsoleTextAttribute(*console, 4);
    printf("[OFF]\n");
}

/*
        Função que atualiza o menu em tempo de execução
*/
void menu_ciclo(float* cps, bool click, HANDLE* console)
{
    moverxy(8, 6);
    if (click)
    {
        SetConsoleTextAttribute(*console, 2);
        printf("[ON] \n");
    }
    else
    {
        SetConsoleTextAttribute(*console, 4);
        printf("[OFF]\n");
    }
    SetConsoleTextAttribute(*console, 14);
    printf("CPS:");
    SetConsoleTextAttribute(*console, 12);
    printf(" %.2lf", *cps);
}

/*
        Função responsável por executar o click do botão esquerdo
*/
void clicker()
{
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

/*
        Função que executará durante todo o tempo de execução do programa
*/
void loop()
{

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // console
    int vkey = 0x77; // Teclado F8
    bool click = false; // Autoclick inicia desligado
    float cps = 14.00; // Limite de cliques por segundo inicial
    float visivel = 0.00; // Valor exibido em tela pós calculo

    menu_principal(&console);

    while (true)
    {
        menu_ciclo(&cps, click, &console);
        while (click) // Executa enquanto o autoclick estiver ativo
        {
            if (GetKeyState(vkey) < 1)
                // Desliga o autoclick caso a vkey seja pressionada e imprime a informação no console
            {
                click = false;
                menu_ciclo(&cps, click, &console);
            }
            else
                // Executa o click e define o intervalo
            {
                click = true;
                float ale = 0.001 + rand() * (1.7 - 0.001) / RAND_MAX; // Gera valor aleatorio para não manter constância
                visivel = cps - ale; // Valor visível de cliques
                menu_ciclo(&visivel, click, &console); // Exibe numero de cliques no momento
                clicker();
                Sleep(1000 / visivel); // Gera o intervalo com base em cliques por segundo
            }
        }
        if (GetKeyState(vkey) > 0)
            // Liga o autoclick caso esteja inativo
        {
            click = true;
            menu_ciclo(&cps, click, &console);
        }
        if (GetKeyState(VK_LSHIFT) & 0x8000)
            // Se o comando SHIFT + Seta para cima ou Seta para baixo for executado,
            // respectivamente, aumenta e diminui a velocidade dos cliques
        {
            if (GetAsyncKeyState(VK_UP) && (cps <= 20))
            {
                cps += 1;
                Sleep(250);
                menu_ciclo(&cps, click, &console);
            }
            if (GetAsyncKeyState(VK_DOWN) && (cps > 1))
            {
                cps -= 1;
                Sleep(250);
                menu_ciclo(&cps, click, &console);
            }
        }
        Sleep(250);
    }
}

int main()
{
    loop();
    return 0;
}