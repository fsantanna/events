gcc -g -DUSE_STDIO=1 -D__GCC_POSIX__=1 -I../Common_Demo/include -I..  -I../FreeRTOS_Kernel/include -I../FreeRTOS_Kernel/portable/GCC/Posix -O2 -Wall -c -fmessage-length=0 -pthread -lrt -MMD -MP -MF"main.d" -MT"main.d" -o"main.o" "../main.c"

gcc -g -DUSE_STDIO=1 -D__GCC_POSIX__=1 -I../Common_Demo/include -I..  -I../FreeRTOS_Kernel/include -I../FreeRTOS_Kernel/portable/GCC/Posix -O2 -Wall -c -fmessage-length=0 -pthread -lrt -MMD -MP -MF"main.d" -MT"main.d" -o"events.o" "../events.c"

gcc -g -DUSE_STDIO=1 -D__GCC_POSIX__=1 -I../Common_Demo/include -I..  -I../FreeRTOS_Kernel/include -I../FreeRTOS_Kernel/portable/GCC/Posix -O2 -Wall -c -fmessage-length=0 -pthread -lrt -MMD -MP -MF"main.d" -MT"main.d" -o"ext_lua.o" "../ext_lua.c"

gcc -g -DUSE_STDIO=1 -D__GCC_POSIX__=1 -I../Common_Demo/include -I..  -I../FreeRTOS_Kernel/include -I../FreeRTOS_Kernel/portable/GCC/Posix -O2 -Wall -c -fmessage-length=0 -pthread -lrt -MMD -MP -MF"main.d" -MT"main.d" -o"ext_timer.o" "../ext_timer.c"

gcc -g -DUSE_STDIO=1 -D__GCC_POSIX__=1 -I../Common_Demo/include -I..  -I../FreeRTOS_Kernel/include -I../FreeRTOS_Kernel/portable/GCC/Posix -O2 -Wall -c -fmessage-length=0 -pthread -lrt -MMD -MP -MF"main.d" -MT"main.d" -o"ext_console.o" "../ext_console.c"

gcc -g -DUSE_STDIO=1 -D__GCC_POSIX__=1 -I../Common_Demo/include -I..  -I../FreeRTOS_Kernel/include -I../FreeRTOS_Kernel/portable/GCC/Posix -O2 -Wall -c -fmessage-length=0 -pthread -lrt -MMD -MP -MF"main.d" -MT"main.d" -o"ext_send.o" "../ext_send.c"

gcc -g -pthread -o"FreeRTOS_Posix"  ./main.o ./events.o ./ext_lua.o ./ext_timer.o ./ext_send.o ./ext_console.o ../lua/src/liblua.a ./ParTest/ParTest.o  ./FreeRTOS_Kernel/croutine.o ./FreeRTOS_Kernel/list.o ./FreeRTOS_Kernel/queue.o ./FreeRTOS_Kernel/tasks.o  ./FreeRTOS_Kernel/portable/MemMang/heap_3.o  ./FreeRTOS_Kernel/portable/GCC/Posix/port.o  ./FileIO/fileIO.o  ./Common_Demo/Minimal/GenQTest.o ./Common_Demo/Minimal/QPeek.o ./Common_Demo/Minimal/blocktim.o ./Common_Demo/Minimal/countsem.o ./Common_Demo/Minimal/crflash.o ./Common_Demo/Minimal/crhook.o ./Common_Demo/Minimal/recmutex.o  ./Common_Demo/Full/BlockQ.o ./Common_Demo/Full/PollQ.o ./Common_Demo/Full/death.o ./Common_Demo/Full/dynamic.o ./Common_Demo/Full/events.o ./Common_Demo/Full/flash.o ./Common_Demo/Full/flop.o ./Common_Demo/Full/integer.o ./Common_Demo/Full/print.o ./Common_Demo/Full/semtest.o  ./AsyncIO/AsyncIO.o ./AsyncIO/AsyncIOSerial.o ./AsyncIO/AsyncIOSocket.o ./AsyncIO/PosixMessageQueueIPC.o -lrt -lm  -Wl,--no-as-needed -ldl -dl
