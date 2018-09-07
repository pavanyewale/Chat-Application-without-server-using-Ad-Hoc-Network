# Chat-Application-without-server-using-Ad-Hoc-Network

#Protocols

*All machines will acts as a server as well as client .
*When one machine have to send a msg to another machine it will acts a client
otherwise its a server.
* All machines contains a information about all other machines which are within the range .
* All machines will update the online machines list (i.e. which neibours are online ) after a specific time.
* When one machine send a msg to another machine it will save the msg will destination address until it will get the acknoledgement.
* When one machine want to send a msg to another machine it must know the ip address of destination machine .

#How to run 

1. Create a ad-hoc network with each machine have a unique IP address.
2. While creating ad-hoc all machines should have same name i.e. all in the same network.
3. All machines must use the same port no.
4. run the Makefile (i.e. make ) to compile the program
5. run the chat i.e. (./chat)
6. Enjoy :)
