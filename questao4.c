#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

// Função para calcular o máximo divisor comum (MDC) com exibição dos passos
int mdcComPassos(int a, int b)
{
    int resto;
    while (b != 0)
    { // [1]
        resto = a % b;
        printf("Algoritmo de Euclides: %d mod %d = %d\n", a, b, resto);
        a = b;
        b = resto; // [2]
    }
    return a;
}

// Função para calcular o inverso modular de 'a' em relação a 'm'
int inversoModular(int a, int m)
{
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;
    int A = a, B = m;

    mdcComPassos(a, m); // [3]

    while (m != 0)
    {
        q = a / m;
        t = m;
        m = a % m;
        a = t;

        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0)
        x1 += m0; // [4]

    printf("\nSubstituindo, temos que o inverso de %d em %d é %d.\n\n", A, B, x1);
    return x1;
}

// Função para exponenciação modular rápida
int powMod(int base, int exp, int mod)
{
    long long res = 1;
    long long b = base % mod;
    while (exp > 0)
    {
        if (exp & 1)
        { // [5]
            res = (res * b) % mod;
        }
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (int)res;
}

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    int H, G, Zn, x, n1;

    printf("Insira H: ");
    scanf("%d", &H);
    printf("Insira G: ");
    scanf("%d", &G);
    printf("Insira Zn: ");
    scanf("%d", &Zn);
    printf("Insira x: ");
    scanf("%d", &x);
    printf("Insira n1: ");
    scanf("%d", &n1);
    printf("\n");

    // Calcula o inverso modular de G em Zn
    int inverso = inversoModular(G, Zn); // [6]

    // Multiplicação modular H * inverso (mod Zn)
    int a = (H * inverso) % Zn;
    printf("Fazendo a multiplicação modular: %d * %d mod %d = %d\n", H, inverso, Zn, a);
    printf("Sendo %d o inverso de %d.\n", inverso, G);

    // Exponenciação modular
    int resultado = powMod(a, x, n1); // [7]
    printf("Valor final da congruência: %d\n", resultado);

    return 0;
}