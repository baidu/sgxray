#include "Enclave.h"

/* 
 * printf: 
 *   Invokes OCALL to display the enclave buffer to the terminal.
 */
int printf(const char *fmt, ...)
{
    char buf[BUFSIZ] = {'\0'};
    va_list ap;
    va_start(ap, fmt);

    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    ocall_print_string(buf);
    return (int)strnlen(buf, BUFSIZ - 1) + 1;
}

int benign1(){
    return 0;
}

int benign2(){
    return 0;
}

int benign3(){
    return 0;
}

int malicious1(){
    int  secret = 12;
    return secret;
}

int fib(int n) 
{ 
    int f[n + 1]; 
    int i; 
  
    f[0] = 0; 
    f[1] = 1; 
  
    for (i = 2; i <= n; i++) { 

        f[i] = f[i - 1] + f[i - 2]; 
    } 
  
    return f[n]; 
} 