#include "autocomplete.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int term_comp(const void* a, const void* b)
{
    term* term_a = (term *)a;
    term* term_b = (term *)b;

    return strcmp(term_a->term, term_b->term);
}

void read_in_terms(struct term **terms, int *pnterms, char *filename)
{
    
    FILE *fp = fopen(filename, "r");

    
    char line[200] = { 0 };
    
    *pnterms = 0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {   
        int i = 0;
        while (isdigit(line[i]))
        {
            *pnterms = *pnterms * 10 + line[i] - '0';
            i++;
        }
        break;
    }

    fclose(fp);

    *terms = (term*)malloc(*pnterms * sizeof(term));

    fp = fopen(filename, "r");

    int count = 0;
    
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (isdigit(line[0]))
        {
            continue;
        }
        double weight = 0;
        char str[200];
        int j = 0;
        int i = 0;

        while(line[i] == ' ')
        {
            i++;
        }

        for(i; i < sizeof(line); i++)
        {
            if (isdigit(line[i]))
            {
                weight = weight * 10 + line[i]-'0';
            }
            else
            {
                break;
            }
        }

        i++;

        for (i; i < sizeof(line); i++)
        {
            if (line[i] == '\n')
            {
                break;
            }
            else 
            {
                str[j] = line[i];
                j++;
            }
        }

        str[j] = '\0';
        strcpy((*terms + count)->term, str);
        (*terms + count)->weight = weight;
        count++;
    }

    fclose(fp);

    qsort(*terms, *pnterms, sizeof(term), term_comp);

}

void slice(char* dest, char* srce, int size)
{
    memset(dest, '\0', sizeof(dest));
    strncpy(dest, srce, size);
}

int lowest_match(struct term *terms, int nterms, char *substr)
{
    int len = strlen(substr);
    int left = 0;
    int right = nterms; 
    while (left < right)
    { 
        int mid = floor((right+left)/2);
        if (mid == 0)
        {
            return mid;
        }
        char mid_term[len];
        char before_mid[len];
        slice(mid_term, (terms + mid)->term, len);
        slice(before_mid, (terms + mid - 1)->term, len);
        if (strcmp(mid_term, substr) == 0 && strcmp(before_mid, substr) != 0)
        {
            return mid;
        }

        else if (strcmp(mid_term, substr) > 0 || strcmp(before_mid, substr) == 0)
        {
            right = mid;
        }
        
        else
        {
            left = mid;
        }
    }
    return -1;
}

int highest_match(struct term *terms, int nterms, char *substr)
{
    int len = strlen(substr);
    int left = 0;
    int right = nterms; 
    while (left < right)
    {
        int mid = floor((right+left)/2);
        char mid_term[len];
        char after_mid[len];
        slice(mid_term, (terms + mid)->term, len);
        slice(after_mid, (terms + mid + 1)->term, len);
        if (mid == (nterms - 1) && strcmp(mid_term, substr) == 0 && strcmp(after_mid, substr) == 0)
        {
            return nterms;
        }

        if (strcmp(mid_term, substr) == 0 && strcmp(after_mid, substr) != 0)
        {
            return mid;
        }

        else if (strcmp(mid_term, substr) < 0 || strcmp(after_mid, substr) == 0)
        {
            left = mid;
        }
        
        else
        {
            right = mid;
        }
    }
    return -1;
}

int term_comp2(const void* a, const void* b)
{
    term* term_a = (term*)a;
    term* term_b = (term*)b;

    return term_b->weight - term_a->weight;
}

void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr)
{
    int low_ind = lowest_match(terms, nterms, substr);
    int high_ind = highest_match(terms, nterms, substr);
    *n_answer = high_ind - low_ind + 1;
    *answer = (term*)malloc(*n_answer * sizeof(term));
    for (int i = 0; i < *n_answer; i++)
    {
        strcpy((*answer + i)->term, (terms + low_ind + i)->term);
        (*answer + i)->weight = (terms + low_ind + i)->weight;
    }

    qsort(*answer, *n_answer, sizeof(term), term_comp2); 
}

void print_struct(term term1)
{
    printf("City name: %s\n", term1.term);
    printf("Weight: %f\n", term1.weight);
}

void print_block(term* p_s, int n_terms)
{
    for (int i = 0; i < n_terms; i++)
    {
        print_struct(*(p_s + i));
    }
}

int main (void)
{
    term* haha;
    int xd;
    char *loc = "C:/Users/alecy/Documents/School/UofTears/Year 1/Semester 2/Algorithms and Data Structures/Projects/p1/cities.txt";
    term* answer;
    int n_ans;
    read_in_terms(&haha, &xd, loc);
    autocomplete(&answer, &n_ans, haha, xd, "\'");
    print_block(answer, n_ans);
}

