#include <bits/stdc++.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <stdlib.h>
#include <openssl/sha.h>
#include <pthread.h>
using namespace std;

static int connFd;

char arr[] = {' ','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U',
        'V','W','X','Y','Z',',','.','?','0','1','2','3','4','5','6','7','8','9','a','b','c','d',
        'e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','!'};

long long int key=0;
char bffer[1024];
long long int binExponentiation(long long int a, long long int b, long long int n)
{
    long long int ans = 1;
    a = a%n;
    while(b>0)
    {
        if(b & 1)
            ans = (ans*a) % n;

        b = b>>1;
        a = (a*a)%n;
    }
    return ans;
}

int Encoding(char c){
int p;
if ( c >= 97 && c <= 122 )
      p = c - 57;
    else if ( c >= 65 && c <= 90 )
      p = (c - 64);
    else if ( c == ' ' ) 
        p = 0;
    else if(c==',')
        p=27;
    else if(c == '.')
        p=28;
    else if(c=='?')
        p=29;
    else if(c=='!')
        p=66;
    else if(c>=48&&c<=57)
        p=c-18;
    return p;
}

string decryptCaesor(string input){
	string plainText="";
	for (int i = 0; i < input.length(); ++i)
	{
		int c=Encoding(input[i]);
		int ind=(c-key)%67;
		if(ind <0)
			ind=67-key+c;
		char p=arr[ind];
		plainText=plainText+p;
	}
	return plainText;
}

string encryptCaeser(string input){
    string cipher="";
    for (int i = 0; i < input.length(); ++i)
    {
        int p=Encoding(input[i]);
        char c=arr[(p+key)%67];
        cipher=cipher+c;
    }
    //cout<<endl;
    return cipher;
}

void LOGINREPLY(string input){
	string encrytInput=encryptCaeser(input);
	memset(bffer,0,sizeof(bffer));
    sprintf(bffer,"%s",encrytInput.c_str());
    send(connFd, bffer, sizeof(bffer), 0);
}

void AUTHREPLY(string input){
	string encrytInput=encryptCaeser(input);
	memset(bffer,0,sizeof(bffer));
    sprintf(bffer,"%s",encrytInput.c_str());
    send(connFd, bffer, sizeof(bffer), 0);
}

void SERVICEDONE(string fileName,string status){
	string encryt_status=encryptCaeser(status);
	memset(bffer,0,sizeof(bffer));
    sprintf(bffer,"%s",encryt_status.c_str());
    send(connFd, bffer, sizeof(bffer), 0);
}

