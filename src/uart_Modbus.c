#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include "../inc/crc16.h"
#include <string.h>
#include <wiringPi.h> 
#include <stdlib.h>


void enviaEstadoSistema (){

    int uart0_filestream = -1;
    int estado;

    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);      //Open in non blocking read/write mode
    if (uart0_filestream == -1)
    {
        printf("Erro - Não foi possível iniciar a UART.\n");
    }
    else
    {
        printf("UART inicializada!\n");
    }    
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    unsigned char tx_buffer[20];
    unsigned char *p_tx_buffer;
    
    unsigned char mensagemCrc[2];

    p_tx_buffer = &tx_buffer[0];
    *p_tx_buffer++ = 0x01;
    *p_tx_buffer++ = 0x16;
    *p_tx_buffer++ = 0xD3;
    *p_tx_buffer++ = 6;
    *p_tx_buffer++ = 9;
    *p_tx_buffer++ = 0;
    *p_tx_buffer++ = 9;
     *p_tx_buffer++ = estado;
    
    short valor_Calculado = calcula_CRC(&tx_buffer[0], 8);

    memcpy(mensagemCrc,&valor_Calculado, sizeof(int));

    *p_tx_buffer++ = mensagemCrc[0];
    *p_tx_buffer++ = mensagemCrc[1];

    printf("Buffers de memória criados!\n");
    
    if (uart0_filestream != -1)
    {
        printf("Escrevendo caracteres na UART ...");
        int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
        if (count < 0)
        {
            printf("UART TX error\n");
        }
        else
        {
            printf("escrito.\n");
        }
    }

    sleep(1);

    //----- CHECK FOR ANY RX BYTES -----
    if (uart0_filestream != -1)
    {
        // Read up to 255 characters from the port if they are there
        unsigned char rx_buffer[256];
        int rx_length = read(uart0_filestream, (void*)rx_buffer, 255);      //Filestream, buffer to store in, number of bytes to read (max)
        if (rx_length < 0)
        {
            printf("Erro na leitura.\n"); //An error occured (will occur if there are no bytes)
        }
        else if (rx_length == 0)
        {
            printf("Nenhum dado disponível.\n"); //No data waiting
        }
        else
        {
           
            
            //Bytes received
            rx_buffer[rx_length] = '\0';
            printf("%i Bytes lidos\n", rx_length);
        }
    }

    close(uart0_filestream);


}


void enviaSinalControle (){

 int uart0_filestream = -1;

    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);      //Open in non blocking read/write mode
    if (uart0_filestream == -1)
    {
        printf("Erro - Não foi possível iniciar a UART.\n");
    }
    else
    {
        printf("UART inicializada!\n");
    }    
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    unsigned char tx_buffer[20];
    unsigned char *p_tx_buffer;
    
    unsigned char mensagemCrc[2];

    p_tx_buffer = &tx_buffer[0];
    *p_tx_buffer++ = 0x01;
    *p_tx_buffer++ = 0x16;
    *p_tx_buffer++ = 0xD1;
    *p_tx_buffer++ = 6;
    *p_tx_buffer++ = 9;
    *p_tx_buffer++ = 0;
    *p_tx_buffer++ = 9;
    
    short valor_Calculado = calcula_CRC(&tx_buffer[0], 11);

    memcpy(mensagemCrc,&valor_Calculado, sizeof(char));

    *p_tx_buffer++ = mensagemCrc[0];
    *p_tx_buffer++ = mensagemCrc[1];

    printf("Buffers de memória criados!\n");
    
    if (uart0_filestream != -1)
    {
        printf("Escrevendo caracteres na UART ...");
        int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
        if (count < 0)
        {
            printf("UART TX error\n");
        }
        else
        {
            printf("escrito.\n");
        }
    }

    sleep(1);

    //----- CHECK FOR ANY RX BYTES -----
    if (uart0_filestream != -1)
    {
        // Read up to 255 characters from the port if they are there
        unsigned char rx_buffer[256];
        int rx_length = read(uart0_filestream, (void*)rx_buffer, 255);      //Filestream, buffer to store in, number of bytes to read (max)
        if (rx_length < 0)
        {
            printf("Erro na leitura.\n"); //An error occured (will occur if there are no bytes)
        }
        else if (rx_length == 0)
        {
            printf("Nenhum dado disponível.\n"); //No data waiting
        }
        else
        {
           
            
            //Bytes received
            rx_buffer[rx_length] = '\0';
            printf("%i Bytes lidos\n", rx_length);
        }
    }

    close(uart0_filestream);

}


