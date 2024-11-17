#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

int k;
double sin(), cos();

// Default settings
double A = 0, B = 0;
float speed = 0.04;  // Default speed
char color[7] = "FFFFFF"; // Default color is white
int rainbow = 0;  // Default no rainbow

void setColor(const char *color) {
    printf("\x1b[38;2;%d;%d;%dm", 
           (int)strtol(color, NULL, 16) >> 16 & 0xFF,  // Red
           (int)strtol(color, NULL, 16) >> 8 & 0xFF,   // Green
           (int)strtol(color, NULL, 16) & 0xFF);       // Blue
}

void resetColor() {
    printf("\x1b[0m");
}

void printHelp() {
    printf("Usage: donut.c [OPTIONS]\n");
    printf("\nOptions:\n");
    printf("  -h, --help           Display help information.\n");
    printf("  -v, --version        Display the version number.\n");
    printf("  -s, --speed SPEED    Set the animation speed (higher is faster).\n");
    printf("  --rainbow            Enable rainbow color mode.\n");
    printf("  --<HEXCOLOR>         Set the color using a hex color code (e.g., --FF5733).\n");
}

void printVersion() {
    printf("donut.c version 0.1a\n");
}

int main(int argc, char *argv[]) {
    // Parse command line arguments
    int opt;
    while ((opt = getopt(argc, argv, "hvs:")) != -1) {
        switch (opt) {
            case 'h':  // Help
                printHelp();
                return 0;
            case 'v':  // Version
                printVersion();
                return 0;
            case 's':  // Speed
                speed = atof(optarg);  // Set speed from argument
                break;
            default:
                printHelp();
                return 1;
        }
    }

    // Parse for hex color and rainbow mode in the remaining arguments
    for (int i = optind; i < argc; i++) {
        if (strcmp(argv[i], "--rainbow") == 0) {
            rainbow = 1;
        } else if (strncmp(argv[i], "--", 2) == 0) {
            strncpy(color, argv[i] + 2, 6);  // Extract color code
            color[6] = '\0';  // Ensure null termination
        }
    }

    // Animation logic
    float i, j;
    float z[1760];
    char b[1760];

    printf("\x1b[2J");  // Clear the screen

    for (;;) {
        memset(b, 32, 1760);  // Fill the buffer with spaces
        memset(z, 0, 7040);   // Reset depth buffer

        for (j = 0; j < 6.28; j += 0.07) {  // 0 to 2*PI for j
            for (i = 0; i < 6.28; i += 0.02) {  // 0 to 2*PI for i
                float sini = sin(i);
                float cosi = cos(i);
                float sinj = sin(j);
                float cosj = cos(j);
                float cosA = cos(A);
                float sinA = sin(A);
                float cosB = cos(B);
                float sinB = sin(B);

                float cosj2 = cosj + 2;  // Offset for torus
                float mess = 1 / (sini * cosj2 * sinA + sinj * cosA + 5);
                float t = sini * cosj2 * cosA - sinj * sinA;

                int x = 40 + 30 * mess * (cosi * cosj2 * cosB - t * sinB);
                int y = 12 + 15 * mess * (cosi * cosj2 * sinB + t * cosB);
                int o = x + 80 * y;
                int N = 8 * ((sinj * sinA - sini * cosj * cosA) * cosB - sini * cosj * sinA - sinj * cosA - cosi * cosj * sinB);

                if (22 > y && y > 0 && x > 0 && 80 > x && mess > z[o]) {
                    z[o] = mess;
                    b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
            }
        }

        printf("\x1b[H");  // Clear the screen and move cursor to top

        // Apply color if not in rainbow mode
        if (!rainbow) {
            setColor(color);
        }

        // Print the buffer
        for (k = 0; k < 1760; k++) {
            putchar(k % 80 ? b[k] : 10);  // Print buffer content
        }

        // Reset color if not in rainbow mode
        if (!rainbow) {
            resetColor();
        }

        A += speed;  // Increment A (angle)
        B += speed / 2;  // Increment B (angle)

        if (rainbow) {
            // Change color rapidly in rainbow mode
            int r = (int)(127 * (1 + sin(A)));
            int g = (int)(127 * (1 + sin(A + 2)));
            int b = (int)(127 * (1 + sin(A + 4)));
            printf("\x1b[38;2;%d;%d;%dm", r, g, b);
        }

        usleep(30000);  // Small delay to control animation speed
    }

    return 0;
}
