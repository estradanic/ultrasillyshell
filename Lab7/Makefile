CC=gcc

ush:	./history/history.o ./linkedlist/linkedList.o ./linkedlist/listUtils.o ./utils/myUtils.o ./alias/alias.o ./utils/ushrcUtils.o ./pipes/pipes.o ./redirect/redirect.o ./tokenize/makeArgs.o
	gcc -g -o ush cscd340Lab7.c ./history/history.o ./linkedlist/linkedList.o ./linkedlist/listUtils.o ./utils/myUtils.o ./alias/alias.o ./utils/ushrcUtils.o ./pipes/pipes.o ./redirect/redirect.o ./tokenize/makeArgs.o

linkedList.o:	./linkedlist/linkedList.c ./linkedlist/linkedList.h
	gcc -g -c ./linkedlist/linkedList.c

listUtils.o:	./linkedlist/listUtils.c ./linkedlist/listUtils.h
	gcc -g -c ./linkedlist/listUtils.c

myUtils.o:	./utils/myUtils.c ./utils/myUtils.h
	gcc -g -c ./utils/myUtils.c

ushrcUtils:	./utils/ushrcUtils.c ./utils/ushrcUtils.h
	gcc -g -c ./utils/ushrcUtils.c

history.o:	./history/history.c ./history/history.h
	gcc -g -c ./history/history.c

alias.o:	./alias/alias.c ./alias/alias.h
	gcc -g -c ./alias/alias.c

pipes.o:	./pipes/pipes.c ./pipes/pipes.h
	gcc -g -c ./pipes/pipes.c

redirect.o:	./redirect/redirect.c ./redirect/redirect.h
	gcc -g -c ./redirect/redirect.

makeArgs.o:	./tokenize/makeArgs.c ./tokenize/makeArgs.h
	gcc -g -c ./tokenize/makeArgs.c

clean:	
	rm history/*.o
	rm alias/*.o
	rm linkedlist/*.o
	rm utils/ushrcUtils.o
	rm pipes/*.o
	rm tokenize/*.o
	rm ush

