# Driver de Teclado e Controle Linux

## Descrição

Este é um driver de teclado e controle Linux que intercepta as teclas pressionadas no teclado padrão AT Translated Set 2 e emula eventos de controle semelhantes aos de um controle Microsoft X-Box One pad (Firmware 2015).

## Requisitos

Kernel Linux com suporte a módulos
Compilador GCC

## Compilação e Instalação

1. Faça o clone ou baixe o repositorio.
2. Abra um terminal no diretório do código-fonte. 
3. Execute o comando make para compilar o módulo.
```
make
```
4. Carregue o módulo.
```
sudo insmod driver.ko
```

5. Verifique se o driver esta ativo.
```
dmesg
```
6. Se a mesagem for "Driver teclado/mouse-> controle descarregado" tudo certo.

## Uso

Ao carregar o módulo, ele começará a interceptar as teclas do teclado padrão. As teclas W, A, S, D emulam os movimentos de um controle X-Box One, e a barra de espaço atua como um botão "Sul" no controle.

## Desinstalação

Para remover o módulo do kernel, execute o comando rmmod keylogger.

## Autor


## Licença

Este driver é distribuído sob a licença GPL.

A Licença Pública Geral GNU (GNU General Public License - GPL) é uma licença de software livre amplamente utilizada que concede aos usuários a liberdade de executar, estudar, modificar e distribuir o software. A GPL foi criada pela Free Software Foundation (FSF) para promover a filosofia do software livre e proteger as liberdades dos usuários.
