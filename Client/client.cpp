#include <bits/stdc++.h>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
using namespace std;


char arr[] = {' ','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U',
        'V','W','X','Y','Z',',','.','?','0','1','2','3','4','5','6','7','8','9','a','b','c','d',
        'e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','!'};
long long int key = 0;
int listenFd;

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

bool RobinMillerTest(long long int d, long long int n)
{
    time_t t;
    srand((unsigned) time(&t));
    long long int a = 2+rand()%(n-4);

    long long int x = binExponentiation(a,d,n);
    if (x == 1|| x == n-1)
        return true;

    while(d != n-1)
    {
        x = (x*x)%n;
        d =d*2;
        if (x == 1)
            return false;
        if (x == n-1)
            return true;
    }
    return false;
}

bool isPrime(long long int num)
{   
    if (num == 2 || num == 3)
        return true;
    if (num == 1 || num == 4)
        return false;
    if(num%2 == 0)
    {
        return false;
    }
    long long int d ;
    d = num -1;
    while(d%2 == 0)
        d = d/2;
    int k = 6;

    while(k)
    {
        if (RobinMillerTest(d,num) == false)
            return false;
        k--;
    }
    return true;
}

long long int permitiveRoot(long long int n)
{
    time_t t;
    srand((unsigned) time(&t));
    long long int capN = n-1;
    long long int capP = n;
    n = n-1;

    bool flag = true;
    long long int arr[1000000];
    
    
    int cnt = 0;

    while(n%2 == 0)
    {
        n = n/2;
        if(flag)
        {
            arr[cnt] = capN/2;
            flag =false;
            cnt++;
        }
    }
    flag =true;
    for(long long int i = 3 ; i*i <= n;i++)
    {
        while(n%i == 0)
        {

            n=n/i;
            if(flag)
            {
                arr[cnt]=capN/i;
                cnt++;
                flag = false;
            }
        }
        flag =true;
    }
    if(n>2)
    {
        arr[cnt] =capN/n;
        cnt++;
    }

    
    bool ProFlag = true;
    long long int pri; //random root
    while(1)
    {
        pri = 2+rand()%(capP-3);
        
        for(long long int i=0;i<cnt;i++)
        {
            
            long long int ans = binExponentiation(pri,arr[i],capP);
            
            if(ans == 1)
            {
                ProFlag = false;
                break;
            }
        }
        if (ProFlag)
            break;
        
        ProFlag= true;
    }
    
    return pri;
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

void LOGINCREAT(string id,string pass,long long int qa){
        char bffer[1024];
        cout <<"*** Before Encryption ***" <<endl;
        cout <<"ID "<<id<<endl;
        cout <<"Password "<<pass<<endl;
        string id1=encryptCaeser(id);
        string pass1=encryptCaeser(pass);
        cout <<"*** After Encryption ***" <<endl;
        cout <<"ID1: "<<id1<<endl;
        cout <<"Password1: "<<pass1<<endl;
        
        memset(bffer,0,sizeof(bffer));
        sprintf(bffer,"%s#%s#%lld",id1.c_str(),pass1.c_str(),qa);
        printf("%s\n",bffer );

        send(listenFd, bffer, sizeof(bffer), 0);

}

void AUTHREQUEST(string id, string pass){
    char bffer[1024];
    cout <<"*** Before Encryption ***" <<endl;
    cout <<"ID "<<id<<endl;
    cout <<"Password "<<pass<<endl;
    string id1=encryptCaeser(id);
    string pass1=encryptCaeser(pass);
    cout <<"*** After Encryption ***" <<endl;
    cout <<"ID1: "<<id1<<endl;
    cout <<"Password1: "<<pass1<<endl;
        
    memset(bffer,0,sizeof(bffer));
    sprintf(bffer,"%s#%s",id1.c_str(),pass1.c_str());
    printf("%s\n",bffer );

    send(listenFd, bffer, sizeof(bffer), 0);
}

void SERVICEREQUEST(string input,string fileName){
    char bffer[1024];
    string id1=encryptCaeser(input);
    string fileName1=encryptCaeser(fileName);
    cout <<"ID1: "<<id1<<endl;
    cout <<"FileName1: "<<fileName1<<endl;
        
    memset(bffer,0,sizeof(bffer));
    sprintf(bffer,"%s#%s",id1.c_str(),fileName1.c_str());
    printf("%s\n",bffer );
    send(listenFd, bffer, sizeof(bffer), 0);

}


int main (int argc, char* argv[])
{
    //printf("%ld\n",strlen(arr));
    //int listenFd, portNo;
    int portNo;
    bool loop = false;
    struct sockaddr_in svrAdd;
    struct hostent *server;
    char bffer[1024];
    if(argc < 3)
    {
        cerr<<"Syntax : ./cl <IP> <Port>"<<endl;
        return 0;
    }
    
    portNo = atoi(argv[2]);
    
    if((portNo > 65535) || (portNo < 2000))
    {
        cerr<<"Please enter port number between 2000 - 65535"<<endl;
        return 0;
    }       
    
    //create client skt
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(listenFd < 0)
    {
        cerr << "Cannot open socket" << endl;
        return 0;
    }
    
    server = gethostbyname(argv[1]);
    
    if(server == NULL)
    {
        cerr << "Host does not exist" << endl;
        return 0;
    }
    
    bzero((char *) &svrAdd, sizeof(svrAdd));
    svrAdd.sin_family = AF_INET;
    
    bcopy((char *) server -> h_addr, (char *) &svrAdd.sin_addr.s_addr, server -> h_length);
    
    svrAdd.sin_port = htons(portNo);
    
    int checker = connect(listenFd,(struct sockaddr *) &svrAdd, sizeof(svrAdd));
    
    if (checker < 0)
    {
        cerr << "Cannot connect!" << endl;
        return 0;
    }
    
    //send stuff to server
        long long int q;
        time_t t;
        srand((unsigned) time(&t));
        while(1)
        {
            q = rand()%1000000000;
            if(isPrime(q))
            {
                break;
            }
        }
        long long int alpha = permitiveRoot(q);
        long long int xa = 2+rand()%(q-3);
        long long int ya = binExponentiation(alpha,xa,q);
        //printf("prime number -%lld permitiveRoot - %lld ya --- %lld\n",q,alpha,ya);
        memset(bffer,0,sizeof(bffer));
        sprintf(bffer,"%lld#%lld#%lld",q,alpha,ya);
        
        send(listenFd, bffer, sizeof(bffer), 0);
        memset(bffer,0,sizeof(bffer));
        recv(listenFd,bffer,1024,0);
        long long int yb = atoi(bffer);
        key = binExponentiation(yb,xa,q);
        key = key%67;
        printf("key-------%lld\n",key); 
        //write(listenFd, s, strlen(s));
        printf("Key generated\n");

        //Login Part
        printf("Do you want to login\n");
        string s;
        cin >> s;
        if(s=="yes"||s=="YES"||s=="Yes")
        {
            string id,pass;
            do{
            printf("Enter the login ID and Password\n");
            
            cin >> id >> pass;
            }while(pass.length()>10);
            
            long long int qa;
            while(1)
            {
            qa = rand()%1000000000;
            if(isPrime(qa))
            {
                break;
            }
            }
            LOGINCREAT(id,pass,qa);
        }
 


        memset(bffer,0,sizeof(bffer));
        recv(listenFd,bffer,1024,0);
        cout << "REPLY from Server: "<<decryptCaesor(string(bffer)) <<endl;
        if(decryptCaesor(string(bffer))=="UNSUCCESSFULL"){
            cout <<" Duplicate ID"<<endl;
            exit(1);
        }
        //File Sharing Part Start
        cout << "Do you want to Access the File"<<endl;
        cin >> s;
        if(s=="yes"||s=="YES"||s=="Yes")
        {
        printf("Enter the login ID and Password for Authorisation\n");
        string pass2,id2;
        cin >> id2 >> pass2;
        AUTHREQUEST(id2,pass2);

        memset(bffer,0,sizeof(bffer));
        recv(listenFd,bffer,1024,0);
        cout << "REPLY from Server: "<<decryptCaesor(string(bffer)) <<endl;
  
        if(decryptCaesor(string(bffer))=="SUCCESS"){
            string fileName;
            cout <<"Enter the fileName Which you want to Access"<<endl;
            cin >> fileName;
            SERVICEREQUEST(id2,fileName);

            //it gets reply from server file exist or not in server
            memset(bffer,0,sizeof(bffer));
            recv(listenFd,bffer,1024,0);
			if (decryptCaesor(string(bffer))=="SUCCESSFULL")
			{

            ofstream fout;
            fout.open(fileName, ios::app);

            char tempBuffer[512];
            memset(tempBuffer,0,sizeof(tempBuffer));
            int r;
            while(r=recv(listenFd,tempBuffer,512,0)>0)
            {
            string txt=decryptCaesor(string(tempBuffer));
            fout << txt;
            memset(tempBuffer,0,sizeof(tempBuffer));
            
            }
            fout.close();

            //status
            if(r==0)
                printf("SUCCESSFULL\n");
             
        }
            else{
            // file don't exist in server
            cout << "REPLY from Server: "<<decryptCaesor(string(bffer)) <<endl;
            }
        
        }

        else{
            cout <<"Please Check Your Password"<<endl;
        }
        
    }
}