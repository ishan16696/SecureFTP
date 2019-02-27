### SecureFTP
###### (works only in debian based LINUX)

### How To run
1. compile server.cpp ---> g++ -o ser server.cpp -lcrypto
2. start server ---> ./ser 9999(PortNo)
3. compile client.cpp --> g++ -o cl client.cpp
4. start client -->  ./cl 127.0.0.1 9999(SamePortNo as Server)

### Inside Working 
1. First client server generate the same key with help of diffie hellman key exchange and MillerRobin Test Algorithms.
2. After that,all the messages and data exchange between client and server will be encrypted using caesar cipherAlgo with Key generated in part1.
3. ID should be unique and then can able to login by entering its Id and password(MAX 10 char).
3. Server will not stored the password of client , instead it generates a saltNumber and qa(Prime no.send by client) and then server generate a hashcode of Password||salt||qa (|| concatenate) using SHA1 and stored it in file.
4. All the files which client have to download are send using the 512B chunks and they are also encrypted using caesar cipherAlgo with Key generated in part1.And client side will decypt it with same key.


**NOTE** --> File must contain only those character which are define in caesar cipherAlgo
  
