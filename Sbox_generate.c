#include <stdio.h>

// pointer to actual place in pi
int p = 0;

// 722 digital number of pi
char pi[] = "314159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214"
            "808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442881097"
            "566593344612847564823378678316527120190914564856692346034861045432664821339360726024914127372458700660631"
            "558817488152092096282925409171536436789259036001133053054882046652138414695194151160943305727036575959195"
            "309218611738193261179310511854807446237996274956735188575272489122793818301194912983367336244065664308602"
            "139494639522473719070217986094370277053921717629317675238467481846766940513200056812714526356082778577134"
            "27577896091736371787214684409012249534301465495853710507922796892589235420199561121290219608";

// Generate pseudo random number in given range; based on pi digital numbers
int pi_random (int n) {
    int x = pi[p] - 48;
    p += 1;
    int y = 10;

    if (n > 10) {
        x = x*10 + pi[p] -48;
        p += 1;
        y = 100;
    }

    if (n > 100) {
        x = x*10 + pi[p] -48;
        p += 1;
        y = 1000;
    }

    // warunkek zapewniający rozkład jednostajny dyskretny, zapewnia równe pradopodobieństwo wylosowania każej liczby
    // w danym przedziale

    //if which gives us uniform distribution
    if (x < n*(y/n)) {
        return x % n;
    }
    else {
        return pi_random(n);
    }
}


int main() {

    // initialization of Sbox array
    int Sbox[256];
    for (int i = 0; i < 256; i++) {
        Sbox[i] = i;
    }

    int j;
    int tmp;

    // Generate random permutation with pi_random function
    for (int i = 1; i < 256; i++) {
        j = pi_random(i + 1);
        tmp = Sbox[j];
        Sbox[j] = Sbox[i];
        Sbox[i] = tmp;
    }

    // Print array
    printf("[");
    for (int i=0; i < 255; i++) {
        printf("%d, ", Sbox[i]);
    }
    printf("%d]\n", Sbox[255]);

    return 0;
    // return Sbox
}

/*
 * Output:
 * [41, 46, 67, 201, 162, 216, 124, 1, 61, 54, 84, 161, 236, 240, 6, 19, 98, 167, 5, 243, 192, 199, 115, 140, 152, 147,
 * 43, 217, 188, 76, 130, 202, 30, 155, 87, 60, 253, 212, 224, 22, 103, 66, 111, 24, 138, 23, 229, 18, 190, 78, 196,
 * 214, 218, 158, 222, 73, 160, 251, 245, 142, 187, 47, 238, 122, 169, 104, 121, 145, 21, 178, 7, 63, 148, 194, 16,
 * 137, 11, 34, 95, 33, 128, 127, 93, 154, 90, 144, 50, 39, 53, 62, 204, 231, 191, 247, 151, 3, 255, 25, 48, 179, 72,
 * 165, 181, 209, 215, 94, 146, 42, 172, 86, 170, 198, 79, 184, 56, 210, 150, 164, 125, 182, 118, 252, 107, 226, 156,
 * 116, 4, 241, 69, 157, 112, 89, 100, 113, 135, 32, 134, 91, 207, 101, 230, 45, 168, 2, 27, 96, 37, 173, 174, 176,
 * 185, 246, 28, 70, 97, 105, 52, 64, 126, 15, 85, 71, 163, 35, 221, 81, 175, 58, 195, 92, 249, 206, 186, 197, 234, 38,
 * 44, 83, 13, 110, 133, 40, 132, 9, 211, 223, 205, 244, 65, 129, 77, 82, 106, 220, 55, 200, 108, 193, 171, 250, 36,
 * 225, 123, 8, 12, 189, 177, 74, 120, 136, 149, 139, 227, 99, 232, 109, 233, 203, 213, 254, 59, 0, 29, 57, 242, 239,
 * 183, 14, 102, 88, 208, 228, 166, 119, 114, 248, 235, 117, 75, 10, 49, 68, 80, 180, 143, 237, 31, 26, 219, 153, 141,
 * 51, 159, 17, 131, 20]
 */
