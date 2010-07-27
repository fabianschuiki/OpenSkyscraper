/*
 *  renderer.cpp
 *  OpenSkyscraper
 *
 *  Created by Fabian Schuiki on 23.07.10.
 *  Copyright 2010 Axamblis. All rights reserved.
 *
 */

#include "opengl.h"
#include "renderer.h"
#include "engine.h"
#include "node.h"

using namespace OSS;


void Renderer::setRootNode(Node * node)
{
	this->rootNode = node;
}

void Renderer::setCamera(Camera * camera)
{
	this->camera = camera;
	if (camera)
		camera->reshape(size);
}

void Renderer::prepareOpenGL()
{
	OSSFuncLog();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_TEXTURE_2D);
}

void Renderer::draw()
{	
	//Clear the frame buffer and load the identity matrix
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
	//Draw the scene graph
	if (rootNode)
		rootNode->draw();
}

void Renderer::reshape(int2 size)
{
	this->size = size;
	
	glViewport(0, 0, size.x, size.y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, size.x, 0, size.y, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	
	if (rootNode)
		rootNode->reshape(size);
}
