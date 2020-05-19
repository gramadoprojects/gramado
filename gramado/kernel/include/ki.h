/*
 * File: ki.h
 * 
 *     Kernel Internal functions support.
 *     It is used as interface for internal functions.
 *
 * History:
 *     2016 - Created by Fred Nora.
 */


#ifndef ____KI_H 
#define ____KI_H


// Runtime support. 
int KiInitRuntime (void); 

// Informations support.
void KiInformation (void); 

// Requests support.   
int KiRequest (void);  

// Spawn support. Spawn a thread.
void KiSpawnTask (int id);


//ipc.
//void KiCalloutTest (void);
//void KiCallfarTest (void);


//## bugbug ##
// Isso deveria estar no módulo /pc em microkernel.

void *createCreateInitThread (void);


void *KiCreateRing0Idle (void);


// #todo: 
// Conferir se esse protótipo já está em outro lugar,
// o lugar dele é aqui.
void KiCpuFaults (unsigned long number);


// Finalizations support.
void KiAbort (void); 


#endif  



//
// End.
//

