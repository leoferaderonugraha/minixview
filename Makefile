all:
	gcc minixview.c functions.c patcher/patcher.c -Wall -Wextra -pedantic -o mxview
