#include <stdio.h>

/*
 * QUESTÃO 03 - Eficiência Numérica
 * Determinar a "razão de eficiência" de um número inteiro N.
 * 
 *      RE(N) = sigma(N) / tau(N)
 * 
 *   - tau(N) é o total de divisores de N
 *   - sigma(N) é a soma desses divisores
 */

int main() {
    int n;
    printf("-RAZAO DE EFICIENCIA DE UM NUMERO-\n");
    printf("Digite um inteiro N (1 <= N <= 100000): ");
    if (scanf("%d", &n) != 1 || n < 1 || n > 100000) {
        printf("Entrada invalida, <= 1 ou >= 100000\n");
        return 1;
    }

    // caso base 
    if (n == 1) {
        printf("\nN = 1 -> tau(1)=1, sigma(1)=1, Razao=1.00\n");
        return 0;
    }

    int bases[30], pot[30];
    int cont = 0;
    int valor = n;

    printf("\n[Etapa 1] Fatoracao de %d:\n", n);

    // divisor 2
    int e = 0;
    while (valor % 2 == 0) {
        valor /= 2;
        e++;
    }
    if (e > 0) {
        bases[cont] = 2;
        pot[cont] = e;
        cont++;
        printf("  2^%d\n", e);
    }

    // divisores ímpares
    for (int d = 3; d * d <= valor; d += 2) {
        e = 0;
        while (valor % d == 0) {
            valor /= d;
            e++;
        }
        if (e > 0) {
            bases[cont] = d;
            pot[cont] = e;
            cont++;
            printf("  %d^%d\n", d, e);
        }
    }

    // se restou algo primo
    if (valor > 1) {
        bases[cont] = valor;
        pot[cont] = 1;
        cont++;
        printf("  %d^1\n", valor);
    }

    int divs = 1;
    for (int i = 0; i < cont; i++) {
        divs *= (pot[i] + 1);
    }

    printf("\n[Etapa 2] tau(%d) = %d\n", n, divs);

    int somaDivs = 1;
    for (int i = 0; i < cont; i++) {
        int p = bases[i];
        int a = pot[i];
        int termo = 0;
        int potP = 1;

        for (int j = 0; j <= a; j++) {
            termo += potP;
            potP *= p;
        }
        somaDivs *= termo;
    }

    printf("[Etapa 3] sigma(%d) = %d\n", n, somaDivs);

    double eficiencia = (double)somaDivs / divs;
    printf("\n[Etapa 4] Razao de Eficiencia:\n");
    printf("  sigma/tau = %.2f\n", eficiencia);
    printf("\n==========================================\n");

    return 0;
}