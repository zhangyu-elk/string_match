//
// Created by zhangyu on 9/9/2019.
//

#include <time.h>
#include <cstdlib>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <windows.h>

/*Brute Force string match*/
bool BF(char *main, char *pattern)
{
    int mlen = strlen(main);
    int plen = strlen(pattern);
    for(int i = 0; i <= (mlen - plen); i++)
    {
        if(0 == strncmp(main + i, pattern, plen)) {
            //printf("BF match succ: index = %d\n", i, GetTickCount());
            return true;
        }
    }
    return false;
}

/*Rabin Karp算法*/
bool RK(const char *main, const char *pattern)
{
    int mlen = strlen(main);
    int plen = strlen(pattern);

    int target = 0;
    for(int i = 0; i < plen; i++)
    {
        target += (pattern[i] - ' ');
    }
    int last = 0;
    for(int i = 0; i <= (mlen - plen); i++)
    {
        if(last > 0)
        {
            last = last - (main[i-1] - ' ') + (main[i+plen-1] - ' ');
        }
        else
        {
            for(int j = 0; j < plen; j++)
            {
                last += (main[i+j] - ' ');
            }
        }
        if(last == target && 0 == strncmp(main+i, pattern, plen))
        {
            //printf("RX match succ: index = %d\n", i);
            return  true;
        }
    }
    //printf("BF match fail!\n");
    return  false;
}

void generateBC(const char* pattern, int bc[])
{
    for(int i = 0; i < strlen(pattern); i++)
    {
        bc[(int)pattern[i]] = i;
    }
}

void generateGS(const char* pattern, int suffix[], bool prefix[])
{
    int plen = strlen(pattern);
    for(int i = 0; i < plen; i++)
    {
        suffix[i] = -1;
        prefix[i] = false;
    }


    for(int i = 0; i < plen - 1; i++)
    {
        int j = i;
        int k = 0;
        while(j >= 0 && pattern[j] == pattern[plen - k - 1])
        {
            k++;
            j--;
            suffix[k] = j + 1;
        }

        if(j == -1)
        {
            prefix[k] = true;
        }
    }
}

int moveByGS(int j, int plen, int suffix[], bool prefix[])
{
    int k = plen - j - 1;   //后缀的长度

    if(suffix[k] != -1)
    {
        return j - suffix[k] + 1;
    }

    for(int i = j + 2; i < plen; i++)
    {
        if(prefix[plen - i])
        {
            return i;
        }
    }

    return plen;
}

/*boyer moore
 * bad char rule
 * good suffic shift
 * */
bool BM(const char *main, const char *pattern)
{
    int bc[128] = { -1 };
    generateBC(pattern, bc);

    int i = 0;
    int mlen = strlen(main);
    int plen = strlen(pattern);

    int *suffix = new int[plen];
    bool *prefix = new bool[plen];
    generateGS(pattern, suffix, prefix);
    while(i < (mlen - plen)) {
        int j;
        /*模式串从后往前匹配*/
        for (j = plen - 1; j >= 0; j--)
        {
            if (main[i + j] != pattern[j])
            {
                //坏字符对应的下边为j
                break;
            }
        }
        if (j < 0)
        {
            //printf("BM match succ: index = %d\n", i);
            return true; //匹配成功
        }

        int update = j - bc[(int) main[i+j]];
        if(j < plen - 1)
        {
            int GSupdate = moveByGS(j, plen, suffix, prefix);
            if(GSupdate > update)
            {
                update = GSupdate;
            }
        }

        i += update;
    }

    return false;
}

void getNext(int *next, const char *pattern)
{
	int plen = strlen(pattern);
	for(int i = 0; i < plen; i++)
	{
		next[i] = -1;
	}

	int j = -1;
	for(int i = 1; i < plen; i++)
	{

		while(j != -1 && pattern[j + 1] != pattern[i])
		{
			j = next[j];
		}
		if(pattern[j + 1] == pattern[i])
		{
			j++;
		}
		next[i] = j;
	}

	/*
	for(int i = 1; i < plen; i++)
	{
		if(next[i - 1] != -1 && pattern[i] == pattern[next[i - 1] + 1])
		{
			next[i] = next[i - 1] + 1;
		}
		else
		{
			int j = 1;
			while(j <= i)
			{

				if(pattern[0] == pattern[j] && 0 == strncmp(pattern, pattern + j, i - j + 1))
				{
					next[i] = i - j;
				}

				j++;
			}
		}
	}
	*/
}

bool KMP(const char *main, const char *pattern)
{
    int mlen = strlen(main);
    int plen = strlen(pattern);

    int *next = new int[plen];
    getNext(next, pattern);


    int j = 0;
    for(int i = 0; i < mlen; i++)
    {
        while(j > 0 && i > 0 && main[i] != pattern[j])
        {
            j = next[j - 1] + 1;
        }

        if(main[i] == pattern[j])
        {
            ++j;
        }
        if(j == plen)
        {
        	//printf("KMP match succ: index = %d\n", i - plen  + 1);
            //return i - plen  + 1;
           	return true;
        }
    }
    return false;
}

/*create a random string and it's a substring
 * between 64-126
 * */
static void random_string(char main[], int mlen, char sub[], int slen)
{
    srand(time(NULL));
    for(int i = 0; i < mlen; i++)
    {
        int r = rand() % (126 - 64) +64;
        main[i] = (char)r;
    }
    int r = rand() % (mlen - slen + 1);
    r = r % (mlen /2) + mlen / 2;
    strncpy(sub, main + r, slen);
    printf("main string: %s\nsub string: %s\n", main, sub);
}


int main()
{
    char mains[1000] = { 0 };
    char subs[101] = { 0 };
    random_string(mains, 199, subs, 20);
    DWORD start = GetTickCount();

    for(int i = 0; i< 100000; i++)
    {
        BF(mains, subs);
    }
    printf("BF time: %ld\n", GetTickCount() - start);

    start = GetTickCount();
    for(int i = 0; i< 100000; i++)
    {
        RK(mains, subs);
    }
    printf("RK time: %ld\n", GetTickCount() - start);

    start = GetTickCount();
    for(int i = 0; i< 100000; i++)
    {
        BM(mains, subs);
    }
    printf("BM time: %ld\n", GetTickCount() - start);

    start = GetTickCount();
    for(int i = 0; i< 100000; i++)
    {
        KMP(mains, subs);
    }
    printf("RK time: %ld\n", GetTickCount() - start);

    return 0;
}
