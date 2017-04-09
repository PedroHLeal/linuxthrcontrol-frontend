#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cpu_infos{
    int maxCPUFreq, minCPUFreq;
}cpu_info;

void getCPUINFO(cpu_info *info){
    int max, min;
    FILE *fmin = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq", "r");
    FILE *fmax = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", "r");
    fscanf(fmin, "%d", &min);
    fscanf(fmax, "%d", &max);

    info->maxCPUFreq = max/1000;
    info->minCPUFreq = min/1000;

    fclose(fmin);
    fclose(fmax);
}

void turnCTLON(){
    FILE *f = fopen("/proc/throttlectl", "w");
    if(f == NULL){
        printf("Permissão Negada: Acesse como superusuário\n");
        return;
    }
    fprintf(f, "thon\n");
    fclose (f);
}

void turnCTLOFF(){
    FILE *f = fopen("/proc/throttlectl", "w");
    if(f == NULL){
        printf("Permissão Negada: Acesse como superusuário\n");
        return;
    }
    fprintf(f, "thoff\n ");
    fclose(f);
}

void setMaxFreq(int maxFreq){
    cpu_info cpuInfo;
    getCPUINFO(&cpuInfo);
    if(maxFreq >= cpuInfo.minCPUFreq && maxFreq <= cpuInfo.maxCPUFreq){
        FILE *f = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq", "w");
        if(f == NULL){
            printf("Permissao negada: Execute como superusuario");
        }
        else{
            fprintf(f, "%d", maxFreq*1000);
            fclose(f);
        }
    }
    else{
        printf("OUT OF BOUNDS min: %d, max: %d\n", cpuInfo.minCPUFreq, cpuInfo.maxCPUFreq);
    }
}
void getInfo(char* info){
    int val;
    cpu_info cpuInfo;
    char freq[10], maxminfreq[40];
    getCPUINFO(&cpuInfo);

    FILE *f = fopen("/proc/throttlectl", "r");
    fscanf(f, "%d", &val);
    fclose(f);

    if((val & 0x1) == 1){
        strcat(info, "on - max freq = ");
    }
    else{
        strcat(info, "off - max freq = ");
    }
    f = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq", "r");
    fscanf(f, "%s", freq);
    fclose(f);

    strcat(info, freq);
    sprintf(maxminfreq, "\nmin cpu: %d, max cpu: %d", cpuInfo.minCPUFreq, cpuInfo.maxCPUFreq);
    strcat(info, maxminfreq);
}

int main(int argc, char *argv[]){
    char info[60] = "";
    if(argc > 1){
        if(strcmp(argv[1], "on") == 0){
            turnCTLON();
        }
        else if(strcmp(argv[1], "off") == 0){
            turnCTLOFF();
        }
        else if(strcmp(argv[1], "info") == 0){
            getInfo(info);
            printf("%s\n", info);
        }
        else if(argc == 3 && strcmp(argv[1], "max-freq") == 0){
            getInfo(info);
            setMaxFreq(atoi(argv[2]));
        }
        else{
            printf("usage: thrctl [info|on|off|max-freq <max-freq(MHz)>]\n");
        }
    }
    else{
        printf("usage: thrctl [info|on|off|max-freq <max-freq(MHz)>]\n");
    }
}
