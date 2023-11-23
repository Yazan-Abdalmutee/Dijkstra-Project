#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define INFINITY 9999
typedef struct node
{
    char first[20];
    char last[10];
    char name[50];
    char dist[10];
} str;
//give initial value for number of vertices
int MAX=0;
int fpeek(FILE *in);
void Readfile(FILE *in,str s[]);
void insertToGraph(str s[],int graph[MAX][MAX], char *ptr[][MAX]);
void inlize(char *ptr[][MAX],int graph[][MAX]);
void dijkstra(int G[MAX][MAX],int n,int startnode,int aim,char* ptr[][MAX],FILE *out);
int finaMax(str s[]);
void checkNodes(str s[],int node1,int node2);
int checkFound=0;
void menu();
int main()
{
    int check=0;
    int from,to;
    FILE *in,*out;
    in=fopen("segments.txt","r");
    out=fopen("route.data.","w");
    str s[500];
    int  time=0,time2=0;
    Readfile(in, s);
    MAX=finaMax(s)+1;//+1 to pass it at array.. array index start from 0

    char *names[MAX][MAX];
    int graph[MAX][MAX];

    int choice=0;
    while(1)
    {
        menu();
        scanf("%d",&choice);
        switch(choice)
        {
        case 1:
        {
            //if file did not read yet
            if(time==0)
            {
                //if fill not found
                if(in==NULL)
                {
                    printf("FILE NOT FOUND");
                }
                else
                {
                    Readfile(in, s);
                    time++;
                    inlize(names, graph);
                    insertToGraph( s, graph,names);
                }
            }
            puts("\n\nOPEARTION NUMBER 1 DONE");
            puts("============================================================================================\n");

            break;
        }
        case 2:
        {
            //if file did not read yet

            if(time==0)
            {
                puts("\nu have to read the file -operation number 1-\n");
            }
            else
            {
                while(1)
                {

                    while(1)
                    {
                        printf("\nEnter The First id for node 1 (source) :\n");
                        scanf("%d",&from);
                        printf("\nEnter The second id for node 2 (destination)  :\n");
                        scanf("%d",&to);
                        // node 1 and node 2 should be postive values
                        if(from>=0&&to>=0)
                        {
                            break;

                        }
                        else
                        {
                            puts("\nNodes id should be positive, try again :-\n");
                        }
                    }
                    //if nodes not found
                    checkNodes( s, from, to);
                    //if nodes found
                    if (checkFound==0)
                    {
                        time2++;
                        dijkstra(graph,MAX, from,to,names,out);
                    }
                    checkFound=0;
                    puts("\n!-Enter any number to continue in this case , -1 to exit from this case:-");
                    scanf("%d",&check);
                    if(check==-1)
                        break;

                }

                puts("\nOPEARTION NUMBER 2 DONE");
            }
            puts("============================================================================================\n");

            break;
        }
        case 3:
        {
            if(time2==0)
            {
                printf("\nU have to perform operation two before choose this one\n") ;
            }
            else
            {
                fclose(out);

                puts("\nThe information for nodes travel at output file\n");

                puts("\nOPEARTION NUMBER 3 DONE\n");
            }

            puts("============================================================================================\n");

            break;
        }
        case 4 :
        {
            puts("\nEXIT DONE\n");
            exit(0);
            break;
        }
        default:
        {
            puts("\nWrong number try again (1-4)\n");
            puts("============================================================================================\n");

        }

        }

    }

    return 0;
}
//insert the values from struct to the graph array
void insertToGraph(str s[],int graph[MAX][MAX], char *ptr[][MAX])
{
    int i=0;
    int from,next,dis;
    while(s[i].first[0]!='\0')
    {
        from=atoi(s[i].first);
        next=atoi(s[i].last);
        dis=atoi(s[i].dist);
        graph[from][next] = dis;
        graph[next][from] = dis;
        ptr[from][next]=s[i].name;
        ptr[next][from]=s[i].name;

        i++;
    }

}
//Dijkstra function to find and print short path
void dijkstra(int G[MAX][MAX],int n,int startnode,int aim,char* ptr[][MAX],FILE *out)
{
    int x[MAX];
    for(int i=0; i<MAX; i++)
    {
        x[i]=-1;
    }
    int cost[MAX][MAX],distance[MAX],pred[MAX];
    int visited[MAX],count,mindistance,nextnode,i,j;

    //pred[] stores the predecessor of each node
    //count gives the number of nodes seen

    //create the cost matrix
    for(i=0; i<n; i++)
        for(j=0; j<n; j++)
            if(G[i][j]==0)
                cost[i][j]=INFINITY;
            else
                cost[i][j]=G[i][j];

    for(i=0; i<n; i++)
    {
        distance[i]=cost[startnode][i];
        pred[i]=startnode;
        visited[i]=0;
    }

    distance[startnode]=0;
    visited[startnode]=1;
    count=1;

    while(count<n-1)
    {
        mindistance=INFINITY;

        //nextnode gives the node at minimum distance
        for(i=0; i<n; i++)
        {


            if(distance[i]<mindistance&&!visited[i])
            {
                mindistance=distance[i];
                nextnode=i;
            }
        }
        //check if a better path exists through nextnode
        visited[nextnode]=1;
        for(i=0; i<n; i++)
            if(!visited[i])
                if(mindistance+cost[nextnode][i]<distance[i])
                {
                    distance[i]=mindistance+cost[nextnode][i];
                    pred[i]=nextnode;
                }
        count++;
    }

    //if node 2 !=node 1
    if(aim!=startnode)
    {
        puts("\n===================================================================\n");
        fprintf(out,"\n===================================================================\n");
        fprintf(out,"Information to travel from node %d to node %d are :-\n",startnode,aim);
        printf("\nShort Distance from node %d to node %d = %d KM",startnode,aim,distance[aim]);
        fprintf(out,"\nShort Distance from node %d to node %d = %d KM",startnode,aim,distance[aim]);
        fprintf(out,"\n");
        j=aim;
        int y=MAX-1;
        x[y]=aim;
        do
        {
            j=pred[j];
            x[y-1]=j;
            y--;
        }
        while(j!=startnode);
        puts("");
        printf("\nPath (nodes) = \n\n");
        fprintf(out,"\nPath (nodes) = \n\n");
        //print the value of shortest path
        for(int i=0; i<MAX; i++)
        {
            if(x[i]>-1)
            {
                if(i==MAX-1)
                {
                    printf("%d",x[i]);
                    fprintf(out,"%d",x[i]);

                }
                else
                {
                    printf("%d -> ",x[i]);
                    fprintf(out,"%d -> ",x[i]);


                }
            }

        }
        puts("\n");
        fprintf(out,"\n\n");
        printf("Path (cites-towns) = \n\n");
        fprintf(out,"Path (cites-towns) = \n\n");

//print the shortest path at console and file
        for(int i=0; i<MAX-1; i++)
        {
            if(x[i]>-1)
            {
                if(i==MAX-2)
                {
                    printf("%s",ptr[x[i]][x[i+1]] );
                    fprintf(out,"%s",ptr[x[i]][x[i+1]] );


                }

                else
                {
                    printf("%s -> ",ptr[x[i]][x[i+1]] );
                    fprintf(out,"%s -> ",ptr[x[i]][x[i+1]] );


                }

            }
        }
        puts("\n\n===================================================================\n");
        fprintf(out,"\n===================================================================\n");

        //printf("hi");

    }
    //IF NODE 1 =NODE 2
    else
    {
        puts("\nu are at this position so distance =0 KM");
    }


}
//read the file
void Readfile(FILE *in,str s[])
{
    fseek(in,0,SEEK_SET);

    char c;
    c=getc(in);

    int i=0,j=0;

    while(!feof(in))
    {
        j=0;
        //for node 1
        while(c>='0'&&c<='9')
        {

            s[i].first[j]=c;
            j++;
            c=getc(in);
        }


        j=0;
        while(c==' ')
        {
            c=getc(in);
        }
        //for node 2
        while(c>='0'&&c<='9')
        {
            s[i].last[j]=c;
            j++;
            c=getc(in);

        }

        while(c==' ')
        {
            c=getc(in);
        }

        j=0;
        //for city
        while(c>'9'||c<'0')
        {
            s[i].name[j]=c;
            j++;
            c=getc(in);
            if(fpeek(in)<='9'&&fpeek(in)>='0')
            {
                c=getc(in);
            }
        }
        j=0;
        while(c==' ')
        {
            c=getc(in);
        }
        //for distance
        while(c>='0'&&c<='9')
        {
            s[i].dist[j]=c;
            j++;
            c=getc(in);
        }

        while(c=='\n')
        {
            c=getc(in);
        }

        i++;
        j=0;

    }

    i=0;
}
void inlize( char *ptr[][MAX],int graph[][MAX])
{
    for(int i=0; i<MAX; i++)
    {
        for(int j=0; j<MAX; j++)
        {
            ptr[i][j]=" ";

        }
    }

}
//menu
void menu()
{
    printf("1-Read the file \"segments.txt\" and load the data\n");
    printf("2-Enter 2 points to compute the shortest path between them\n");
    printf("3-Print the route of the shortest distance to a file called \"route.txt\"\n");
    printf("4-Exit\n");
    puts("\nEnter the number of operation u want:-");

}
//function to check next char without change the pointer
int fpeek(FILE *in)
{
    int c;
    c = fgetc(in);
    ungetc(c, in);
    return c;
}
//get the max number of vertices in the fill
int finaMax(str s[])
{
    int i=0;
    int from,next,max;
    while(s[i].first[0]!='\0')
    {
        from=atoi(s[i].first);
        next=atoi(s[i].last);
        if(from>max)
        {
            max=from;
        }
        if(next>max)
        {
            max=next;
        }
        i++;
    }
    return max;

}
//function to check if the nodes id is EXSIT or not
void checkNodes(str s[],int node1,int node2)
{
    int i=0;
    int from,next;
    int c1=0;
    int c2=0;
    while(s[i].first[0]!='\0')
    {
        from=atoi(s[i].first);
        next=atoi(s[i].last);
        if(node1==from||node1==next)
        {
            c1++;
        }
        if(node2==from||node2==next)
        {
            c2++;
        }
        i++;
    }
    if(c1==0)
    {
        printf("\nThe node %d not Found \n",node1);
        checkFound++;
    }
    if(c2==0)
    {
        printf("\nThe node %d not Found \n",node2);
        checkFound++;
    }


}



