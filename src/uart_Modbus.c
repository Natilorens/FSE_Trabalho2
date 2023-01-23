#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include "../inc/crc16.h"
#include <string.h>
#include <wiringPi.h> 



int LigaResistor(){

 const int PWM_pin = 4;
 int intensity ;            

  if (wiringPiSetup () == -1)
    exit (1) ;

  pinMode (PWM_pin, PWM_OUTPUT) ; /* set PWM pin as output */

  while (1)
  {
	
    for (intensity = 0 ; intensity < 100 ; ++intensity)
    {
      pwmWrite (PWM_pin, intensity) ;	/* provide PWM value for duty cycle */
      delay (1) ;
    }
    delay(1);

    for (intensity = 100 ; intensity >= 0 ; --intensity)
    {
      pwmWrite (PWM_pin, intensity) ;
      delay (1) ;
    }
    delay(1);
  }
}

int LigaVentoinha(){

 const int PWM_pin = 5;
 int intensity ;            

  if (wiringPiSetup () == -1)
    exit (1) ;

  pinMode (PWM_pin, PWM_OUTPUT) ; /* set PWM pin as output */

  while (1)
  {
	
    for (intensity = 0 ; intensity < 100 ; ++intensity)
    {
      pwmWrite (PWM_pin, intensity) ;	/* provide PWM value for duty cycle */
      delay (1) ;
    }
    delay(1);

    for (intensity = 100 ; intensity >= 0 ; --intensity)
    {
      pwmWrite (PWM_pin, intensity) ;
      delay (1) ;
    }
    delay(1);
  }
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
            printf("%i Bytes lidos : %.2f\n", rx_length, tempInt);
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
            float tempInt;
            memcpy(&tempInt, &rx_buffer[3], sizeof(float));
            
            //Bytes received
            rx_buffer[rx_length] = '\0';
            printf("%i Bytes lidos : %.2f\n", rx_length, tempInt);
        }
    }

    close(uart0_filestream);
}

void leComandosUsuario () {

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
            //float tempInt;
           // memcpy(&tempInt, &rx_buffer[3], sizeof(float));
            
            //Bytes received
            rx_buffer[rx_length] = '\0';
            printf("%i Bytes lidos : %.2f\n", rx_length/*, tempInt*/);
        }
    }

    close(uart0_filestream);
}



int main(int argc, const char * argv[]) {

    //solicitaTemperaturaInterna();
    //solicitaTemperaturaReferencia();
    //leComandosUsuario();
    int opcao;

    while(1){
        printf("Menu:\n");
        printf("1. Solicita Temperatura Interna\n");
        printf("2. Solicita Temperatura de referência\n");
        printf("3. Lê comandos do usuário\n");
        printf("4. Exit\n");

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
                LigaResistor();
                break;
            case 5:
                LigaVentoinha();
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
