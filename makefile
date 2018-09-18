main: sendmessage receivemessage range
sendmessage: sendMessage.c
	gcc sendMessage.c -o send -pthread
receivemessage:	receiveMessage.c
	gcc receiveMessage.c -o receive -pthread
range:	range.c
	gcc range.c -o range -pthread
