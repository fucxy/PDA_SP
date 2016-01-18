# PDA_SP
This is our sequence pair implentation for Physical design automation in NCTU.

We follow the paper "Implement : Rectangle Packing Based Module Placement - Sequence Pair" to make it.

# How We Make It

We will decribe how we made it!

## From a floorplan to its sequence pair

>Stretch modules one by one to obtain rooms, each room containing only one module.

![alt tag](https://raw.githubusercontent.com/fucxy/PDA_SP/master/image/sp.png)

## Positive / Negative loci

>For each module i, we draw two rectilinear curves, right-up locus and left-down locus.
>The union of the two loci of module i forms the positive locus of module i. 
>Negative loci can be obtained similar to the positive loci.
>The difference is that a negative locus is the union of the up-left locus and down-right locus.
> 
>(Г+ , Г-) = (124536, 326145)

![alt tag](https://raw.githubusercontent.com/fucxy/PDA_SP/master/image/sp1.png)


##From a sequence pair to its floorplan

>(Г+ , Г-) = (124536, 326145)

![alt tag](https://raw.githubusercontent.com/fucxy/PDA_SP/master/image/sp2.png)

>Rule 1 (horizontal constraint): module i is left to module j if i appears before j in both Г+ & Г- (...i... j ... , ...i... j ...). 
>Rule 2 (vertical constraint): module i is below module j if i appears after j in Г+ and i appears before j in Г- (...j... i ... , ...i... j ...).

#Example :(Г+ , Г-) = (124536, 326145)

![alt tag](https://raw.githubusercontent.com/fucxy/PDA_SP/master/image/sp3.png)

##Longest path

>Floorplan width = 17. (s->3->6->t)
>Floorplan height  = 17. (s->6->5->t)

![alt tag](https://raw.githubusercontent.com/fucxy/PDA_SP/master/image/sp3.png)

![alt tag](https://raw.githubusercontent.com/fucxy/PDA_SP/master/image/sp4.png)

##Operations

>Op1: Rotate a module. 
>Op2: Swap two module names in only one sequence. 
>Op3: Swap two module names in both sequences.

![alt tag](https://raw.githubusercontent.com/fucxy/PDA_SP/master/image/sp5.png)

##Comparison

![alt tag](https://raw.githubusercontent.com/fucxy/PDA_SP/master/image/sp6.png)

##Reference

>Murata, et. al, “Rectangle-packing based module placement,” ICCAD-95.

# Version 1.0

# Bug
