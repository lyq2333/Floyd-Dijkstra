#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<windows.h>
typedef struct node1
{
	int w;
	int num;
	struct node1* next;
}*node,node2;
typedef struct graphnode
{
	int father;
	int d;
	node next;
}graphvertex;
int h[257];
int versize=8 , size;
FILE* fp,*fp1;
bool BF(graphvertex* vertex, int k);
void DIJKSTRA(graphvertex* vertex, int k);
void main()
{	
	int i,a,b,w,k;
	double f;
	char string[100], string1[100], string2[100];
	node p;
	graphvertex vertex[257];
	LARGE_INTEGER  fre, start, fin;
	for (k = 0; k < 6; k++)
	{
		switch (versize)
		{
		case 8:strcpy(string, "../input/size1/input.txt");
			strcpy(string1, "../output/size1/output2.txt");
			strcpy(string2, "../output/size1/time2.txt");
			break;
		case 16:strcpy(string, "../input/size2/input.txt");
			strcpy(string1, "../output/size2/output2.txt");
			strcpy(string2, "../output/size2/time2.txt");
			break;
		case 32:strcpy(string, "../input/size3/input.txt");
			strcpy(string1, "../output/size3/output2.txt");
			strcpy(string2, "../output/size3/time2.txt");
			break;
		case 64:strcpy(string, "../input/size4/input.txt");
			strcpy(string1, "../output/size4/output2.txt");
			strcpy(string2, "../output/size4/time2.txt");
			break;
		case 128:strcpy(string, "../input/size5/input.txt");
			strcpy(string1, "../output/size5/output2.txt");
			strcpy(string2, "../output/size5/time2.txt");
			break;
		case 256:strcpy(string, "../input/size6/input.txt");
			strcpy(string1, "../output/size6/output2.txt");
			strcpy(string2, "../output/size6/time2.txt");
			break;
		}
		i = versize;
		size = 0;
		while (i != 1)
		{
			i >>= 1;
			size++;
		}
		size *= versize;
		for (i = 0; i <= versize; i++)
		{
			vertex[i].next = NULL;
		}
		if ((fp = fopen(string, "r")) == NULL)
			printf("error");
		for (i = 0; i < size; i++)
		{
			fscanf(fp, "%d,%d %d", &b, &a, &w);
			p = (node)malloc(sizeof(node2));
			p->w = w;
			p->num = a;
			p->next = vertex[b].next;
			vertex[b].next = p;
		}
		if ((fp = fopen(string1, "w")) == NULL)
			printf("error");
		QueryPerformanceFrequency(&fre);
		QueryPerformanceCounter(&start);
		for (i = 0; i < versize; i++)
		{
			p = (node)malloc(sizeof(node2));
			p->w = 0;
			p->num = i;
			p->next = vertex[versize].next;
			vertex[versize].next = p;
		}
		if (BF(vertex, versize) == false)
			printf("the input graph contains a negative-weight cycle");
		for (i = 0; i <= versize; i++)
		{
			p = vertex[i].next;
			while (p != NULL)
			{
				p->w = p->w + h[i] - h[p->num];
				p = p->next;
			}
		}
		for (i = 0; i < versize; i++)
		{	
			fprintf(fp, "From vertex %d:\n", i);
			DIJKSTRA(vertex, i);
		}
		QueryPerformanceCounter(&fin);
		fp1 = fopen(string2, "w");
		f = (double)(fin.QuadPart - start.QuadPart) / (double)fre.QuadPart;
		fprintf(fp1, "time for size %d:%.0fus\n", k + 1, f*pow(10.0, 6));
		fclose(fp1);
		fclose(fp);
		versize *= 2;
	}
}
bool BF(graphvertex* vertex, int k)
{
	int i,j;
	node p;
	for (i = 0; i < versize; i++)
	{
		vertex[i].d = 2*size * versize;
		vertex[i].father = -1;
	}
	vertex[versize].d = 0;
	for (i = 0; i < versize; i++)
	{
		for (j = versize; j >= 0; j--)
		{
			p = vertex[j].next;
			while (p != NULL)
			{
				if (vertex[p->num].d > vertex[j].d + p->w)
				{
					vertex[p->num].d = vertex[j].d + p->w;
					vertex[p->num].father = j;
				}
				p = p->next;
			}
		}
	}
	for (i = 0; i <= versize; i++)
	{
		p = vertex[i].next;
		while (p != NULL)
		{
			if (vertex[p->num].d > vertex[i].d + p->w)
			{
				printf("%d %d ", p->num, i);
				return false;
			}
			p = p->next;
		}
	}
	for (i = 0; i <= versize; i++)
	{
		h[i] = vertex[i].d;
	}
	return true;
}
void DIJKSTRA(graphvertex* vertex, int k)
{
	int i, q[256],min,j,position,count;
	node p;
	for (i = 0; i < versize; i++)
	{
		vertex[i].d = size * versize;
		vertex[i].father = -1;
		q[i] = i;
	}
	vertex[k].d = 0;
	for (i = 0; i < versize; i++)
	{
		min = q[i];
		position = i;
		for (j = i+1; j < versize; j++)
		{
			if (vertex[q[j]].d < vertex[min].d)
			{
				min = q[j];
				position = j;
			}
		}
		p = vertex[min].next;
		while (p)
		{
			if (vertex[p->num].d > vertex[min].d + p->w)
			{
				vertex[p->num].d = vertex[min].d + p->w;
				vertex[p->num].father = min;
			}
			p = p->next;
		}
		j = q[position];
		q[position] = q[i];
		q[i] = j;
	}
	for (i = 0; i < versize; i++)
	{
		vertex[i].d = vertex[i].d + h[i] - h[k];
	}
	for (i = 0; i < versize; i++)
	{
		if (i != k)
		{
			count = 0;
			j = i;
			while (j != k )
			{
				q[count++] = vertex[j].father;
				j = vertex[j].father;
				if (j == -1) break;
			}
			count--;
			if (q[count] != -1)
			{
				fprintf(fp, "To vertex %d:%d (%d", i, vertex[i].d, q[count]);
				count--;
				while (count >= 0)
				{
					fprintf(fp, ",%d", q[count--]);
				}
				fprintf(fp, ",%d)\n",i);
			}
			else fprintf(fp, "To vertex %d:can not arrived", i);
		}
	}
}

