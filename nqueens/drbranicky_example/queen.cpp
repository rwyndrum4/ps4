// Michael S. Branicky, 04 February 2005
// compiles with g++ -lc -lm
//
// This code offered with no guarantees whatsoever.
//
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;
#define TEST    1
#define ELITISM 0

int    N=1000;      // number of generations
int    k=1001;      // number of individuals
int    EVALS;       // number of fitness evaluations
double pmut=0.1;    // mutation probability

// returns a uniformly distributed random double between 0.0 and 1.0
double myrand() {
  return drand48();
}

// Use: Metropolis Algorithm for Maximization
//
// Source: Based on the code and explanation
// of a routine for minimization found in
// Press et al., _Numerical_Recipes_in_C_,
// Cambridge Univ. Press, 1988.  Page 351.
//
// Returns a boolean value on whether or not to
// accept a reconfiguration which leads to a
// change dE in the objective function E.
// If dE>0, returns 1 (TRUE); if dE<=0, returns
// 1 with probablity exp(dE/T), where T is a
// temperature determined by the annealing schedule.
//
int metrop(double dE, double T) {
  double r=myrand();
#if TEST
  cout << exp(dE/T) << endl;;
#endif
  return ((dE>0)||(r<exp(dE/T)));
}

// returns 1 if queens q1 and q2 are attacking each other, 0 o.w.
int attacking(int q1col, int q1row, int q2col, int q2row) {
  if (q1col==q2col) return(1); // same column
  if (q1row==q2row) return(1); // same row
  int coldiff=q1col-q2col;
  int rowdiff=q1row-q2row;
  if (abs(coldiff)==abs(rowdiff)) return(1); // same diagonal
  return(0);
}

// evaluates the fitness of an encoding, defined as the number of
// non-attacking pairs of queens (28 - number of attacking pairs)
//
// the global variable EVALS keeps track of the number of times called
int fitness(int encoding[]) {
  EVALS++;
  int E=28;
  for (int i=1; i<8; i++) {
    for (int j=i+1; j<=8; j++) {
      E-=attacking(i, encoding[i-1], j, encoding[j-1]);
    }
  }
  return(E);
}

// the following is useful in a variety of algorithms
// returns the nth successor of an encoding
void getsuccessor(int init[], int n, int succ[]) {
  n--;
  int remainder=(n%7);
  int quotient=(n-remainder)/7;
  // int newrow=1+((remainder+init[quotient]-1)%8);
  int newrow=init[quotient]+remainder+1;
  if (newrow>8) newrow-=8;
  for (int j=0; j<8; j++) {
    if (j==quotient) succ[j]=newrow;
    else succ[j]=init[j];
  }
}

// copy each digit of an encoding with a prob. of error p (in place)
void mutate(int enc[], double p) {
  for(int i=1; i<=8; i++) {
    double r=myrand();
    if (r<p) enc[i-1]=(int) ceil(myrand()*8);
  }
}

// copy each digit of an encoding with a prob. of error p
void mutate2(int src[], int dest[], double p) {
  for(int i=1; i<=8; i++) {
    double r=myrand();
    if (r<p) dest[i-1]=(int) ceil(myrand()*8);
    else dest[i-1]=src[i-1];
  }
}

// produces TWO offspring via crossover at a random location on two encodings (in place)
void xover(int e1[], int e2[]) {
  // choose a andom integer c between 1 and 7, inclusive
  int c=(int) ceil(myrand()*7);
#if TEST
  cout << c << endl;
#endif
  for (int i=(c+1); i<=8; i++) {
    int tmp=e1[i-1];
    e1[i-1]=e2[i-1];
    e2[i-1]=tmp;
  }
}

void printenc(char s[], int encoding[]) {
  for(int i=0; i<8; i++) s[i]='0'+((char) encoding[i]);
}

