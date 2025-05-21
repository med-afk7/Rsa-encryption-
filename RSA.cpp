#include <iostream>
#include <gmpxx.h>
#include <fstream>
#include <ctime>

using namespace std;

#define BIT_SIZE 1024 //  we are using 1024 bits here but 2048 can be used aswell 



void GenerateKey() {
   mpz_t p , q , n , phi , e , d ;

   mpz_inits(p, q, n, phi, e, d, NULL);


    //gmp_randstate_t help us get the state that we use in random number generation for our keys 
    gmp_randstate_t state;
    gmp_randinit_mt (state);
    gmp_randseed_ui(state, time(NULL));

    // mpz_urandom gives us a randomnumber 
   mpz_urandomb(p,state,BIT_SIZE/2);
   mpz_urandomb(q,state,BIT_SIZE/2);
   // since gmp random doesnt give us a prime we use Mpz_nextprime to get the closest prime after our random number 

   mpz_nextprime(p,p);
   mpz_nextprime(q,q);

   mpz_mul(n,p,q);// callculating n from the two random primes we got from gmp .


   mpz_t q1 , p1 ;
   mpz_inits(p1, q1, NULL);
   // we callculate phi using these numbers ;

   mpz_sub_ui(q1 , q , 1);
   mpz_sub_ui(p1 , p , 1);
   mpz_mul(phi, q1,p1);

   //now we choose e and calculate d 
   mpz_set_ui(e, 65537);
//d
//this takes the inverse of e and mods it with phi which will give us the value for phi 
   mpz_invert (d , e , phi);


       // Save keys to files
    std::ofstream pubFile("Pub.txt"), privFile("Priv.txt");
    pubFile << e << "\n" << n;
    privFile << d << "\n" << n;
    pubFile.close();
    privFile.close();
    


// Clean up as reccomended by the gmp doc
mpz_clears(p, q, n, phi, e, d, p1, q1, NULL);
gmp_randclear(state);

  
}

void encrypt(const string &a ){
mpz_t e,n,m,c ;
mpz_inits(e,n,m,c,NULL);

ifstream pubFile("Pub.txt");
pubFile >> e >> n ;
pubFile.close();

//this take the message from chars to integers 
mpz_import(m, a.size(), 1, sizeof(a[0]), 0, 0, a.c_str());

mpz_powm(c , m , e , n);
 gmp_printf("Encrypted message: %Zd\n", c);
 std::ofstream encFile("cipher.txt");
    encFile << c;
    encFile.close();
    



//clearing as a good practice 
mpz_clears(e,n,m,c);

};
void  decrypt (){
    mpz_t d,n,m,c;
    mpz_inits(d,n,m,c,NULL);

   ifstream privFile("Priv.txt"), encFile("cipher.txt");
    privFile >> d >> n;
    encFile >> c;
    privFile.close();
    encFile.close();
    mpz_powm(m ,c, d ,n );

    // now we have to transform the cipher text from integers back to string ;
    // Convert integer back to string
    size_t count;
    char *decrypted = (char*) mpz_export(NULL, &count, 1, sizeof(char), 0, 0, m);

    //stores the message and displayes it 
    string message(decrypted, count);
   cout << "\nDecrypted message: " << message << endl;


   free(decrypted);
   mpz_clears(d,n,m,c);

 };

int main (){


// generating the key 
cout<<"Key is being generated !\n\n";
GenerateKey();
// getting user input 

string input ;
cout<<"Please Enter The text you want to Encrypt"<<endl;
getline(cin, input);
 encrypt(input);


 decrypt();

}
