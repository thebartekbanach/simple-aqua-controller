# Serwozawory DIY

![servo-front](../../assets/docs/img/servo-front.jpg)
![servo-left](../../assets/docs/img/servo-left.jpg)
![servo-right](../../assets/docs/img/servo-right.jpg)
![servo-far-right](../../assets/docs/img/servo-far-right.jpg)
![servo-back](../../assets/docs/img/servo-back.jpg)
![servo-top](../../assets/docs/img/servo-top.jpg)

## Czego potrzeba do złożenia serwozaworów?
- cztery serwa w rozmiarze standard, np. `TowerPro MG945`
- cztery zawory kulowe
- cztery stabilizatory napięcia na 5V,
- cztery radiatory do stabilizatorów napięć
- cztery krańcówki
- cztery listwy montażowe 4-pinowe raster 5.08mm żeńskie + superglue do ich wklejenia
- cztery listwy montażowe 4-pinowe raster 5.08mm męskie
- trochę kabli
- trochę filamentu do drukarki 3D, innego niż PLA

## Jak silne muszą być serwa użyte w projekcie?
Moje serwa `TowerPro MG945` mają udźwig 10 kg na cm przy zasilaniu 5V.
Z moich obserwacji wynika, że są one ledwo wystarczające
do zaworów kulowych 1/2 cala. Najlepiej użyć serw
z udźwigiem większym niż te które opisałem wyżej.
Należy również pamiętać o wystarczająco mocnym
stabilizatorze napięcia i odpowiednim radiatorze.
Moja obudowa serwozaworu z PLA przy radiatorze stabilizatora
staje się miękka po częstym przełączaniu
serwozaworów w krótkim okresie czasu,
tak więc na to również radzę uważać.

## Pliki STL
Pliki stl znajdziesz w folderze
`assets/stl/Servo-valve`.

## Wtyczka
1. Czerwony: od +6V do +32V podłączone do stabilizatora napięcia na 5V, serwo podłączone do wyjścia 5V stabilizatora napięcia
2. Czarny: wspólna masa zasilacza serwozaworu oraz zasilacza kontrolera akwarystycznego
3. Biały: sygnał sterujący PWM podłączony bezpośrednio do serwa
4. Niebieski: krańcówka wykrywająca domyk serwozaworu, jeśli jest załączona - przewodzi wspólną masę, **nie +5V albo +12V!**

# API
Główny interfejs sterownika serwozaworów znajduje się w pliku
`include/control/valves/ValveModule.hpp`.

Dostępne mamy dwie instancje sterownika serwozaworów:
1. `ValveModuleUsingInternalPwm` - ten sterownik używa
    Arduino jako urządzenie wytwarzające sygnał PWM.
    Wyjścia PWM dla tej instancji konfiguruje się za
    pomocą pinout w sekcji `Valves servos pwm pinout definition`.
    Reszta pinout jest wspólna z drugim sterownikiem.
2. `ValveModuleUsingExternalDriver` - tego sterownika
    można użyć w razie problemów z PWM'em generowanym
    przez Arduino Due. Ja musiałem go użyć.
    Ten sterownik używa `Adafruit PWM Servo Driver`
    lub jakiegokolwiek klona obsługiwanego przez bibliotekę
    `Adafruit PWM Servo Driver library`.
    Dodatkowa w przypadku użycia tego modułu nie trzeba
    się martwić sekcją konfiguracji
    `Valves servos pwm pinout definition`,
    nie jest ona używana przy konfiuracji tego sterownika.

## Główne API sterownika serwozaworów składa się z pięciu metod:
- `bool open(unsigned short valveId);`
- `bool close(unsigned short valveId);`
- `bool set(unsigned short valveId, bool state);`
- `bool isClosed(unsigned short valveId);`
- `bool areRemotesAvailable();`

