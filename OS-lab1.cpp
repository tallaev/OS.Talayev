﻿#include <iostream>
#include <pthread.h>
#include <unistd.h>

pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int ready = 0;

void* producer(void*) {
    while (true) {
        pthread_mutex_lock(&lock);
        if (ready == 1) {
            pthread_mutex_unlock(&lock);
            continue;
        }
        ready = 1;
        std::cout << "Поставщик: отправляю событие" << std::endl;
        pthread_cond_signal(&cond1);
        pthread_mutex_unlock(&lock);
        sleep(1); // Задержка в 1 секунду
    }
    return nullptr;
}

void* consumer(void*) {
    while (true) {
        pthread_mutex_lock(&lock);
        while (ready == 0) {
            pthread_cond_wait(&cond1, &lock);
        }
        ready = 0;
        std::cout << "Потребитель: получено событие" << std::endl;
        pthread_mutex_unlock(&lock);
    }
    return nullptr;
}

int main() {
    pthread_t producerThread, consumerThread;

    // Запускаем потоки
    pthread_create(&producerThread, nullptr, producer, nullptr);
    pthread_create(&consumerThread, nullptr, consumer, nullptr);

    // Ожидаем завершения потоков (хотя в данном случае они выполняются в бесконечном цикле)
    pthread_join(producerThread, nullptr);
    pthread_join(consumerThread, nullptr);

    return 0;
}
