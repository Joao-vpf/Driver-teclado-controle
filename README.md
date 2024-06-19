# Driver para o sistema operacional linux que simula o controle pelo teclado e mouse

## Autor


## Descrição

Este é um driver de teclado e mouse para simular um controle em um sistema Linux que intercepta as teclas pressionadas no teclado e mouse padrão e emula eventos de controle semelhantes aos de um controle Microsoft XBOX ONE.

## Requisitos

Kernel Linux com suporte a módulos</br>
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
6. Se a mesagem for "Driver teclado/mouse-> controle carregado" tudo certo.

## Uso

Ao carregar o módulo, ele começará a interceptar as teclas do teclado padrão e o mouse simulando assim o controle do XBOX ONE.

### Teclado

##### Direcional esquerdo

**V**: Associado ao direcional esquerdo para cima.</br>
**B**: Associado ao direcional esquerdo para baixo.</br>
**Q**: Associado ao direcional esquerdo para esquerda.</br>
**E**: Associado ao direcional esquerdo para direita.</br>

##### Direcional direito

**W**: Associado ao direcional direito para cima.</br>
**S**: Associado ao direcional direito para baixo.</br>
**A**: Associado ao direcional direito para esquerda.</br>
**D**: Associado ao direcional direito para direita.</br>

##### Botões superiores direita

**Espaço**: Associado a letra __A__ do controle.</br>
**Z**: Associado a letra __B__ do controle.</br>
**X**: Associado a letra __X__ do controle.</br>
**F**: Associado a letra __Y__ do controle.</br>

#### Botões superiores meio

**NUM LOCK**: Associado ao __START__ do controle.</br>
**END**: Associado ao __MENU__ do controle.</br>
**HOME**: Associado ao __XBOX__ do controle.</br>

#### Setas do controle

**Arrow up**: Associado a seta do controle para cima.</br>
**Arrow down**: Associado a seta do controle para baixo.</br>
**Arrow left**: Associado a seta do controle para esquerda.</br>
**Arrow right**: Associado a seta do controle para direita.</br>

#### Gatilhos

**1**: Associado ao __LB__.</br>
**2**: Associado ao __RB__.</br>
**3*: Associado ao __LT__.</br>
**4**:  Associado ao __RT__.</br>

## Desinstalação

1. Abra um terminal no diretório do código-fonte.
2. Execute o comando para retirar o módulo do linux.
```
sudo rmmod driver
```
3. Verifique se o driver esta ativo.
```
dmesg
```
4. Se a mesagem for "Driver teclado/mouse-> controle descarregado" tudo certo.


## Obsevações

1. Como o driver executa usando o teclado e o mouse pode acontecer de desconectar um ou ambos do computador, caso aconteça reiniciar o sistema.
2. No caso do módulo não ser retirado com sucesso é necessário reiniciar o sistema.
3. O verdadeiro nome dos botões do meio são: (START == START), (XBOX == MODE) e (MENU == SELECT).

## Licença

Este driver é distribuído sob a licença GPL.

A Licença Pública Geral GNU (GNU General Public License - GPL) é uma licença de software livre amplamente utilizada que concede aos usuários a liberdade de executar, estudar, modificar e distribuir o software. A GPL foi criada pela Free Software Foundation (FSF) para promover a filosofia do software livre e proteger as liberdades dos usuários.
