#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

int k;
double sin(), cos();

void setColor(const char *color) {
    // This function sets the terminal text color using ANSI escape codes
    printf("\x1b[38;2;%d;%d;%dm", 
           (int)strtol(color, NULL, 16) >> 16 & 0xFF,    // Red
           (int)strtol(color, NULL, 16) >> 8 & 0xFF,     // Green
           (int)strtol(color, NULL, 16) & 0xFF);         // Blue
}

void resetColor() {
    // Reset terminal color to default
    printf("\x1b[0m");
}

int main(int argc, char *argv[]) {
    // Default color code (white)
    char color[7] = "FFFFFF";
    int rainbow = 0;

    // Check for command-line arguments
    if (argc > 1) {
        if (strcmp(argv[1], "--rainbow") == 0) {
            rainbow = 1; // Enable rainbow mode
        } else if (strncmp(argv[1], "--", 2) == 0) {
            // User passed a hex color code
            strncpy(color, argv[1] + 2, 6);
            color[6] = '\0'; // Ensure it's null-terminated
        }
    }

    float A = 0, B = 0;
    float i, j;
    float z[1760];
    char b[1760];

    printf("\x1b[2J"); // Clear the screen

    for (;;) {
        memset(b, 32, 1760); // Fill the buffer with spaces
        memset(z, 0, 7040);   // Reset the depth buffer

        for (j = 0; j < 6.28; j += 0.07) {    // 0 to 2*PI for j
            for (i = 0; i < 6.28; i += 0.02) { // 0 to 2*PI for i
                float sini = sin(i);
                float cosi = cos(i);
                float sinj = sin(j);
                float cosj = cos(j);
                float cosA = cos(A);
                float sinA = sin(A);
                float cosB = cos(B);
                float sinB = sin(B);

                float cosj2 = cosj + 2;  // offset for torus
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

        // Clear the screen and move the cursor to the top left
        printf("\x1b[H");

        // Apply color if not in rainbow mode
        if (!rainbow) {
            setColor(color);
        }

        // Print the buffer
        for (k = 0; k < 1760; k++) {
            putchar(k % 80 ? b[k] : 10); // Print buffer contents, new line after every 80 chars
        }

        // Reset color if not in rainbow mode
        if (!rainbow) {
            resetColor();
        }

        A += 0.04;
        B += 0.02;

        if (rainbow) {
            // Change color rapidly in rainbow mode
            int r = (int)(127 * (1 + sin(A)));
            int g = (int)(127 * (1 + sin(A + 2)));
            int b = (int)(127 * (1 + sin(A + 4)));
            printf("\x1b[38;2;%d;%d;%dm", r, g, b);
        }

        usleep(30000); // Small delay to control the speed of the animation
    }

    return 0;
}
