# Czujniki poziomu wody

## Ja używam czegoś w tym rodzaju:
![water-level-sensor-white](../../assets/docs/img/water-level-sensor-white.jpg)
![water-level-sensor-black](../../assets/docs/img/water-level-sensor-black.jpg)

## Podłączenie
Wspólny przewód łączący wszystkie kontraktony
podłączamy pod wspólną masę. Wyjścia czujników
podpinamy pod wejścia Arduino podane w pliku
`include/control/waterLevelSensor/devices.hpp`.

## Testowanie
Aby przetestować poprawność połączeń należy wejść w
`Menu główne -> Ustawienia -> Tryb serwisowy -> Sensory`
a następnie ręcznie załączać każdy czujnik i sprawdzać czy
odpowiada on odpowiedniemu czujnikowi w kontrolerze.