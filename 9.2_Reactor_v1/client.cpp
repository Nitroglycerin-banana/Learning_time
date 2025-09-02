#include <cstdlib>
#include "func.h"
#include <sys/socket.h>
#include <unistd.h>






int main()
{

    int clientfd=socket(AF_INET,SOCK_STREAM,0);
    if(clientfd<0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }
    printf("clientfd:%d\n",clientfd);

    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(8000);
    serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    int ret=connect(clientfd,(const struct sockaddr*)&serveraddr,sizeof(serveraddr));
    if(ret<0)
    {
        perror("connect");
        close(clientfd);
        return EXIT_FAILURE;
    }
    printf("connect success.\n");
    fd_set readfds;
    char buff[1000]={0};
    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO,&readfds);
        FD_SET(clientfd,&readfds);
        int ret=select(clientfd+1,&readfds,NULL,NULL,NULL);
        printf(">> %d fd ready.\n",ret);

        if(FD_ISSET(STDIN_FILENO,&readfds))
        {
            memset(buff,0,sizeof(buff));
            ret=read(STDIN_FILENO,buff,sizeof(buff));
            printf("\nread %d bytes from stdin.\n",ret);

            ret=send(clientfd,buff,strlen(buff),0);
            printf("\nsend %d bytes.\n",ret);
        }
        if(FD_ISSET(clientfd,&readfds))
        {
            memset(buff,0,sizeof(buff));
            ret=recv(clientfd,buff,sizeof(buff),0);
            if(ret==0)
            {
                printf("\nserver closed.\n");
                close(clientfd);
                break;
            }
            printf("\nrecv %d bytes,msg:%s\n",ret,buff);
        }
    }
    close(clientfd);
    return 0;
}

