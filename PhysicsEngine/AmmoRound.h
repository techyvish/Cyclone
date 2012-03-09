//
//  AmmoRound.h
//  PhysicsEngine
//
//  Created by Vishal Patel on 11/06/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "Geometry.hpp"
#include "particle.h"

struct AmmoRound {
    
    Cyclone::Particle particle;
    
    
    void Render() {
        
        Cyclone::Vector3 position;
        particle.getPosition(&position);
        
    
        glColor4f(0,0,0,1);
        glPushMatrix();
        glTranslatef(position.x,position.y,position.z);
        glutSolidSphere(0.3,20,16);
        glPopMatrix();
        
        
        glColor4f(0.75,0.75,0.75,1);
        glPushMatrix();
        glTranslatef(position.x,0.0,position.z);
        glScalef(1.0f, 0.1f, 1.0f);
        glutSolidSphere(0.2f,20,16);
        glPopMatrix();
        
    }
    
};