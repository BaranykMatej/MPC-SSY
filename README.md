# Bezdrátový senzor vlhkosti půdy

## Funkce

- **Režim senzorů**: Čtení teploty (pomocí TMP102) a vlhkosti půdy přes ADC.
- **Režim úloh**: Zpracování systémových úloh, jako je komunikace v síti a přenos dat.
- **Síťová komunikace**:
  - Vlastní rámce UART zpráv s kontrolním součtem.
  - Přiřazování adres uzlům (požadavek, odpověď, potvrzení).
  - Příprava a odesílání dat přes vrstvu NWK.
- **Debug výstup**: Diagnostika a stavový výstup přes USB/seriové rozhraní.
- **Dva režimy ovládání**: Přepínání mezi režimy pomocí vstupu `1`, `2`, nebo `q`.

## Klíčové moduly

- `appUartSendMessageHR()` – Zobrazí přijatá síťová data v čitelném formátu.
- `appDataInd()` – Hlavní handler pro příchozí NWK zprávy.
- `appSendData()` – Připravuje a odesílá rámce se senzory.
- `TMP102Sensor` – Ovladač pro teplotní senzor TMP102 přes TWI/I2C.
- `convertAdcToHumidityPercent()` – Převádí hodnotu z ADC na procentuální vlhkost.
