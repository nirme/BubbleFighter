#pragma once

#include "../Vector2.h"




namespace core
{
	namespace _2d
	{

		class AxisAlignedBox
		{
		protected:
			Vector2 vMin;
			Vector2 vMax;

		public:

			//  3---0
			//  |   |
			//  2---1

			enum Corner
			{
				TOP_RIGHT = 0,
				BOTTOM_RIGHT = 1,
				BOTTOM_LEFT = 2,
				TOP_LEFT = 3,
			};


			inline AxisAlignedBox() :
				vMin(-0.5f), vMax(0.5f)
			{};

			inline AxisAlignedBox(float _minX, float _minY, float _maxX, float _maxY) :
				vMin(_minX, _minY), vMax(_maxX, _maxY)
			{};

			inline AxisAlignedBox(const Vector2& _min, const Vector2& _max) :
				vMin(_min), vMax(_max)
			{};


			inline AxisAlignedBox& operator= (const AxisAlignedBox& _box)
			{
				vMin = _box.getMinimum();
				vMax = _box.getMaximum();
			};



			inline void setMinimum(const Vector2& _min)
			{
				vMin = _min;
			};

			inline void setMaximum(const Vector2& _max)
			{
				vMax = _max;
			};


			inline const Vector2& getMinimum() const
			{
				return vMin;
			};

			inline const Vector2& getMaximum() const
			{
				return vMax;
			};


			inline AxisAlignedBox& merge(const AxisAlignedBox& _box)
			{
				if (vMin.x > _box.getMinimum().x)
					vMin.x = _box.getMinimum().x;

				if (vMin.y > _box.getMinimum().y)
					vMin.y = _box.getMinimum().y;

				if (vMax.x > _box.getMaximum().x)
					vMax.x = _box.getMaximum().x;

				if (vMax.y > _box.getMaximum().y)
					vMax.y = _box.getMaximum().y;
			};


			static AxisAlignedBox merge(const AxisAlignedBox& _box1, const AxisAlignedBox& _box2)
			{
				return AxisAlignedBox(
					_box1.getMinimum().x < _box2.getMinimum().x ? _box1.getMinimum().x : _box2.getMinimum().x,
					_box1.getMinimum().y < _box2.getMinimum().y ? _box1.getMinimum().y : _box2.getMinimum().y,
					_box1.getMaximum().x < _box2.getMaximum().x ? _box1.getMaximum().x : _box2.getMaximum().x,
					_box1.getMaximum().y < _box2.getMaximum().y ? _box1.getMaximum().y : _box2.getMaximum().y
				);
			};

		};

	}
}
