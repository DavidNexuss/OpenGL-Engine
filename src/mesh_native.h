#pragma once
#include "core.h"

    static const GLfloat triangle_mesh[] = {
       -1.0f, -1.0f, 0.0f, 1.0,0.0,0.0,
       1.0f, -1.0f, 0.0f,  0.0,1.0,0.0,
       0.0f,  1.0f, 0.0f,  0.0,0.0,1.0f
    };

    static const GLfloat textured_plain_mesh[] = {
       -1.0f, -1.0f, 0.0f, 1.0,0.0,0.0,  0.0,0.0,
       1.0f, -1.0f, 0.0f,  0.0,1.0,0.0,  1.0,0.0,
       0.0f,  1.0f, 0.0f,  0.0,0.0,1.0f, 0.5,0.5,
    };

    static const GLfloat cube_mesh[] = {
    
        // -Z
        -1.0,1.0,-1.0,    0.0,1.0,0.0,  0.0,1.0,
        -1.0,-1.0,-1.0,   1.0,0.0,0.0,  0.0,0.0,
        1.0,1.0,-1.0,     0.0,0.0,1.0,  1.0,1.0,
    
        1.0,-1.0,-1.0,    0.0,1.0,0.0,  1.0,0.0,
        1.0,1.0,-1.0,     0.0,0.0,1.0,  1.0,1.0,
        -1.0,-1.0,-1.0,   1.0,0.0,0.0,  0.0,0.0,
    
        // +Z
    
        -1.0,-1.0,1.0,    1.0,0.0,0.0,  1.0,1.0,
        -1.0,1.0,1.0,     0.0,1.0,0.0,  1.0,0.0,
        1.0,1.0,1.0,      0.0,0.0,1.0,  0.0,0.0,
                                                
        1.0,1.0,1.0,      0.0,0.0,1.0,  0.0,0.0,
        1.0,-1.0,1.0,     0.0,1.0,0.0,  0.0,1.0,
        -1.0,-1.0,1.0,    1.0,0.0,0.0,  1.0,1.0,
    
        // +Y
    
        -1.0,1.0,-1.0,    0.0,1.0,0.0,  0.0,1.0,
        1.0,1.0,-1.0,     0.0,0.0,1.0,  0.0,0.0,
        -1.0,1.0,1.0,     0.0,1.0,0.0,  1.0,1.0,
                                                
                                        
        -1.0,1.0,1.0,     0.0,1.0,0.0,  1.0,1.0,
        1.0,1.0,-1.0,     0.0,0.0,1.0,  0.0,0.0,
        1.0,1.0,1.0,      0.0,0.0,1.0,  1.0,0.0,        
                                                
                                                
        // -Y                           
                                        
        1.0,-1.0,-1.0,    0.0,1.0,0.0,  1.0,1.0,
        -1.0,-1.0,-1.0,   1.0,0.0,0.0,  1.0,0.0,
        -1.0,-1.0,1.0,    1.0,0.0,0.0,  0.0,0.0,
                                                
                                        
        1.0,-1.0,-1.0,    0.0,1.0,0.0,  1.0,1.0,
        -1.0,-1.0,1.0,    1.0,0.0,0.0,  0.0,0.0,
        1.0,-1.0,1.0,     0.0,1.0,0.0,  0.0,1.0,
    
        // +X
    
        1.0,1.0,-1.0,     0.0,0.0,1.0,   0.0,0.0,
        1.0,-1.0,-1.0,    0.0,1.0,0.0,   0.0,1.0,
        1.0,-1.0,1.0,     0.0,1.0,0.0,   1.0,1.0,
                                                 
        1.0,-1.0,1.0,     0.0,1.0,0.0,   1.0,1.0,
        1.0,1.0,1.0,      0.0,0.0,1.0,   1.0,0.0,
        1.0,1.0,-1.0,     0.0,0.0,1.0,   0.0,0.0,
                                                 
        // -X                                    
                                                 
        -1.0,-1.0,-1.0,    1.0,0.0,0.0,  0.0,1.0,
        -1.0,1.0,-1.0,     0.0,1.0,0.0,  0.0,0.0,
        -1.0,-1.0,1.0,     1.0,0.0,0.0,  1.0,1.0,
                                                 
        -1.0,1.0,1.0,      0.0,1.0,0.0,  1.0,0.0,
        -1.0,-1.0,1.0,     1.0,0.0,0.0,  1.0,1.0,
        -1.0,1.0,-1.0,     0.0,1.0,0.0,  0.0,0.0
    
    };
    
    static const float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
