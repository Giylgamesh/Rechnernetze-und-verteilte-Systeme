initialisieren des Projekts mit cmake
    
    neleporto@Neles-Air RN-Projekt1 % cmake -B build -DCMAKE_BUILD_TYPE=Debug

generiert [Makefile](build/Makefile).
Aufrufen von make auf dem build-folder 

    neleporto@Neles-Air RN-Projekt1 % make -C build <optionales target>

Starten des Server:

    neleporto@Neles-Air RN-Projekt1 % ./build/webserver <optionale parmse>

Run tests:

    neleporto@Neles-Air RN-Projekt1 % pytest test