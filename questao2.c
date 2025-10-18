#include <stdio.h>

// Função para calcular o Máximo Divisor Comum (MDC)
// utilizando o Algoritmo de Euclides.
// a = bq + r -> usando mod
int mdc(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int mmc(int a, int b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    // fórmula do mmc = (a * b) / mdc(a, b) => (a / mdc(a,b)) * b;
    return (a / mdc(a, b)) * b;
}

int main() {
    int n; 
    int ciclos[10]; 
    int i;

    printf("Digite o numero de chaves (N): ");
    scanf("%d", &n);

    // lê os N ciclos
    printf("Digite os %d ciclos separados por espaco: ", n);
    for (i = 0; i < n; i++) {
        scanf("%d", &ciclos[i]);
    }

    // cálculo do mmc (começa com o primeiro)
    int resultado_mmc = ciclos[0];

    // iteração a partir do segundo termo para pegar o (ciclo[0], ciclo[1]) e assim por diante...
    for (i = 1; i < n; i++) {
        resultado_mmc = mmc(resultado_mmc, ciclos[i]);
    }

    // verifica se o limite do resultado foi atingido
    if (resultado_mmc > 50) {
        printf("\nNao foi possivel encontrar um ano para sincronizacao dentro do limite de 50 anos.\n");
    } else {
        printf("\nO primeiro ano para sincronizacao e: %d\n", resultado_mmc);
    }

    return 0;
}