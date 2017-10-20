.globl write
write:
    mov    x8, #64
    svc    #0
    ret

.globl read
read:
    mov    x8, #63
    svc    #0
    ret
    
.globl malloc 
malloc: 
    mov    x8, #20
    mov    x1, #1
    svc    #0
    ret

.globl free 
free: 
    mov    x8, #21
    svc    #0
    ret
    
.globl pid
pid:
    mrs    x0, TPIDR_EL0
    ret

.globl thread_create
thread_create:
    dsb st
    mov    x8, #220
    svc    #0
    ret

.globl thread_join
thread_join:
    dsb st
    mov    x8, #221
    svc    #0
    ret

.globl EXIT
EXIT:
    mov    x8, #93
    svc    #0
    ret
  
.globl process_create 
process_create:
    dsb st
    mov    x8, #223
    svc    #0
    ret
    