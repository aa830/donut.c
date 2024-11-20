#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int k;
double sin(), cos();

// Default settings
double A = 0, B = 0;
float speed = 0.04;        // Default speed
float radius = 1.0;        // Default radius
char color1[7] = "FFFFFF"; // Default color1 is white
char color2[7] = "FFFFFF"; // Default color2 is white (no gradient)
int gradient = 0;          // Default no gradient
int rainbow = 0;           // Rainbow mode flag

void setColor(const char *color) {
  printf("\x1b[38;2;%d;%d;%dm",
         (int)strtol(color, NULL, 16) >> 16 & 0xFF, // Red
         (int)strtol(color, NULL, 16) >> 8 & 0xFF,  // Green
         (int)strtol(color, NULL, 16) & 0xFF);      // Blue
}

void resetColor() { printf("\x1b[0m"); }

void printHelp() {
  printf("Usage: donut.c [OPTIONS]\n");
  printf("\nOptions:\n");
  printf("  --help              Display help information.\n");
  printf("  --version           Display the version number.\n");
  printf("  --speed SPEED       Set the animation speed (higher is faster).\n");
  printf("  --radius RADIUS     Set the donut radius (default is 1.0).\n");
  printf("  --rainbow           Enable rainbow color mode.\n");
  printf("  --<HEXCOLOR1>       Set a solid color using a hex code (e.g., "
         "--FF5733).\n");
  printf("  --<HEXCOLOR1> --<HEXCOLOR2> Enable gradient mode between two hex "
         "colors.\n");
}

void printVersion() { printf("donut.c version v0.1-pre\n"); }

void interpolateColor(char *result, const char *color1, const char *color2,
                      float t) {
  int r1 = (int)strtol(color1, NULL, 16) >> 16 & 0xFF;
  int g1 = (int)strtol(color1, NULL, 16) >> 8 & 0xFF;
  int b1 = (int)strtol(color1, NULL, 16) & 0xFF;

  int r2 = (int)strtol(color2, NULL, 16) >> 16 & 0xFF;
  int g2 = (int)strtol(color2, NULL, 16) >> 8 & 0xFF;
  int b2 = (int)strtol(color2, NULL, 16) & 0xFF;

  int r = (int)((1 - t) * r1 + t * r2);
  int g = (int)((1 - t) * g1 + t * g2);
  int b = (int)((1 - t) * b1 + t * g2);

  sprintf(result, "%02X%02X%02X", r, g, b);
}

void hueToRGB(int hue, char *result) {
  float h = (float)(hue % 360) / 60.0;
  int c = 255;
  int x = (int)(c * (1 - fabs(fmod(h, 2) - 1)));

  int r, g, b;
  if (h < 1) {
    r = c;
    g = x;
    b = 0;
  } else if (h < 2) {
    r = x;
    g = c;
    b = 0;
  } else if (h < 3) {
    r = 0;
    g = c;
    b = x;
  } else if (h < 4) {
    r = 0;
    g = x;
    b = c;
  } else if (h < 5) {
    r = x;
    g = 0;
    b = c;
  } else {
    r = c;
    g = 0;
    b = x;
  }

  sprintf(result, "%02X%02X%02X", r, g, b);
}

int main(int argc, char *argv[]) {
  int colorCount = 0;

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
        speed = atof(argv[++i]); // Set speed from argument
      } else {
        fprintf(stderr, "Error: --speed requires a value.\n");
        return 1;
      }
    } else if (strncmp(argv[i], "--radius", 8) == 0) {
      if (i + 1 < argc) {
        radius = atof(argv[++i]); // Set radius from argument
      } else {
        fprintf(stderr, "Error: --radius requires a value.\n");
        return 1;
      }
    } else if (strcmp(argv[i], "--rainbow") == 0) {
      rainbow = 1; // Enable rainbow mode
    } else if (argv[i][0] == '-' && argv[i][1] == '-') {
      if (strlen(argv[i]) == 8) { // Check for hex color format "--XXXXXX"
        if (colorCount == 0) {
          strncpy(color1, argv[i] + 2, 6); // Set color1 from argument
          color1[6] = '\0';                // Ensure null termination
          colorCount++;
        } else if (colorCount == 1) {
          strncpy(color2, argv[i] + 2, 6); // Set color2 from second argument
          color2[6] = '\0';                // Ensure null termination
          gradient = 1; // Enable gradient if two colors are provided
          colorCount++;
        }
      } else {
        fprintf(stderr,
                "Error: Invalid color format %s. Use --XXXXXX format.\n",
                argv[i]);
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

  printf("\x1b[2J"); // Clear the screen

  int hue = 0; // Starting hue for rainbow mode

  for (;;) {
    memset(b, 32, 1760); // Fill the buffer with spaces
    memset(z, 0, 7040);  // Reset depth buffer

    for (j = 0; j < 6.28; j += 0.07) {   // 0 to 2*PI for j
      for (i = 0; i < 6.28; i += 0.02) { // 0 to 2*PI for i
        float sini = sin(i);
        float cosi = cos(i);
        float sinj = sin(j);
        float cosj = cos(j);
        float cosA = cos(A);
        float sinA = sin(A);
        float cosB = cos(B);
        float sinB = sin(B);

        float cosj2 = cosj + 2 * radius; // Adjust for radius
        float mess = 1 / (sini * cosj2 * sinA + sinj * cosA + 5 * radius);
        float t = sini * cosj2 * cosA - sinj * sinA;

        int x = 40 + 30 * mess * (cosi * cosj2 * cosB - t * sinB);
        int y = 12 + 15 * mess * (cosi * cosj2 * sinB + t * cosB);
        int o = x + 80 * y;
        int N = 8 * ((sinj * sinA - sini * cosj * cosA) * cosB -
                     sini * cosj * sinA - sinj * cosA - cosi * cosj * sinB);

        if (22 > y && y > 0 && x > 0 && 80 > x && mess > z[o]) {
          z[o] = mess;
          b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
        }
      }
    }

    printf("\x1b[H"); // Clear the screen and move cursor to top

    // Apply rainbow mode if enabled
    if (rainbow) {
      static char rainbowColor[7];
      hueToRGB(hue, rainbowColor);
      setColor(rainbowColor);
      hue = (hue + 5) % 360; // Increment hue for next frame
    } else if (gradient) {
      static char gradientColor[7];
      float t = (sin(A) + 1) / 2; // t varies from 0 to 1
      interpolateColor(gradientColor, color1, color2, t);
      setColor(gradientColor);
    } else {
      setColor(color1); // Use the first color if gradient is not enabled
    }

    // Print the buffer
    for (k = 0; k < 1760; k++) {
      putchar(k % 80 ? b[k] : 10); // Print buffer content
    }

    resetColor(); // Reset color after each frame

    A += speed;     // Increment A (angle)
    B += speed / 2; // Increment B (angle)

    usleep(30000); // Small delay to control animation speed
  }

  return 0;
}
