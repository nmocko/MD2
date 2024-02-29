#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Sbox (generated in Sbox_genrate.c program)

int Sbox[] = {41, 46, 67, 201, 162, 216, 124, 1, 61, 54, 84, 161,
              236, 240, 6, 19, 98, 167, 5, 243, 192, 199, 115,
              140, 152, 147, 43, 217, 188, 76, 130, 202, 30,
              155, 87, 60, 253, 212, 224, 22, 103, 66, 111,
              24, 138, 23, 229, 18, 190, 78, 196, 214, 218,
              158, 222, 73, 160, 251, 245, 142, 187, 47, 238,
              122, 169, 104, 121, 145, 21, 178, 7, 63, 148,
              194, 16, 137, 11, 34, 95, 33, 128, 127, 93, 154,
              90, 144, 50, 39, 53, 62, 204, 231, 191, 247, 151,
              3, 255, 25, 48, 179, 72, 165, 181, 209, 215,
              94, 146, 42, 172, 86, 170, 198, 79, 184, 56,
              210, 150, 164, 125, 182, 118, 252, 107, 226,
              156, 116, 4, 241, 69, 157, 112, 89, 100, 113,
              135, 32, 134, 91, 207, 101, 230, 45, 168, 2,
              27, 96, 37, 173, 174, 176, 185, 246, 28, 70,
              97, 105, 52, 64, 126, 15, 85, 71, 163, 35,
              221, 81, 175, 58, 195, 92, 249, 206, 186, 197,
              234, 38, 44, 83, 13, 110, 133, 40, 132, 9,
              211, 223, 205, 244, 65, 129, 77, 82, 106, 220,
              55, 200, 108, 193, 171, 250, 36, 225, 123, 8,
              12, 189, 177, 74, 120, 136, 149, 139, 227, 99,
              232, 109, 233, 203, 213, 254, 59, 0, 29, 57,
              242, 239, 183, 14, 102, 88, 208, 228, 166, 119,
              114, 248, 235, 117, 75, 10, 49, 68, 80, 180,
              143, 237, 31, 26, 219, 153, 141, 51, 159, 17,
              131, 20};


// take input from user dynamically

unsigned char * input_string () {
    printf("Enter message: ");
    int size = 2;
    int length = 0;
    int ch;
    unsigned char * input = (unsigned char*)malloc((size++)*sizeof(unsigned char));
    if (input == NULL) {
        printf("Error: memory not allocated in input_string, when length = %d\n", length);
        exit(1);
    }
    while(EOF != (ch = fgetc(stdin))  ) {  // && ch != '\n'
        input[length++] = ch;
        input = (unsigned char*)realloc(input, (size++)*sizeof(unsigned char));
        if (input == NULL) {
            printf("Error: memory not allocated in input_string, when length = %d\n", length);
            exit(1);
        }
    }
    input[length] = '\0';
    return input;
}

// add padding

unsigned char * add_padding(unsigned char * padded_message) {
    size_t length = strlen((char *) padded_message);
    size_t padding_length = 16 - length % 16;
    padded_message = realloc(padded_message, (padding_length + length) * sizeof(unsigned char));
    if (padded_message == NULL) {
        printf("Error: memory not allocated in add_padding, when length = %ld\n", length + padding_length);
        exit(1);
    }

    for (int i = 0; i < padding_length; i++) {
        padded_message[length++] = padding_length;
    }

    return padded_message;

}

// add control sum

unsigned char * append_checksum(unsigned char * msg) {

    unsigned char checksum[16] = {0};
    unsigned char buffer_l = 0;
    unsigned char buffer_c;
    size_t iteration = strlen((char*)msg) / 16;
    for (int i=0; i< iteration; i++) {
        for (int j=0; j < 16; j++) {
            buffer_c = msg[i*16 + j];
            checksum[j] = Sbox[buffer_c ^ buffer_l] ^ checksum[j];
            buffer_l = checksum[j];
        }
    }
    size_t size = strlen((char*)msg);
    msg = realloc(msg, (size + 16) * sizeof(unsigned char));
    for (int i = 0; i < 16; i++) {
        msg[size + i] = checksum[i];
    }

    return msg;
}

// count hash of message

unsigned char * md2sum(unsigned char * message_to_hash) {

    unsigned char* MD_buffer = (unsigned char*)malloc(48 * sizeof(unsigned char));

    for (int i=0; i<16; i++) {
        MD_buffer[i] = 0;
    }

    size_t iteration = strlen((char*)message_to_hash) / 16;
    unsigned char buffer_t = 0;

    for (int i=0; i<iteration; i++) {

        for (int j=0; j<16; j++) {
            MD_buffer[16+j] = message_to_hash[i*16 + j];
            MD_buffer[32+j] = MD_buffer[16+j] ^ MD_buffer[j];
        }

        // start of compression function
        printf("Hash %i: %s\n", i, MD_buffer);

        buffer_t = 0;
        for (int j=0; j<18; j++) {
            for (int k=0; k<48; k++) {
                buffer_t = MD_buffer[k] ^ Sbox[buffer_t];
                MD_buffer[k] = buffer_t;
            }
            buffer_t = (buffer_t + j) % 256;
        }
        printf("Hash %i: %s\n", i, MD_buffer);

        // end of compression function

    }

    unsigned char* MD_hash = (unsigned char*)malloc(16 * sizeof(unsigned char));

    for (int i=0; i< 16; i++) {
        MD_hash[i] = MD_buffer[i];
    }

    free(MD_buffer);

    return MD_hash;
}

// Print hash

void write_hash(unsigned char * hash) {
    printf("\nMD2 hash: ");
    for (int i=0; i< 16; i++) {
        printf("%.2X", hash[i]);
    }
    printf("\n");
}

int main() {

    // Take message
    unsigned char * message = input_string();

    // Add paddingu
    message = add_padding(message);

    // Add control sum
    message = append_checksum(message);

    // Count hash
    unsigned char * hash = md2sum(message);

    // Print hash
    write_hash(hash);

    return 0;
}
