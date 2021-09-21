# TPE ARQUITECTURA DE COMPUTADORAS - 1er CUATRIMESTRE 2021
***
## Autores:
* De Luca, Juan Manuel - 60103 - jdeluca@itba.edu.ar
* Kim, Azul Candelaria - 60264 - azkim@itba.edu.ar
* López Guzmán, Pedro Jeremías - 60711 - pelopez@itba.edu.ar
***
## Manual de usuario:

### Prerrequisitos
* Contar con el archivo _60103-60264-60711.zip_ descargado y descomprimido en su computadora.
* Contar con el programa qemu instalado.
* Contar con el programa docker instalado.
* Contar con acceso a la terminal de su computadora.

### Compilación
1. Ubicándose en el directorio del trabajo, correr el comando `docker pull agodio/itba-so:1.0`.
2. Ubicándose en el directorio del trabajo, correr el comando `docker run -v ${PWD}:/root --security-opt seccomp:unconfined -ti agodio/itba-so:1.0`. Esto hará que se ingrese a docker.
3. Una vez dentro de docker, dirigirse al directorio _Toolchain_ con el comando `cd root/Toolchain` y ejecutar el comando `make all`.
4. Salir del directorio _Toolchain_ con `cd ..` y ejecutar nuevamente `make all`.
5. Salir de docker con el comando `exit`.

### Ejecución
* Una vez completada la compilación y ubicado en el directrio del trabajo, correr el comando `./run.sh` si se encuentra en Linux, o `./run.bat` si se encuentra en Windows. Esto abrirá las dos terminales correspondientes al trabajo y podrá comenzar a usarse.

### Funcionalidades

#### Comandos
* `datetime` Muestra la fecha y hora del Sistema Operativo.
* `inforeg` Muestra el estado de los registros. Para su correcto funcionamiento, presionar la tecla F12 antes de correrlo para así guardar el estado de los registros.
* `printmem [ARGUMENTO]` Realiza un volcado de 32 bytes de memoria a partir de la dirección provista en el argumento.
* `clear` Limpia la pantalla de la shell donde se corra el comando.
* `echo [ARGUMENTO]` Imprime en pantalla el string provisto en el argumento.
* `divzero` Fuerza una división por 0 para que se prodzca una excepción. Luego reinicia la shell y muestra el mensaje de error correspondiente junto con el estado de los registros.
* `invalidopcode` Fuerza un código de operación inválido para que se produzca una excepción. Luego reinicia la shell y muestra el mensaje de error correspondiente junto con el estado de los registros.
* `help` Despliega el centro de ayuda.

#### Teclas especiales
* F1 - Sirve para cambiar la terminal seleccionada.
* F12 - Sirve para guardar el estado de los registros.

Además, un progrmador podría setear la funcionalidad que quiera para las teclas F1 a F10 mediante punteros a función.

#### Syscalls
Las syscalls a disposición del usuario son las siguientes: <br>

| %rax | Name                 | %rdi                 | %rsi             | %rdx              | %rcx            | %r8                | %r9               | %r10              | %r11              |
|------|----------------------|----------------------|------------------|-------------------|-----------------|--------------------|-------------------|-------------------|-------------------|
| 0    | readKeyboard         | char * buffer        | uint64_t size    | uint64_t * count  | -               | -                  | -                 | -                 | -                 |
| 1    | drawString           | char * buffer        | uint64_t size    | uint64_t x        | uint64_t y      | uint64_t fontColor | uint64_t bkgColor | uint64_t fontSize | uint64_t alphaBkg |
| 2    | clearDisplay         | uint64_t color       | -                | -                 | -               | -                  | -                 | -                 | -                 |
| 3    | drawLine             | uint64_t x           | uint64_t y       | uint64_t xEnd     | uint64_t yEnd   | uint64_t color     | -                 | -                 | -                 |
| 4    | drawPixel            | uint64_t x           | uint64_t y       | uint64_t color    | -               | -                  | -                 | -                 | -                 |
| 5    | drawRect             | uint64_t xi          | uint64_t yi      | uint64_t width    | uint64_t height | uint64_t color     | -                 | -                 | -                 |
| 6    | drawMatrix           | uint64_t x           | uint64_t y       | uint64_t * matrix | uint64_t width  | uint64_t height    | uint64_t size     | -                 | -                 |
| 7    | getTicks             | uint64_t * ticks     | -                | -                 | -               | -                  | -                 | -                 | -                 |
| 8    | getTime              | uint8_t mode         | uint8_t * target | -                 | -               | -                  | -                 | -                 | -                 |
| 9    | getRegisters         | uint64_t * registers | -                | -                 | -               | -                  | -                 | -                 | -                 |
| 10   | getMem               | uint64_t address     | uint64_t * bytes | uint8_t size      | -               | -                  | -                 | -                 | -                 |
| 11   | isKeyboardEmpty      | uint64_t * target    | -                | -                 | -               | -                  | -                 | -                 | -                 |
| 12   | readError            | uint64_t * err       | -                | -                 | -               | -                  | -                 | -                 | -                 |
| 13   | setFunctionKeyMethod | uint64_t index       | void(*)()        | -                 | -               | -                  | -                 | -                 | -                 |

#### stdio
Las funciones implementadas son:
* `void setConsoleUpdateFunction(void (*f)(char *, int))` sirve como endPoint para la librería para imprimir strings
* `char getChar()` devuelve el siguiente char del buffer del teclado
* `void scanf(char * buffer)` escribe en el buffer los char del buffer del teclado hasta que aparezca un '\n'.
* `void printf(char * fmt, ...)` imprime el string formateado
* `void setFunctionKey(int index, void(*func)())` setea en una tecla especial (F1-F10) la funcionalidad deseada
* `void putChar(char ch)` imprime un char
* `uint64_t getError()` devuelve el error de la última excepcion, sino devuelve 32

#### stdlib
Las funciones implementadas son:
* `int atoi(char * str)` convierte un caracter ASCII a un entero
* `int intToString(unsigned long long num, char * buffer)` convierte un entero a un string
* `int strlen(char *str)` devuelve el largo del string
* `int strcmp(char * str1, char * str2)` devuelve 0 si los strings son iguales, sino distinto a 0 si son distintos
* `int intToBase(unsigned long long num, int base, char*buffer)` convierte un entero a una base dada
* `int iabs(int num)` devuelve el valor absoluto del entero
* `char * strcpy(char * dest, char * src)` copia src en dest
* `int atohex(char * str)` convierte un hex ASCII a entero
* `char toUpper(char letter)` pasa el string a mayúscula
* `char toLower(char letter)` pasa el string a minúscula

#### stdGraphics
Las funciones implementadas son:
* `void drawString(int x, int y, char * buffer, int dim, uint64_t fontColor, uint64_t backgroundColor, int fontSize, int alphaBackground);` dibuja el string desde la posición (x, y). Si alphaBackground es distinto a 0 entonces no dibuja el fondo.
* `void clearScreen(uint64_t color)` limpia la pantalla con el color dado
* `void drawRect(int xi, int yi, int width, int height, uint64_t color)` dibuja un rectángulo desde las coordenadas (xi, yi) con la altura, el ancho y el color dado.

#### time
Las funciones implementadas son:
* `int getSeconds()` devuelve los segundos del tiempo actual
* `int getMinutes()` devuelve los minutos del tiempo actual
* `int getHours()` devuelve la hora del tiempo actual
* `int getDays()` devuelve el día de la fecha actual
* `int getMonth()` devuelve el mes de la fecha actual
* `int getYear()` devuelve el año de la fecha actual
* `int getTicks()` devuelve la cantidad total de ticks del Timer Tick
