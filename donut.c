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
char color1[7] = "FFFFFF"; // Default color1 is white
char color2[7] = "FFFFFF"; // Default color2 is white (no gradient)
int gradient = 0;  // Default no gradient
int rainbow = 0;   // Rainbow mode flag

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
    printf("  --help              Display help information.\n");
    printf("  --version           Display the version number.\n");
    printf("  --speed SPEED       Set the animation speed (higher is faster).\n");
    printf("  --rainbow           Enable rainbow color mode.\n");
    printf("  --gradient          Enable gradient color mode between two hex colors.\n");
    printf("  --color1 <HEXCODE>  Set the first color for the gradient or solid color using a hex code (e.g., --color1 FF5733).\n");
    printf("  --color2 <HEXCODE>  Set the second color for the gradient using a hex code (e.g., --color2 3498DB).\n");
}

void printVersion() {
    printf("donut.c version v0.1-pre\n");
}

// Function to interpolate between two RGB colors
void interpolateColor(char *result, const char *color1, const char *color2, float t) {
    int r1 = (int)strtol(color1, NULL, 16) >> 16 & 0xFF;
    int g1 = (int)strtol(color1, NULL, 16) >> 8 & 0xFF;
    int b1 = (int)strtol(color1, NULL, 16) & 0xFF;

    int r2 = (int)strtol(color2, NULL, 16) >> 16 & 0xFF;
    int g2 = (int)strtol(color2, NULL, 16) >> 8 & 0xFF;
    int b2 = (int)strtol(color2, NULL, 16) & 0xFF;

    int r = (int)((1 - t) * r1 + t * r2);
    int g = (int)((1 - t) * g1 + t * g2);
    int b = (int)((1 - t) * b1 + t * b2);

    sprintf(result, "%02X%02X%02X", r, g, b);
}

// Function to convert hue to RGB
void hueToRGB(int hue, char *result) {
    float h = (float)(hue % 360) / 60.0;
    int c = 255;
    int x = (int)(c * (1 - fabs(fmod(h, 2) - 1)));

    int r, g, b;
    if (h < 1) {
        r = c; g = x; b = 0;
    } else if (h < 2) {
        r = x; g = c; b = 0;
    } else if (h < 3) {
        r = 0; g = c; b = x;
    } else if (h < 4) {
        r = 0; g = x; b = c;
    } else if (h < 5) {
        r = x; g = 0; b = c;
    } else {
        r = c; g = 0; b = x;
    }

    sprintf(result, "%02X%02X%02X", r, g, b);
}

int main(int argc, char *argv[]) {
    // Check for command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            printHelp();
            return 0;
        } else if (strcmp(argv[i], "--version") == 0) {
            printVersion();
            return 0;
        } else if (strncmp(argv[i], "--speed", 7) == 0) {
            if (i + 1 < argc) {
                speed = atof(argv[++i]);  // Set speed from argument
            } else {
                fprintf(stderr, "Error: --speed requires a value.\n");
                return 1;
            }
        } else if (strcmp(argv[i], "--rainbow") == 0) {
            rainbow = 1;  // Enable rainbow mode
        } else if (strcmp(argv[i], "--gradient") == 0) {
            gradient = 1;  // Enable gradient mode
        } else if (strcmp(argv[i], "--color1") == 0) {
            if (i + 1 < argc) {
                strncpy(color1, argv[++i], 6);  // Set color1 from argument
                color1[6] = '\0';  // Ensure null termination
            } else {
                fprintf(stderr, "Error: --color1 requires a hex color code.\n");
                return 1;
            }
        } else if (strcmp(argv[i], "--color2") == 0) {
            if (i + 1 < argc) {
                strncpy(color2, argv[++i], 6);  // Set color2 from argument
                color2[6] = '\0';  // Ensure null termination
            } else {
                fprintf(stderr, "Error: --color2 requires a hex color code.\n");
                return 1;
            }
        } else {
            fprintf(stderr, "Error: Unknown option %s\n", argv[i]);
            return 1;
        }
    }

    // Animation logic
    float i, j;
    float z[1760];
    char b[1760];

    printf("\x1b[2J");  // Clear the screen

    int hue = 0;  // Starting hue for rainbow mode

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

        // Apply rainbow mode if enabled
        if (rainbow) {
            static char rainbowColor[7];
            hueToRGB(hue, rainbowColor);
            setColor(rainbowColor);
            hue = (hue + 5) % 360;  // Increment hue for next frame
        } else if (gradient) {
            static char gradientColor[7];
            float t = (sin(A) + 1) / 2;  // t varies from 0 to 1
            interpolateColor(gradientColor, color1, color2, t);
            setColor(gradientColor);
        } else {
            setColor(color1);  // Use the first color if gradient is not enabled
        }

        // Print the buffer
        for (k = 0; k < 1760; k++) {
            putchar(k % 80 ? b[k] : 10);  // Print buffer content
        }

        resetColor();  // Reset color after each frame

        A += speed;  // Increment A (angle)
        B += speed / 2;  // Increment B (angle)

        usleep(30000);  // Small delay to control animation speed
    }

    return 0;
}
