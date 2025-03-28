#include "pcbmanager.h"
#include "system.h"


PCBManager::PCBManager(int maxProcesses) {

    bitmap = new BitMap(maxProcesses);
    pcbs = new PCB*[maxProcesses];

    for(int i = 0; i < maxProcesses; i++) {
        pcbs[i] = NULL;
    }

}


PCBManager::~PCBManager() {

    delete bitmap;
    delete pcbs;

}


PCB* PCBManager::AllocatePCB() {

    // Aquire pcbManagerLock
    pcbManagerLock -> Acquire();
    int pid = bitmap->Find();
    pcbManagerLock->Release();
    // Release pcbManagerLock
    ASSERT(pid != -1);
    pcbs[pid] = new PCB(pid);
    return pcbs[pid];

}


int PCBManager::DeallocatePCB(PCB* pcb) {

    // Check is pcb is valid -- check pcbs for pcb->pid
    if (!(pcb->pid >= 0 && pcbs[pcb->pid] != nullptr)) {
        return -1;
    }

     // Aquire pcbManagerLock
     pcbManagerLock->Acquire();

    bitmap->Clear(pcb->pid);

    // Release pcbManagerLock
    pcbManagerLock->Release();

    int pid = pcb->pid;

    delete pcbs[pid];

    pcbs[pid] = NULL;

    return 0;

}

PCB* PCBManager::GetPCB(int pid) {
    return pcbs[pid];
}