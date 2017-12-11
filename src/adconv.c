#include <avr/io.h>

uint16_t adc_read_channel(uint8_t mux)
{ 
    uint8_t i;
    uint16_t result; 
    ADMUX = mux; /* Kanal waehlen */
    ADMUX |= (1<<REFS1) | (1<<REFS0); /* interne Referenzspannung nutzen */
    ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS2); /* Frequenzvorteiler 
                                                 setzen auf 64 und ADC aktivieren */
                                                
    /* nach Aktivieren des ADC wird ein "Dummy-Readout" empfohlen, 
     * man liest also einen Wert und verwirft diesen, um den ADC "warmlaufen zu lassen" */ 
    ADCSRA |= (1<<ADSC); /* eine ADC-Wandlung */ 
    while ( ADCSRA & (1<<ADSC) ) 
    { ; /* auf Abschluss der Konvertierung warten */ 
    } 
    result = ADCW; /* ADCW muss einmal gelesen werden,
                      sonst wird Ergebnis der nächsten Wandlung nicht übernommen. */
   
    /* Eigentliche Messung - Mittelwert aus 4 aufeinanderfolgenden Wandlungen */ 
    result = 0; 
    for( i=0; i<4; i++ ) 
    { 
        ADCSRA |= (1<<ADSC); /* eine Wandlung "single conversion" */ 
        while ( ADCSRA & (1<<ADSC) ) 
        { ; /* auf Abschluss der Konvertierung warten */ 
        } 
        result += ADCW; /* Wandlungsergebnisse aufaddieren */ 
    } 
    ADCSRA &= ~(1<<ADEN); /* ADC deaktivieren (2) */ 
    result /= 4; /* Summe durch vier teilen = arithm. Mittelwert */ 
    return result; 
}
