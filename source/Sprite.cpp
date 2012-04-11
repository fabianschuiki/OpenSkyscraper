////////////////////////////////////////////////////////////
//
// This is a modified version of sf::Sprite to work around the "pixel perfect" rendering issue.
// Refer to the original Sprite.hpp and Sprite.cpp files for details.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Sprite.h"
#include <SFML/Graphics/Image.hpp>
#include <GL/gl.h>


namespace OT
{
	////////////////////////////////////////////////////////////
	/// Default constructor
	////////////////////////////////////////////////////////////
	Sprite::Sprite() :
	mySubRect   (0, 0, 1, 1),
	myIsFlippedX(false),
	myIsFlippedY(false)
	{
		
	}
	
	
	////////////////////////////////////////////////////////////
	/// Construct the sprite from a source image
	////////////////////////////////////////////////////////////
	Sprite::Sprite(const sf::Image& Img, const sf::Vector2f& Position, const sf::Vector2f& Scale, float Rotation, const sf::Color& Col) :
	Drawable    (Position, Scale, Rotation, Col),
	mySubRect   (0, 0, 1, 1),
	myIsFlippedX(false),
	myIsFlippedY(false)
	{
		SetImage(Img);
	}
	
	
	////////////////////////////////////////////////////////////
	/// Set the image of the sprite
	////////////////////////////////////////////////////////////
	void Sprite::SetImage(const sf::Image& Img)
	{
		// If there was no source image before and the new image is valid, adjust the source rectangle
		if (!myImage && (Img.GetWidth() > 0) && (Img.GetHeight() > 0))
		{
			SetSubRect(sf::IntRect(0, 0, Img.GetWidth(), Img.GetHeight()));
		}
		
		// Assign the new image
		myImage = &Img;
	}
	
	
	////////////////////////////////////////////////////////////
	/// Set the sub-rectangle of the sprite inside the source image
	////////////////////////////////////////////////////////////
	void Sprite::SetSubRect(const sf::IntRect& SubRect)
	{
		mySubRect = SubRect;
	}
	
	
	////////////////////////////////////////////////////////////
	/// Resize the sprite (by changing its scale factors) (take 2 values).
	/// The default size is defined by the subrect
	////////////////////////////////////////////////////////////
	void Sprite::Resize(float Width, float Height)
	{
		int LocalWidth  = mySubRect.GetWidth();
		int LocalHeight = mySubRect.GetHeight();
		
		if ((LocalWidth > 0) && (LocalHeight > 0))
			SetScale(Width / LocalWidth, Height / LocalHeight);
	}
	
	
	////////////////////////////////////////////////////////////
	/// Resize the object (by changing its scale factors) (take a 2D vector)
	/// The default size is defined by the subrect
	////////////////////////////////////////////////////////////
	void Sprite::Resize(const sf::Vector2f& Size)
	{
		Resize(Size.x, Size.y);
	}
	
	
	////////////////////////////////////////////////////////////
	/// Flip the sprite horizontally
	////////////////////////////////////////////////////////////
	void Sprite::FlipX(bool Flipped)
	{
		myIsFlippedX = Flipped;
	}
	
	
	////////////////////////////////////////////////////////////
	/// Flip the sprite vertically
	////////////////////////////////////////////////////////////
	void Sprite::FlipY(bool Flipped)
	{
		myIsFlippedY = Flipped;
	}
	
	
	////////////////////////////////////////////////////////////
	/// Get the source image of the sprite
	////////////////////////////////////////////////////////////
	const sf::Image* Sprite::GetImage() const
	{
		return myImage;
	}
	
	
	////////////////////////////////////////////////////////////
	/// Get the sub-rectangle of the sprite inside the source image
	////////////////////////////////////////////////////////////
	const sf::IntRect& Sprite::GetSubRect() const
	{
		return mySubRect;
	}
	
	
	////////////////////////////////////////////////////////////
	/// Get the sprite size
	////////////////////////////////////////////////////////////
	sf::Vector2f Sprite::GetSize() const
	{
		return sf::Vector2f(mySubRect.GetWidth() * GetScale().x, mySubRect.GetHeight() * GetScale().y);
	}
	
	
	////////////////////////////////////////////////////////////
	/// Get the color of a given pixel in the sprite
	/// (point is in local coordinates)
	////////////////////////////////////////////////////////////
	sf::Color Sprite::GetPixel(unsigned int X, unsigned int Y) const
	{
		if (myImage)
		{
			unsigned int ImageX = mySubRect.Left + X;
			unsigned int ImageY = mySubRect.Top  + Y;
			
			if (myIsFlippedX) ImageX = mySubRect.GetWidth()  - ImageX - 1;
			if (myIsFlippedY) ImageY = mySubRect.GetHeight() - ImageY - 1;
			
			return myImage->GetPixel(ImageX, ImageY) * GetColor();
		}
		else
		{
			return GetColor();
		}
	}
	
	
	////////////////////////////////////////////////////////////
	/// /see sfDrawable::Render
	////////////////////////////////////////////////////////////
	void Sprite::Render(sf::RenderTarget&) const
	{
		// Get the sprite size
		float Width  = static_cast<float>(mySubRect.GetWidth());
		float Height = static_cast<float>(mySubRect.GetHeight());
		
		// Check if the image is valid
		if (myImage && (myImage->GetWidth() > 0) && (myImage->GetHeight() > 0))
		{
			// Use the "offset trick" to get pixel-perfect rendering
			// see http://www.opengl.org/resources/faq/technical/transformations.htm#tran0030
			
			//NOTE: The following needed to be commented so now pseudo-pixelperfect rendering takes place.
			//glTranslatef(0.375f, 0.375f, 0.f);
			
			// Bind the texture
			myImage->Bind();
			
			// Calculate the texture coordinates
			sf::FloatRect TexCoords = myImage->GetTexCoords(mySubRect);
			sf::FloatRect Rect(myIsFlippedX ? TexCoords.Right  : TexCoords.Left,
						   myIsFlippedY ? TexCoords.Bottom : TexCoords.Top,
				  myIsFlippedX ? TexCoords.Left   : TexCoords.Right,
				  myIsFlippedY ? TexCoords.Top    : TexCoords.Bottom);
			
			// Draw the sprite's triangles
			glBegin(GL_QUADS);
			glTexCoord2f(Rect.Left,  Rect.Top);    glVertex2f(0,     0);
			glTexCoord2f(Rect.Left,  Rect.Bottom); glVertex2f(0,     Height);
			glTexCoord2f(Rect.Right, Rect.Bottom); glVertex2f(Width, Height);
			glTexCoord2f(Rect.Right, Rect.Top);    glVertex2f(Width, 0) ;
			glEnd();
		}
		else
		{
			// Disable texturing
			glDisable(GL_TEXTURE_2D);
			
			// Draw the sprite's triangles
			glBegin(GL_QUADS);
			glVertex2f(0,     0);
			glVertex2f(0,     Height);
			glVertex2f(Width, Height);
			glVertex2f(Width, 0);
			glEnd();
		}
	}
	
} // namespace sf
