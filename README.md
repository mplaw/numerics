# Numerics

A small library of basic data types and helpful functions/macros, which are primarily aimed towards numerical computation.
## Data types
```
    bound     : a number that's bound between a minimum and a maximum (min <= num <= max)
  
    cyclic    : a number that cycles between a minimum and a maximum, so that min <= num <= max. 
    
    fraction  : elementary fraction, e.g. 5/3, stored as a numerator and a denominator. 
  
    number    : a normal number, that also keeps track of it's uncertainty and units.
    
    unit      : a physical compound unit, e.g. kg m^3 s^-1.
```
## Helpful functions/macros
```
    constants : a list of mathematical and physical constants (as number objects).
    
    macros    : debug and compiler-independent optimisations macros.
    
    typedefs  : small set of standard typedefs used throughout the other files.
```


Author : Matt Law
