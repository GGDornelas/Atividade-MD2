#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* mapa_pre */
int mapa_pre(char simbolo) {
    if (simbolo >= 'A' && simbolo <= 'Z') return 11 + (simbolo - 'A');
    if (simbolo >= 'a' && simbolo <= 'z') return 11 + (simbolo - 'a');
    if (simbolo == ' ') return 0;
    return -1;
}

/* mapa_de */
char mapa_de(int codigo) {
    if (codigo == 0) return ' ';
    return 'A' + (codigo - 11);
}

/* mdc */
long long mdc(long long x, long long y) {
    while (y != 0) {
        long long resto = x % y;
        x = y;
        y = resto;
    }
    return x;
}

/* g_pollard */
long long g_pollard(long long valor, long long modulo) {
    return (valor * valor + 1) % modulo;
}

/* fator_pollard */
long long fator_pollard(long long numero) {
    if (numero % 2 == 0) return 2;
    long long a = 2, b = 2, d = 1;
    while (d == 1) {
        a = g_pollard(a, numero);
        b = g_pollard(g_pollard(b, numero), numero);
        d = mdc(llabs(a - b), numero);
    }
    return d;
}

/* ler_N */
long long ler_N(const char *texto) {
    long long valor;
    int valido = 0;
    do {
        if (scanf("%lld", &valor) != 1) {
            while (getchar() != '\n')
                ;  // limpa buffer (corpo vazio intencional)
            continue;
        }
        if (valor >= 100 && valor <= 9999) valido = 1;
    } while (!valido);
    return valor;
}

/* inverso_extenso */
long long inverso_extenso(long long base, long long mod) {
    long long r1 = base, r2 = mod, s1 = 1, s2 = 0, t1 = 0, t2 = 1;
    while (r2 != 0) {
        long long q = r1 / r2;
        long long r = r1 - q * r2;
        long long s = s1 - q * s2;
        long long t = t1 - q * t2;
        r1 = r2; r2 = r; s1 = s2; s2 = s; t1 = t2; t2 = t;
    }
    if (r1 != 1) return -1;
    if (s1 < 0) s1 += mod;
    return s1;
}

/* escolherE */
long long escolherE(long long fi) {
    for (long long expo = 2; expo < fi; expo++)
        if (mdc(expo, fi) == 1) return expo;
    return -1;
}

/* powmod */
long long powmod(long long base, long long expoente, long long mod, long long fi) {
    long long resultado = 1, b = base % mod, e = expoente;
    while (e > 0) {
        if (e & 1) resultado = (resultado * b) % mod;
        b = (b * b) % mod;
        e >>= 1;
    }
    return resultado;
}

/* etapa3_processa */
void etapa3_processa(long long expoente_pub, long long expoente_priv, long long modulo, long long fi) {
    char texto[1024];
    while (getchar() != '\n')
        ; // limpa resto da linha
    fgets(texto, sizeof(texto), stdin);

    int blocos[1024], tam = 0;
    char minusculo[1024];
    char original[1024];

    for (int i = 0; texto[i] != '\0' && texto[i] != '\n'; i++) {
        int cod = mapa_pre(texto[i]);
        if (cod == -1) continue;
        blocos[tam] = cod;
        original[tam] = texto[i];
        minusculo[tam] = (texto[i] >= 'a' && texto[i] <= 'z') ? 1 : 0;
        tam++;
    }

    int largura = 1;
    for (long long tmp = modulo - 1; largura = 0, tmp > 0; largura++, tmp /= 10);

    long long cifrado[1024];
    for (int i = 0; i < tam; i++) cifrado[i] = powmod(blocos[i], expoente_pub, modulo, fi);

    char decod[tam + 1];
    for (int i = 0; i < tam; i++) {
        long long msg = powmod(cifrado[i], expoente_priv, modulo, fi);
        char letra = mapa_de((int)msg);
        if (msg != 0 && minusculo[i]) letra += (char)('a' - 'A');
        decod[i] = letra;
    }
    decod[tam] = '\0';
}

/* eh_primo */
int eh_primo(long long valor) {
    if (valor < 2) return 0;
    if (valor % 2 == 0) return valor == 2;
    for (long long i = 3; i * i <= valor; i += 2)
        if (valor % i == 0) return 0;
    return 1;
}

/* main */
int main(void) {
    long long num1 = ler_N("N1");
    long long num2 = ler_N("N2");

    long long fator1 = fator_pollard(num1);
    long long outro1 = (fator1 != 0) ? (num1 / fator1) : 0;
    if (fator1 <= 1 || outro1 <= 1 || fator1 == outro1 || !eh_primo(fator1) || !eh_primo(outro1)) return 1;

    long long fator2 = fator_pollard(num2);
    long long outro2 = (fator2 != 0) ? (num2 / fator2) : 0;
    if (fator2 <= 1 || outro2 <= 1 || fator2 == outro2 || !eh_primo(fator2) || !eh_primo(outro2)) return 1;

    long long modulo = fator1 * fator2;
    long long fi = (fator1 - 1) * (fator2 - 1);

    long long expoente_pub = escolherE(fi);
    if (expoente_pub == -1) return 1;
    long long expoente_priv = inverso_extenso(expoente_pub, fi);
    if (expoente_priv == -1) return 1;

    etapa3_processa(expoente_pub, expoente_priv, modulo, fi);
    return 0;
}