Trzy blokujące metody o nazwach `open`, `close` i `set`
służą do otwierania oraz zamykania serwozaworów.
Jak widzisz, zwracają one zmienną typu boolean.
Jest to wynik wykonania polecenia.
Jeśli metody te zwrócą `true`, wtedy
sterownik wie, że serwozawór wykonał
polecenie poprawnie. W przeciwnym
wypadku, gdy zwrócony zostanie `false`,
sterownik zamyka wszystkie serwozawory
jakie tylko może i zwraca użytkownikowi
informację, że jest problem z serwozaworami
i trzeba się im przyjrzeć oraz naprawić je.

Metoda `isClosed` jak sama nazwa wskazuje zwraca
informację na temat aktualnego statusu serwozaworu.
Informację tę zczytuje z krańcówki domyku
wybranego serwozaworu. Jeśli krańcówka jest zamknięta
zwraca `true`, w przeciwnym wypadku `false`.

Metoda `areRemotesAvailable` jest metodą specjalną,
która również powinna zostać zaimplementowana
jeżeli akwarium nie jest podpięte na stałe do
źródła czystej wody oraz ścieku. Służy ona do
wykrycia, czy kabel sterujący serwozaworami w innym
pomieszczeniu został podpięty pod serwozawory.
Może to być np. kabel LAN, w którym jedna żyła
jest masą, dwie kolejne to sterowanie serwami za
pomocą sygnału PWM, dwie następne to detekcja domyku
serwozaworów a ostatnia żyła to właśnie detekcja
czy kabel jest podłączony dzięki spięciu masy do
detekcji podłączenia po stronie żeńskiej części
wtyczki LAN.

![remote-servovalve-connector](../../assets/docs/img/remote-servovalves-connector.jpg)

Tak więc, podmieniając sterownik serwozaworów mamy możliwość
zgłoszenia błędów otwarcia i zamknięcia, które powodują przerwanie
aktualnie trwającego procesu i zamknięcie wszystkich otwartych serwozaworów.

Aktualne API działa w taki sposób, że serwozawór ma 2-3 sekundy na otwarcie
się lub zamknięcie, jeśli nie zrobi tego w tym czasie - zgłaszany jest błąd.

Główna definicja znajduje się w `include/control/valves/ValveModule.hpp`.
Poszczególne implementacje znajdują się w tym samym folderze.

## ValveModuleBase
Jest to klasa którą można odziedziczyć. Jest to baza sterownika serwozaworów
ze specjalną funkcjonalnością - odłącza sygnał PWM serwozawów po określonym czasie.
Jeśli używamy zwykłych serw w naszych serwozaworach to możemy po prostu zaimplementować
dwie chronione metody i nie martwić się automatycznym odłączaniem serwozaworów.

### Dlaczego warto odłączać sygnał serwozaworów?
Sterowniki wbudowane w serwa czasami powodują
buczenie ich, co oznacza że serwo próbuje przesunąć orczyk do
wybranego położenia, mimo że brakuje mu np. 0.1 stopnia,
przez co nie używa całej siły serwa i powoduje używanie silnika
przez cały czas. Bardzo nagrzewa to stabilizatory,
przez co mogą w wymaganym momencie nie podać wymaganego prądu do serwa,
co spowoduje problem z jego przesunięciem do wybranej pozycji.


### Implementacja:
Aby zaimplementować własny sterownik dziedzicząc z `ValveModuleBase` należy
nadpisać dwie chronione, abstrakcyjne metody:
- `void moveServo(unsigned short valveId, bool state);`
- `void detach(unsigned short valveId);`

Metoda `moveServo` służy do obsługi ruchu serwa - jego otwarcia lub
zamknięcia w zależności od wartości argumentu `bool state`.

Metoda `detach` służy do przerwania podawania sygnału sterującego PWM
do serwa. Jest wywoływana po ustalonym czasie. `ValveModuleBase` automatycznie
zapisuje ostatni czas poruszenia serwa i wywołuje tę metodę.