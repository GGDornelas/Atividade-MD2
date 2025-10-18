#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int mp_pre(char c);
char mp_de(int c);
long long mdc_ll(long long a, long long b);
long long g_rho(long long x, long long n);
long long f_rho(long long n);
long long ler_n(const char *nome);
long long inv_mod(long long a, long long m);
long long esc_e(long long ph);
long long pmod(long long m, long long e, long long n, long long ph);
void etapa3(long long e, long long d, long long n, long long ph);
int primo_ll(long long n);

int mp_pre(char c) {
    if (c >= 'A' && c <= 'Z') return 11 + (c - 'A');
    if (c >= 'a' && c <= 'z') return 11 + (c - 'a');
    if (c == ' ') return 0;
    return -1;
}

char mp_de(int c) {
    if (c == 0) return ' ';
    return 'A' + (c - 11);
}

long long mdc_ll(long long a, long long b) {
    long long A = a, B = b;
    printf("  -> Executando Euclides para mdc(%lld,%lld)\n", A, B);
    int p = 0;
    while (b != 0) {
        long long r = a % b;
        p++;
        printf("     passo %d: %lld = (%lld * %lld) + %lld\n", p, a, a / b, b, r);
        a = b;
        b = r;
    }
    printf("  -> Resultado final: mdc(%lld,%lld) = %lld\n", A, B, a);
    return a;
}

long long g_rho(long long x, long long n) {
    long long x2 = x * x;
    long long out = (x2 + 1) % n;
    printf("     g(%lld) = (%lld^2 + 1) %% %lld = %lld\n", x, x, n, out);
    return out;
}

long long f_rho(long long n) {
    if (n % 2 == 0) {
        printf("  Observação: %lld é par — fator trivial = 2\n", n);
        return 2;
    }
    long long x = 2, y = 2, d = 1;
    int it = 0;
    printf("\n  >>> Iniciando Pollard ρ para N = %lld\n", n);
    printf("      semente x0 = 2\n");
    while (d == 1) {
        it++;
        printf("\n    Iteração #%d\n", it);
        x = g_rho(x, n);
        long long t = g_rho(y, n);
        y = g_rho(t, n);
        long long diff = llabs(x - y);
        printf("    |x - y| = |%lld - %lld| = %lld\n", x, y, diff);
        d = mdc_ll(diff, n);
        if (d == 1) printf("    sem divisor não-trivial nesta rodada.\n");
        else if (d == n) {
            printf("    gcd == n -> falha na tentativa (tente outro N).\n");
            return -1;
        } else {
            printf("    divisor encontrado: %lld\n", d);
            break;
        }
    }
    return d;
}

long long ler_n(const char *nome) {
    long long n;
    int ok = 0;
    do {
        printf("Digite %s (produto de primos distintos entre 100 e 9999): ", nome);
        if (scanf("%lld", &n) != 1) {
            printf("  Entrada inválida — insira apenas números.\n");
            while (getchar() != '\n');
            continue;
        }
        if (n < 100 || n > 9999) printf("  Valor fora do intervalo. Tente novamente.\n");
        else ok = 1;
    } while (!ok);
    return n;
}

long long inv_mod(long long a, long long m) {
    long long r1 = a, r2 = m, s1 = 1, s2 = 0, t1 = 0, t2 = 1;
    int it = 0;
    printf("\n  >>> Iniciando Euclides Estendido para inverso de %lld mod %lld\n", a, m);
    while (r2 != 0) {
        it++;
        long long q = r1 / r2;
        long long r = r1 - q * r2;
        long long s = s1 - q * s2;
        long long t = t1 - q * t2;
        printf("    iter %d: q=%lld, r=%lld, s=%lld, t=%lld\n", it, q, r, s, t);
        r1 = r2; r2 = r;
        s1 = s2; s2 = s;
        t1 = t2; t2 = t;
    }
    if (r1 != 1) {
        printf("  Não existe inverso (mdc != 1). mdc=%lld\n", r1);
        return -1;
    }
    long long inv = s1;
    if (inv < 0) inv += m;
    printf("  Inverso modular: %lld (verificação: (%lld * %lld) %% %lld = %lld)\n", inv, a, inv, m, (a * inv) % m);
    return inv;
}

long long esc_e(long long ph) {
    printf("\n  Buscando expoente público E coprimo com φ=%lld...\n", ph);
    for (long long e = 2; e < ph; e++) {
        if (mdc_ll(e, ph) == 1) {
            printf("  -> E escolhido = %lld\n", e);
            return e;
        }
    }
    return -1;
}

long long pmod(long long m, long long e, long long n, long long ph) {
    printf("\n  Calculando %lld^%lld (mod %lld)\n", m, e, n);
    if (m % n == 0) {
        printf("   M é múltiplo de n -> resultado 0.\n");
        return 0;
    }
    long long exp = e;
    if (mdc_ll(m, n) == 1) {
        if (ph != n - 1) {
            printf("   Aplicando Teorema de Euler: reduzir expoente modulo φ(%lld)=%lld\n", n, ph);
            exp = e % ph;
            printf("   expo reduzido = %lld mod %lld = %lld\n", e, ph, exp);
        } else {
            printf("   Aplicando Pequeno Teorema de Fermat: n é primo\n");
            exp = e % (n - 1);
            printf("   expo reduzido = %lld mod %lld = %lld\n", e, (n - 1), exp);
        }
    } else {
        printf("   Aplicando redução pelo Algoritmo da Divisão Euclidiana\n");
        exp = e % (n - 1);
        printf("   expo reduzido = %lld mod %lld = %lld\n", e, (n - 1), exp);
    }
    long long r = 1, b = m % n, e1 = exp;
    while (e1 > 0) {
        if (e1 & 1) {
            long long a1 = r;
            r = (r * b) % n;
            printf("    multiplica: (%lld * %lld) mod %lld = %lld\n", a1, b, n, r);
        }
        long long b1 = b;
        b = (b * b) % n;
        e1 >>= 1;
        printf("    base^2: (%lld^2) mod %lld = %lld, expo agora = %lld\n", b1, n, b, e1);
    }
    printf("   -> Resultado: %lld\n", r);
    return r;
}

