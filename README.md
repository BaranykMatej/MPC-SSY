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

## Použití

1. **Zkompilujte a nahrajte** kód na cílový mikrokontrolér.
2. Připojte se přes USB/seriové rozhraní pro zobrazení výstupu.
3. Stiskněte:
   - `1` – Spuštění systémových úloh (síť, zprávy).
   - `2` – Zobrazení aktuálních hodnot senzorů.
   - `q` – Ukončení smyčky (softwarové ukončení).

## Požadavky

- AVR/GCC toolchain
- Senzor TMP102 připojený přes TWI/I2C
- UART/USB rozhraní pro ladění
- Atmel Software Framework nebo kompatibilní HAL

## Poznámky

- Většina globálních deklarací (např. `appMsg`, `appState`) musí být definována jinde.
- Struktury zpráv jako `AppMessage_t` a `AppAddress_t` musí být předem nadefinovány.