void solicitaTemperaturaInterna () {

    int uart0_filestream = -1;

    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);      //Open in non blocking read/write mode
    if (uart0_filestream == -1)
    {
        printf("Erro - Não foi possível iniciar a UART.\n");
    }
    else
    {
        printf("UART inicializada!\n");
    }    
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    unsigned char tx_buffer[20];
    unsigned char *p_tx_buffer;
    
    unsigned char mensagemCrc[2];

    p_tx_buffer = &tx_buffer[0];
    *p_tx_buffer++ = 0x01;
    *p_tx_buffer++ = 0x23;
    *p_tx_buffer++ = 0xC1;
    *p_tx_buffer++ = 6;
    *p_tx_buffer++ = 9;
    *p_tx_buffer++ = 0;
    *p_tx_buffer++ = 9;
    
    short valor_Calculado = calcula_CRC(&tx_buffer[0], 7);

    memcpy(mensagemCrc,&valor_Calculado, sizeof(short));

    *p_tx_buffer++ = mensagemCrc[0];
    *p_tx_buffer++ = mensagemCrc[1];

    printf("Buffers de memória criados!\n");
    
    if (uart0_filestream != -1)
    {
        printf("Escrevendo caracteres na UART ...");
        int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
        if (count < 0)
        {
            printf("UART TX error\n");
        }
        else
        {
            printf("escrito.\n");
        }
    }

    sleep(1);

    //----- CHECK FOR ANY RX BYTES -----
    if (uart0_filestream != -1)
    {
        // Read up to 255 characters from the port if they are there
        unsigned char rx_buffer[256];
        int rx_length = read(uart0_filestream, (void*)rx_buffer, 255);      //Filestream, buffer to store in, number of bytes to read (max)
        if (rx_length < 0)
        {
            printf("Erro na leitura.\n"); //An error occured (will occur if there are no bytes)
        }
        else if (rx_length == 0)
        {
            printf("Nenhum dado disponível.\n"); //No data waiting
        }
        else
        {
            float tempInt;
            memcpy(&tempInt, &rx_buffer[3], sizeof(float));
            
            //Bytes received
            rx_buffer[rx_length] = '\0';
            printf("Temperatura Interna: %.2f\n", tempInt);
        }
    }

    close(uart0_filestream);
}

void solicitaTemperaturaReferencia () {

    int uart0_filestream = -1;

    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);      //Open in non blocking read/write mode
    if (uart0_filestream == -1)
    {
        printf("Erro - Não foi possível iniciar a UART.\n");
    }
    else
    {
        printf("UART inicializada!\n");
    }    
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    unsigned char tx_buffer[20];
    unsigned char *p_tx_buffer;
    
    unsigned char mensagemCrc[2];

    p_tx_buffer = &tx_buffer[0];
    *p_tx_buffer++ = 0x01;
    *p_tx_buffer++ = 0x23;
    *p_tx_buffer++ = 0xC2;
    *p_tx_buffer++ = 6;
    *p_tx_buffer++ = 9;
    *p_tx_buffer++ = 0;
    *p_tx_buffer++ = 9;
    
    short valor_Calculado = calcula_CRC(&tx_buffer[0], 7);

    memcpy(mensagemCrc,&valor_Calculado, sizeof(short));

    *p_tx_buffer++ = mensagemCrc[0];
    *p_tx_buffer++ = mensagemCrc[1];

    printf("Buffers de memória criados!\n");
    
    if (uart0_filestream != -1)
    {
        printf("Escrevendo caracteres na UART ...");
        int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
        if (count < 0)
        {
            printf("UART TX error\n");
        }
        else
        {
            printf("escrito.\n");
        }
    }

    sleep(1);

    //----- CHECK FOR ANY RX BYTES -----
    if (uart0_filestream != -1)
    {
        // Read up to 255 characters from the port if they are there
        unsigned char rx_buffer[256];
        int rx_length = read(uart0_filestream, (void*)rx_buffer, 255);      //Filestream, buffer to store in, number of bytes to read (max)
        if (rx_length < 0)
        {
            printf("Erro na leitura.\n"); //An error occured (will occur if there are no bytes)
        }
        else if (rx_length == 0)
        {
            printf("Nenhum dado disponível.\n"); //No data waiting
        }
        else
        {
            float tempRef;
            memcpy(&tempRef, &rx_buffer[3], sizeof(float));
            
            //Bytes received
            rx_buffer[rx_length] = '\0';
            printf("Temperatura de Referência: %.2f\n", tempRef);
        }
    }

    close(uart0_filestream);
}

