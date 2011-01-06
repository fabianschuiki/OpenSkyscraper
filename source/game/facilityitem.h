#ifndef OSS_FACILITYITEM_H
#define OSS_FACILITYITEM_H


#include "../general.h"
#include "item.h"
#include "tower.h"


namespace OSS {
	class FacilityItem : public Item {
	public:
		//Initialization
		FacilityItem(Tower * tower, Item::Descriptor * descriptor);
		
		/**
		 * Basic Sprites
		 */
		void initBasicSprites();
		void updateBasicSprites();
		
		//Ceiling
	protected:
		Sprite ceiling;
	private:
		bool hasCeiling;
	public:
		virtual void initCeiling();
		virtual void updateCeiling();
		bool getHasCeiling() const;
		void setHasCeiling(bool flag);
		
		//Background
		virtual void updateBackground();
		
		
		/**
		 * Variant
		 */
	private:
		unsigned int variant;
	public:
		unsigned int getVariant() const;
		void setVariant(const unsigned int variant);
		virtual void onChangeVariant();
		
		
		/**
		 * Lighting
		 */
	private:
		bool lit;
	public:
		bool isLit() const;
		void setLit(bool lit);
		bool shouldBeLitDueToTime();
		virtual void updateLighting();
		virtual void onChangeLit();
		
		
		/**
		 * Simulation
		 */
		virtual void advance(double dt);
		
		
		/**
		 * Drawing
		 */
		virtual void draw(rectd visibleRect);
	};
}


#endif
