////////////////////////////////////////////////////////////
//
// This is a modified version of sf::Sprite to work around the "pixel perfect" rendering issue.
// Refer to the original Sprite.hpp and Sprite.cpp files for details.
//
////////////////////////////////////////////////////////////
#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Resource.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>


namespace sf { class Image; }

namespace OT
{
	
	////////////////////////////////////////////////////////////
	/// Sprite defines a sprite : texture, transformations,
	/// color, and draw on screen
	////////////////////////////////////////////////////////////
	class SFML_API Sprite : public sf::Drawable
	{
	public :
		
		////////////////////////////////////////////////////////////
		/// Default constructor
		///
		////////////////////////////////////////////////////////////
		Sprite();
		
		////////////////////////////////////////////////////////////
		/// Construct the sprite from a source image
		///
		/// \param Img :      Image of the sprite
		/// \param Position : Position of the sprite (0, 0 by default)
		/// \param Scale :    Scale factor (1, 1 by default)
		/// \param Rotation : Orientation, in degrees (0 by default)
		/// \param Col :      Color of the sprite (white by default)
		///
		////////////////////////////////////////////////////////////
		explicit Sprite(const sf::Image& Img, const sf::Vector2f& Position = sf::Vector2f(0, 0), const sf::Vector2f& Scale = sf::Vector2f(1, 1), float Rotation = 0.f, const sf::Color& Col = sf::Color(255, 255, 255, 255));
		
		////////////////////////////////////////////////////////////
		/// Change the image of the sprite
		///
		/// \param Img : New image
		///
		////////////////////////////////////////////////////////////
		void SetImage(const sf::Image& Img);
		
		////////////////////////////////////////////////////////////
		/// Set the sub-rectangle of the sprite inside the source image.
		/// By default, the subrect covers the entire source image
		///
		/// \param SubRect : New sub-rectangle
		///
		////////////////////////////////////////////////////////////
		void SetSubRect(const sf::IntRect& SubRect);
		
		////////////////////////////////////////////////////////////
		/// Resize the sprite (by changing its scale factors) (take 2 values).
		/// The default size is defined by the subrect
		///
		/// \param Width :  New width (must be strictly positive)
		/// \param Height : New height (must be strictly positive)
		///
		////////////////////////////////////////////////////////////
		void Resize(float Width, float Height);
		
		////////////////////////////////////////////////////////////
		/// Resize the sprite (by changing its scale factors) (take a 2D vector).
		/// The default size is defined by the subrect
		///
		/// \param Size : New size (both coordinates must be strictly positive)
		///
		////////////////////////////////////////////////////////////
		void Resize(const sf::Vector2f& Size);
		
		////////////////////////////////////////////////////////////
		/// Flip the sprite horizontally
		///
		/// \param Flipped : True to flip the sprite
		///
		////////////////////////////////////////////////////////////
		void FlipX(bool Flipped);
		
		////////////////////////////////////////////////////////////
		/// Flip the sprite vertically
		///
		/// \param Flipped : True to flip the sprite
		///
		////////////////////////////////////////////////////////////
		void FlipY(bool Flipped);
		
		////////////////////////////////////////////////////////////
		/// Get the source image of the sprite
		///
		/// \return Pointer to the image (can be NULL)
		///
		////////////////////////////////////////////////////////////
		const sf::Image* GetImage() const;
		
		////////////////////////////////////////////////////////////
		/// Get the sub-rectangle of the sprite inside the source image
		///
		/// \return Sub-rectangle
		///
		////////////////////////////////////////////////////////////
		const sf::IntRect& GetSubRect() const;
		
		////////////////////////////////////////////////////////////
		/// Get the sprite size
		///
		/// \return Size of the sprite
		///
		////////////////////////////////////////////////////////////
		sf::Vector2f GetSize() const;
		
		////////////////////////////////////////////////////////////
		/// Get the color of a given pixel in the sprite
		/// (point is in local coordinates)
		///
		/// \param X : X coordinate of the pixel to get
		/// \param Y : Y coordinate of the pixel to get
		///
		/// \return Color of pixel (X, Y)
		///
		////////////////////////////////////////////////////////////
		sf::Color GetPixel(unsigned int X, unsigned int Y) const;
		
	protected :
		
		////////////////////////////////////////////////////////////
		/// /see Drawable::Render
		///
		////////////////////////////////////////////////////////////
		virtual void Render(sf::RenderTarget& Target) const;
		
	private :
		
		////////////////////////////////////////////////////////////
		// Member data
		////////////////////////////////////////////////////////////
		sf::ResourcePtr<sf::Image> myImage;      ///< Image used to draw the sprite
		sf::IntRect            mySubRect;    ///< Sub-rectangle of source image to assign to the sprite
		bool               myIsFlippedX; ///< Is the sprite flipped on the X axis ?
		bool               myIsFlippedY; ///< Is the sprite flipped on the Y axis ?
	};
	
} // namespace sf

