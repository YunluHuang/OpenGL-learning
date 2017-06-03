//
//  Control.hpp
//  CSE163-final
//
//  Created by Ah Huang on 6/2/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#ifndef Control_hpp
#define Control_hpp

#include <stdio.h>
#include "variables.h"

void keyboard(unsigned char key, int x, int y);
void arrowKey(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouseRotate(int x, int y);
void mouseMove(int x, int y);

#endif /* Control_hpp */
