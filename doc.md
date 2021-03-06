# Quick Start

```
> git clone https://github.com/fsantanna/events

> cd events/FreeRTOS_Posix/lua/src
> make linux

> cd -

> cd events/FreeRTOS_Posix/Release/
> make      # ignorar o erro

# repetir para cada modificacao
> ./compile.sh
> ./FreeRTOS_Posix
```

# Código fonte

* main.c
    Entry point do FreeRTOS.
    Nunca muda.
    Depende de `app.c` e `events.h`.

    Inclui o arquivo com a aplicação (`app.c`) e chama sua função `app_init()`.

    Inclui a interface/API de eventos (`events.h`) e dispara a task 
    principal/única `events_task` que toma o controle total.

    A idéia é que a aplicação carregue suas extenções/drivers em `app_init` 
    (e.g. `ext_timer_init()`) e em seguida interligue-as através de registros
    de callbacks.

* app.c
    Parte variável de `main.c`.
    Os exemplos atuais carregam algumas extensões, definem alguns listeners e 
    geram alguns eventos explicitamente (para testes).
    Inicialmente, `app.c` aponta para `app1.c`.
    Para outros testes, basta alterar o link para outro arquivo (app*.c) e 
    recompilar a aplicação.

* main.lua
    Entry point das aplicações Lua.
    Nunca muda.
    Testado em `app4.c`.

    Carrega as extensões disponíveis e prepara o ambiente de execução.
    Executa o arquivo com a aplicação do usuário (`app.lua`).

* app[4-6].lua
    Exemplos de aplicações Lua.
    Para testar cada um, basta alterar `main.lua`.

* app.h
    Define as extensões e eventos utilizados.
    Todas as extensões precisam incluir este arquivo para terem acesso aos 
    identificadores dos eventos.

* events.c
    Implementa o scheduler (dispatcher) e as funções para manipular eventos.
* events.h
    Declara as funções e tipos os quais as extensões dependem (e.g.  `evt_listener_add`).

* ext_(...).c
    Define uma extensão e exporta uma única função `ext_(...)_init` que deve ser chamada em `app_init`.
    A extensão deve incluir as definições das extensões com as quais se comunica para ter acesso aos tipos que ela define.
* ext_(...).h
    Declara a função de inicialização da extensão e seus tipos de eventos.

* compile.sh
    Recompila `main.c`, `events.c` e todas as extensões (e.g. `ext_lua.c`, 
    `ext_timer.c`).
    Lembrando que


