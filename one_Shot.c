#include <stdio.h> // Biblioteca padrão do C.
#include "pico/stdlib.h" // Biblioteca padrão do Raspberry Pi Pico para controle de GPIO, temporização e comunicação serial.
#include "pico/time.h"   // Biblioteca para gerenciamento de temporizadores e alarmes.

const uint LED_PIN_VERDE = 11;  // Definindo o Pino verde
const uint LED_PIN_AZUL = 12; // Definindo o Pino Azul
const uint LED_PIN_VERMELHO = 13; // definindo o Pino vermelho

const uint BUTTON_PIN = 5; // Definição do botão
    
bool led_active = false;   // Indica se o LED está atualmente aceso.

// Função call back para desligar o LED após o tempo programado
int64_t turn_off_callback(alarm_id_t id, void *user_data) {

    //Desliga os LEDS configurado para nível baixo(0 ou false)
    gpio_put(LED_PIN_AZUL, false);
    gpio_put(LED_PIN_VERMELHO, false);
    gpio_put(LED_PIN_VERDE, false);
    
    
    // Atualiza o estado do led
    led_active = false;

    // Retorna 0 para indicar que o alarme não deve se repetir
    return 0;
}

int main() { //função principal

    stdio_init_all();


    // Configurando os LEDS(AZUL,VERMELHO E VERDE) todos como Saída
    gpio_init(LED_PIN_AZUL);
    gpio_set_dir(LED_PIN_AZUL, GPIO_OUT);
    
    gpio_init(LED_PIN_VERMELHO);
    gpio_set_dir(LED_PIN_VERMELHO, GPIO_OUT);
    
    gpio_init(LED_PIN_VERDE);
    gpio_set_dir(LED_PIN_VERDE, GPIO_OUT);
    // configurando o BOTAO(Entrada).
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);

    // Habilita o resistor pull-up interno para o pino do botão.
    // Isso garante que o pino seja lido como alto (3,3 V) quando o botão não está pressionado.
    gpio_pull_up(BUTTON_PIN);

    while (true) {
        // verifica se um botão foi pressionado e se o led não está ativo
        if (gpio_get(BUTTON_PIN) == 0 && !led_active) {
            //Adiciona um pequeno atraso para o debounce
            sleep_ms(50);

                // Verifica o estado do botão novamente
            if (gpio_get(BUTTON_PIN) == 0) {

                gpio_put(LED_PIN_AZUL, true);
                gpio_put(LED_PIN_VERMELHO, true);
                gpio_put(LED_PIN_VERDE, true);
                sleep_ms(2000);

                led_active = true;

                // Agenda DESLIGAR um alarme após 3 segundos.
                // turn_off_callback - Será chamada após esse tempo
                add_alarm_in_ms(3000, turn_off_callback, NULL, false);


                gpio_put(LED_PIN_AZUL, false);
                sleep_ms(1000);

                gpio_put(LED_PIN_VERMELHO, false);
                sleep_ms(1000);

                gpio_put(LED_PIN_VERDE, false);
                sleep_ms(1000);
            }
        }
        sleep_ms(10);
    }

    return 0;
}
