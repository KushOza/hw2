<<<<<<< HEAD
#ifndef SPINLOCK_H 
#define SPINLOCK_H 

=======
>>>>>>> 4ea29fc72969f8654b03e3daf1a041f777962f12
// Mutual exclusion lock.
struct spinlock {
  uint locked;       // Is the lock held?
  
  // For debugging:
  char *name;        // Name of lock.
  struct cpu *cpu;   // The cpu holding the lock.
  uint pcs[10];      // The call stack (an array of program counters)
                     // that locked the lock.
};

<<<<<<< HEAD
#endif
=======
>>>>>>> 4ea29fc72969f8654b03e3daf1a041f777962f12
