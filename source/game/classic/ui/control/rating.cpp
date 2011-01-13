#include "rating.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

RatingView::RatingView(ControlWindow * window) : controlWindow(window)
{
	//Since the rating view has a fixed size we can set it right here
	setFrameSize(double2(108, 22));
	
	//Load the star textures
	emptyStarTexture = Texture::named("simtower/ui/control/star/empty");
	filledStarTexture = Texture::named("simtower/ui/control/star/filled");
	
	//Load the tower rating texture
	towerRatingTexture = Texture::named("simtower/ui/control/towerrating");
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Drawing
//----------------------------------------------------------------------------------------------------

void RatingView::draw(rectd dirtyRect)
{
	//Get the tower from our control window parent
	Tower * tower = controlWindow->getTower();
	if (!tower)
		return;
	
	//Get the rating of the tower
	unsigned int rating = tower->environment->getRating();
	
	//If the tower rating has been achieved, draw its texture
	if (rating == 6) {
		//Create a textured quad with the tower rating texture
		TexturedQuad quad;
		quad.rect.size = getFrameSize();
		quad.texture = towerRatingTexture;
		
		//Draw the quad
		quad.draw();
	}
	
	//Otherwise draw the 5 stars
	else {
		//Create a textured quad to be used to draw the stars
		TexturedQuad quad;
		quad.rect = rectd(0, 2, 24, 19);
		
		//Draw the stars
		for (int i = 0; i < 5; i++) {
			//Calculate the star location
			quad.rect.origin.x = (1 + i * 21);
			
			//Set the texture to filled if the star is lower than the current tower rating
			quad.texture = (i < rating ? filledStarTexture : emptyStarTexture);
			
			//Actually draw the star
			quad.draw();
		}
	}
}