int main(int argc, char* argv[])
{
    int pId, portNo, listenFd;
    socklen_t len;
    
    struct sockaddr_in svrAdd, clntAdd;

    //DataStructures
    map<string, pair<long long int,long long int>> info_ID;// id as key and values as (qa,saltNumber) 
    unordered_map<long long int, string> info_hashedPassword;// qa as key and hashCode as value
    map<string, pair<long long int,long long int>>::iterator it1;
    unordered_map<long long int, string>::iterator it2;

    //Populate the dataStructures from files
    ifstream file("login.txt");
    string takenFromLoginFile;
    while (getline(file, takenFromLoginFile))
    {
        string idforLogin=takenFromLoginFile.substr(0,takenFromLoginFile.find(","));
        takenFromLoginFile=takenFromLoginFile.substr(takenFromLoginFile.find(",")+1);
        long long int saltForLogin =atoi(takenFromLoginFile.substr(0,takenFromLoginFile.find(",")).c_str());
        takenFromLoginFile=takenFromLoginFile.substr(takenFromLoginFile.find(",")+1);
        string hashedFromLogin=takenFromLoginFile.substr(0,takenFromLoginFile.find(","));
        takenFromLoginFile=takenFromLoginFile.substr(takenFromLoginFile.find(",")+1);
        long long int qafromLogin=atoi(takenFromLoginFile.c_str());

        
        info_ID.insert(pair<string,pair<long long int,long long int>>(idforLogin,make_pair(qafromLogin,saltForLogin)));
        info_hashedPassword.insert(make_pair(qafromLogin,hashedFromLogin));
    }

    //no. of files which are present in server
    std::vector<string> v;
    v.push_back("f1.txt");
    v.push_back("f2.txt");
    v.push_back("f3.txt");

    if (argc < 2)
    {
        cerr << "Syntax : ./server <port>" << endl;
        return 0;
    }
    
    portNo = atoi(argv[1]);
    
    if((portNo > 65535) || (portNo < 2000))
    {
        cerr << "Please enter a port number between 2000 - 65535" << endl;
        return 0;
    }
    
    //create socket
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(listenFd < 0)
    {
        cerr << "Cannot open socket" << endl;
        return 0;
    }
    
    bzero((char*) &svrAdd, sizeof(svrAdd));
    
    svrAdd.sin_family = AF_INET;
    svrAdd.sin_addr.s_addr = INADDR_ANY;
    svrAdd.sin_port = htons(portNo);
    
    //bind socket
    if(bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0)
    {
        cerr << "Cannot bind" << endl;
        return 0;
    }
    
    listen(listenFd, 5);
    
    len = sizeof(clntAdd);
    
    
    cout << "Listening.." << endl;

    //this is where client connects. svr will hang in this mode until client conn
    connFd = accept(listenFd, (struct sockaddr *)&clntAdd, &len);

    if (connFd < 0)
    {
        cerr << "Cannot accept connection" << endl;
        return 0;
    }
    else
    {
        cout << "Connection successful" << endl;
    }
        
    memset(bffer,0,sizeof(bffer));

    recv(connFd,bffer,1024,0);
    long long int ya;
    long long int q ;
    long long int alpha;
    sscanf(bffer,"%lld#%lld#%lld",&q,&alpha,&ya);


    long long int xb = 2+rand()%(q-3);
    long long int yb = binExponentiation(alpha,xb,q);
    memset(bffer,0,sizeof(bffer));
    sprintf(bffer,"%lld",yb);
    send(connFd, bffer, sizeof(bffer), 0);

    key = binExponentiation(ya,xb,q);
    key = key%67;
    printf("key-------%lld\n",key); 
    
    printf("Key generated\n");
    
   //Login data received from client
   	memset(bffer,0,sizeof(bffer));
    recv(connFd,bffer,1024,0);
    string str(bffer);
    string id=str.substr(0,str.find("#"));
    str=str.substr(str.find("#")+1);
    string pass=str.substr(0,str.find("#"));
    str=str.substr(str.find("#")+1);
    //cout << str << endl;
    long long int qa=atoi(str.c_str());


    cout <<"**** Before Decryption ***"<<endl;
    cout << "id: " <<id << endl;
    cout << "pass:" <<pass <<endl;
    cout << "qa:"<<qa << endl;
    id=decryptCaesor(id);
    pass=decryptCaesor(pass);
    cout<<"**** After Decryption ****"<<endl;
    cout << "id: " <<id << endl;
    cout << "pass:" <<pass <<endl;
    cout << "qa:"<<qa << endl;



    //Salt number
    long long int salt;
    salt=(rand()*qa)%10000;

    string forHash=pass+to_string(salt)+to_string(qa);
   
    char buf[SHA_DIGEST_LENGTH*2];
	unsigned char hash[SHA_DIGEST_LENGTH];// length  of HASH is 20 Bytes
	memset(buf, 0x0, SHA_DIGEST_LENGTH*2);
	memset(hash, 0x0, SHA_DIGEST_LENGTH);

  	SHA1((unsigned char *)forHash.c_str(), forHash.length(), hash);
    for (int i=0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf((char*)&(buf[i*2]), "%02x", hash[i]); 
	}
  	
    
    // inserting into map and check for duplicate keys
    if(info_ID.count(id)>0){
        LOGINREPLY("UNSUCCESSFULL");
        exit(1);
    }
    else{
        info_ID.insert(pair<string,pair<long long int,long long int>>(id,make_pair(qa,salt)));
        info_hashedPassword.insert(make_pair(qa,string(buf))); 
    }
    
    

    //Print the map
    // for(it1=info_ID.begin();it1!=info_ID.end();it1++)
    //     cout << it1->first<<" "<<it1->second.first<<" "<<it1->second.second<<endl;

    
    // for(it2=info_hashedPassword.begin();it2!=info_hashedPassword.end();it2++)
    //     cout << it2->first<<" "<<it2->second<<endl;



    //to write into file
  	string tableEntry=id+","+to_string(salt)+","+string(buf)+","+to_string(qa)+"\n";
  	//cout << tableEntry << endl;
  	ofstream myfile;
	myfile.open ("login.txt",ios::app);
	myfile << tableEntry;
	myfile.close();
	

	LOGINREPLY("SUCCESS");


	//Users information Login To Access file
	memset(bffer,0,sizeof(bffer));
    recv(connFd,bffer,1024,0);
    cout << bffer<<endl;
    string str1(bffer);
    string idForAccess=str1.substr(0,str1.find("#"));
    string passForAccess=str1.substr(str1.find("#")+1);
    cout <<"*** Before Decryption ***" <<endl;
    cout << idForAccess << endl;
    cout << passForAccess << endl;
    idForAccess=decryptCaesor(idForAccess);
    passForAccess=decryptCaesor(passForAccess);
    cout <<"*** After Decryption ***" <<endl;
    cout << "id: " <<idForAccess << endl;
    cout << "pass:" <<passForAccess <<endl;
    

    long long int qaForAccess=info_ID[idForAccess].first;
    long long int SaltForAccess=info_ID[idForAccess].second;
    string hashedPasswordForAccess=info_hashedPassword[qaForAccess];

    string forAccess=passForAccess+to_string(SaltForAccess)+to_string(qaForAccess);
    memset(buf, 0x0, SHA_DIGEST_LENGTH*2);
	memset(hash, 0x0, SHA_DIGEST_LENGTH);

  	SHA1((unsigned char *)forAccess.c_str(), forAccess.length(), hash);
    for (int i=0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf((char*)&(buf[i*2]), "%02x", hash[i]); 
	}

	if(string(buf)==hashedPasswordForAccess){
		AUTHREPLY("SUCCESS");

		memset(bffer,0,sizeof(bffer));
	    recv(connFd,bffer,1024,0);
	    cout << bffer<<endl;
	    string str2(bffer);
	    
	    string idFile=str2.substr(0,str2.find("#"));
	    string fileName=str2.substr(str2.find("#")+1);
	    cout<<"***** Before Decryption ***"<<endl;
        cout <<"id "<< idFile << endl;
	    cout << "fileName "<<fileName << endl;
	    idFile=decryptCaesor(idFile);
	    fileName=decryptCaesor(fileName);
	    cout<<"***** After Decryption ***"<<endl;
	    cout << "id: " <<idFile << endl;
	    cout << "fileName: " <<fileName <<endl;


	    if(find(v.begin(),v.end(),fileName)!=v.end()){
	    
        SERVICEDONE(fileName,"SUCCESSFULL");
        // send the file to the client
	    FILE *fp=fopen(fileName.c_str(),"r");
	    
        char plaintext[512];
	    while(1)
    	{
    	memset(bffer,0,sizeof(bffer));
        memset(plaintext,0,sizeof(plaintext));
        int nread = fread(plaintext,1,512,fp);
        //cout << plaintext <<endl;
        plaintext[511]='\0';
        string cipher=encryptCaeser(string(plaintext));
        
         if(nread > 0)
			{
            
			memset(bffer,0,sizeof(bffer));
    		sprintf(bffer,"%s",cipher.c_str());
    		//cout << bffer <<endl;
    		send(connFd, bffer, sizeof(bffer), 0);
    	}
        if (nread < 512)
        {
            if (feof(fp))
                printf("FILE SENT SUCCESSFULLY\n");
            if (ferror(fp))
                printf("Error reading\n");
            break;
        }
    }//endof while
   
	    	
}//endof if

	    else{
            //because file is not present in server
	    	SERVICEDONE(fileName,"UNSUCCESSFULL");
	    }

	}
	else
		{
			AUTHREPLY("FAILED");
		}
    return 0;
    
    
}

