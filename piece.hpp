#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <stdint.h>

const int Size=4;
const int NShapes=7;
const int NOrientation=4;
const int NColours=4;

/*On choisit de représenter toutes les formes de pièces avec chancune de leurs rotations pour ne pas
avoir à faire de calculs pour les faire tourner : on changera juste le 2e indice du tableau.
On a mis le point de rotation de toutes les pièces sur la 3e case de la 2e ligne. Par exemple pour une pièce I :
{{0, 0, 1, 0},    est la première rotation, et {{0, 0, 0, 0},    est la deuxième.
 {0, 0, 1, 0},                                  {1, 1, 1, 1},
 {0, 0, 1, 0},                                  {0, 0, 0, 0},
 {0, 0, 1, 0},                                  {0, 0, 0, 0},
}                                              }

NOTE : la rotation n'est pas exacte, par exemple pour la pièce 0 la rotation ne change rien pour éviter de déplacer la pièce, idem pour deux rotation d'un I.

On met les pièces dans l'ordre arbitraire "I O T L J Z S".

*/

// Study which one is faster

// std::tuple <int,int,int,int> pieceColour[NShapes] = {
//     {3,65,174,255},
//     {114,203,59,255},
//     {255,213,0,255},
//     {255,151,28,255},
//     {255,50,19,255},
//     {242,53,83,255},
//     {86,159,177,255}
// };


std::tuple <int,int,int,int> pieceColour(int shapeNumber)
{
    switch(shapeNumber){

        case 9: return {220,220,220,255}; // walls

        case 1: return {3,65,174,255};
        case 2: return {114,203,59,255};
        case 3: return {255,213,0,255};
        case 4: return {255,151,28,255};
        case 5: return {255,50,19,255};
        case 6: return {242,53,83,255};
        case 7: return {86,159,177,255};

        default: return {255,255,255,255};

    }
   
};



int pieceData[NShapes][NOrientation][Size][Size] =
//signification des accolades : Jaune = tableau global, Violet =  pièce, Bleu=shape 

{
    {   //Pièces I
        {   
            {0,0,1,0},
            {0,0,1,0}, 
            {0,0,1,0},
            {0,0,1,0},
        },    
        {   {1,1,1,1},
            {0,0,0,0}, 
            {0,0,0,0},
            {0,0,0,0},
        },       
        {   {0,0,1,0},
            {0,0,1,0}, 
            {0,0,1,0},
            {0,0,1,0},
        },    
        {   {1,1,1,1},
            {0,0,0,0}, 
            {0,0,0,0},
            {0,0,0,0},    
        }
    },

    {   //Pièces O
        {   
            {0,2,2,0},
            {0,2,2,0}, 
            {0,0,0,0},
            {0,0,0,0},
        },    
        {   {0,2,2,0},
            {0,2,2,0}, 
            {0,0,0,0},
            {0,0,0,0},
        },       
        {   {0,2,2,0},
            {0,2,2,0}, 
            {0,0,0,0},
            {0,0,0,0},
        },    
        {   {0,2,2,0},
            {0,2,2,0}, 
            {0,0,0,0},
            {0,0,0,0},    
        }
    },

    {   //Pièces T
        {   
            {3,3,3,0},
            {0,3,0,0}, 
            {0,0,0,0},
            {0,0,0,0},
        },    
        {   {0,0,3,0},
            {0,3,3,0}, 
            {0,0,3,0},
            {0,0,0,0},
        },       
        {   {0,3,0,0},
            {3,3,3,0}, 
            {0,0,0,0},
            {0,0,0,0},
        },    
        {   {0,3,0,0},
            {0,3,3,0}, 
            {0,3,0,0},
            {0,0,0,0},    
        }
    },

    {   //Pièces L
       {   
            {0,4,0,0},
            {0,4,0,0}, 
            {0,4,4,0},
            {0,0,0,0},
        },    
        {   {0,4,4,4},
            {0,4,0,0}, 
            {0,0,0,0},
            {0,0,0,0},
        },       
        {   {0,4,4,0},
            {0,0,4,0}, 
            {0,0,4,0},
            {0,0,0,0},
        },    
        {   {0,0,4,0},
            {4,4,4,0}, 
            {0,0,0,0},
            {0,0,0,0},    
        } 
    },

    {   //Pièces J
        {   
            {0,0,5,0},
            {0,0,5,0}, 
            {0,5,5,0},
            {0,0,0,0},
        },    
        {   {5,0,0,0},
            {5,5,5,0}, 
            {0,0,0,0},
            {0,0,0,0},
        },       
        {   {0,5,5,0},
            {0,5,0,0}, 
            {0,5,0,0},
            {0,0,0,0},
        },    
        {   {5,5,5,0},
            {0,0,5,0}, 
            {0,0,0,0},
            {0,0,0,0},    
        }
    },

    {   //Pièces Z
        {   
            {6,6,0,0},
            {0,6,6,0}, 
            {0,0,0,0},
            {0,0,0,0},
        },    
        {   {0,0,6,0},
            {0,6,6,0}, 
            {0,6,0,0},
            {0,0,0,0},
        },       
        {   {6,6,0,0},
            {0,6,6,0}, 
            {0,0,0,0},
            {0,0,0,0},
        },    
        {   {0,0,6,0},
            {0,6,6,0}, 
            {0,6,0,0},
            {0,0,0,0},    
        }
    },

    {   //Pièces S
        {   
            {0,7,7,0},
            {7,7,0,0}, 
            {0,0,0,0},
            {0,0,0,0},
        },    
        {   {0,7,0,0},
            {0,7,7,0}, 
            {0,0,7,0},
            {0,0,0,0},
        },       
        {   {0,7,7,0},
            {7,7,0,0}, 
            {0,0,0,0},
            {0,0,0,0},
        },    
        {   {0,7,0,0},
            {0,7,7,0}, 
            {0,0,7,0},
            {0,0,0,0},    
        }
    }
};

#endif