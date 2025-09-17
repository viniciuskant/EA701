#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include <stdio.h>
#include <string.h>

#define FLASH_TARGET_OFFSET (256 * 1024)  // ajuste conforme sua imagem
#define SECTOR_SIZE FLASH_SECTOR_SIZE     // tip. 4096
#define PAGE_SIZE FLASH_PAGE_SIZE         // tip. 256

uint8_t WRBUF[PAGE_SIZE];
double erase_times[3], prog_times[3], big_prog_times[3];

int main() {
    stdio_init_all();
    sleep_ms(5000); // dá tempo para conectar terminal

    // Preenche buffer
    for (int i=0;i<PAGE_SIZE;i++) WRBUF[i] = i & 0xFF;

    // --- mede ERASE de 1 setor ---
    for (int j = 0; j < 3; j++) {
        absolute_time_t t0 = get_absolute_time();
        uint32_t ints = save_and_disable_interrupts();
        flash_range_erase(FLASH_TARGET_OFFSET, SECTOR_SIZE);
        restore_interrupts(ints);
        absolute_time_t t1 = get_absolute_time();

        erase_times[j] = absolute_time_diff_us(t0, t1)/1000.0;
        printf("Flash ERASE (setor %d bytes) [%d]: %.3f ms\n", SECTOR_SIZE, j+1, erase_times[j]);
    }
    double erase_avg = (erase_times[0] + erase_times[1] + erase_times[2]) / 3.0;
    printf("Média ERASE: %.3f ms\n", erase_avg);

    // --- mede PROGRAM de 1 página ---
    for (int j = 0; j < 3; j++) {
        absolute_time_t t0 = get_absolute_time();
        uint32_t ints = save_and_disable_interrupts();
        flash_range_program(FLASH_TARGET_OFFSET, WRBUF, PAGE_SIZE);
        restore_interrupts(ints);
        absolute_time_t t1 = get_absolute_time();

        prog_times[j] = absolute_time_diff_us(t0, t1)/1000.0;
        printf("Flash PROGRAM (pagina %d bytes) [%d]: %.3f ms\n", PAGE_SIZE, j+1, prog_times[j]);
    }
    double prog_avg = (prog_times[0] + prog_times[1] + prog_times[2]) / 3.0;
    printf("Média PROGRAM (pagina): %.3f ms\n", prog_avg);

    // --- mede PROGRAM de bloco maior (ex. 32 KB) ---
    const int TOTAL = 32 * 1024;
    static uint8_t bigbuf[32*1024];
    for (int i=0;i<TOTAL;i++) bigbuf[i] = i & 0xFF;

    for (int j = 0; j < 3; j++) {
        // precisa apagar setor(es) correspondente(s) antes se for regravar
        uint32_t ints = save_and_disable_interrupts();
        flash_range_erase(FLASH_TARGET_OFFSET, SECTOR_SIZE * 8); // 8*4KB = 32KB
        restore_interrupts(ints);

        absolute_time_t t0 = get_absolute_time();
        ints = save_and_disable_interrupts();
        for (int off=0; off<TOTAL; off+=PAGE_SIZE)
            flash_range_program(FLASH_TARGET_OFFSET + off, bigbuf + off, PAGE_SIZE);
        restore_interrupts(ints);
        absolute_time_t t1 = get_absolute_time();

        big_prog_times[j] = absolute_time_diff_us(t0, t1)/1000.0;
        printf("Flash PROGRAM (32KB) [%d]: %.3f ms\n", j+1, big_prog_times[j]);
    }
    double big_prog_avg = (big_prog_times[0] + big_prog_times[1] + big_prog_times[2]) / 3.0;
    printf("Média PROGRAM (32KB): %.3f ms\n", big_prog_avg);

    while (1) tight_loop_contents();
}