#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

double totalfitness;

double fitness(double x) {
	double response = (4 + (2*x) + (2*sin(20*x)) - (4*(x*x)));
		return(response);
}

void mutate_numbers(double* arr, double* fit){
  totalfitness = 0;
  for(int i = 0; i < 10; i++){
  fit[i] = fitness(arr[i]);
  totalfitness = totalfitness + fit[i];
  }
  for(int i = 0; i < 10; i++){
    float num = (rand() % 10)/float(10);
    if((fit[i]/totalfitness) > num){
      float r = (rand() % 100)/float(100);
      if(r < 0.3){
        arr[i] = arr[i] - 0.01;
        if(arr[i] < 0){
          arr[i] = 0;
        }
      }
      else if(r > 0.3 && r < 0.6){
        arr[i] = arr[i] + 0.01;
        if(arr[i] > 1){
          arr[i] = 1;
        }
      }
      else{
        arr[i] = num;
      }
    }
  }
  return;
}

int main() {
  double* arr = new double[10];
  double* fit = new double[10];
  srand(time(NULL));
	for(int i = 0; i < 10; i++){
    float num = (rand() % 100)/float(100);
    arr[i] = num;
    fit[i] = fitness(arr[i]);
  }
for(int i = 0; i < 10; i++){
  for(int daganothkings = 0; daganothkings < 10; daganothkings++){
    fit[daganothkings] = fitness(arr[daganothkings]);
  }
  mutate_numbers(arr, fit);
}
totalfitness = 0;
for(int i = 0; i < 10; i++){
totalfitness = totalfitness + fit[i];
std::cout << "Value: " << arr[i] << " || Fitness: " << fit[i] << "\n";
}
  std::cout << "Total fitness: " << totalfitness << "\n";
}
//2 arrays, 1 of numbers 1 of their fitnesses
