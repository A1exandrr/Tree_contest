#=========================================================
# Основные флаги компиляции и запуска для работы программы
CC       			   = g++
CFLAGS  			   = -Wall -g
ASan	 			   = -fsanitize=address
VALGRIND-leak-check    = valgrind --leak-check=full --show-leak-kinds=all
VALGRIND-variable-null = valgrind --leak-check=full --track-origins=yes --show-reachable=yes
RM                     = rm -f
FILE_NAME_COMPILING	   = Tree.cpp
FILE_EXECUTABLE		   = main_tree

# Цель по умолчанию
default: main

main: $(FILE_NAME_COMPILING)
	$(CC) $(CFLAGS) -o $(FILE_EXECUTABLE) $(FILE_NAME)
	./$(FILE_EXECUTABLE)
	$(RM) $(FILE_EXECUTABLE)
#=========================================================

#Второстепенные точки запуска программы для специальных вызывов

#=========================================================

#Для контеста(осуществляется проверки работы программы с памятью через -fsanitize=address)
contest: $(FILE_NAME_COMPILING)
	$(CC) $(CFLAGS) $(ASan) -o $(FILE_EXECUTABLE) $(FILE_NAME)
	./$(FILE_EXECUTABLE)
	$(RM) $(FILE_EXECUTABLE)

#Каждая отдельная утечка будет показана подробно и
#Показать все «определенные, косвенные, возможные, достижимые» виды утечек в «полном» отчете.
leak_check: $(FILE_NAME_COMPILING)
	$(CC) $(CFLAGS) -o $(FILE_EXECUTABLE) $(FILE_NAME)
	$(VALGRIND-leak-check) ./$(FILE_EXECUTABLE)
	$(RM) $(FILE_EXECUTABLE)

#
variable_null: $(FILE_NAME_COMPILING)
	$(CC) $(CFLAGS) -o $(FILE_EXECUTABLE) $(FILE_NAME)
	$(VALGRIND-leak-check) ./$(FILE_EXECUTABLE)
	$(RM) $(FILE_EXECUTABLE)