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

// We define the colours of the respective pieces over here
uint8_t piecesColours[NShapes][NColours] = {
    {3,65,174,255},
    {114,203,59,255},
    {255,213,0,255},
    {255,151,28,255},
    {255,50,19,255},
    {242,53,83,255},
    {86,159,177,255}
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
        {   {0,0,0,0},
            {1,1,1,1}, 
            {0,0,0,0},
            {0,0,0,0},
        },       
        {   {0,0,1,0},
            {0,0,1,0}, 
            {0,0,1,0},
            {0,0,1,0},
        },    
        {   {0,0,0,0},
            {1,1,1,1}, 
            {0,0,0,0},
            {0,0,0,0},    
        }
    },

    {  //Pièces O
        {   
            {0,0,0,0},
            {0,1,1,0}, 
            {0,1,1,0},
            {0,0,0,0},
        },    
        {   {0,0,0,0},
            {0,1,1,0}, 
            {0,1,1,0},
            {0,0,0,0},
        },       
        {   {0,0,0,0},
            {0,1,1,0}, 
            {0,1,1,0},
            {0,0,0,0},
        },    
        {   {0,0,0,0},
            {0,1,1,0}, 
            {0,1,1,0},
            {0,0,0,0},    
        }
    },

    {   //Pièces T
        {   
            {0,0,0,0},
            {0,1,1,1}, 
            {0,0,1,0},
            {0,0,0,0},
        },    
        {   {0,0,1,0},
            {0,1,1,0}, 
            {0,0,1,0},
            {0,0,0,0},
        },       
        {   {0,0,1,0},
            {0,1,1,1}, 
            {0,0,0,0},
            {0,0,0,0},
        },    
        {   {0,0,1,0},
            {0,0,1,1}, 
            {0,0,1,0},
            {0,0,0,0},    
        }
    },

    {   //Pièces L
       {   
            {0,0,1,0},
            {0,0,1,0}, 
            {0,0,1,1},
            {0,0,0,0},
        },    
        {   {0,0,0,0},
            {0,1,1,1}, 
            {0,1,0,0},
            {0,0,0,0},
        },       
        {   {0,1,1,0},
            {0,0,1,0}, 
            {0,0,1,0},
            {0,0,0,0},
        },    
        {   {0,0,0,1},
            {0,1,1,1}, 
            {0,0,0,0},
            {0,0,0,0},    
        } 
    },

    {   //Pièces J
        {   
            {0,0,1,0},
            {0,0,1,0}, 
            {0,1,1,0},
            {0,0,0,0},
        },    
        {   {0,1,0,0},
            {0,1,1,1}, 
            {0,0,0,0},
            {0,0,0,0},
        },       
        {   {0,0,1,1},
            {0,0,1,0}, 
            {0,0,1,0},
            {0,0,0,0},
        },    
        {   {0,0,0,0},
            {0,1,1,1}, 
            {0,0,0,1},
            {0,0,0,0},    
        }
    },

    {   //Pièces Z
        {   
            {0,0,0,0},
            {0,1,1,0}, 
            {0,0,1,1},
            {0,0,0,0},
        },    
        {   {0,0,1,0},
            {0,1,1,0}, 
            {0,1,0,0},
            {0,0,0,0},
        },       
        {   {0,0,0,0},
            {0,1,1,0}, 
            {0,0,1,1},
            {0,0,0,0},
        },    
        {   {0,0,1,0},
            {0,1,1,0}, 
            {0,1,0,0},
            {0,0,0,0},    
        }
    },

    {   //Pièces S
        {   
            {0,0,0,0},
            {0,0,1,1}, 
            {0,1,1,0},
            {0,0,0,0},
        },    
        {   {0,1,0,0},
            {0,1,1,0}, 
            {0,0,1,0},
            {0,0,0,0},
        },       
        {   {0,0,0,0},
            {0,0,1,1}, 
            {0,1,1,0},
            {0,0,0,0},
        },    
        {   {0,1,0,0},
            {0,1,1,0}, 
            {0,0,1,0},
            {0,0,0,0},    
        }
    }
};

#endif