void leComandosUsuario () {

    int uart0_filestream = -1;
    int estado;

    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);      //Open in non blocking read/write mode
    if (uart0_filestream == -1)
    {
        printf("Erro - Não foi possível iniciar a UART.\n");
    }
    else
    {
        printf("UART inicializada!\n");
    }    
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    unsigned char tx_buffer[20];
    unsigned char *p_tx_buffer;
    
    unsigned char mensagemCrc[2];

    p_tx_buffer = &tx_buffer[0];
    *p_tx_buffer++ = 0x01;
    *p_tx_buffer++ = 0x23;
    *p_tx_buffer++ = 0xC3;
    *p_tx_buffer++ = 6;
    *p_tx_buffer++ = 9;
    *p_tx_buffer++ = 0;
    *p_tx_buffer++ = 9;
    
    short valor_Calculado = calcula_CRC(&tx_buffer[0], 7);

    memcpy(mensagemCrc,&valor_Calculado, sizeof(int));

    *p_tx_buffer++ = mensagemCrc[0];
    *p_tx_buffer++ = mensagemCrc[1];

    printf("Buffers de memória criados!\n");
    
    if (uart0_filestream != -1)
    {
        printf("Escrevendo caracteres na UART ...");
        int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
        if (count < 0)
        {
            printf("UART TX error\n");
        }
        else
        {
            printf("escrito.\n");
        }
    }

    sleep(1);

    //----- CHECK FOR ANY RX BYTES -----
    if (uart0_filestream != -1)
    {
        // Read up to 255 characters from the port if they are there
        unsigned char rx_buffer[256];

        while(1){
            int rx_length = read(uart0_filestream, (void*)rx_buffer, 255);      //Filestream, buffer to store in, number of bytes to read (max)
            if (rx_length < 0)
            {
                printf("Erro na leitura.\n"); //An error occured (will occur if there are no bytes)
            }
            else if (rx_length == 0)
            {
                printf("Nenhum dado disponível.\n"); //No data waiting
            }
            else
            {
                      int comandoLido;
                      memcpy(&comandoLido, &rx_buffer[3], sizeof(int));

                    rx_buffer[rx_length] = '\0';
                     printf("Comando lido %i\n", comandoLido);
                switch (rx_buffer[0]) {
                case 0xA1:
                    estado = 1; //ligado
                    enviaEstadoSistema(estado);
                    break;
                case 0xA2:
                    estado = 0; //desligado
                enviaEstadoSistema(estado);
                    break;
                case 0xA3:
                   // não foi feito
                    break;
                default:
                    printf("Comando inválido.\n");
                    break;
                }
            }

            usleep(500000); // ler a cada 500 ms
    }

        close(uart0_filestream);
    }

}

int main(void) {

   
    int opcao;
    int estado;

    while(1){
        printf("Menu:\n");
        printf("1. Solicita Temperatura Interna\n");
        printf("2. Solicita Temperatura de referência\n");
        printf("3. Lê comandos do usuário\n");
        printf("4. Envia sinal de controle\n");
        printf("5. Envia Estado do Sistema\n");
        printf("6. Exit\n");

        printf("Digite a opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                solicitaTemperaturaInterna();
                break;
            case 2:
                solicitaTemperaturaReferencia();
                break;
            case 3:
                leComandosUsuario();
                break;
            case 4:
                enviaSinalControle(); //está com erro de CRC...
                break;
            case 5:
                enviaEstadoSistema(estado);
                break;
            case 6:
                printf("Exiting...\n");
                return 0;
                break;
            default:
                printf("Opcao invalida.\n");
                break;
        }
    }

    return 0;
}


