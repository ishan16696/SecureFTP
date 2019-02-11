### SecureFTP
###### (works only in debian based LINUX)

### How To run
1. compile server.cpp ---> g++ -o ser server.cpp
2. start server ---> ./ser 9999(PortNo)
3. compile client.cpp --> g++ -o cl client.cpp
4. start client -->  ./cl 127.0.0.1 9999(SamePortNo as Server)

### Inside Working 
1. First client server generate the same key with use of diffie hellman key exchange and MillerRobin Test.
2. After that,All the messages and data exchange between client and server will be encrypted using caesar cipherAlgo and Key generated in part1.
3. ID should be unique and then can able to login.
3. Server will not stored the password of client , instead it generates a saltNumber and qa(Prime no.send by client) and server generate a hashcode of (Password||salt||qa) and stored it in file.
4. All the files which client have to download are send using the 512B chunks and they are also encrypted.And client side will decypt it.


**NOTE** --> File must contain only those character which are define in caesar cipherAlgo
  