void etapa3(long long e, long long d, long long n, long long ph) {
    printf("\n--- Etapa 3: Criptografia / Descriptografia ---\n");
    char msg[1024];
    printf("Escreva a mensagem (somente letras e espaços): ");
    while (getchar() != '\n');
    fgets(msg, sizeof(msg), stdin);
    int bl[1024]; char org[1024]; char min[1024]; int t = 0;
    for (int i = 0; msg[i] != '\0' && msg[i] != '\n'; i++) {
        int cd = mp_pre(msg[i]);
        if (cd == -1) continue;
        bl[t] = cd; org[t] = msg[i];
        min[t] = (msg[i] >= 'a' && msg[i] <= 'z') ? 1 : 0;
        t++;
    }
    int w = 1;
    { long long tmp = n - 1; w = 0; do { w++; tmp /= 10; } while (tmp > 0); }
    long long cf[1024];
    printf("\n-- Criptografando cada bloco (2 dígitos) --\n");
    for (int i = 0; i < t; i++) {
        printf("\n Bloco M = %02d  (caractere '%c')\n", bl[i], org[i]);
        cf[i] = pmod(bl[i], e, n, ph);
        if (cf[i] == 0) printf("  Cifrado C = %02lld\n", cf[i]);
        else printf("  Cifrado C = %0*lld\n", w, cf[i]);
    }
    printf("\nMensagem cifrada (blocos):\n");
    for (int i = 0; i < t; i++) {
        if (cf[i] == 0) printf("%02lld ", cf[i]);
        else printf("%0*lld ", w, cf[i]);
    }
    printf("\n");
    printf("\n-- Decifrando --\n");
    char dec[t + 1];
    for (int i = 0; i < t; i++) {
        if (cf[i] == 0) printf("\n Bloco C = %02lld\n", cf[i]);
        else printf("\n Bloco C = %0*lld\n", w, cf[i]);
        long long M = pmod(cf[i], d, n, ph);
        char l = mp_de((int)M);
        if (M != 0 && min[i]) l = (char)(l + ('a' - 'A'));
        printf("  M recuperado = %02lld -> '%c'\n", M, l);
        dec[i] = l;
    }
    dec[t] = '\0';
    printf("\nMensagem decodificada: %s\n", dec);
}

int primo_ll(long long n) {
    if (n < 2) return 0;
    if (n % 2 == 0) return n == 2;
    for (long long i = 3; i * i <= n; i += 2) if (n % i == 0) return 0;
    return 1;
}

int main(void) {
    printf("=== Etapa 1: Fatoração interativa (ρ de Pollard) ===\n");
    printf("Nota: insira números resultantes do produto de primos distintos.\n\n");
    long long n1 = ler_n("N1");
    long long n2 = ler_n("N2");
    long long p1 = f_rho(n1);
    if (p1 == -1) { printf("\nErro na fatoração de N1. Encerrando.\n"); return 1; }
    long long cf1 = (p1 != 0) ? (n1 / p1) : 0;
    if (p1 <= 1 || cf1 <= 1 || p1 == cf1 || !primo_ll(p1) || !primo_ll(cf1)) {
        printf("\n[Validação] N1 não satisfaz a condição (primos distintos). Fatores detectados: %lld e %lld\n", p1, cf1);
        return 1;
    }
    long long q1 = f_rho(n2);
    if (q1 == -1) { printf("\nErro na fatoração de N2. Encerrando.\n"); return 1; }
    long long cf2 = (q1 != 0) ? (n2 / q1) : 0;
    if (q1 <= 1 || cf2 <= 1 || q1 == cf2 || !primo_ll(q1) || !primo_ll(cf2)) {
        printf("\n[Validação] N2 não satisfaz a condição (primos distintos). Fatores detectados: %lld e %lld\n", q1, cf2);
        return 1;
    }
    printf("\n=== Fatores finais obtidos ===\n");
    printf("p (de N1) = %lld\n", p1);
    printf("q (de N2) = %lld\n", q1);
    printf("\n=== Etapa 2: Geração das chaves RSA ===\n");
    long long nn = p1 * q1;
    printf("Módulo n = p * q = %lld * %lld = %lld\n", p1, q1, nn);
    long long ph = (p1 - 1) * (q1 - 1);
    printf("Totiente φ(n) = (p-1)*(q-1) = %lld\n", ph);
    long long en = esc_e(ph);
    if (en == -1) { printf("Não foi possível escolher E. Abortando.\n"); return 1; }
    long long dn = inv_mod(en, ph);
    if (dn == -1) { printf("Não existe inverso modular para E. Abortando.\n"); return 1; }
    printf("\n--- Par de chaves ---\n");
    printf("Chave pública  (n, e) = (%lld, %lld)\n", nn, en);
    printf("Chave privada (n, d) = (%lld, %lld)\n", nn, dn);
    etapa3(en, dn, nn, ph);
    return 0;
}