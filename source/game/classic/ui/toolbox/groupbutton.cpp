#include "groupbutton.h"

#include "../../items/item.h"

using namespace OSS;
using namespace Classic;





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Construction
//----------------------------------------------------------------------------------------------------

ToolsGroupButton::ToolsGroupButton(ToolsWindow * window, ItemGroup group) :
window(window), group(group)
{
	//Reset the variables to 0
	selectedItem = kNoType;
	
	//The group buttons have a fixed width, so set it.
	setFrameSize(double2(32, 32));
	
	//Initialize the popup view
	popup = new View;
	
	//Initialize the item buttons. To do this we iterate through all item types.
	for (unsigned int i = kNoType + 1; i < kMaxType; i++) {
		
		//Fetch the descriptor for this type
		ItemDescriptor * descriptor = Item::descriptorForItemType((ItemType)i);
		if (!descriptor)
			continue;
		
		//Skip items that don't belong to our group.
		if (descriptor->group != group)
			continue;
		
		//Create the new button instance with the same frame size than we have.
		Button * button = new Button;
		button->setFrameSize(getFrameSize());
		
		//Load the textures
		button->normalTexture = Texture::named("simtower/ui/toolbox/construction/normal");
		button->pressedTexture = Texture::named("simtower/ui/toolbox/construction/selected");
		
		//Calculate the texture rect for this item type. Since we structured the ItemType enumerator
		//to have the same order as these textures, we may simple use the enumerator value as index
		//into the texture.
		rectd texrect(0, 0, 1.0 / 8, 1.0 / 4);
		unsigned int sliceIndex = (i - kLobbyType);
		texrect.origin.x = texrect.size.x * (sliceIndex % 8);
		texrect.origin.y = texrect.size.y * (3 - sliceIndex / 8);
		
		//Both texture rects are the same
		button->normalTextureRect = texrect;
		button->pressedTextureRect = texrect;
		
		//Add the button to the popup
		popup->addSubview(button);
		
		//Keep track of the button
		itemButtons[(ItemType)i] = button;
	}
	
	//Select a default item
	setSelectedItem(kLobbyType);
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark PopUp
//----------------------------------------------------------------------------------------------------

ItemType ToolsGroupButton::getSelectedItem()
{
	return selectedItem;
}

void ToolsGroupButton::setSelectedItem(ItemType type)
{
	if (selectedItem != type) {
		//If we don't have this item type, choose the first one we actually do have.
		if (!itemButtons.count(type))
			type = itemButtons.begin()->first;
		
		selectedItem = type;
		updateIfNeeded.setNeeded();
	}
}

Button * ToolsGroupButton::getSelectedButton()
{
	if (itemButtons.count(getSelectedItem()))
		return itemButtons[getSelectedItem()];
	return NULL;
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark State
//----------------------------------------------------------------------------------------------------

void ToolsGroupButton::update()
{
	//Make sure the item we show is the same as the one that is selected
	Button * button = getSelectedButton();
	if (button) {
		normalTexture = button->normalTexture;
		pressedTexture = button->pressedTexture;
		normalTextureRect = button->normalTextureRect;
		pressedTextureRect = button->pressedTextureRect;
	}
	
	//Update the popup view
	updatePopUp();
}

void ToolsGroupButton::updatePopUp()
{
	//Calculate the button offset
	double offset = itemButtons.size() * 32;
	
	//Iterate through the item buttons and order them. Calculate the overall required rect while
	//doing so. Also note the rect of the selected button so we can position the popup the right
	//way.
	int buttonIndex = 0;
	for (ItemButtonMap::iterator it = itemButtons.begin(); it != itemButtons.end(); it++) {
		
		//Calculate the button position
		double2 position(0, offset - (buttonIndex + 1) * 32);
		
		//Position the button
		it->second->setFrameOrigin(position);
		
		//Increase the button index
		buttonIndex++;
	}
	
	//Adjust the popup size
	popup->setFrameSize(double2(32, offset));
}





//----------------------------------------------------------------------------------------------------
#pragma mark -
#pragma mark Events
//----------------------------------------------------------------------------------------------------

bool ToolsGroupButton::eventMouseDown(MouseButtonEvent * event)
{
	if (!popup->getSuperview()) {
		window->getRootView()->addSubview(popup);
	}
	return true;
}

bool ToolsGroupButton::eventMouseUp(MouseButtonEvent * event)
{
	popup->removeFromSuperview();
	return true;
}
