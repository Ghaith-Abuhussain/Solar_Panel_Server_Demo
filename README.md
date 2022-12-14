# Solar_Panel_Server_Demo

This project represents an extention of the project (solar-panel-aquisition-card-demo) "https://github.com/Ghaith-Abuhussain/solar-panel-aquisition-card-demo". 
In this project we present a demo server which is responsible of serving the solar panels aquisition cards. 
The server use the multi-threading concept to serve each one of the connecting aquisition cards. There are many types of threads:
1_ receive connection thread which is responsible of listening on the port 27015 and the localhost address to accept the incoming sockets.
2_ proccess_client thread which is opened for each one of the clients after acception the connection and checking the login information. This thread is responsible of receiving the client's packets, parsing these packets and finally store the data into database.
3_ websocket_client thread which is responsible of accepting the insomming websocket connections and executing the websocket API's.

here is the database diagram:
![Solar_Pannel_DB](https://user-images.githubusercontent.com/110384824/207532341-615d8d7c-5988-4caf-ba4b-772eb3843c92.PNG)
