#include "texturedquad.h"

using namespace OSS;


TexturedQuad::TexturedQuad()
{	
	color = (color4d){1, 1, 1, 1};
	alpha = 1;
	
	textureRect = rectd(0, 0, 1, 1);
}

void TexturedQuad::autogenerateTextureRect(bool x, bool y, double2 offset, double2 scale)
{
	if (!texture) return;
	if (x) {
		textureRect.origin.x = (rect.origin.x - offset.x) / texture->size.x * scale.x;
		textureRect.size.x = rect.size.x / texture->size.x * scale.x;
	}
	if (y) {
		textureRect.origin.y = (rect.origin.y - offset.x) / texture->size.y * scale.y;
		textureRect.size.y = rect.size.y / texture->size.y * scale.y;
	}
}

void TexturedQuad::draw()
{
	//Calculate the final color
	color4d final = color;
	final.c.a *= alpha;
	
	//Set the color
	glColor4dv(final.v);
	
	//Bind the texture if there's one
	if (texture)
		texture->bind();
	else
		Texture::unbind();
	
	//Move the texture rect into a positive coordinate space to simplify the rest of the process
	rectd transformedTextureRect = textureRect;
	if (transformedTextureRect.origin.x < 0)
		transformedTextureRect.origin.x += -floor(transformedTextureRect.origin.x);
	if (transformedTextureRect.origin.y < 0)
		transformedTextureRect.origin.y += -floor(transformedTextureRect.origin.y);
	
	//Draw the tiles
	glBegin(GL_QUADS);
	
	//Draw with the texture if there's one
	if (texture) {
		rectd textureSubrect;
		for (textureSubrect.origin.x = transformedTextureRect.minX();
			 textureSubrect.origin.x < transformedTextureRect.maxX();
			 textureSubrect.origin.x += textureSubrect.size.x) {
			
			//Calculate the width of the texture subrect
			textureSubrect.size.x = mind(ceil(textureSubrect.origin.x + 1e-2) - textureSubrect.origin.x, 1);
			textureSubrect.size.x -= maxd(textureSubrect.maxX() - transformedTextureRect.maxX(), 0);
			if (textureSubrect.size.x < 1e-6) break;
			
			//Iterate through the subrect vertically
			for (textureSubrect.origin.y = transformedTextureRect.minY();
				 textureSubrect.origin.y < transformedTextureRect.maxY();
				 textureSubrect.origin.y += textureSubrect.size.y) {
				
				//Calculate the height of the texture subrect
				textureSubrect.size.y = mind(ceil(textureSubrect.origin.y + 1e-2) - textureSubrect.origin.y, 1);
				textureSubrect.size.y -= maxd(textureSubrect.maxY() - transformedTextureRect.maxY(), 0);
				if (textureSubrect.size.y < 1e-6) break;
				
				//Calculate the tile rect for the given texture subrect
				rectd tileRect = textureSubrect;
				tileRect.origin -= transformedTextureRect.origin;
				tileRect.origin *= rect.size / transformedTextureRect.size;
				tileRect.size *= rect.size / transformedTextureRect.size;
				tileRect.origin += rect.origin;
				
				//Calculate the pixel coordinates of the textureSubrect, since GL_TEXTURE_RECTANGLE_ARB
				//works that way
				rectd pixelSubrect = textureSubrect;
				pixelSubrect.origin.x = fmod(pixelSubrect.origin.x, 1);
				pixelSubrect.origin.y = fmod(pixelSubrect.origin.y, 1);
				pixelSubrect.origin *= texture->size;
				pixelSubrect.size *= texture->size;
				
				//Draw the textured tile
				glTexCoord2d(pixelSubrect.minX(), pixelSubrect.minY());
				glVertex2d(tileRect.minX(), tileRect.minY());
				glTexCoord2d(pixelSubrect.maxX(), pixelSubrect.minY());
				glVertex2d(tileRect.maxX(), tileRect.minY());
				glTexCoord2d(pixelSubrect.maxX(), pixelSubrect.maxY());
				glVertex2d(tileRect.maxX(), tileRect.maxY());
				glTexCoord2d(pixelSubrect.minX(), pixelSubrect.maxY());
				glVertex2d(tileRect.minX(), tileRect.maxY());
			}
		}
	}
	
	//Or just draw the colored sprite
	else {
		glVertex2d(rect.minX(), rect.minY());
		glVertex2d(rect.maxX(), rect.minY());
		glVertex2d(rect.maxX(), rect.maxY());
		glVertex2d(rect.minX(), rect.maxY());
	}
	
	glEnd();
}
