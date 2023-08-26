#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gnuplot_i.h"

#ifdef _WIN32
    #define TERMINAL "windows"
    #define PID      system("tasklist | findstr gnuplot")
#elif __linux__
    #define TERMINAL "x11"
    #define PID      system("pidof -x gnuplot")
#endif

#define SECONDS 1000
#define NPOINTS 50

char equation[100];
char plot[] = "plot ";
char prev;


void format_equation(char* s);
int is_digit(float value);
void insert(char* s, const char* insert, int pos);
void removeChar(char *str, char garbage);
void remove_spaces(char* s);

int main(int argc, char *argv[]) {
    gnuplot_ctrl *h1, *h2, *h3;
    double x[NPOINTS], y[NPOINTS], z[NPOINTS];
    int i;

    /** Initialize the gnuplot handle */

    printf("*** enter math equation to plot: ***\n");
    fgets(equation, sizeof(equation), stdin);

    remove_spaces(equation);
    format_equation(equation);

    printf("%s\n", equation);

    strcat(plot, equation);
    h1 = gnuplot_init();

    /** Equations */

    printf("\n*** creating terminal ***\n");
    gnuplot_setterm(h1, TERMINAL, 1280, 960);
    //gnuplot_cmd(h1, "plot sin(x) w lines, cos(x) w lines");
    gnuplot_setstyle(h1, "lines");
    gnuplot_cmd(h1, "set grid");
    //gnuplot_cmd(h1, "set xtics 1");
    gnuplot_cmd(h1, "set xzeroaxis");
    gnuplot_cmd(h1, "set yzeroaxis");
    gnuplot_cmd(h1, "set border 0   ");
    gnuplot_cmd(h1, "set ytics axis");
    gnuplot_cmd(h1, "set xtics axis");
    gnuplot_cmd(h1, "set arrow 1 from 0,0 to graph 1, first 0 filled head");
    gnuplot_cmd(h1, "set arrow 2 from 0,0 to first 0, graph 1 filled head");
    gnuplot_cmd(h1, "set arrow 3 from 0,0 to graph 0, first 0 filled");
    gnuplot_cmd(h1, "set arrow 4 from 0,0 to first 0, graph 0 filled");
    gnuplot_cmd(h1, "bind h 'tmppos=(GPVAL_X_MAX-GPVAL_X_MIN)*0.8;set xrange[GPVAL_X_MIN-tmppos:GPVAL_X_MAX-tmppos]; replot'");
    gnuplot_cmd(h1, "bind l 'tmppos=(GPVAL_X_MAX-GPVAL_X_MIN)*0.8;set xrange[GPVAL_X_MIN+tmppos:GPVAL_X_MAX+tmppos]; replot'");
    //gnuplot_plot_equation(h1, equation, equation);
    gnuplot_cmd(h1, plot);

    while(PID){}

    sleep(SECONDS);

    if(PID == 0){
        gnuplot_close(h1);
    }

    return 0;
}

void format_equation(char* s){
    int num   = 0;
    int count = 0;
    int k     = 1;
    int index[100];

    for(int i = 0; s[i] != 0; i++){
        if(s[i] == ' '){
            continue;
        }

        else if(s[i] == 'x' && is_digit(s[i-1])){
            insert(s, "*", i);
        }

        else if(s[i] == '^' && is_digit(s[i+1])){
            count = 0;
            num = 0;
            k = 1;
            // TODO: check if number is larger than 1 digit
            while(is_digit(s[i+k]) && s[i+k] != 0){
                num = num*10 + (s[i+k] - '0');
                k++;
            }

            // remove ^ and associated number
            memmove(&s[i+1], &s[i+k], strlen(s) - i+k);
            memmove(&s[i], &s[i + 1], strlen(s) - i);

            printf("%d\n", num);
            count = num - 2;

            char buffer[100];

            printf("count: %c\n", count);
            printf("char: %c\n", s[i+1]);
            printf("char-1: %c\n", s[i-1]);

            char temp[] = "*";

            strncat(temp, &s[i-1], sizeof(s[i-1]));
            //strcat(temp, val);

            printf("temp:%s\n", temp);

            strcpy(buffer, temp);
            
            for(int j = 0; j < num - 2; j++){
                strcat(buffer, temp);
            }

            insert(s, buffer, i);

            printf("buffer: %s\n", buffer);
        
        }
        prev = s[i];
    }
}

void insert(char* s, const char* insert, int pos){
    char buf[100] = {};

    strncpy(buf, s, pos);
    int len = strlen(buf);
    strcpy(buf+len, insert);
    len += strlen(insert);
    strcpy(buf+len, s+pos);

    strcpy(s, buf);
}

void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

int is_digit(float value){
    return value >= '0' && value <= '9' ? 1 : 0;
}

void remove_spaces(char* s) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}