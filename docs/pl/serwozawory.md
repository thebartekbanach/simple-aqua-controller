# Serwozawory DIY

![servo-front](../../assets/docs/img/servo-front.jpg)
![servo-left](../../assets/docs/img/servo-left.jpg)
![servo-right](../../assets/docs/img/servo-right.jpg)
![servo-far-right](../../assets/docs/img/servo-far-right.jpg)
![servo-back](../../assets/docs/img/servo-back.jpg)
![servo-top](../../assets/docs/img/servo-top.jpg)

## Czego potrzeba do złożenia serwozaworów?
- cztery serwa w rozmiarze standard, np. TowerPro MG945
- cztery zawory kulowe
- cztery stabilizatory napięcia na 5V,
- cztery radiatory do stabilizatorów napięć
- cztery krańcówki
- cztery listwy montażowe 4-pinowe raster 5.08mm żeńskie + superglue do ich wklejenia
- cztery listwy montażowe 4-pinowe raster 5.08mm męskie
- trochę kabli
- trochę filamentu do drukarki 3D

## Stl? Stl? Stl?
Pliki stl znajdziesz w folderze
`assets/stl/Servo-valve`.

## Wtyczka
1. Czerwony: od +6V do +32V podłączone do stabilizatora napięcia na 5V, serwo podłączone do wyjścia 5V stabilizatora napięcia
2. Czarny: wspólna masa zasilacza serwozaworu oraz zasilacza kontrolera akwarystycznego
3. Biały: sygnał sterujący PWM podłączony bezpośrednio do serwa
4. Niebieski: krańcówka wykrywająca domyk serwozaworu, jeśli jest załączona - przewodzi wspólną masę, **nie +5V albo +12V!**

# API
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

Aktualne API działa w taki sposób, że serwozawór ma 3 sekundy na otwarcie
się lub zamknięcie, jeśli nie zrobi tego w tym czasie - zgłaszany jest błąd.

## ServoValvesStandbyControlModule
Klasa ServoValvesStandbyControlModule służy do odłączania sygnału PWM
od serwozaworów po określonym czasie, fabrycznie jest to 10 sekund,
można to zmienić jak i całkowicie wyłączyć za pomocą menu ustawień.

Powodem utworzenia tej klasy jest mocne grzanie się stabilizatorów
napięcia podczas gdy serwo jest w stanie otwartym. Sterowniki
wbudowane w serwa czasami powodują buczenie ich, co oznacza
że serwo próbuje przesunąć orczyk do wybranego położenia,
mimo że brakuje mu np. 0.1 stopnia, przez co nie używa całej siły
serwa i powoduje używanie silnika przez cały czas. Bardzo
nagrzewa to stabilizatory, przez co mogą w wymaganym momencie
nie podać wymaganego prądu do serwa, co spowoduje problem
z jego przesunięciem do wybranej pozycji.
