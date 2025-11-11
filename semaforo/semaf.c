#include <stdio.h>
#include <windows.h>

HANDLE semaforo;

DWORD WINAPI carro(LPVOID param)
{
    int id = *(int *)param;

    printf("Carro %d chegou ao semáforo\n", id);
    WaitForSingleObject(semaforo, INFINITE); // espera sinal verde
    printf("Carro %d passou no sinal\n", id);
    Sleep(1000);
    printf("Carro %d saiu do cruzamento\n", id);
    ReleaseSemaphore(semaforo, 1, NULL); // libera o semáforo
    return 0;
}

int main()
{
    HANDLE threads[5];
    int ids[5];

    semaforo = CreateSemaphore(NULL, 2, 2, NULL); // 2 carros por vez

    for (int i = 0; i < 5; i++)
    {
        ids[i] = i + 1;
        threads[i] = CreateThread(NULL, 0, carro, &ids[i], 0, NULL);
    }

    WaitForMultipleObjects(5, threads, TRUE, INFINITE);
    CloseHandle(semaforo);
    return 0;
}