// test the routines
int test() {
  char s[9], s2[9];

  for (double T=10000000.; T>0.000001; T/=10.) {
    int i=metrop(-1.,T);
    // int j=metrop(1.,T);
    // cout << i << ", " << j << endl;
    cout << i << endl;
  }

  s[8]=0;
  s2[8]=0;
  // configs from R&N, p 112 [p 123 in 4th edition]
  int enc1[8]={5, 6, 7, 4, 5, 6, 7, 6};
  int enc2[8]={8, 3, 7, 4, 2, 5, 1, 6};

  // configs from R&N, p 117 [p 127 in 4th edition]
  int enc3[8]={2, 4, 7, 4, 8, 5, 5, 2};
  int enc4[8]={3, 2, 7, 5, 2, 4, 1, 1};
  int enc5[8]={2, 4, 4, 1, 5, 1, 2, 4};
  int enc6[8]={3, 2, 5, 4, 3, 2, 1, 3};

  // test fitness
  printenc(s,enc1); cout << "Fitness ( " << s << " ) = " << fitness(enc1) << "\n";
  printenc(s,enc2); cout << "Fitness ( " << s << " ) = " << fitness(enc2) << "\n\n";
  printenc(s,enc3); cout << "Fitness ( " << s << " ) = " << fitness(enc3) << "\n";
  printenc(s,enc4); cout << "Fitness ( " << s << " ) = " << fitness(enc4) << "\n";
  printenc(s,enc5); cout << "Fitness ( " << s << " ) = " << fitness(enc5) << "\n";
  printenc(s,enc6); cout << "Fitness ( " << s << " ) = " << fitness(enc6) << "\n\n";

  // test mutate
  mutate2(enc1,enc2,1.0); printenc(s,enc2); cout << "Random Encoding: " << s << "\n";
  mutate2(enc1,enc2,0.5); printenc(s,enc2); cout << "Random Encoding: " << s << "\n";
  mutate2(enc1,enc2,0.0); printenc(s,enc2); cout << "Random Encoding: " << s << "\n\n";
  mutate(enc1,1.0); printenc(s,enc1); cout << "Random Encoding: " << s << "\n";
  mutate(enc1,0.5); printenc(s,enc1); cout << "Random Encoding: " << s << "\n";
  mutate(enc1,0.0); printenc(s,enc1); cout << "Random Encoding: " << s << "\n\n";

  // test xover
  xover(enc3,enc4);
  printenc(s,enc3); printenc(s2,enc4); cout << "Offspring = " << s << ", " << s2 << endl;
  xover(enc5,enc6);
  printenc(s,enc5); printenc(s2,enc6); cout << "Offspring = " << s << ", " << s2 << endl;

  // test successor function
  int enc7[8]={1, 2, 3, 4, 5, 6, 7, 8};
  int enc8[8];
  int bestE=0;
  int bestSuccIndex;
  for (int i=1; i<=56; i++) {
    getsuccessor(enc7, i, enc8);
    int f=fitness(enc8);
    if ((f>bestE)||((f==bestE)&&(myrand()<0.1))) { bestE=f; bestSuccIndex=i; }
    printenc(s,enc8); cout << "Successor ( " << i << " ) = " << s << ", " << f << endl;
  }
  getsuccessor(enc7, bestSuccIndex, enc8);
  printenc(s,enc8); cout << "Best Successor ( " << bestSuccIndex << " ) = " << s << endl;
  // generate a random successor
  int RandomSuccIndex=(int) ceil(myrand()*56); // random int in {1, 2, ..., 56}
  getsuccessor(enc7, RandomSuccIndex, enc8);
  printenc(s,enc8); cout << "Successor ( " << RandomSuccIndex << " ) = " << s << endl;

}



int main() {
  int pop[k][8], newpop[k][8], popE[k];
  int bestE, sumE, bestind;
  char s[9];
  double cumprobs[k];

  srand48((long) getpid());
  s[8]=0;
#if TEST
  test();
  return(1);
#endif

  EVALS=0;
  // start with a completely random population
  // for (int i=0; i<k; i++) (void) mutate2(pop[i],pop[i],1.0);
  for (int i=0; i<k; i++) {
    (void) mutate(pop[i],1.0);
    // printenc(s,pop[i]); cout <<  i << ": " << s << "\n";
  }

  bestE=0;
  // loop over generations
  for (int t=1; t<=N; t++) {
    cout << "Generation " << t << " ... " << endl;

    // evaluate population's fitness
    sumE=0;
    bestind=0;
    for (int i=0; i<k; i++) {
      int E=fitness(pop[i]);
      sumE+=E;
      popE[i]=E;
      if (E>bestE) {
        bestE=E;
        bestind=i;
        printenc(s,pop[bestind]);
        cout << "New Best ( " << s << " ): " << EVALS << ", " << bestE << "\n";
        if (bestE==28) {
          cout << "Solution Found" << endl;
          return(1);
        }
      }
    }

    // compute selection probabilities
    cumprobs[0]=(double) popE[0]/sumE;
    // cout << cumprobs[0] << endl;
    for (int i=1; i<k; i++) {
      cumprobs[i]=cumprobs[i-1]+((double) popE[i]/sumE);
      // cout << cumprobs[i] << endl;
    }

    // selection, possibly with ELITISM
    if (ELITISM) for (int j=0; j<8; j++) newpop[0][j]=pop[bestind][j];
    for (int i=ELITISM; i<k; i++) {
      int index;
      double r=myrand();
      for (int j=0; j<k; j++)
        if (cumprobs[j]>=r) { index=j; break; }
      for (int j=0; j<8; j++)
        newpop[i][j]=pop[index][j];
    }

    // crossover
    for (int i=ELITISM; i<k; i+=2) (void) xover(newpop[i], newpop[i+1]);

    // mutation
    for (int i=ELITISM; i<k; i++) (void) mutate2(newpop[i], pop[i], pmut);

    // THE FOLLOWING 6 LINES ARE FOR PERFORMING MUTATION IN PLACE
    // for (int i=ELITISM; i<k; i++) (void) mutate(newpop[i], pmut);
    // for (int i=0; i<k; i++) {
      // for (int j=0; j<8; j++)
        // pop[i][j]=newpop[i][j];
      //// printenc(s,pop[i]); cout <<  i+1 << ": " << s << "\n";
    // }
  }
  return(0);
}
