#include "utils.h"
#include <iostream>

using namespace std;

int ran_top = 0;

void printArray(double a[][8])
{
    int i, j;
    cout << "\nMatrix: \n";
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            cout << "\t" << a[i][j];
        }
        cout << "\n";
    }
}

int returnRandom(int ran[])
{
    return ran[ran_top++];
}

int available_actions(int state, int available_acts[], double rMatrix[][8])
{
    int k = 0, j = 0;
    while (j < 8) {
        if (rMatrix[state][j] >= 0.0) {
            available_acts[k] = j;
            k++;
        }
        j++;
    }
    cout << "\n";
    return k;
}

int sample_next_action(int size, int available_acts[], int ran[])
{
    int a = returnRandom(ran);
    int next_action = available_acts[a % size];
    return next_action;
}

double update(int current_state, int action, double rMatrix[][8], double qMatrix[][8], int max_index[], int ran[], double gammaLR)
{
    int i = 0, j = 0, k = 0, index_of_max;
    double temp_max = 0.0, max_value = 0.0, sumA = 0.0;
    
    // Collect indices with maximum Q value for the given action row
    for (i = 0; i < 8; i++) {
        max_index[i] = 0;
        if (temp_max == qMatrix[action][i]) {
            max_index[j] = i;
            j++;
        } else if (temp_max < qMatrix[action][i]) {
            j = 0;
            temp_max = qMatrix[action][i];
            max_index[j] = i;
            j++;
        }
    }
    
    // Randomly select one index among those with maximum value
    int a = returnRandom(ran) % j;
    index_of_max = max_index[a];
    max_value = qMatrix[action][index_of_max];
    
    // Update Q value using the Q-learning update rule
    qMatrix[current_state][action] = rMatrix[current_state][action] + (gammaLR * max_value);
    temp_max = 0.0;
    
    // Find the maximum value in the Q-matrix after the update
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (qMatrix[i][j] > temp_max) {
                temp_max = qMatrix[i][j];
            }
        }
    }
    cout << "\nQ Max: " << temp_max;
    printArray(qMatrix);
    
    // Sum the normalized Q-matrix values and return a score
    if (temp_max > 0) {
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                sumA += qMatrix[i][j] / temp_max;
            }
        }
        sumA = sumA * 100;
        return sumA;
    }
    else {
        return 0.0;
    }
}
