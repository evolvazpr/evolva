# Evolva - game of life
 
##URUCHAMIANIE NA LINUXIE: 
Wymagane pakiety: 
 
g++  
qt5-default  
libboost-all-dev 
 
Kompilacja i pierwsze uruchomienie: 
Gotowy kod do skompilowania aplikacji (celu projektu) znajduje się w katalogu executables, w pliku Evolva.cpp. 

W katalogu executables zawarte są programy testowe oraz testy jednostkowe. 
Celem kompilacji plików wykonywalnych i uruchomienia ich na systemie Linux Mint należy:  

* Wybrać i uruchomić odpowiedni plik Makefile (tj. wykonać polecenie "make -f Makefile_mint"). 

* Upewnić się, że pliki gui.xml i logic.xml zawarte są w tym samym katalogu. 

* Upewnić się, że pliki sprite zawarte są w katalogu sprites/. Jeśl nie - uruchomić skrypt scripts/generate_gui_xml.sh. 
 
* Uruchomić program

Plik logic.xml jest od razu dołączony do projektu. Plik gui.xml jest dołączony, jak również może zostać wygenerowany oddzielnie (scripts/generate\_gui\_xml.sh). Podczas generacji skrypt ściąga pliki sprite i tworzy plik xml. 

##URUCHAMIANIE NA WINDOWSIE: 
Wymagane programy: 
Qt5.6.0 z toolchainem MinGW. 
 
Kompilacja i pierwsze uruchomienie: 
Tak samo jako na Linuxie, prócz tego, że koniecznie trzeba wygenerować plik gui.xml (różnice w oznaczaniu ścieżek dostępu do plików). Plik gui.xml dołączony do projektu nie pasuje do tego systemu.
 
##PLIKI LOGIC.XML I GUI.XML: 
 
Plik logic.xml zawiera początkowe wartości parametrów "kodu DNA" dla mięsożerców i roślinożerców, początkową liczbę i lokalizację obiektów ruchomych i nieruchomych i wymiary mapy. 

Plik gui.xml zawiera lokalizację plików graficznych, reprezentujących obiekty, ich wymiar (sprite'y ruchome mają np. wymiar równy 4 np.) oraz wartość długości boku komórki pola gry przypadający na 1 obiekt. 

##W PRZYPADKU BŁĘDU:
Proszę o kontakt przez mail z:
 
* Filip Wróbel - f.wrobel@stud.elka.pw.edu.pl 

* Konrad Gotfryd - k.gotfryd@stud.elka.pw.edu.pl 
