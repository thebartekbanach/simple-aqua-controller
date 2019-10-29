# System
System składa się z 11 nie blokujących głównego wątku
modułów. Każdy moduł zarządza tylko swoją częścią systemu.
Różne podstawowe implementacje klas bazowych znajdują się
w pliku `include/system/SystemModule.hpp`.

# ArduinoMenu
Aplikacja używa biblioteki ArduinoMenu do tworzenia, zarządzania
i wyświetlania całego menu na wyświetlaczu urządzenia.

Jest to mój [fork](https://github.com/thebartekbanach/ArduinoMenu/archive/master.zip)
tej biblioteki której najnowszą wersję można znaleźć [tutaj](https://github.com/neu-rah/ArduinoMenu).
Dzięki za stworzenie tej biblioteki, [neu-rah](https://github.com/neu-rah)!

## Dlaczego fork i dlaczego musiałem zmienić oryginalną bibliotekę?
Otóż, oryginalna wersja tej biblioteki nie dawała możliwości
zarejestrowania eventu w postaci wskaźnika na metodę klasy.
Teraz jest to możliwe, za pomocą klasy `actionReceiver`.
Przykład użycia można zobaczyć w pliku
`include/system/ActionReceiver.hpp`.

# SystemModuleSettings
Jest to klasa szablonowa jako szablon przyjmująca strukturę ustawień.
Główne API składa się z czterech głównych metod:
- `TSettings& data()`: zwraca strukturę ustawień zapisaną w pamięci
- `void saveSettings()`: metodę tę należy wywołać po zmianie obiektu ustawień w celu zapisania ich w pamięci EEPROM
- `void resetSettings()`: restartuje ustawienia do fabrycznych,
czyli podanych w konstruktorze
- `void restoreSettings()`: wczytuje z pamięci EEPROM i nadpisuje
bieżące ustawienia, można użyć w celu przywrócenia edytowanych wartości

# GlobalEventBus
Jest to szyna zdarzeń służąca do wysyłania globalnych zdarzeń do
każdego modułu. Może posłużyć do komunikacji między dwoma
naszymi modułami. Najlepszym przykładem użycia może być moduł
`TimeSetupModule` znajdujący się w pliku
`include/modules/timeSetup/Module.hpp`.

Wysyła on głownie dwa eventy:
- `DAY_CYCLE_BEGIN` - początek dnia
- `NIGHT_CYCLE_BEGIN` - początek nocy

# ActionCreator
Jest to typ modułu który całkowicie przejmuje kontrolę
nad wyświetlaczem oraz joystickem. Może posłużyć do
zrobienia czegoś bardziej zaawansowanego.

Przykładem takiego kreatora akcji może być
`include/modules/feedingControl/FeedingActionCreator.hpp`

Przed aktywowaniem kreatora akcji należy upewnić się,
że żaden kreator nie jest aktualnie wykonywany.
Przykład takiego sprawdzenia znajduje się w metodzie
`void startFeedingModeEvent()` klasy `FeedingControlModule`
w pliku `include/modules/feedingControl/Module.hpp`.