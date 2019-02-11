### SecureFTP
###### (works only in debian based LINUX)

### How To run
1. compile server.cpp ---> g++ -o ser server.cpp
2. start server ---> ./ser <PortNo>
3. compile client.cpp --> g++ -o cl client.cpp
4. start client -->  ./cl 127.0.0.1 <SamePortNo as Server>

### Inside Working 
1. First client server generate the same key with use of diffie hellman key exchange and MillerRobin Test.
2. After that,All the message and data exchange between client and server will be encrypted using caesar cipherAlgo and Key generates in part1.
3. Client will be unique and can able to login(all are encrypted)
3. Server will not stored the password of client , instead it generates a saltNumber and qa(send by client) and server generate a hashcode of (Password||salt||qa) and stored it in file.
4. All the files which have to download are also in encrypted.
**NOTE** --> File must contain only those character which are define in caesar cipherAlgo
  
