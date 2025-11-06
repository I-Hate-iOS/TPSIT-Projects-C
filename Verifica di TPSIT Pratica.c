/*
Scrivere il codice in c di un programma concorrente che presi due numeri in ingresso dall’utente, generi 8 processi. 
Ogni processo effettua i seguenti calcoli: 1) Se la differenza tra il pid del processo e il pid del processo padre è <3, fare la moltiplicazione tra i due numeri,
altrimenti fare la divisione del primo sul secondo numero. Nel caso del processo padre originale fare la moltiplicazione tra il primo numero e ½ del secondo. 
2) Se la differenza tra il pid del processo e il pid del processo padre originale è <5, fare la somma tra i due numeri, altrimenti fare la sottrazione fra di essi. 
Nel caso del processo padre originale fare la somma tra il primo numero e ½ del secondo.
*/ 

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int numero1, numero2;
    pid_t pid_padre = getpid();

    printf("Inserisci il primo numero: ");
    scanf("%d", &numero1);
    printf("Inserisci il secondo numero: ");
    scanf("%d", &numero2);

    for (int i = 0; i < 8; i++) {
        pid_t pid = fork();

        if (pid == 0) {  // codice del figlio
            pid_t mio_pid = getpid();
            pid_t padre_locale = getppid();

            int differenza1 = mio_pid - padre_locale;
            if (differenza1 < 0) differenza1 = -differenza1;

            int differenza2 = mio_pid - pid_padre;
            if (differenza2 < 0) differenza2 = -differenza2;

            printf("\n[FIGLIO %d] PID=%d | Padre=%d | diff1=%d | diff2=%d\n",
                   i + 1, mio_pid, padre_locale, differenza1, differenza2);

            // --- Parte 1 ---
            if (differenza1 < 3)
                printf("[FIGLIO %d] Moltiplicazione: %d * %d = %d\n",
                       i + 1, numero1, numero2, numero1 * numero2);
            else
                printf("[FIGLIO %d] Divisione: %d / %d = %.2f\n",
                       i + 1, numero1, numero2, (float)numero1 / numero2);

            // --- Parte 2 ---
            if (differenza2 < 5)
                printf("[FIGLIO %d] Somma: %d + %d = %d\n",
                       i + 1, numero1, numero2, numero1 + numero2);
            else
                printf("[FIGLIO %d] Sottrazione: %d - %d = %d\n",
                       i + 1, numero1, numero2, numero1 - numero2);

            exit(0); // fine figlio
        }
    }

    // padre attende tutti i figli
    for (int i = 0; i < 8; i++) {
        wait(NULL);
    }

    // calcoli del padre
    float moltiplicazione_padre = numero1 * (numero2 / 2.0);
    float somma_padre = numero1 + (numero2 / 2.0);

    printf("\n[PADRE] Moltiplicazione speciale: %d * (%d / 2) = %.2f\n",
           numero1, numero2, moltiplicazione_padre);
    printf("[PADRE] Somma speciale: %d + (%d / 2) = %.2f\n",
           numero1, numero2, somma_padre);

    printf("\n[PADRE] Tutti i processi hanno terminato.\n");

    return 0;
}
