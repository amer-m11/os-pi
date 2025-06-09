#include "shell/banner.h"
#include "utils/printf.h"

void print_block_banner(void)
{
    printf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
    printf("   ######   #####         ######   #####\r\n");
    printf("   #    #  #     #        #     #    #  \r\n");
    printf("   #    #  #              #     #    #  \r\n");
    printf("   #    #   #####  ----   ######     #  \r\n");
    printf("   #    #        #        #          #  \r\n");
    printf("   #    #  #     #        #          #  \r\n");
    printf("   ######   #####         #        #####\r\n");
    printf("=====================================\r\n");
    printf("         OS-Pi Shell v1.0\r\n");
    printf("    Raspberry Pi Operating System\r\n");
    printf("=====================================\r\n");
    printf("\r\n");
    printf("Welcome! Type 'help' for commands.\r\n");
    printf("\r\n");
}