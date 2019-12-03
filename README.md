# Escape Groom
Jogo para testar a comunicação e dedução (eu acho).

O jogo funciona em dois computadores separados ao mesmo tempo utilizando conecção entre sockets.
Para isso, tem o *INPUT* que recebe os comandos de teclado usando SDL e passa 1 byte por vez quando muda os botões apertados e o
*OUTPUT* que recebe essas mudanças do teclado por socket e faz toda a renderização no console utilizando ncurses.

## Dependencias
### INPUT
- SDL2
- SDL2_image

### OUTPUT
- ncurses

## Compilar
Para compilar, o código usa CMake\

`mkdir build && cd build` - cria um diretório\
`cmake ..` - cria os arquivos para compilar o código\
`make` - compila o código e cria dois arquivos `escape_groom_input` e `escape_groom_output`\

pode também passar os parametros `escape_groom_input` ou `escape_groom_output` para compilar somente a instância de preferencia.
