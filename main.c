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

    // ---------------------------------------------
    // ---------------  STEP 0  --------------------
    // ---------------------------------------------

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


    if ((seg = (segment *)shmat(shmid, NULL, 0)) == (segment *)-1) {
        perror("Erreur lors de l'attachement du segment mémoire");
        exit(1);
    }

    for (int req = 0; req < 100; req++) {

    // ---------------------------------------------
    // ---------------  STEP 1  --------------------
    // ---------------------------------------------

        acq_sem(semid, seg_dispo);
        //wait_sem(semid, seg_dispo);

    // ---------------------------------------------
    // ---------------  STEP 2  --------------------
    // ---------------------------------------------

        seg->pid = getpid();
        seg->req = req;
        long clientRes = 0;
        long value;
        for (int i = 0; i < maxval; i++) {
            value = getrand()%1000;
            seg->tab[i] = value;
            clientRes += value;
        }
        clientRes = clientRes / maxval;

    // ---------------------------------------------
    // ---------------  STEP 3  --------------------
    // ---------------------------------------------

        acq_sem(semid, seg_init);
    
    // ---------------------------------------------
    // ---------------  STEP 4  --------------------
    // ---------------------------------------------
        
        wait_sem(semid, res_ok); // Attente du résultat calculé par le serveur
        long serverRes = seg->result;
        lib_sem(semid, seg_init);

    // ---------------------------------------------
    // ---------------  STEP 5  --------------------
    // ---------------------------------------------
        
        acq_sem(semid, res_ok);
        lib_sem(semid, res_ok);   // Libérer res_ok pour le serveur

    // ---------------------------------------------
    // ---------------  STEP 6  --------------------
    // ---------------------------------------------
        
        lib_sem(semid, seg_dispo); // Libérer seg_dispo pour le serveur

    // ---------------------------------------------
    // ---------------  STEP 7  --------------------
    // ---------------------------------------------
        
        printf("Requête %d : Résultat calculé par le serveur : %ld\n", req, serverRes);
        printf("Requête %d : Résultat calculé par le client : %ld\n", req, clientRes);


    // ---------------------------------------------
    // ---------------  STEP 8  --------------------
    // ---------------------------------------------

        sleep(1); // Attend la libération de res_ok
    }


    if (shmdt(seg) == -1) {
        perror("Erreur lors du détachement du segment mémoire");
        exit(1);
    }

    return 0;
}