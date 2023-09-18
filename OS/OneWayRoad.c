#include <windows.h>
#include <stdio.h>

#define N 100
#define TRUE 1
typedef int Semaphore;
Semaphore A = 0, B = 0;
HANDLE Go_mutex,Come_mutex;
HANDLE A_mutex,B_mutex;
HANDLE mutex;

void down(HANDLE handle) {
    WaitForSingleObject(handle, INFINITE);
}

void up(HANDLE handle) {
    ReleaseSemaphore(handle, 1, NULL);
}

DWORD WINAPI Come(LPVOID v) {

    while(TRUE) {

        down(Come_mutex);

        down(A_mutex);
        A = A+1;
        if(A == 1) {
            down(Go_mutex);
            printf("                    <<<=====��ʼ�Զ�����\n");
        }
        up(A_mutex);

        up(Come_mutex);

        down(mutex);
        //�Զ�����ͨ����·��
        printf("                    <<<=====��%s����\n",(char *)v);
        printf("         END        <<<=====��%s����\n",(char *)v);
        up(mutex);

        down(A_mutex);
        A = A-1;
        if(A == 0) {
            up(Go_mutex);
            printf("                    �Զ����������г�����ʻ���\n");
        }
        up(A_mutex);

        Sleep(2000);
    }
    return 1;
}

DWORD WINAPI Go(LPVOID v) {

    while(TRUE) {

        down(Go_mutex);

        down(B_mutex);
        B = B+1;
        if(B == 1) {
            down(Come_mutex);
            printf("��ʼ������====>\n");
        }
        up(B_mutex);

        up(Go_mutex);

        down(mutex);
        //������ͨ����·��
        printf("��%s����=====>>>\n",(char *)v);
        printf("��%s����=====>>>     END\n",(char *)v);
        up(mutex);

        down(B_mutex);
        B = B-1;
        if(B == 0) {
            up(Come_mutex);
            printf("�����򶫵����г�����ʻ���\n");
        }
        up(B_mutex);

        Sleep(2000);
    }
    return 1;
}

int main()
{
    DWORD Tid;
    char AThread[12][10];
    char BThread[12][10];

    mutex      = CreateSemaphore(NULL, 1, 1, NULL);
    A_mutex    = CreateSemaphore(NULL, 1, 1, NULL);
    B_mutex    = CreateSemaphore(NULL, 1, 1, NULL);
    Go_mutex   = CreateSemaphore(NULL, 1, 1, NULL);
    Come_mutex = CreateSemaphore(NULL, 1, 1, NULL);

    for(int i=0;i<4;i++) {
        AThread[i][0] = i+1+'0';
        AThread[i][1] = '\0';
        CreateThread(NULL,0,Come,AThread[i],0,&Tid);
    }

    for(int i=4;i<8;i++) {
        BThread[i][0] = i+1+'0';
        BThread[i][1] = '\0';
        CreateThread(NULL,0,Go,BThread[i],0,&Tid);
    }

    Sleep(2000);
    system("pause");
    return 0;
}