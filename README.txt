Trabalho de Software Básico - Tradutor do assembly inventado

Pedro Nogueira - 14/0065032
Gabriel Moretto - 15/0154917
UnB - 2022/1


------------------------------------------------------------------------------------------------------------------------


Seção do não quero ler tudo

********************************************************************************
ANTES DE MONTAR O CÓDIGO COM A FLAG -o, SEMPRE SEMPRE SEMPRE É PRECISO PRÉ-PROCESSÁ-LO COM A FLAG -p, MESMO SEM DIRETIVAS DE PRÉ-PROCESSAMENTO!!!
********************************************************************************

Sugestão de uso:

make clean && make debug
./tradutor teste.obj


------------------------------------------------------------------------------------------------------------------------


O que roda

O projeto não saiu 100% como o último, então listarei aqui as coisas que foram pedidas do projeto que não foram implementadas:
- OUTPUT opcode 13 que printa inteiro do acumulador: 0% feito
- INPUT opcode 12 que lê inteiro da tela: não foi feita a parte de número negativo


------------------------------------------------------------------------------------------------------------------------


Como rodar

O projeto usa makefile (dentro da pasta ./code), e ele tem algumas instruções:

make clean - limpa os arquivos compilados do projeto
make debug - compila todos os arquivos do projeto em modo debug (devagar) - gera um program.exe
make release - compila todos os arquivos do projeto sem debug (e mais rápido) - gera um program.exe
make cppcheck - usa o cppcheck em todos os arquivos do projeto
make help - mostra todas as instruções do make que criei aqui (essas mesmas instruções)

Para executar, ele faz como foi pedido no trabalho:
Rode o executável do projeto (./tradutor) junto do nome do arquivo objeto que você deseja traduzir
Exemplo: ./tradutor teste.obj  --  o arquivo de saída será o teste.s

Os códigos assembly ia-32 gerados foram testados com a linha
nasm -f elf -o teste.o teste.s && ld -m elf_i386 -s -o teste teste.o && ./teste


------------------------------------------------------------------------------------------------------------------------


Exemplos de teste

Todos os arquivos .obj que começam com teste_ se comportaram como esperado.
Para testar input, por não haver output, eu sempre testo com valores da tabela ASCII para bater com o resultado.

Exemplo, para rodar o teste_input.obj:
make clean && make debug && make cppcheck
./tradutor teste_input.obj
nasm -f elf -o teste.o teste_input.s && ld -m elf_i386 -s -o teste teste.o && ./teste
65
A


------------------------------------------------------------------------------------------------------------------------


Git

Um projeto com um repositório.
https://github.com/bananahell/Trabalho-SB-Traducao - git do projeto


------------------------------------------------------------------------------------------------------------------------


Diretório

Na root aqui tem
  .gitignore para não commitar tudo para o git,
  os READMEs, tanto esse quanto o que é mostrado no git,
  a pasta .vscode com configurações do vscode,
  e a pasta code, que tem todo o código, o makefile, os assemblies de exemplo, e o executável.

Na pasta ./code tem
  makefile que é onde compila o projeto com todas as instruções ditas lá em cima
  program.exe que é o executável gerado ao compilar o projeto
  pastas src e include que têm respectivamente os .cpp e os .h do projeto
  um monte de .asm, .o, e .obj que são as saídas e entradas do projeto em si
  pastas dep e bin, geradas depois de compilar o projeto, têm os objetos e deps compilados


------------------------------------------------------------------------------------------------------------------------


Versão do g++

$ g++ -v
Using built-in specs.
COLLECT_GCC=g++
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/5/lto-wrapper
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 5.4.0-6ubuntu1~16.04.12' --with-bugurl=file:///usr/share/doc/gcc-5/README.Bugs --enable-languages=c,ada,c++,java,go,d,fortran,objc,obj-c++ --prefix=/usr --program-suffix=-5 --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --with-sysroot=/ --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-libmpx --enable-plugin --with-system-zlib --disable-browser-plugin --enable-java-awt=gtk --enable-gtk-cairo --with-java-home=/usr/lib/jvm/java-1.5.0-gcj-5-amd64/jre --enable-java-home --with-jvm-root-dir=/usr/lib/jvm/java-1.5.0-gcj-5-amd64 --with-jvm-jar-dir=/usr/lib/jvm-exports/java-1.5.0-gcj-5-amd64 --with-arch-directory=amd64 --with-ecj-jar=/usr/share/java/eclipse-ecj.jar --enable-objc-gc --enable-multiarch --disable-werror --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
Thread model: posix
gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.12)

Versão do NASM

$ nasm -v
NASM version 2.11.08

Versão do Ubuntu

$ lsb_release -a
No LSB modules are available.
Distributor ID:	Ubuntu
Description:	Ubuntu 16.04.7 LTS
Release:	16.04
Codename:	xenial
