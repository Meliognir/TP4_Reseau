//
// Created by trias on 24/10/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include "segdef.h" // Inclut la structure SEGMENT et les constantes


int main() {
    int shmid, semid;
    segment *seg;

    // Initialisation du générateur de nombres aléatoires
    init_rand();

    printf("Tentative de récupération du segment de mémoire et des sémaphores avec clé: %d\n", cle);

    if ((shmid = shmget(cle, 0, 0)) == -1) {
        perror("Erreur lors de la récupération du segment mémoire");
        exit(1);
    }
    printf("Segment mémoire récupéré avec succès. ID: %d\n", shmid);

    if ((semid = semget(cle, 3, 0)) == -1) {
        perror("Erreur lors de la récupération des sémaphores");
        exit(1);
    }
    printf("Sémaphores récupérés avec succès. ID: %d\n", semid);

    if ((seg = (segment *)shmat(shmid, NULL, 0666)) == (segment *)-1) {
        perror("Erreur lors de l'attachement du segment mémoire");
        exit(1);
    }

    for (int req = 0; req < 10; req++) {
        wait_sem(semid, seg_dispo);

        seg->pid = getpid();
        seg->req = req;
        for (int i = 0; i < maxval; i++) {
            seg->tab[i] = getrand();
        }

        lib_sem(semid, seg_init);

        wait_sem(semid, res_ok); // Attente du résultat calculé par le serveur

        printf("Requête %d : Résultat calculé par le serveur : %ld\n", req, seg->result);

        lib_sem(semid, seg_init); // Libérer seg_init pour indiquer que le résultat a été lu
        lib_sem(semid, res_ok);   // Libérer res_ok pour le serveur
        lib_sem(semid, seg_dispo); // Libérer seg_dispo pour le serveur

        sleep(1);
    }


    if (shmdt(seg) == -1) {
        perror("Erreur lors du détachement du segment mémoire");
        exit(1);
    }

    return 0;
}